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

#include <vector>
#include <stack>
#include "../physical_operator/physical_nest_loop_join.h"
#include "../Executor/expander_tracker.h"
#include "../common/Block/BlockStream.h"
#include "../physical_operator/physical_operator_base.h"
#include "../common/expression/expr_node.h"
#include "../common/memory_handle.h"
using claims::common::ExprNode;
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "../common/log/logging.h"

namespace claims {
namespace physical_operator {

PhysicalNestLoopJoin::PhysicalNestLoopJoin()
    : PhysicalOperator(2, 2), block_buffer_(NULL), join_condi_process_(NULL) {
  set_phy_oper_type(kPhysicalNestLoopJoin);
  InitExpandedStatus();
}

PhysicalNestLoopJoin::~PhysicalNestLoopJoin() {
  DELETE_PTR(state_.child_left_);
  DELETE_PTR(state_.child_right_);
  //  DELETE_PTR(state_.input_schema_left_);
  //  DELETE_PTR(state_.input_schema_right_);
  //  for (int i = 0; i < state_.join_condi_.size(); ++i) {
  //    DELETE_PTR(state_.join_condi_[i]);
  //  }
  //  state_.join_condi_.clear();
}
PhysicalNestLoopJoin::PhysicalNestLoopJoin(State state)
    : PhysicalOperator(2, 2),
      state_(state),
      block_buffer_(NULL),
      join_condi_process_(NULL) {
  set_phy_oper_type(kPhysicalNestLoopJoin);
  InitExpandedStatus();
}
PhysicalNestLoopJoin::State::State(PhysicalOperatorBase *child_left,
                                   PhysicalOperatorBase *child_right,
                                   Schema *input_schema_left,
                                   Schema *input_schema_right,
                                   Schema *output_schema, unsigned block_size,
                                   std::vector<ExprNode *> join_condi)
    : child_left_(child_left),
      child_right_(child_right),
      input_schema_left_(input_schema_left),
      input_schema_right_(input_schema_right),
      output_schema_(output_schema),
      block_size_(block_size),
      join_condi_(join_condi) {}

/**
 * @brief  Method description : describe the open method which gets results from
 * the left child and copy them into its local buffer, say the block buffer. the
 * block buffer is a dynamic block buffer since all the expanded threads will
 * share the same block buffer.
 */
bool PhysicalNestLoopJoin::Open(SegmentExecStatus *const exec_status,
                                const PartitionOffset &partition_offset) {
  RETURN_IF_CANCELLED(exec_status);

  RegisterExpandedThreadToAllBarriers();
  unsigned long long int timer;
  bool winning_thread = false;
  if (TryEntryIntoSerializedSection(0)) {  // the first thread of all need to do
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_desc, "nest loop", 0));
    winning_thread = true;
    timer = curtick();
    block_buffer_ = new DynamicBlockBuffer();
    if (state_.join_condi_.size() == 0) {
      join_condi_process_ = WithoutJoinCondi;
    } else {
      join_condi_process_ = WithJoinCondi;
    }
    LOG(INFO) << "[NestloopJoin]: [the first thread opens the nestloopJoin "
                 "physical operator]" << std::endl;
  }
  RETURN_IF_CANCELLED(exec_status);

  state_.child_left_->Open(exec_status, partition_offset);
  RETURN_IF_CANCELLED(exec_status);

  BarrierArrive(0);

  NestLoopJoinContext *jtc = CreateOrReuseContext(crm_numa_sensitive);
  // create a new block to hold the results from the left child
  // and add results to the dynamic buffer
  //  jtc->block_for_asking_ == BlockStreamBase::createBlock(
  //                                state_.input_schema_left_,
  //                                state_.block_size_);
  CreateBlockStream(jtc->block_for_asking_, state_.input_schema_left_);
  //  auto temp = jtc->block_for_asking_->getBlock();
  //  cout << "temp start" << temp << endl;
  //
  //  cout << "init block_for_asking_ : " << jtc->block_for_asking_->getBlock()
  //       << " is reference : " << jtc->block_for_asking_->isIsReference() <<
  //       endl;
  while (state_.child_left_->Next(exec_status, jtc->block_for_asking_)) {
    if (exec_status->is_cancelled()) {
      if (NULL != jtc->block_for_asking_) {
        delete jtc->block_for_asking_;
        jtc->block_for_asking_ = NULL;
      }
      return false;
    }
    //    cout << "after assgin start :" << jtc->block_for_asking_->getBlock()
    //         << " is reference : " << jtc->block_for_asking_->isIsReference()
    //         << endl;
    block_buffer_->atomicAppendNewBlock(jtc->block_for_asking_);
    //    if (!jtc->block_for_asking_->isIsReference()) {
    CreateBlockStream(jtc->block_for_asking_, state_.input_schema_left_);
    //    } else {
    //      //      cout << "temp after" << temp << endl;
    //      //      delete temp;
    //      CreateBlockStream(jtc->block_for_asking_,
    //      state_.input_schema_left_);
    //      jtc->block_for_asking_->setIsReference(false);
    //    }
    //    cout << "new start :" << jtc->block_for_asking_->getBlock()
    //         << " is reference : " << jtc->block_for_asking_->isIsReference()
    //         << endl;
  }
  //  cout << "buffer_size_ : " << block_buffer_->GetBufferSize() << endl;
  //  the last block is created without storing the results from the left
  // child

