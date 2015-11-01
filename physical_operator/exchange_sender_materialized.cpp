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
#include "../physical_operator/exchange_sender_materialized.h"
#include <malloc.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <glog/logging.h>
#include <stdio.h>
#include <stdlib.h>
#include <iosfwd>
#include <sstream>
#include <string>
#include "../configure.h"
#include "../common/rename.h"
#include "../common/Logging.h"
#include "../Environment.h"
#include "../Executor/exchange_tracker.h"
#include "../physical_operator/exchange_sender.h"
namespace claims {
namespace physical_operator {
ExchangeSenderMaterialized::ExchangeSenderMaterialized(State state)
    : state_(state), ExchangeSender() {}

ExchangeSenderMaterialized::~ExchangeSenderMaterialized() {}

ExchangeSenderMaterialized::ExchangeSenderMaterialized() {}
bool ExchangeSenderMaterialized::Open(const PartitionOffset&) {
  state_.child_->Open(state_.partition_offset_);

  /** get the number of mergers **/
  nuppers_ = state_.upper_id_list_.size();

  /** set the child exhausted bit **/
  child_exhausted_ = false;

  /** socket fd for mergers, each corresponding to one merger. **/
  socket_fd_upper_list_ = new int[nuppers_];

  /** fd for local partition on disk. **/
  disk_fd_list_ = new int[nuppers_];

  /** initialize a temporary structure that holds the space for calling child's
   * next() **/
  block_stream_for_asking_ =
      BlockStreamBase::createBlock(state_.schema_, state_.block_size_);

  /** initialize a block that is used for sending **/
  block_for_sending_ =
      new Block(block_stream_for_asking_->getSerializedBlockSize());

  /** initialize a block that is used as the intermediate structure for block
   * serialization**/
  block_for_serialization_ =
      new Block(block_stream_for_asking_->getSerializedBlockSize());

  /** initialize a partitioned block streams to accumulate the tuples obtained
   * from child iterator's next() **/
  partitioned_block_stream_ = new BlockStreamBase* [nuppers_];
  for (unsigned i = 0; i < nuppers_; i++) {
    partitioned_block_stream_[i] =
        BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
  }
  partitioned_data_buffer_ = new PartitionedBlockBuffer(
      nuppers_, block_stream_for_asking_->getSerializedBlockSize(), 1000);

  /** connect to the mergers **/
  for (unsigned upper_id = 0; upper_id < state_.upper_id_list_.size();
       upper_id++) {
    if (!ConnectToUpper(ExchangeID(state_.exchange_id_, upper_id),
                        state_.upper_id_list_[upper_id],
                        socket_fd_upper_list_[upper_id])) {
      return false;
    }
  }

  /** create the Sender thread **/
  CreateWorkerThread();

  return true;
}
bool ExchangeSenderMaterialized::Next(BlockStreamBase* no_block) {
  void* tuple_from_child;
  void* tuple_in_cur_block_stream;
  while (true) {
    block_stream_for_asking_->setEmpty();
    if (state_.child_->Next(block_stream_for_asking_)) {
      /** a new block is obtained from child iterator **/
      if (state_.partition_schema_.isHashPartition()) {
        BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator =
            block_stream_for_asking_->createIterator();
        while ((tuple_from_child = traverse_iterator->nextTuple()) > 0) {
          /** for each tuple in the newly obtained block, insert the tuple to
           * one partitioned block according to the
           * partition hash value**/
          const unsigned partition_id = GetHashPartitionId(
              tuple_from_child, state_.schema_,
              state_.partition_schema_.partition_key_index, nuppers_);

          /** calculate the tuple size for the current tuple **/
          const unsigned bytes =
              state_.schema_->getTupleActualSize(tuple_from_child);

          /** insert the tuple into the corresponding partitioned block **/
          while (!(tuple_in_cur_block_stream =
                       partitioned_block_stream_[partition_id]->allocateTuple(
                           bytes))) {
            /** if the destination block is full, we insert the block into the
             * buffer **/

            partitioned_block_stream_[partition_id]->serialize(
                *block_for_serialization_);
            partitioned_data_buffer_->insertBlockToPartitionedList(
                block_for_serialization_, partition_id);
            partitioned_block_stream_[partition_id]->setEmpty();
          }
          /** thread arriving here means that the space for the tuple is
           * successfully allocated, so we copy the tuple **/
          state_.schema_->copyTuple(tuple_from_child,
                                    tuple_in_cur_block_stream);
        }
      } else if (state_.partition_schema_.isBoardcastPartition()) {
        block_stream_for_asking_->serialize(*block_for_serialization_);
        for (unsigned i = 0; i < nuppers_; i++) {
          partitioned_data_buffer_->insertBlockToPartitionedList(
              block_for_serialization_, i);
        }
      }
    } else {
      /* the child iterator is exhausted. We add the remaining data in
       * partitioned data blocks into the buffer*/
      for (unsigned i = 0; i < nuppers_; i++) {
        Logging_ExpandableBlockStreamExchangeLM("||||||Fold the last||||||!");
        partitioned_block_stream_[i]->serialize(*block_for_serialization_);
        partitioned_data_buffer_->insertBlockToPartitionedList(
            block_for_serialization_, i);
        /* The following lines send an empty block to the upper, indicating that
         * all
                                         * the data from current sent has been
         * transmit to the uppers.
                                         */
        if (!partitioned_block_stream_[i]->Empty()) {
          partitioned_block_stream_[i]->setEmpty();
          partitioned_block_stream_[i]->serialize(*block_for_serialization_);
          partitioned_data_buffer_->insertBlockToPartitionedList(
              block_for_serialization_, i);
        }
      }
      /*
       * waiting until all the block in the buffer has been
       * transformed to the uppers.
       */
      LOG(INFO) << "Waiting until all the blocks in the buffer is sent!"
                << std::endl;
      child_exhausted_ = true;

      while (!partitioned_data_buffer_->isEmpty()) {
        usleep(1);
      }
      /*
       * waiting until all the uppers send the close notification which means
       * that blocks in the uppers' socket buffer have all been
       * consumed.
       */
      LOG(INFO) << "Waiting for close notification!" << std::endl;

      for (unsigned i = 0; i < nuppers_; i++) {
        WaitingForCloseNotification(socket_fd_upper_list_[i]);
      }
      return false;
    }
  }
}

bool ExchangeSenderMaterialized::Close() {
  Logging_ExpandableBlockStreamExchangeLM(
      "The sender thread is killed in the close() function!");

  /* close the files*/
  CloseDiskFiles();
  /* Delete the files */
  DeleteDiskFiles();

  state_.child_->Close();

  delete block_stream_for_asking_;
  delete block_for_sending_;
  delete block_for_serialization_;
  for (unsigned i = 0; i < nuppers_; i++) {
    delete partitioned_block_stream_[i];
  }
  delete partitioned_data_buffer_;
  delete[] partitioned_block_stream_;
  delete[] socket_fd_upper_list_;

  return true;
}
int ExchangeSenderMaterialized::GetBlockFromFile(Block& block) {
  unsigned seed = rand() % nuppers_;
  for (unsigned i = 0; i < nuppers_; i++) {
    const int read_size =
        read(disk_fd_list_[seed], block.getBlock(), block.getsize());
    if (read_size == -1) {
      LOG(ERROR) << "Error occurs when read file. Reason: " << strerror(errno)
                 << std::endl;
    } else if (read_size == 0) {
      seed = (seed + 1) % nuppers_;
      continue;
    } else if (!(read_size == block.getsize())) {
      LOG(ERROR) << "read_size= " << read_size
                 << " ,  block.getsize()= " << block.getsize();
      assert(false);
    }
    return seed;
  }
  return -1;
}
void ExchangeSenderMaterialized::Send() {
  /* open the file*/
  LOG(INFO) << "open the file!" << std::endl;
  for (unsigned i = 0; i < nuppers_; i++) {
    disk_fd_list_[i] = FileOpen(GetPartititionedFileName(i).c_str(), O_RDONLY);
    if (disk_fd_list_[i] == -1) {
      LOG(ERROR) << "Cannot open file " << GetPartititionedFileName(i).c_str()
                 << "reason: " << strerror(errno) << std::endl;
      return;
    }
    /** set the read cursor at the beginner **/
  }
  LOG(INFO) << "the file is opened!" << std::endl;
  while (true) {
    int partition_id = GetBlockFromFile(*block_for_sending_);
    if (partition_id < 0) {
      break;
    }
    int sendtotal = 0;
    while (sendtotal < block_for_sending_->getsize()) {
      int recvbytes;
      if ((recvbytes = send(
               socket_fd_upper_list_[partition_id],
               reinterpret_cast<char*>(block_for_sending_->getBlock()) +
                   sendtotal,
               block_for_sending_->getsize() - sendtotal, MSG_WAITALL)) == -1) {
        LOG(ERROR) << "receive error!" << std::endl;
        return;
      }
      sendtotal += recvbytes;
    }
    LOG(INFO) << "Waiting the connection notification from "
              << state_.upper_id_list_[partition_id] << std::endl;
    LOG(INFO) << "The block is received the upper "
              << state_.upper_id_list_[partition_id] << std::endl;
  }
}
bool ExchangeSenderMaterialized::Materialize() {
  /* create the file*/
  for (unsigned i = 0; i < nuppers_; i++) {
    disk_fd_list_[i] =
        FileOpen(GetPartititionedFileName(i).c_str(),
                 O_RDWR | O_APPEND | O_TRUNC | O_CREAT, S_IWUSR | S_IRUSR);
    if (disk_fd_list_[i] == -1) {
      LOG(ERROR) << "Cannot open file " << GetPartititionedFileName(i).c_str()
                 << "reason: " << strerror(errno);
      printf(
          "Hint: the most possible problem is that you do not have directory "
          "/tmp/claims/exchange/. Please create the directory and retry.\n");
      return false;
    }
  }
  LOG(INFO) << "The intermediate file is created!\n";
  Block* block_for_storing =
      new Block(block_stream_for_asking_->getSerializedBlockSize());
  int counter = 0;
  while (!child_exhausted_) {
    if (partitioned_data_buffer_->getBlockInBuffer() > 0) {
      const unsigned partition_id =
          partitioned_data_buffer_->getBlock(*block_for_storing);
      const unsigned write_bytes =
          write(disk_fd_list_[partition_id], block_for_storing->getBlock(),
                block_for_storing->getsize());
      if (write_bytes != block_for_storing->getsize()) {
        LOG(ERROR)
            << "Error occurs when writing the block into the disk, reason: "
            << strerror(errno) << std::endl;
      }
    }
  }

  /*send the remaining block in the buffer*/
  while (partitioned_data_buffer_->getBlockInBuffer() > 0) {
    const unsigned partition_id =
        partitioned_data_buffer_->getBlock(*block_for_storing);
    const unsigned write_bytes =
        write(disk_fd_list_[partition_id], block_for_storing->getBlock(),
              block_for_storing->getsize());
    if (write_bytes != block_for_storing->getsize()) {
      LOG(ERROR)
          << "Error occurs when writing the block into the disk, reason: "
          << strerror(errno) << std::endl;
    }
  }

  /* the child iterator is exhausted and the all the block in the
   * buffer have been store in the file.*/
  LOG(INFO) << "closing the local file!" << std::endl;

  /* close the file*/
  CloseDiskFiles();
  LOG(INFO) << "the local files are closed!" << std::endl;
  return true;
}
void* ExchangeSenderMaterialized::MaterializeAndSend(void* arg) {
  ExchangeSenderMaterialized* Pthis =
      reinterpret_cast<ExchangeSenderMaterialized*>(arg);
  if (!Pthis->Materialize()) {
    LOG(ERROR) << "Error occurs when materializing the intermediate result!\n";
  }
  LOG(INFO) << "The intermediate is completely stored in the disk!\n";
  Pthis->Send();
  return 0;
}
void* ExchangeSenderMaterialized::debug(void* arg) {
  ExchangeSenderMaterialized* Pthis =
      reinterpret_cast<ExchangeSenderMaterialized*>(arg);
  while (true) {
    printf("%d blocks in buffer.\n",
           Pthis->partitioned_data_buffer_->getBlockInBuffer());
    usleep(100000);
  }
}

bool ExchangeSenderMaterialized::CreateWorkerThread() {
  if (true == g_thread_pool_used) {
    Environment::getInstance()->getThreadPool()->add_task(MaterializeAndSend,
                                                          this);
  } else {
    int error;
    error = pthread_create(&sender_thread_id_, NULL, MaterializeAndSend, this);
    if (error != 0) {
      LOG(ERROR) << "Failed to create the sender thread." << std::endl;
      return false;
    }
  }
  return true;
}

void ExchangeSenderMaterialized::CancelWorkerThread() {
  if (false == g_thread_pool_used) {
    pthread_cancel(sender_thread_id_);
    void* res;
    pthread_join(sender_thread_id_, &res);
    if (res != PTHREAD_CANCELED || res != 0)
      LOG(WARNING) << "thread is not canceled!\n";
    sender_thread_id_ = 0;
  }
}

void ExchangeSenderMaterialized::CloseDiskFiles() {
  for (unsigned i = 0; i < nuppers_; i++) {
    FileClose(disk_fd_list_[i]);
  }
}

void ExchangeSenderMaterialized::DeleteDiskFiles() {
  for (unsigned i = 0; i < nuppers_; i++) {
    unlink(GetPartititionedFileName(i).c_str());
  }
}

std::string ExchangeSenderMaterialized::GetPartititionedFileName(
    int partition_index) const {
  std::string temp_file_dir = "/home/imdb/exchange_for_claims/";
  std::ostringstream file_name;
  file_name << temp_file_dir << "exchange_" << state_.exchange_id_ << "_"
            << state_.partition_offset_ << "_" << partition_index;
  return file_name.str();
}
}  // namespace physical_operator
}  // namespace claims
