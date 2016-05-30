/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * physical_operator/exchange_merger.h
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#include "../physical_operator/exchange_sender_pipeline.h"

#include <glog/raw_logging.h>
#include <malloc.h>
#include <stack>

#include "../../configure.h"
#include "../../common/rename.h"
#include "../../common/Logging.h"
#include "../../common/ids.h"
//#include "../../common/log/logging.h"
#include "../../Environment.h"
#include "../../utility/ThreadSafe.h"
#include "../../utility/rdtsc.h"
#include "../Executor/exchange_tracker.h"
namespace claims {
namespace physical_operator {

ExchangeSenderPipeline::ExchangeSenderPipeline(State state)
    : state_(state),
      sender_thread_id_(0),
      partitioned_block_stream_(NULL),
      partitioned_data_buffer_(NULL),
      block_for_asking_(NULL),
      block_for_sending_buffer_(NULL),
      block_for_serialization_(NULL),
      sending_buffer_(NULL),
      partition_function_(NULL),
      socket_fd_upper_list_(NULL) {
  set_phy_oper_type(kphysicalExchangeSender);
  assert(state.partition_schema_.partition_key_index < 100);
}

ExchangeSenderPipeline::ExchangeSenderPipeline()
    : sender_thread_id_(0),
      partitioned_block_stream_(NULL),
      partitioned_data_buffer_(NULL),
      block_for_asking_(NULL),
      block_for_sending_buffer_(NULL),
      block_for_serialization_(NULL),
      sending_buffer_(NULL),
      partition_function_(NULL),
      socket_fd_upper_list_(NULL) {
  set_phy_oper_type(kphysicalExchangeSender);
}

ExchangeSenderPipeline::~ExchangeSenderPipeline() {
  if (NULL != state_.schema_) {
    delete state_.schema_;
    state_.schema_ = NULL;
  }
  if (NULL != state_.child_) {
    delete state_.child_;
    state_.child_ = NULL;
  }
}
/**
 * pay attention to the work of different block buffer according to the
 * comments near it
 */
bool ExchangeSenderPipeline::Open(SegmentExecStatus* const exec_status,
                                  const PartitionOffset&) {
  RETURN_IF_CANCELLED(exec_status);
  state_.child_->Open(exec_status, state_.partition_offset_);
  RETURN_IF_CANCELLED(exec_status);

  upper_num_ = state_.upper_id_list_.size();
  partition_function_ =
      PartitionFunctionFactory::createBoostHashFunction(upper_num_);
  socket_fd_upper_list_ = new int[upper_num_];

  /**
   * initialize the block that is used to accumulate the block obtained
   * by calling child iterator's next()
   */
  block_for_asking_ =
      BlockStreamBase::createBlock(state_.schema_, state_.block_size_);

  /**
   * partitioned_data_buffer_ stores the tuples received from child iterator.
   * Note the tuples are partitioned and stored.
   */
  partitioned_data_buffer_ = new PartitionedBlockBuffer(
      upper_num_, block_for_asking_->getSerializedBlockSize());

  /**
   * the temporary block that is used to transfer a block from partitioned data
   * buffer into sending_buffer.
   */
  block_for_sending_buffer_ =
      new BlockContainer(block_for_asking_->getSerializedBlockSize());

  /**
   * Initialize the buffer that is used to hold the blocks being sent. There are
   * upper_num blocks, each corresponding to a merger.
   */
  sending_buffer_ = new PartitionedBlockContainer(
      upper_num_, block_for_asking_->getSerializedBlockSize());

  // Initialized the temporary block to hold the serialized block.

  block_for_serialization_ =
      new Block(block_for_asking_->getSerializedBlockSize());

  /**
   * Initialize the blocks that are used to accumulate the tuples from child so
   * that the insertion to the buffer
   * can be conducted at the granularity of blocks rather than tuples.
   */
  partitioned_block_stream_ = new BlockStreamBase* [upper_num_];
  for (unsigned i = 0; i < upper_num_; ++i) {
    partitioned_block_stream_[i] =
        BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
  }
  RETURN_IF_CANCELLED(exec_status);

  /** connect to all the mergers **/
  for (unsigned upper_offset = 0; upper_offset < state_.upper_id_list_.size();
       ++upper_offset) {
    RETURN_IF_CANCELLED(exec_status);

    LOG(INFO) << "(exchane_id= " << state_.exchange_id_
              << " partition_offset= " << state_.partition_offset_
              << " ) try to connect to upper( " << upper_offset << " , "
              << state_.upper_id_list_[upper_offset] << " ) ";

    if (ConnectToUpper(ExchangeID(state_.exchange_id_, upper_offset),
                       state_.upper_id_list_[upper_offset],
                       socket_fd_upper_list_[upper_offset]) != true) {
      LOG(INFO) << "unsuccessfully !" << std::endl;
      return false;
    }
  }
  LOG(INFO) << "connect to all mereger successfully !" << std::endl;

  RETURN_IF_CANCELLED(exec_status);

  /** create the Sender thread **/
  int error = pthread_create(&sender_thread_id_, NULL, Sender, this);
  if (error != 0) {
    LOG(ERROR) << "(exchane_id= " << state_.exchange_id_
               << " partition_offset= " << state_.partition_offset_
               << " ) Failed to create the sender thread>>>>>>>>>>"
               << std::endl;
    return false;
  }
  return true;
}
/**
 * Note the process from getting block of child to sending to mergers in
 * different buffer.
 * if the state_.partition_schema_ is hash partitioned, every tuple of the block
 * which get from child will be hash repartition and copied into
 * partitioned_block_stream_, if it is full, then
 * serialize it and insert into corresponding partition buffer.
 * else the state_.partition_schema_ is broadcast, straightly insert the block
 * from child into each partition buffer.
 */
bool ExchangeSenderPipeline::Next(SegmentExecStatus* const exec_status,
                                  BlockStreamBase* no_block) {
  void* tuple_from_child;
  void* tuple_in_cur_block_stream;
  while (true) {
    RETURN_IF_CANCELLED(exec_status);

    block_for_asking_->setEmpty();
    if (state_.child_->Next(exec_status, block_for_asking_)) {
      RETURN_IF_CANCELLED(exec_status);

      /**
       * if a blocks is obtained from child, we repartition the tuples in the
       * block to corresponding partition_block_stream_.
       */
      if (state_.partition_schema_.isHashPartition()) {
        BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator =
            block_for_asking_->createIterator();
        while ((tuple_from_child = traverse_iterator->nextTuple()) > 0) {
          /**
           * for each tuple in the newly obtained block, insert the tuple to
           * one partitioned block according to the partition hash value
           */
          const unsigned partition_id = GetHashPartitionId(
              tuple_from_child, state_.schema_,
              state_.partition_schema_.partition_key_index, upper_num_);

          // calculate the tuple size for the current tuple
          const unsigned bytes =
              state_.schema_->getTupleActualSize(tuple_from_child);

          // insert the tuple into the corresponding partitioned block
          while (!(tuple_in_cur_block_stream =
                       partitioned_block_stream_[partition_id]->allocateTuple(
                           bytes))) {
            /**
             * if the destination block is full, it should be serialized and
             * inserted into the partitioned_data_buffer.
             */
            partitioned_block_stream_[partition_id]->serialize(
                *block_for_serialization_);
            partitioned_data_buffer_->insertBlockToPartitionedList(
                block_for_serialization_, partition_id);
            partitioned_block_stream_[partition_id]->setEmpty();
          }
          /**
           * thread arriving here means that the space for the tuple is
           * successfully allocated, so we copy the tuple
           */
          state_.schema_->copyTuple(tuple_from_child,
                                    tuple_in_cur_block_stream);
        }
        DELETE_PTR(traverse_iterator);  // by hAN MEMORY LEAK
      } else if (state_.partition_schema_.isBroadcastPartition()) {
        /**
         * for boardcast case, all block from child should inserted into all
         * partitioned_data_buffer
         */
        block_for_asking_->serialize(*block_for_serialization_);
        for (unsigned i = 0; i < upper_num_; ++i) {
          partitioned_data_buffer_->insertBlockToPartitionedList(
              block_for_serialization_, i);
        }
      }
    } else {
      RETURN_IF_CANCELLED(exec_status);

      if (state_.partition_schema_.isHashPartition()) {
        /* the child iterator is exhausted. We add the last block stream block
         * which would be not full into the buffer for hash partitioned case.
         */
        for (unsigned i = 0; i < upper_num_; ++i) {
          partitioned_block_stream_[i]->serialize(*block_for_serialization_);
          partitioned_data_buffer_->insertBlockToPartitionedList(
              block_for_serialization_, i);
        }
        /* The following lines send an empty block to the upper, indicating that
         * all the data from current sent has been transmit to the uppers.
         */
        for (unsigned i = 0; i < upper_num_; ++i) {
          if (!partitioned_block_stream_[i]->Empty()) {
            partitioned_block_stream_[i]->setEmpty();
            partitioned_block_stream_[i]->serialize(*block_for_serialization_);
            partitioned_data_buffer_->insertBlockToPartitionedList(
                block_for_serialization_, i);
          }
        }
      } else if (state_.partition_schema_.isBroadcastPartition()) {
        /* The following lines send an empty block to the upper, indicating that
         * all the data from current sent has been transmit to the uppers.
         */
        block_for_asking_->setEmpty();
        block_for_asking_->serialize(*block_for_serialization_);
        for (unsigned i = 0; i < upper_num_; ++i) {
          partitioned_data_buffer_->insertBlockToPartitionedList(
              block_for_serialization_, i);
        }
      }

      /*
       * waiting until all the block in the buffer has been
       * transformed to the uppers.
       */
      LOG(INFO) << "(exchane_id= " << state_.exchange_id_
                << " partition_offset= " << state_.partition_offset_
                << " ) Waiting until all the blocks in the buffer is sent!"
                << std::endl;
      RETURN_IF_CANCELLED(exec_status);

      while (!partitioned_data_buffer_->isEmpty()) {
        RETURN_IF_CANCELLED(exec_status);

        usleep(1);
      }

      /*
       * waiting until all the uppers send the close notification which means
       * that
       * blocks in the uppers' socket buffer have all been
       * consumed.
       */
      LOG(INFO) << "(exchane_id= " << state_.exchange_id_
                << " partition_offset= " << state_.partition_offset_
                << " ) Waiting for close notification from all merger!"
                << std::endl;
      RETURN_IF_CANCELLED(exec_status);

      for (unsigned i = 0; i < upper_num_; i++) {
        RETURN_IF_CANCELLED(exec_status);

        WaitingForCloseNotification(socket_fd_upper_list_[i]);
      }
      LOG(INFO) << " received all close notification, closing.. " << endl;
      return false;
    }
  }
}

bool ExchangeSenderPipeline::Close(SegmentExecStatus* const exec_status) {
  CancelSenderThread();
  state_.child_->Close(exec_status);
  // free temporary space
  if (NULL != partitioned_data_buffer_) {
    delete partitioned_data_buffer_;
    partitioned_data_buffer_ = NULL;
  }
  if (NULL != block_for_asking_) {
    delete block_for_asking_;
    block_for_asking_ = NULL;
  }
  if (NULL != block_for_serialization_) {
    delete block_for_serialization_;
    block_for_serialization_ = NULL;
  }
  if (NULL != sending_buffer_) {
    delete sending_buffer_;
    sending_buffer_ = NULL;
  }

  if (NULL != block_for_sending_buffer_) {
    delete block_for_sending_buffer_;
    block_for_sending_buffer_ = NULL;
  }
  for (unsigned i = 0; NULL != partitioned_block_stream_ && i < upper_num_;
       i++) {
    if (NULL != partitioned_block_stream_[i]) {
      delete partitioned_block_stream_[i];
      partitioned_block_stream_[i] = NULL;
    }
  }
  if (NULL != partitioned_block_stream_) {
    delete[] partitioned_block_stream_;
    partitioned_block_stream_ = NULL;
  }
  if (NULL != socket_fd_upper_list_) {
    delete[] socket_fd_upper_list_;
    socket_fd_upper_list_ = NULL;
  }
  if (NULL != partition_function_) {
    delete partition_function_;
    partition_function_ = NULL;
  }
  return true;
}
/**
 * sending_buffer_ contains one block from every partition (e.t. socket
 * connection), if one block is send completely, supply one from
 * partitioned_data_buffer_
 */
void* ExchangeSenderPipeline::Sender(void* arg) {
  ExchangeSenderPipeline* Pthis =
      reinterpret_cast<ExchangeSenderPipeline*>(arg);
  pthread_testcancel();

  //  LOG(INFO) << "(exchange_id = " << Pthis->state_.exchange_id_
  //            << " , partition_offset = " << Pthis->state_.partition_offset_
  //            << " ) sender thread created successfully!";
  RAW_LOG(INFO,
          "exchange_id= %d, par_off= %d sender thread is created successfully!",
          Pthis->state_.exchange_id_, Pthis->state_.partition_offset_);
  Pthis->sending_buffer_->Initialized();
  Pthis->sendedblocks_ = 0;
  try {
    while (true) {
      pthread_testcancel();
      bool consumed = false;
      BlockContainer* block_for_sending = NULL;
      int partition_id =
          Pthis->sending_buffer_->getBlockForSending(block_for_sending);
      if (partition_id >= 0) {
        // get one block from sending_buffer which isn't empty
        pthread_testcancel();
        if (block_for_sending->GetRestSizeToHandle() > 0) {
          int recvbytes;
          recvbytes =
              send(Pthis->socket_fd_upper_list_[partition_id],
                   reinterpret_cast<char*>(block_for_sending->getBlock()) +
                       block_for_sending->GetCurSize(),
                   block_for_sending->GetRestSizeToHandle(), MSG_DONTWAIT);
          if (recvbytes == -1) {
            if (errno == EAGAIN) {
              continue;
            }
            LOG(ERROR) << "(exchange_id = " << Pthis->state_.exchange_id_
                       << " , partition_offset = "
                       << Pthis->state_.partition_offset_
                       << " ) sender send error: " << errno
                       << " fd = " << Pthis->socket_fd_upper_list_[partition_id]
                       << std::endl;
            break;
          } else {
            if (recvbytes < block_for_sending->GetRestSizeToHandle()) {
/* the block is not entirely sent. */
#ifdef GLOG_STATUS

              LOG(INFO)
                  << "(exchange_id = " << Pthis->state_.exchange_id_
                  << " , partition_offset = " << Pthis->state_.partition_offset_
                  << " ) doesn't send a block completely, actual send bytes = "
                  << recvbytes << " rest bytes = "
                  << block_for_sending->GetRestSizeToHandle() << std::endl;
#endif
              block_for_sending->IncreaseActualSize(recvbytes);
              continue;
            } else {
              /** the block is sent in entirety. **/
              block_for_sending->IncreaseActualSize(recvbytes);
              ++Pthis->sendedblocks_;
              /*
               can not be executed in case of abort in glog in this phase
               one of the following should be executed after rewriting
                Pthis->logging_->log(
                    "[%llu,%u]Send the %u block(bytes=%d, rest
                    size=%d) to [%d]",
                    Pthis->state_.exchange_id_,
                    Pthis->state_.partition_offset_,
                    Pthis->sendedblocks_, recvbytes,
                    block_for_sending_->GetRestSize(),
                    Pthis->state_.upper_id_list_[partition_id]);
              LOG(INFO) << "[ExchangeEagerLower]: "
                        << "[" << Pthis->state_.exchange_id_ << ","
                        << Pthis->state_.partition_offset_ << "]Send the "
                        << Pthis->sendedblocks_ << " block(bytes=" << recvbytes
                        << ", rest size=" <<
              block_for_sending->GetRestSizeToHandle()
                        << ") to ["
                        << Pthis->state_.upper_id_list_[partition_id] << "]"
                        << std::endl;
                            cout << "[ExchangeEagerLower]: " << "["
                                  << Pthis->state_.exchange_id_ << ","
                                  << Pthis->state_.partition_offset_ << "]Send
                                  the "
                                  << Pthis->sendedblocks_ << " block(bytes=" <<
                                  recvbytes
                                  << ", rest size=" <<
                                  block_for_sending_->GetRestSize()
                                  << ") to [" <<
                                  Pthis->state_.upper_id_list_[partition_id]
                                  << "]" << std::endl;
                              */
              consumed = true;
            }
          }
        } else {
          consumed = true;
        }
      } else {
        /* "partition_id<0" means that block_for_sending_ is empty, so we get
         * one block from the buffer into the block_for_sending_
         */
        unsigned index = Pthis->partitioned_data_buffer_->getBlock(
            *Pthis->block_for_sending_buffer_);
        Pthis->block_for_sending_buffer_->reset();
        Pthis->sending_buffer_->insert(index, Pthis->block_for_sending_buffer_);
      }
      if (consumed == true) {
        /* In the current loop, we have sent an entire block to the Receiver,
         * so we should get a new block into the block_for_sender_, but note
         * one empty block is also appended in partitioned_data_buffer_ in
         * next()
         */
        pthread_testcancel();
        if (Pthis->partitioned_data_buffer_->getBlock(
                *Pthis->block_for_sending_buffer_, partition_id)) {
          Pthis->block_for_sending_buffer_->reset();
          Pthis->sending_buffer_->insert(partition_id,
                                         Pthis->block_for_sending_buffer_);
        } else {
          /**
           * TODO: test the effort of the following sleeping statement and
           * consider whether it should be replaced by conditioned wait
           **/
          usleep(1);
        }
      }
    }
  } catch (std::exception& e) {
    pthread_cancel(pthread_self());
  }
}

void ExchangeSenderPipeline::CancelSenderThread() {
  if (0 != sender_thread_id_) {
    pthread_cancel(sender_thread_id_);
    void* res;
    pthread_join(sender_thread_id_, &res);
    if (res != PTHREAD_CANCELED)
      LOG(WARNING) << "(exchange_id = " << state_.exchange_id_
                   << " , partition_offset = " << state_.partition_offset_
                   << " ) thread is not canceled!" << std::endl;
    LOG(INFO) << "(exchange_id = " << state_.exchange_id_
              << " , partition_offset = " << state_.partition_offset_
              << " ) thread is canceled!" << std::endl;
    sender_thread_id_ = 0;
  }
}
RetCode ExchangeSenderPipeline::GetAllSegments(stack<Segment*>* all_segments) {
  RetCode ret = rSuccess;
  if (NULL != state_.child_) {
    return state_.child_->GetAllSegments(all_segments);
  }
  return ret;
}
}  // namespace physical_operator
}  // namespace claims
