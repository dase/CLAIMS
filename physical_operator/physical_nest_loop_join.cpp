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
 * /physical_operator/physical_nest_loop_join.h
 *
 *  Created on: Jul 14, 2014
 *      Author: minqi, imdb
 *       Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description:
 *
 */

#include "../physical_operator/physical_nest_loop_join.h"

#include "../Executor/expander_tracker.h"
#include "../common/Block/BlockStream.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "../common/log/logging.h"

namespace claims {
namespace physical_operator {

PhysicalNestLoopJoin::PhysicalNestLoopJoin() : PhysicalOperator(2, 1) {
  InitExpandedStatus();
}

PhysicalNestLoopJoin::~PhysicalNestLoopJoin() {
  // TODO Auto-generated destructor stub
}
PhysicalNestLoopJoin::PhysicalNestLoopJoin(State state)
    : state_(state), PhysicalOperator(2, 1) {
  InitExpandedStatus();
}
PhysicalNestLoopJoin::State::State(PhysicalOperatorBase *child_left,
                                   PhysicalOperatorBase *child_right,
                                   Schema *input_schema_left,
                                   Schema *input_schema_right,
                                   Schema *output_schema, unsigned block_size)
    : child_left_(child_left),
      child_right_(child_right),
      input_schema_left_(input_schema_left),
      input_schema_right_(input_schema_right),
      output_schema_(output_schema),
      block_size_(block_size) {}

/**
 * @brief  Method description : describe the open method which gets results from
 * the left child and copy them into its local buffer, say the block buffer. the
 * block buffer is a dynamic block buffer since all the expanded threads will
 * share the same block buffer.
 */
bool PhysicalNestLoopJoin::Open(const PartitionOffset &partition_offset) {
  RegisterExpandedThreadToAllBarriers();
  unsigned long long int timer;
  bool winning_thread = false;
  if (TryEntryIntoSerializedSection(0)) {  // the first thread of all need to do
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_desc, "nest loop", 0));
    winning_thread = true;
    timer = curtick();
    block_buffer_ = new DynamicBlockBuffer();
    LOG(INFO) << "[NestloopJoin]: "
              << "["
              << "the first thread opens the nestloopJoin physical operator"
              << "]" << std::endl;
  }
  state_.child_left_->Open(partition_offset);
  BarrierArrive(0);
  NestLoopJoinContext *jtc = new NestLoopJoinContext();
  // create a new block to hold the results from the left child
  // and add results to the dynamic buffer
  CreateBlockStream(jtc->block_for_asking_, state_.input_schema_left_);
  while (state_.child_left_->Next(jtc->block_for_asking_)) {
    block_buffer_->atomicAppendNewBlock(jtc->block_for_asking_);
    CreateBlockStream(jtc->block_for_asking_, state_.input_schema_left_);
  }
  // the last block is created without storing the results from the left child
  if (NULL != jtc->block_for_asking_) {
    delete jtc->block_for_asking_;
    jtc->block_for_asking_ = NULL;
  }
  // when the finished expanded thread finished its allocated work, it can be
  // called back here. What should be noticed that the callback meas the to
  // exit
  // on the of the thread
  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(1);
    LOG(INFO) << "[NestloopJoin]: "
              << "["
              << "the" << pthread_self() << "the thread is called to exit"
              << "]" << std::endl;
    return true;  // the
  }
  BarrierArrive(1);  // ??ERROR
                     //	join_thread_context* jtc=new join_thread_context();
  CreateBlockStream(jtc->block_for_asking_, state_.input_schema_right_);
  jtc->block_stream_iterator_ = jtc->block_for_asking_->createIterator();
  InitContext(jtc);  // rename this function, here means to store the thread
                     // context in the operator context
  state_.child_right_->Open(partition_offset);
  return true;
}

bool PhysicalNestLoopJoin::Next(BlockStreamBase *block) {
  /**
   * @brief it describes the sequence of the nestloop join. As the intermediate
   * result of the left child has been stored in the dynamic block buffer in the
   * open function. in this next function, it get the intermediate result of the
   * right child operator, one block after one block. Within each block, it gets
   * each tuple in the block and joins with each tuple in the dynamic block
   * buffer
   * when traversing them.
   * Method description :
   * @param
   * @ return
   * @details   (additional)
   */
  void *tuple_from_buffer_child;
  void *tuple_from_right_child;
  void *result_tuple;
  BlockStreamBase *buffer_block = NULL;
  NestLoopJoinContext *jtc =
      reinterpret_cast<NestLoopJoinContext *>(GetContext());
  while (1) {
    while ((tuple_from_right_child =
                jtc->block_stream_iterator_->currentTuple()) > 0) {
      jtc->buffer_iterator_ = block_buffer_->createIterator();
      while ((buffer_block = jtc->buffer_iterator_.nextBlock()) > 0) {
        jtc->buffer_stream_iterator_->~BlockStreamTraverseIterator();
        jtc->buffer_stream_iterator_ = buffer_block->createIterator();
        while ((tuple_from_buffer_child =
                    jtc->buffer_stream_iterator_->currentTuple()) > 0) {
          if ((result_tuple = block->allocateTuple(
                   state_.output_schema_->getTupleMaxSize())) > 0) {
            const unsigned copyed_bytes = state_.input_schema_left_->copyTuple(
                tuple_from_buffer_child, result_tuple);
            state_.input_schema_right_->copyTuple(
                tuple_from_right_child,
                reinterpret_cast<char *>(result_tuple + copyed_bytes));
          } else {
            LOG(INFO) << "[NestloopJoin]: "
                      << "["
                      << "a block of the result is full of the nest loop "
                         "join result"
                      << "]" << std::endl;
            return true;
          }
          jtc->buffer_stream_iterator_->increase_cur_();
        }
      }
      jtc->block_stream_iterator_->increase_cur_();
    }
    jtc->block_for_asking_->setEmpty();
    if (false == state_.child_right_->Next(jtc->block_for_asking_)) {
      if (true == block->Empty()) {
        LOG(WARNING) << "[NestloopJoin]: "
                     << "["
                     << "no join result is stored in the block after traverse "
                        "the right child operator"
                     << "]" << std::endl;
        return false;
      } else {
        return true;
      }
    }
    jtc->block_stream_iterator_->~BlockStreamTraverseIterator();
    jtc->block_stream_iterator_ = jtc->block_for_asking_->createIterator();
  }
  return Next(block);
}
bool PhysicalNestLoopJoin::Close() {
  InitExpandedStatus();
  DestoryAllContext();
  if (NULL != block_buffer_) {
    delete block_buffer_;
    block_buffer_ = NULL;
  }
  state_.child_left_->Close();
  state_.child_right_->Close();
  return true;
}

/**
 * @brief  Method description : create a block buffer based on the given left
 * or right input schema
 * @param  target : the dynamic buffer to be created based on the given buffer
 * @param schema: the left or the right input schema
  */
bool PhysicalNestLoopJoin::CreateBlockStream(BlockStreamBase *&target,
                                             Schema *&schema) const {
  target = BlockStreamBase::createBlock(schema, state_.block_size_);
  if (NULL == target) {
    assert(false);
    return false;
  } else {
    return true;
  }
}

void PhysicalNestLoopJoin::Print() {
  printf("NestLoopJoin\n");
  printf("------Join Left-------\n");
  state_.child_left_->Print();
  printf("------Join Right-------\n");
  state_.child_right_->Print();
}

}  // namespace physical_operator
}  // namespace claims
