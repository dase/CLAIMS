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
 * /CLAIMS/physical_operator/physical_sort.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: zhanglei
 *       Email:
 *
 *  Modified on: Oct 20, 2015
 *       Author: tonglanxuan
 *        Email: lxtong0526@163.com
 *
 * Description: Detailedly describes how the PhysicalSort class functions.
 *
 */

#include "../physical_operator/physical_sort.h"
#include <glog/logging.h>
#include <stack>
#include <utility>
#include <vector>

#include "../common/expression/data_type_oper.h"
#include "../common/expression/expr_node.h"

using claims::common::DataTypeOper;
using claims::common::ExprNode;
using claims::common::OperFuncInfoData;
using claims::common::OperType;
using std::vector;
using std::pair;
namespace claims {
namespace physical_operator {
unsigned PhysicalSort::order_by_pos_ = 0;
PhysicalSort::State *PhysicalSort::cmp_state_ = NULL;
OperFuncInfo PhysicalSort::fcinfo = NULL;
PhysicalSort::PhysicalSort() : PhysicalOperator(3, 2), block_buffer_(NULL) {
  set_phy_oper_type(kPhysicalSort);
  lock_ = new Lock();
  InitExpandedStatus();
}

PhysicalSort::PhysicalSort(State state)
    : PhysicalOperator(3, 2), state_(state), block_buffer_(NULL) {
  set_phy_oper_type(kPhysicalSort);
  cmp_state_ = &state_;
  lock_ = new Lock();
  InitExpandedStatus();
}

PhysicalSort::~PhysicalSort() {
  if (NULL != lock_) {
    delete lock_;
    lock_ = NULL;
  }
}

PhysicalSort::State::State()
    : input_schema_(0), child_(0), block_size_(0), partition_offset_(0) {}

PhysicalSort::State::State(Schema *input_schema, PhysicalOperatorBase *child,
                           const unsigned block_size,
                           vector<pair<ExprNode *, int>> order_by_attrs,
                           const PartitionOffset partition_offset)
    : input_schema_(input_schema),
      child_(child),
      block_size_(block_size),
      order_by_attrs_(order_by_attrs),
      order_by_attrs_copy_(order_by_attrs),
      partition_offset_(partition_offset) {}
// TODO(FZH): every time compare 2 tuples, it should be calculated, it may be
// calculated before there and fetch the result straightly here.
bool PhysicalSort::Compare(void *a_tuple, void *b_tuple) {
  cmp_state_->eecnxt_.tuple[0] = a_tuple;
  cmp_state_->eecnxt1_.tuple[0] = b_tuple;
  void *a_result =
      cmp_state_->order_by_attrs_[order_by_pos_].first->ExprEvaluate(
          cmp_state_->eecnxt_);
  void *b_result =
      cmp_state_->order_by_attrs_copy_[order_by_pos_].first->ExprEvaluate(
          cmp_state_->eecnxt1_);
  fcinfo->args_[0] = a_result;
  fcinfo->args_[1] = b_result;
  fcinfo->args_num_ = 2;
  bool cmp_result_ = false;
  fcinfo->result_ = &cmp_result_;
  if (cmp_state_->order_by_attrs_[order_by_pos_].second) {
    // for descending order and preserving order, it should return false, so
    // return a_result > b_result;
    cmp_state_->compare_funcs_[order_by_pos_][1](fcinfo);
    //    DataTypeOper::data_type_oper_func_
    //        [cmp_state_->order_by_attrs_[order_by_pos_]
    //             .first->get_type_][OperType::oper_great](fcinfo);
    return cmp_result_;
  } else {
    // for ascending order and preserving order, it should return false, so
    // return a_result < b_result;
    cmp_state_->compare_funcs_[order_by_pos_][0](fcinfo);
    //    DataTypeOper::data_type_oper_func_
    //        [cmp_state_->order_by_attrs_[order_by_pos_]
    //             .first->get_type_][OperType::oper_less](fcinfo);
    return cmp_result_;
  }
}
/*  Sorts the elements in the range @p [__first,__last) in ascending order,
 *  such that for each iterator @p i in the range @p [__first,__last-1),
 *  @p __comp(*(i+1),*i) is false.
 *
 *  The relative ordering of equivalent elements is preserved, so any two
 *  elements @p x and @p y in the range @p [__first,__last) such that
 *  @p __comp(x,y) is false and @p __comp(y,x) is false will have the same
 *  relative ordering after calling @p stable_sort().
 */
void PhysicalSort::Order() {
  for (order_by_pos_ = 0; order_by_pos_ < state_.order_by_attrs_.size();
       ++order_by_pos_) {
    stable_sort(all_tuples_.begin(), all_tuples_.end(), Compare);
  }
}

/**
 *  first we can store all the data which will be bufferred
 * 1, buffer is the first phase. multi-threads will be applyed to the data
 *    in the buffer.
 * 2, sort the data in the buffer, we choose stable_sort() to sort the records
 *    by specifying the column to be sorted
 * 3, whether to register the buffer into the blockmanager.
 * */
bool PhysicalSort::Open(SegmentExecStatus *const exec_status,
                        const PartitionOffset &part_off) {
  RETURN_IF_CANCELLED(exec_status);

  RegisterExpandedThreadToAllBarriers();
  if (TryEntryIntoSerializedSection(0)) {
    all_cur_ = 0;
    thread_id_ = -1;
    all_tuples_.clear();
    block_buffer_ = new DynamicBlockBuffer();
  }
  BarrierArrive(0);
  BlockStreamBase *block_for_asking;
  if (CreateBlock(block_for_asking) == false) {
    LOG(ERROR) << "error in the create block stream!!!" << endl;
    return 0;
  }
  //  state_.partition_offset_ = part_off;
  state_.child_->Open(exec_status, part_off);
  RETURN_IF_CANCELLED(exec_status);

  /**
   *  phase 1: store the data in the buffer!
   *          by using multi-threads to speed up
   */
  vector<void *> thread_tuple;
  thread_tuple.clear();
  void *tuple_ptr = NULL;
  BlockStreamBase::BlockStreamTraverseIterator *block_it;

  while (state_.child_->Next(exec_status, block_for_asking)) {
    RETURN_IF_CANCELLED(exec_status);

    block_buffer_->atomicAppendNewBlock(block_for_asking);
    block_it = block_for_asking->createIterator();
    while (NULL != (tuple_ptr = block_it->nextTuple())) {
      thread_tuple.push_back(tuple_ptr);
    }
    if (NULL != block_it) {
      delete block_it;
      block_it = NULL;
    }
    if (CreateBlock(block_for_asking) == false) {
      LOG(ERROR) << "error in the create block stream!!!" << endl;
      return 0;
    }
  }

  if (NULL != block_for_asking) {
    delete block_for_asking;
    block_for_asking = NULL;
  }
  lock_->acquire();
  all_tuples_.insert(all_tuples_.end(), thread_tuple.begin(),
                     thread_tuple.end());
  lock_->release();
  thread_tuple.clear();

  // guarantee the block_buffer get all data blocks completely
  BarrierArrive(1);

  // phase 2: sort the data in the buffer, only just one thread!
  if (TryEntryIntoSerializedSection(1)) {
    // reverse the order of order_by_attrs for preserve The relative ordering of
    // equivalent elements
    reverse(state_.order_by_attrs_.begin(), state_.order_by_attrs_.end());
    // one expression for 2 tuples results in overwriting result, so copy the
    // expression for 2 different tuples calculating
    state_.order_by_attrs_copy_ = state_.order_by_attrs_;
    OperFuncInfoData oper_info;
    fcinfo = &oper_info;
    state_.compare_funcs_ =
        new DataTypeOperFunc[state_.order_by_attrs_.size()][2];
    for (int i = 0; i < state_.order_by_attrs_.size(); ++i) {
      state_.order_by_attrs_copy_[i].first =
          state_.order_by_attrs_[i].first->ExprCopy();  // deep copy
      state_.order_by_attrs_[i].first->InitExprAtPhysicalPlan();
      state_.order_by_attrs_copy_[i].first->InitExprAtPhysicalPlan();

      state_.compare_funcs_[i][0] = DataTypeOper::data_type_oper_func_
          [state_.order_by_attrs_[i].first->get_type_][OperType::oper_less];
      state_.compare_funcs_[i][1] = DataTypeOper::data_type_oper_func_
          [state_.order_by_attrs_[i].first->get_type_][OperType::oper_great];
    }
    //    int64_t time = curtick();
    state_.eecnxt_.schema[0] = state_.input_schema_;
    state_.eecnxt1_.schema[0] = state_.input_schema_;
    RETURN_IF_CANCELLED(exec_status);

    Order();
  }
  BarrierArrive(2);
  return true;
}
// just only thread can fetch this result
bool PhysicalSort::Next(SegmentExecStatus *const exec_status,
                        BlockStreamBase *block) {
  RETURN_IF_CANCELLED(exec_status);

  lock_->acquire();
  if (thread_id_ == -1) {
    thread_id_ = pthread_self();
    lock_->release();
  } else {
    if (thread_id_ != pthread_self()) {
      lock_->release();
      return false;
    } else {
      lock_->release();
    }
  }

  unsigned tuple_size = state_.input_schema_->getTupleMaxSize();
  void *desc = NULL;
  int tmp_tuple = -1;
  while (true) {
    if (all_cur_ < all_tuples_.size()) {
      if (NULL != (desc = block->allocateTuple(tuple_size))) {
        tmp_tuple = all_cur_++;
        memcpy(desc, all_tuples_[tmp_tuple], tuple_size);
      } else {  // block is full
        return true;
      }
    } else {                  // all tuple are fetched
      if (tmp_tuple == -1) {  // but this block is empty
        return false;
      } else {  // get several tuples
        return true;
      }
    }
  }
  return false;
}

bool PhysicalSort::Close(SegmentExecStatus *const exec_status) {
  if (NULL != block_buffer_) {
    delete block_buffer_;
    block_buffer_ = NULL;
  }

  state_.child_->Close(exec_status);
  return true;
}

void PhysicalSort::Print() {
  cout << "Sort: " << endl;
  for (int i = 0; i < state_.order_by_attrs_.size(); ++i) {
    cout << state_.order_by_attrs_[i].first->alias_ << "    "
         << (state_.order_by_attrs_[i].second == 0 ? "ASC" : "DESC") << endl;
  }
  state_.child_->Print();
}
bool PhysicalSort::CreateBlock(BlockStreamBase *&target) const {
  /**
   * TODO: the block allocation should apply for the memory budget from the
   * buffer manager first.
   */
  target =
      BlockStreamBase::createBlock(state_.input_schema_, state_.block_size_);
  return target != 0;
}
RetCode PhysicalSort::GetAllSegments(stack<Segment *> *all_segments) {
  RetCode ret = rSuccess;
  if (NULL != state_.child_) {
    ret = state_.child_->GetAllSegments(all_segments);
  }
  return ret;
}

}  // namespace physical_operator
}  // namespace claims
