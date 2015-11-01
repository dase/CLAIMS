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
 * /Claims/storage/physical_operator_limit.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: wangli,Hanzhang
 *		   Email: wangli1426@gmail.com
 *
 * Description: Implementation of Limit operator in physical layer.
 *
 */

#include "../physical_operator/physical_limit.h"

namespace claims {
namespace physical_operator {

PhysicalLimit::PhysicalLimit() : received_tuples_(0), block_for_asking_(NULL) {}
PhysicalLimit::PhysicalLimit(State state)
    : state_(state), received_tuples_(0), block_for_asking_(NULL) {}

PhysicalLimit::State::State(Schema* schema, PhysicalOperatorBase* child,
                            unsigned long limits, unsigned block_size,
                            unsigned long start_position)
    : schema_(schema),
      child_(child),
      limit_tuples_(limits),
      block_size_(block_size),
      start_position_(start_position) {}
PhysicalLimit::State::State()
    : schema_(NULL), child_(NULL), limit_tuples_(0), block_size_(0) {}

PhysicalLimit::~PhysicalLimit() {
  // TODO(wangli):Auto-generated destructor stub.
}
/**
 * Initialize of the position of current tuple and target tuple
 */
bool PhysicalLimit::Open(const PartitionOffset& kPartitionOffset) {
  tuple_cur_ = 0;
  block_for_asking_ =
      BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
  received_tuples_ = 0;
  return state_.child_->Open(kPartitionOffset);
}
/**
 * if the limit has already been exhausted, the current loop breaks
 * to fetch the next block from child iterator.
 */

// TODO(wangli): ideally, fetching blocks from child iterator in cases
// that the limit is exhausted is not necessary. However, in the current
// implementation, the child iterator sub-tree leaded by exchange
// lower iterator cannot be closed if not all the blocks are called.
bool PhysicalLimit::Next(BlockStreamBase* block) {
  while (state_.child_->Next(block_for_asking_)) {
    void* tuple_from_child;
    BlockStreamBase::BlockStreamTraverseIterator* it =
        block_for_asking_->createIterator();
    while (NULL != (tuple_from_child = it->currentTuple())) {
      if (!LimitExhausted()) {
        if (!ShouldSkip()) {
          const unsigned tuple_size =
              state_.schema_->getTupleActualSize(tuple_from_child);
          void* target_tuple;
          if (NULL != (target_tuple = block->allocateTuple(tuple_size))) {
            state_.schema_->copyTuple(tuple_from_child, target_tuple);
            received_tuples_++;
            tuple_cur_++;
            it->increase_cur_();
          } else {
            it->~BlockStreamTraverseIterator();
            return true;
          }
        } else {
          tuple_cur_++;
          it->increase_cur_();
          continue;
        }
      } else {
        break;  // to consume the next block;
      }
    }
  }
  return !block->Empty();
}

bool PhysicalLimit::Close() {
  state_.child_->Close();
  block_for_asking_->~BlockStreamBase();
  return true;
}

void PhysicalLimit::Print() {
  printf("Limit %ld\n", state_.limit_tuples_);
  state_.child_->Print();
}

}  // namespace physical_operator
}  // namespace claims
