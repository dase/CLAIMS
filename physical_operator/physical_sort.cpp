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
namespace claims {
namespace physical_operator {

unsigned *PhysicalSort::sort_order_by_key_pos_ = NULL;
PhysicalSort::State *PhysicalSort::sort_state_ = NULL;

PhysicalSort::PhysicalSort() {
  sema_open_.set_value(1);

  sema_open_finished_.set_value(0);
  order_by_key_pos_ = 0;
}

PhysicalSort::PhysicalSort(State state)
    : finished_thread_count_(0), registered_thread_count_(0), state_(state) {
  sema_open_.set_value(1);
  sema_open_finished_.set_value(0);
  order_by_key_pos_ = 0;
  sort_state_ = &state_;
}

PhysicalSort::~PhysicalSort() {}

PhysicalSort::State::State()
    : input_(0), child_(0), block_size_(0), partition_offset_(0) {}

PhysicalSort::State::State(Schema *input, vector<unsigned> orderbyKey,
                           PhysicalOperatorBase *child,
                           const unsigned block_size, vector<int> direction,
                           const PartitionOffset partition_offset)
    : input_(input),
      order_by_key_(orderbyKey),
      child_(child),
      block_size_(block_size),
      partition_offset_(partition_offset),
      direction_(direction) {}

void PhysicalSort::Swap(void *&desc, void *&src) {
  swap_num_++;
  void *temp = 0;
  temp = desc;
  desc = src;
  src = temp;
}

bool PhysicalSort::Compare(const void *a, const void *b) {
  const void *l = sort_state_->input_->getColumnAddess(
      sort_state_->order_by_key_[*sort_order_by_key_pos_], a);
  const void *r = sort_state_->input_->getColumnAddess(
      sort_state_->order_by_key_[*sort_order_by_key_pos_], b);
  if (sort_state_->direction_[*sort_order_by_key_pos_])
    return sort_state_->input_
        ->getcolumn(sort_state_->order_by_key_[*sort_order_by_key_pos_])
        .operate->duplicateOperator()
        ->greate(l, r);
  else
    return sort_state_->input_
        ->getcolumn(sort_state_->order_by_key_[*sort_order_by_key_pos_])
        .operate->duplicateOperator()
        ->less(l, r);
}

void PhysicalSort::Order() {
  sort_order_by_key_pos_ = &order_by_key_pos_;
  while (order_by_key_pos_ < state_.order_by_key_.size()) {
    stable_sort(tuple_vector_.begin(), tuple_vector_.end(), Compare);
    order_by_key_pos_++;
  }
}

bool PhysicalSort::Open(const PartitionOffset &part_off) {
  /**
   * TODO(anyone): multi threads can be used to pipeline!!!
   */
  swap_num_ = 0;
  temp_cur_ = 0;
  /**
   *  first we can store all the data which will be bufferred
   * 1, buffer is the first phase. multi-threads will be applyed to the data
   *    in the buffer.
   * 2, sort the data in the buffer, we choose quicksort to sort the records
   *    by specifying the column to be sorted
   * 3, whether to register the buffer into the blockmanager.
   * */
  BlockStreamBase *block_for_asking;

  state_.partition_offset_ = part_off;

  state_.child_->Open(state_.partition_offset_);

  if (sema_open_.try_wait()) {
    block_buffer_iterator_ = block_buffer_.createIterator();
    open_finished_ = true;
  } else {
    while (!open_finished_) {
      usleep(1);
    }
  }

  if (CreateBlockStream(block_for_asking) == false) {
    LOG(ERROR) << "error in the create block stream!!!" << endl;
    return 0;
  }
  /**
   *  phase 1: store the data in the buffer!
   *          by using multi-threads to speed up
   */
  unsigned block_offset = 0;
  unsigned tuple_count_sum = 0;
  BlockStreamBase::BlockStreamTraverseIterator *iterator_for_scan;
  while (state_.child_->Next(block_for_asking)) {
    tuple_count_sum += block_for_asking->getTuplesInBlock();
    block_buffer_.atomicAppendNewBlock(block_for_asking);
    iterator_for_scan = block_buffer_.getBlock(block_offset)->createIterator();
    void *tuple_ptr = 0;
    while ((tuple_ptr = iterator_for_scan->nextTuple()) != 0) {
      tuple_vector_.push_back(tuple_ptr);
    }
    block_offset++;
    if (CreateBlockStream(block_for_asking) == false) {
      LOG(ERROR) << "error in the create block stream!!!" << endl;
      return 0;
    }
  }

  /**
   *  phase 2: sort the data in the buffer!
   *          by using multi-threads to speed up?
   * TODO(anyone): whether to store the sorted data into the blockmanager.
   */
  //    cout<<"check the memory usage!!!"<<endl;
  unsigned long long int time = curtick();
  //    order(state_.orderbyKey_,tuple_count_sum);
  Order();

  // cout<<"the tuple_count is: "<<tuple_count_sum<<"Total time:
  // "<<getSecond(time)<<" seconds, the swap num is: "<<swap_num<<endl;
  return true;
}

bool PhysicalSort::Next(BlockStreamBase *block) {
  /* multi-threads to send the block out*/
  unsigned tuple_size = state_.input_->getTupleMaxSize();
  while (temp_cur_ < tuple_vector_.size()) {
    void *desc = 0;
    while ((desc = block->allocateTuple(tuple_size))) {
      memcpy(desc, tuple_vector_[temp_cur_], tuple_size);
      temp_cur_++;
      if (temp_cur_ < tuple_vector_.size())
        continue;
      else
        break;
    }
    return true;
  }
  return false;
}

bool PhysicalSort::Close() {
  state_.child_->Close();
  return true;
}

void PhysicalSort::Print() {
  LOG(INFO) << "Sort" << endl;
  for (int i = 0; i < state_.direction_.size(); i++) {
    LOG(INFO) << "[" << state_.order_by_key_[i] << "]" << (0 ==
            state_.direction_[i])
        ? "asc"
        : "desc";
  }
  LOG(INFO) << endl;
  state_.child_->Print();
}

bool PhysicalSort::CreateBlockStream(BlockStreamBase *&target) const {
  /**
   * TODO: the block allocation should apply for the memory budget from the
   * buffer manager first.
   */
  // cout<<"state_.block_size_: "<<state_.block_size_<<endl;
  target = BlockStreamBase::createBlock(state_.input_, state_.block_size_);
  return target != 0;
}

}  // namespace physical_operator
}  // namespace claims