  if (NULL != jtc->block_for_asking_) {
    delete jtc->block_for_asking_;
    jtc->block_for_asking_ = NULL;
  }
  // when the finished expanded thread finished its allocated work, it can be
  // called back here. What should be noticed that the callback meas the to
  // exit on the of the thread
  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(1);
    LOG(INFO) << "[NestloopJoin]: [the" << pthread_self()
              << "the thread is called to exit]" << std::endl;
    return true;  // the
  }
  BarrierArrive(1);  // ??ERROR
                     //	join_thread_context* jtc=new join_thread_context();
                     //  jtc->block_for_asking_ == BlockStreamBase::createBlock(
  //                                state_.input_schema_right_,
  //                                state_.block_size_);
  CreateBlockStream(jtc->block_for_asking_, state_.input_schema_right_);
  jtc->block_for_asking_->setEmpty();
  jtc->block_stream_iterator_ = jtc->block_for_asking_->createIterator();
  jtc->buffer_iterator_ = block_buffer_->createIterator();

  // underlying bug: as for buffer_iterator may be NULL, it's necessary to let
  // every buffer_iterator of each thread point to an empty block
  // jtc->buffer_stream_iterator_ =
  //    jtc->buffer_iterator_.nextBlock()->createIterator();

  InitContext(jtc);  // rename this function, here means to store the thread
                     // context in the operator context
  RETURN_IF_CANCELLED(exec_status);
  state_.child_right_->Open(exec_status, partition_offset);
  LOG(INFO) << "Nest_loop open() finish" << endl;
  return true;
}

