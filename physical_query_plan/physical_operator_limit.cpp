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

#include "../physical_query_plan/physical_operator_limit.h"

namespace claims {
namespace physical_query_plan {

BlockStreamLimit::BlockStreamLimit()
    : received_tuples_(0), block_for_asking_(NULL) {}
BlockStreamLimit::BlockStreamLimit(State state)
    : state_(state), received_tuples_(0), block_for_asking_(NULL) {}

BlockStreamLimit::State::State(Schema* schema, BlockStreamIteratorBase* child,
                               unsigned long limits, unsigned block_size,
                               unsigned long start_position)
    : schema_(schema),
      child_(child),
      limits_(limits),
      block_size_(block_size),
      start_position_(start_position) {}
BlockStreamLimit::State::State()
    : schema_(NULL), child_(NULL), limits_(0), block_size_(0) {}

BlockStreamLimit::~BlockStreamLimit() {
  // TODO(wangli):Auto-generated destructor stub
}

bool BlockStreamLimit::Open(const PartitionOffset& par) {
  tuple_cur_ = 0;
  block_for_asking_ =
      BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
  received_tuples_ = 0;
  return state_.child_->Open(par);
}
bool BlockStreamLimit::Next(BlockStreamBase* block) {
  while (state_.child_->Next(block_for_asking_)) {
    void* tuple_from_child;
    BlockStreamBase::BlockStreamTraverseIterator* it =
        block_for_asking_->createIterator();
    while ((tuple_from_child = it->currentTuple())) {
      if (!limitExhausted()) {
        if (!shouldSkip()) {
          const unsigned tuple_size =
              state_.schema_->getTupleActualSize(tuple_from_child);
          void* target_tuple;
          if ((target_tuple = block->allocateTuple(tuple_size)) > 0) {
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
        /**
         * if the limit has already been exhausted, the current loop breaks
         * to fetch the next block from child iterator.
         * TODO(wangli): ideally, fetching blocks from child iterator in cases that
         * the limit is exhausted is not necessary. However, in the current
         * implementation, the child iterator sub-tree leaded by exchange
         * lower iterator cannot be closed if not all the blocks are called.
         */
        break;  // to consume the next block;
      }
    }
  }
  return !block->Empty();
}
bool BlockStreamLimit::Close() {
  state_.child_->Close();
  block_for_asking_->~BlockStreamBase();
  return true;
}

void BlockStreamLimit::Print() {
  printf("Limit %ld\n", state_.limits_);
  state_.child_->Print();
}

}  // physical_query_plan
}  // claims
