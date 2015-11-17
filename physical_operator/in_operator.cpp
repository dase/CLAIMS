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
 * /CLAIMS/physical_operator/in_operator.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: scdong
 */

#include "../physical_operator/in_operator.h"

#include <assert.h>
#include <vector>

namespace claims {
namespace physical_operator {
InOperator::InOperator(State state)
    : state_(state),
      hash_func_(0),
      hash_table_(0),
      open_finished_(false),
      reached_end(0) {
  sema_open_.set_value(1);
  barrier_ = new Barrier(1);
}

InOperator::InOperator()
    : hash_func_(0), hash_table_(0), open_finished_(false), reached_end(0) {
  sema_open_.set_value(1);
  barrier_ = new Barrier(1);
}

InOperator::~InOperator() {}

InOperator::State::State(PhysicalOperatorBase* child_set,
                         PhysicalOperatorBase* child_in,
                         Schema* schema_child_set, Schema* schema_child_in,
                         unsigned index_child_set, unsigned index_child_in,
                         unsigned block_size, unsigned ht_nbuckets,
                         unsigned ht_bucketsize)
    : child_set_(child_set),
      child_in_(child_in),
      schema_child_set_(schema_child_set),
      schema_child_in_(schema_child_in),
      index_child_set_(index_child_set),
      index_child_in_(index_child_in),
      block_size_(block_size),
      ht_nbuckets_(ht_nbuckets),
      ht_bucket_size_(ht_bucketsize) {}

InOperator::State::State(PhysicalOperatorBase* child_set,
                         PhysicalOperatorBase* child_in,
                         Schema* schema_child_set, Schema* schema_child_in,
                         unsigned index_child_set, unsigned index_child_in,
                         unsigned block_size)
    : child_set_(child_set),
      child_in_(child_in),
      schema_child_set_(schema_child_set),
      schema_child_in_(schema_child_in),
      index_child_set_(index_child_set),
      index_child_in_(index_child_in),
      block_size_(block_size),
      ht_nbuckets_(1024),
      ht_bucket_size_(64) {}

bool InOperator::Open(const PartitionOffset& partition_offset) {
  state_.child_set_->Open(partition_offset);
  state_.child_in_->Open(partition_offset);
  AtomicPushFreeHtBlockStream(BlockStreamBase::createBlock(
      state_.schema_child_set_, state_.block_size_));
  AtomicPushFreeBlockStream(BlockStreamBase::createBlock(
      state_.schema_child_in_, state_.block_size_));

  if (sema_open_.try_wait()) {
    // initialize hash table, use the child_set to build hash table
    hash_func_ =
        PartitionFunctionFactory::createBoostHashFunction(state_.ht_nbuckets_);
    vector<unsigned> ht_index;
    ht_index.push_back(state_.index_child_set_);
    hash_table_ = new BasicHashTable(
        state_.ht_nbuckets_, state_.ht_bucket_size_,
        (state_.schema_child_set_->getSubSchema(ht_index))->getTupleMaxSize());
    ht_index.clear();
    open_finished_ = true;
  } else {
    while (!open_finished_) usleep(1);
  }

  void* cur_tuple = NULL;
  void* tuple_in_hashtable = NULL;
  unsigned bn = 0;

  BlockStreamBase* bsb = AtomicPopFreeHtBlockStream();
  while (state_.child_set_->Next(bsb)) {
    BlockStreamBase::BlockStreamTraverseIterator* bsti = bsb->createIterator();
    bsti->reset();
    while (cur_tuple = bsti->nextTuple()) {
      bn = state_.schema_child_set_->getcolumn(state_.index_child_set_)
               .operate->getPartitionValue(
                   state_.schema_child_set_->getColumnAddess(
                       state_.index_child_set_, cur_tuple),
                   state_.ht_nbuckets_);
      tuple_in_hashtable = hash_table_->atomicAllocate(bn);
      state_.schema_child_set_->getcolumn(state_.index_child_set_)
          .operate->assignment(state_.schema_child_set_->getColumnAddess(
                                   state_.index_child_set_, cur_tuple),
                               tuple_in_hashtable);
    }
    bsb->setEmpty();
  }
  barrier_->Arrive();
  printf("-----------In Iterator Open Successful!-----------\n");
  return true;
}

bool InOperator::Next(BlockStreamBase* block) {
  unsigned bn;
  RemainingBlock rb;
  void* tuple_from_child_in = NULL;
  void* tuple_in_output_block = NULL;
  void* tuple_in_hashtable;
  void* key_in_input;
  bool passIn = false;
  BasicHashTable::Iterator hashtable_iterator = hash_table_->CreateIterator();

  if (AtomicPopRemainingBlock(rb)) {
    while ((tuple_from_child_in = rb.blockstream_iterator_->currentTuple()) >
           0) {
      passIn = false;
      bn = state_.schema_child_in_->getcolumn(state_.index_child_in_)
               .operate->getPartitionValue(
                   state_.schema_child_in_->getColumnAddess(
                       state_.index_child_in_, tuple_from_child_in),
                   state_.ht_nbuckets_);
      hash_table_->placeIterator(hashtable_iterator, bn);
      while ((tuple_in_hashtable = hashtable_iterator.readnext()) > 0) {
        key_in_input = state_.schema_child_in_->getColumnAddess(
            state_.index_child_in_, tuple_from_child_in);
        if (state_.schema_child_in_->getcolumn(state_.index_child_in_)
                .operate->equal(tuple_in_hashtable, key_in_input)) {
          passIn = true;
          break;
        }
      }
      if (passIn) {
        const unsigned bytes = state_.schema_child_in_->getTupleMaxSize();
        if ((tuple_in_output_block = block->allocateTuple(bytes)) > 0) {
          state_.schema_child_in_->copyTuple(tuple_from_child_in,
                                             tuple_in_output_block);
          rb.blockstream_iterator_->increase_cur_();
        } else {
          AtomicPushRemainingBlock(rb);
          return true;
        }
      } else
        rb.blockstream_iterator_->increase_cur_();
    }
    AtomicPushFreeBlockStream(rb.bsb_in_);
  }

  BlockStreamBase* block_for_asking = AtomicPopFreeBlockStream();
  block_for_asking->setEmpty();
  while (state_.child_in_->Next(block_for_asking)) {
    BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator =
        block_for_asking->createIterator();
    while ((tuple_from_child_in = traverse_iterator->currentTuple()) > 0) {
      passIn = false;
      bn = state_.schema_child_in_->getcolumn(state_.index_child_in_)
               .operate->getPartitionValue(
                   state_.schema_child_in_->getColumnAddess(
                       state_.index_child_in_, tuple_from_child_in),
                   state_.ht_nbuckets_);
      hash_table_->placeIterator(hashtable_iterator, bn);
      while ((tuple_in_hashtable = hashtable_iterator.readCurrent()) != 0) {
        key_in_input = state_.schema_child_in_->getColumnAddess(
            state_.index_child_in_, tuple_from_child_in);
        if (state_.schema_child_in_->getcolumn(state_.index_child_in_)
                .operate->equal(tuple_in_hashtable, key_in_input)) {
          passIn = true;
          break;
        }
        hashtable_iterator.increase_cur_();
      }
      if (passIn) {
        const unsigned bytes = state_.schema_child_in_->getTupleMaxSize();
        if ((tuple_in_output_block = block->allocateTuple(bytes)) > 0) {
          state_.schema_child_in_->copyTuple(tuple_from_child_in,
                                             tuple_in_output_block);
          traverse_iterator->increase_cur_();
        } else {
          AtomicPushRemainingBlock(
              RemainingBlock(block_for_asking, traverse_iterator));
          return true;
        }
      } else
        traverse_iterator->increase_cur_();
    }
    traverse_iterator->~BlockStreamTraverseIterator();
    block_for_asking->setEmpty();
  }
  AtomicPushFreeBlockStream(block_for_asking);
  if (!block->Empty()) return true;
  return false;
}

bool InOperator::Close() {
  sema_open_.post();
  open_finished_ = false;
  //	barrier_->~Barrier();
  free_block_stream_list_.clear();
  ht_free_block_stream_list_.clear();
  remaining_block_list_.clear();
  //	hash->~PartitionFunction();
  hash_table_->~BasicHashTable();
  state_.child_set_->Close();
  state_.child_in_->Close();
  return true;
}

bool InOperator::AtomicPopRemainingBlock(RemainingBlock& rb) {
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

void InOperator::AtomicPushRemainingBlock(RemainingBlock rb) {
  lock_.acquire();
  remaining_block_list_.push_back(rb);
  lock_.release();
}

BlockStreamBase* InOperator::AtomicPopFreeBlockStream() {
  assert(!free_block_stream_list_.empty());
  lock_.acquire();
  BlockStreamBase* block = free_block_stream_list_.front();
  free_block_stream_list_.pop_front();
  lock_.release();
  return block;
}

void InOperator::AtomicPushFreeBlockStream(BlockStreamBase* block) {
  lock_.acquire();
  free_block_stream_list_.push_back(block);
  lock_.release();
}

BlockStreamBase* InOperator::AtomicPopFreeHtBlockStream() {
  assert(!ht_free_block_stream_list_.empty());
  lock_.acquire();
  BlockStreamBase* block = ht_free_block_stream_list_.front();
  ht_free_block_stream_list_.pop_front();
  lock_.release();
  return block;
}

void InOperator::AtomicPushFreeHtBlockStream(BlockStreamBase* block) {
  lock_.acquire();
  ht_free_block_stream_list_.push_back(block);
  lock_.release();
}
}  // namespace physical_operator
}  // namespace claims