bool PhysicalNestLoopJoin::Next(SegmentExecStatus *const exec_status,
                                BlockStreamBase *block) {
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
  RETURN_IF_CANCELLED(exec_status);

  void *tuple_from_buffer_child = NULL;
  void *tuple_from_right_child = NULL;
  void *result_tuple = NULL;
  bool pass = false;
  BlockStreamBase *buffer_block = NULL;
  NestLoopJoinContext *jtc =
      reinterpret_cast<NestLoopJoinContext *>(GetContext());
  while (1) {
    RETURN_IF_CANCELLED(exec_status);

    while (NULL != (tuple_from_right_child =
                        jtc->block_stream_iterator_->currentTuple())) {
      while (1) {
        while (NULL != (tuple_from_buffer_child =
                            jtc->buffer_stream_iterator_->currentTuple())) {
          pass = join_condi_process_(tuple_from_buffer_child,
                                     tuple_from_right_child, jtc);
          if (pass) {
            if (NULL != (result_tuple = block->allocateTuple(
                             state_.output_schema_->getTupleMaxSize()))) {
              const unsigned copyed_bytes =
                  state_.input_schema_left_->copyTuple(tuple_from_buffer_child,
                                                       result_tuple);
              state_.input_schema_right_->copyTuple(
                  tuple_from_right_child,
                  reinterpret_cast<char *>(result_tuple + copyed_bytes));
            } else {
              //            LOG(INFO) << "[NestloopJoin]:  [a block of the
              //            result
              //            is full of "
              //                         "the nest loop join result ]" <<
              //                         std::endl;
              return true;
            }
          }
          jtc->buffer_stream_iterator_->increase_cur_();
        }

        //        jtc->buffer_stream_iterator_->~BlockStreamTraverseIterator();
        if (jtc->buffer_stream_iterator_ != NULL) {
          delete jtc->buffer_stream_iterator_;
          jtc->buffer_stream_iterator_ = NULL;
        }
        if (NULL != (buffer_block = jtc->buffer_iterator_.nextBlock())) {
          jtc->buffer_stream_iterator_ = buffer_block->createIterator();
        } else {
          break;
        }
      }

      jtc->buffer_iterator_.ResetCur();
      if (NULL == (buffer_block = jtc->buffer_iterator_.nextBlock())) {
        LOG(ERROR) << "[NestloopJoin]: this block shouldn't be NULL in nest "
                      "loop join!";
        assert(
            false &&
            "[NestloopJoin]: this block shouldn't be NULL in nest loop join!");
      }
      if (jtc->buffer_stream_iterator_ != NULL) {
        delete jtc->buffer_stream_iterator_;
        jtc->buffer_stream_iterator_ = NULL;
      }
      jtc->buffer_stream_iterator_ = buffer_block->createIterator();
      jtc->block_stream_iterator_->increase_cur_();
    }

    // if buffer is empty, return false directly
    jtc->buffer_iterator_.ResetCur();
    if (NULL == (buffer_block = jtc->buffer_iterator_.nextBlock())) {
      LOG(WARNING) << "[NestloopJoin]: the buffer is empty in nest loop join!";
      // for getting all right child's data
      jtc->block_for_asking_->setEmpty();
      while (state_.child_right_->Next(exec_status, jtc->block_for_asking_)) {
        jtc->block_for_asking_->setEmpty();
      }
      return false;
    }
    if (jtc->buffer_stream_iterator_ != NULL) {
      delete jtc->buffer_stream_iterator_;
      jtc->buffer_stream_iterator_ = NULL;
    }
    jtc->buffer_stream_iterator_ = buffer_block->createIterator();

    // ask block from right child
    jtc->block_for_asking_->setEmpty();
    if (false ==
        state_.child_right_->Next(exec_status, jtc->block_for_asking_)) {
      if (true == block->Empty()) {
        LOG(WARNING) << "[NestloopJoin]: [no join result is stored in the "
                        "block after traverse the right child operator]"
                     << std::endl;
        return false;
      } else {
        LOG(INFO) << "[NestloopJoin]: get a new block from right child "
                  << std::endl;
        return true;
      }
    }
    if (jtc->block_stream_iterator_ != NULL) {
      delete jtc->block_stream_iterator_;
      jtc->block_stream_iterator_ = NULL;
    }
    jtc->block_stream_iterator_ = jtc->block_for_asking_->createIterator();
  }
  return Next(exec_status, block);
}
bool PhysicalNestLoopJoin::Close(SegmentExecStatus *const exec_status) {
  InitExpandedStatus();
  DestoryAllContext();
  DELETE_PTR(block_buffer_);
  state_.child_left_->Close(exec_status);
  state_.child_right_->Close(exec_status);
  return true;
}
bool PhysicalNestLoopJoin::WithJoinCondi(void *tuple_left, void *tuple_right,
                                         NestLoopJoinContext *const nljcnxt) {
  nljcnxt->expr_eval_cnxt_.tuple[0] = tuple_left;
  nljcnxt->expr_eval_cnxt_.tuple[1] = tuple_right;
  bool pass = false;
  for (int i = 0; i < nljcnxt->join_condi_.size(); ++i) {
    pass = *(bool *)(nljcnxt->join_condi_[i]->ExprEvaluate(
        nljcnxt->expr_eval_cnxt_));
    if (pass == false) {
      return false;
    }
  }
  return true;
}
bool PhysicalNestLoopJoin::WithoutJoinCondi(
    void *tuple_left, void *tuple_right, NestLoopJoinContext *const nljcnxt) {
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
  //  if (target->isIsReference()) {
  //    target = BlockStreamBase::createBlock2(schema, state_.block_size_);
  //  } else {
  target = BlockStreamBase::createBlock(schema, state_.block_size_);
  //  }
  if (NULL == target) {
    assert(false);
    return false;
  } else {
    return true;
  }
}
PhysicalNestLoopJoin::NestLoopJoinContext::NestLoopJoinContext(
    const vector<ExprNode *> &join_condi, const Schema *left_schema,
    const Schema *right_schema)
    : block_for_asking_(NULL),
      block_stream_iterator_(NULL),
      buffer_stream_iterator_(NULL) {
  ExprNode *new_node = NULL;
  for (int i = 0; i < join_condi.size(); ++i) {
    new_node = join_condi[i]->ExprCopy();
    new_node->InitExprAtPhysicalPlan();
    join_condi_.push_back(new_node);
  }
  expr_eval_cnxt_.schema[0] = left_schema;
  expr_eval_cnxt_.schema[1] = right_schema;
}
PhysicalNestLoopJoin::NestLoopJoinContext::~NestLoopJoinContext() {
  DELETE_PTR(block_for_asking_);
  DELETE_PTR(block_stream_iterator_);
  DELETE_PTR(buffer_stream_iterator_);
  for (int i = 0; i < join_condi_.size(); ++i) {
    DELETE_PTR(join_condi_[i]);
  }
  join_condi_.clear();
}
ThreadContext *PhysicalNestLoopJoin::CreateContext() {
  NestLoopJoinContext *jtc =
      new NestLoopJoinContext(state_.join_condi_, state_.input_schema_left_,
                              state_.input_schema_right_);
  return jtc;
}
void PhysicalNestLoopJoin::Print() {
  printf("NestLoopJoin\n");
  printf("------Join Left-------\n");
  state_.child_left_->Print();
  printf("------Join Right-------\n");
  state_.child_right_->Print();
}
RetCode PhysicalNestLoopJoin::GetAllSegments(stack<Segment *> *all_segments) {
  RetCode ret = rSuccess;
  if (NULL != state_.child_right_) {
    ret = state_.child_right_->GetAllSegments(all_segments);
  }
  if (NULL != state_.child_left_) {
    ret = state_.child_left_->GetAllSegments(all_segments);
  }
  return ret;
}

}  // namespace physical_operator
}  // namespace claims
