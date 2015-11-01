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
 * /CLAIMS/physical_operator/in_operator.h
 *
 *  Created on: Mar 4, 2014
 *      Author: scdong
 */

#ifndef PHYSICAL_OPERATOR_IN_OPERATOR_H_
#define PHYSICAL_OPERATOR_IN_OPERATOR_H_
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "../Debug.h"
#include "../utility/rdtsc.h"
#include "../utility/lock.h"
#include "../common/hashtable.h"
#include "../common/hash.h"
#include "../physical_operator/physical_operator_base.h"
namespace claims {
namespace physical_operator {

/**
 * used to implement "in (subquery)", eg, select * from TB where a in (select a
 * from TB1); First, buffer the result of subquery to a hash_table in Open(),
 * then fetch each tuple from the block got from child, and compare with
 * corresponding tuple in hash_table, if all is matched, then return true,
 * otherwise false.
 */
class InOperator : public PhysicalOperatorBase {
 public:
  struct RemainingBlock {
    RemainingBlock(BlockStreamBase *bsb_right,
                   BlockStreamBase::BlockStreamTraverseIterator *bsti)
        : bsb_in_(bsb_right), blockstream_iterator_(bsti) {}
    RemainingBlock() : bsb_in_(0), blockstream_iterator_(0) {}
    RemainingBlock(const RemainingBlock &r) {
      bsb_in_ = r.bsb_in_;
      blockstream_iterator_ = r.blockstream_iterator_;
    }
    BlockStreamBase *bsb_in_;
    BlockStreamBase::BlockStreamTraverseIterator *blockstream_iterator_;
  };

  class State {
    friend class InOperator;

   public:
    State(PhysicalOperatorBase *child_set, PhysicalOperatorBase *child_in,
          Schema *schema_child_set, Schema *schema_child_in,
          unsigned index_child_set, unsigned index_child_in,
          unsigned block_size, unsigned ht_nbuckets, unsigned ht_bucketsize);
    State(PhysicalOperatorBase *child_set, PhysicalOperatorBase *child_in,
          Schema *schema_child_set, Schema *schema_child_in,
          unsigned index_child_set, unsigned index_child_in,
          unsigned block_size);

    State() {}
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &child_set_ &child_in_ &schema_child_set_ &schema_child_in_ &
          index_child_set_ &index_child_in_ &ht_nbuckets_ &ht_bucket_size_ &
              block_size_;
    }

   public:
    // input and output
    PhysicalOperatorBase *child_set_, *child_in_;
    Schema *schema_child_set_, *schema_child_in_;

    // how to choose the legal record
    unsigned index_child_set_;
    unsigned index_child_in_;

    // hash table
    unsigned ht_nbuckets_;
    unsigned ht_bucket_size_;
    unsigned block_size_;
  };

  InOperator(State state);
  InOperator();
  virtual ~InOperator();
  // buffer result of sub_query in a hash_table
  bool Open(const PartitionOffset &partition_offset = 0);
  // get block from child, and fetch each tuple, then compare with every tuple
  // in corresponding hash_bucket
  bool Next(BlockStreamBase *block);
  bool Close();

 private:
  bool AtomicPopRemainingBlock(RemainingBlock &rb);
  void AtomicPushRemainingBlock(RemainingBlock rb);
  BlockStreamBase *AtomicPopFreeBlockStream();
  void AtomicPushFreeBlockStream(BlockStreamBase *block);
  BlockStreamBase *AtomicPopFreeHtBlockStream();
  void AtomicPushFreeHtBlockStream(BlockStreamBase *block);

  State state_;

  PartitionFunction *hash_func_;
  BasicHashTable *hash_table_;

  std::list<RemainingBlock> remaining_block_list_;
  std::list<BlockStreamBase *> free_block_stream_list_;
  std::list<BlockStreamBase *> ht_free_block_stream_list_;

  semaphore sema_open_;
  volatile bool open_finished_;
  unsigned reached_end;
  Lock lock_;
  Barrier *barrier_;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};
}  // namespace physical_operator
}  // namespace claims

#endif  //  PHYSICAL_OPERATOR_IN_OPERATOR_H_
