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
 * physical_operator/combine_tuple.cpp
 *
 *  Created on: 2013-8-27
 *      Author: casa, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#include "../physical_operator/combine_tuple.h"
namespace claims {
namespace physical_operator {

CombineTuple::CombineTuple(State state) : state_(state) {}

CombineTuple::State::State(std::vector<Schema *> input_schemas,
                           Schema *output_schema,
                           std::vector<PhysicalOperatorBase *> children)
    : input_schemas_(input_schemas),
      output_schema_(output_schema),
      children_(children) {}

CombineTuple::~CombineTuple() {}

bool CombineTuple::Open(const PartitionOffset &partition_offset) {
  // first
  std::vector<BlockStreamBase *> v_b;
  for (unsigned i = 0; i < state_.children_.size(); i++) {
    BlockStreamBase *bsb = new BlockStreamFix(BLOCK_SIZE_CAO, 4);
    v_b.push_back(bsb);
  }

  free_block_stream_list_.push_back(v_b);

  for (unsigned i = 0; i < state_.children_.size(); i++) {
    if (!state_.children_[i]->Open(partition_offset)) {
      // TODO: handle the failure
      return false;
    }
  }
  return true;
}

bool CombineTuple::Next(BlockStreamBase *block) {
  unsigned total_length_ = 0;
  for (unsigned i = 0; i < state_.input_schemas_.size(); i++) {
    total_length_ += state_.input_schemas_[i]->getTupleMaxSize();
  }
  void *tuple = 0;
  void *column_in_combinedTuple = 0;
  void *combinedTuple_ =
      memalign(cacheline_size, state_.output_schema_->getTupleMaxSize());
  void *cur = 0;

  RemainingBlock rb;
  if (AtomicPopRemainingBlock(rb)) {
    while (1) {
      for (unsigned j = 0; j < state_.children_.size(); j++) {
        if ((cur = rb.bsti_list_[j]->currentTuple()) == 0) {
          rb.buffer_[j]->setEmpty();
          if (state_.children_[j]->Next(rb.buffer_[j]) == false) {
            if (!block->Empty()) {
              AtomicPushRemainingBlock(rb);
              return true;
            }
            return false;
          }
          rb.bsti_list_[j]->reset();
          cur = rb.bsti_list_[j]->currentTuple();
        }
        column_in_combinedTuple =
            state_.output_schema_->getColumnAddess(j, combinedTuple_);
        state_.output_schema_->columns[j].operate->assign(
            cur, column_in_combinedTuple);
      }
      if ((tuple = block->allocateTuple(total_length_)) > 0) {
        memcpy(tuple, combinedTuple_, total_length_);
        for (unsigned k = 0; k < state_.children_.size(); k++) {
          rb.bsti_list_[k]->increase_cur_();
        }
      } else {
        AtomicPushRemainingBlock(rb);
        return true;
      }
    }
  }

  lock_.acquire();
  std::vector<BlockStreamBase *> v_bsb;
  if (!free_block_stream_list_.empty()) {
    v_bsb = free_block_stream_list_.front();
    free_block_stream_list_.pop_front();
  } else {
    return false;
  }
  lock_.release();

  for (unsigned i = 0; i < v_bsb.size(); i++) {
    v_bsb[i]->setEmpty();
    BlockStreamBase::BlockStreamTraverseIterator *traverse_iterator =
        v_bsb[i]->createIterator();
    rb.bsti_list_.push_back(traverse_iterator);
  }

  AtomicPushRemainingBlock(RemainingBlock(v_bsb, rb.bsti_list_));

  return Next(block);
}

bool CombineTuple::Close() {
  for (unsigned i = 0; i < state_.children_.size(); i++) {
    state_.children_[i]->Close();
  }
  return true;
}

bool CombineTuple::AtomicPopRemainingBlock(RemainingBlock &rb) {
  lock_.acquire();
  if (remaining_block_list_.size() > 0) {
    rb = remaining_block_list_.front();
    remaining_block_list_.pop_front();
    lock_.release();
    return true;
  } else {
    lock_.release();
    return false;
  }
}

void CombineTuple::AtomicPushRemainingBlock(RemainingBlock rb) {
  lock_.acquire();
  remaining_block_list_.push_back(rb);
  lock_.release();
}
}  // namespace physical_operator
}  // namespace claims
