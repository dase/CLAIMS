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
#ifndef PHYSICAL_OPERATOR_PHYSICAL_NEST_LOOP_JOIN_H_
#define PHYSICAL_OPERATOR_PHYSICAL_NEST_LOOP_JOIN_H_
#include "../physical_operator/physical_nest_loop_join.h"

#include <boost/serialization/base_object.hpp>
#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/physical_operator.h"
#include "../Debug.h"

namespace claims {
namespace physical_operator {
class PhysicalNestLoopJoin : public PhysicalOperator {
 protected:
  class NestLoopJoinContext : public ThreadContext {
   public:
    BlockStreamBase *block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator *block_stream_iterator_;
    DynamicBlockBuffer::Iterator buffer_iterator_;
    BlockStreamBase::BlockStreamTraverseIterator *buffer_stream_iterator_;
  };

  struct RemainingBlock {
    RemainingBlock(BlockStreamBase *bsb_right,
                   BlockStreamBase::BlockStreamTraverseIterator *bsti)
        : bsb_right_(bsb_right),
          blockstream_iterator_(bsti),
          buffer_iterator_(NULL),
          buffer_stream_iterator_(NULL) {}
    RemainingBlock()
        : bsb_right_(NULL),
          blockstream_iterator_(NULL),
          buffer_iterator_(NULL),
          buffer_stream_iterator_(NULL) {}
    RemainingBlock(const RemainingBlock &r) {
      bsb_right_ = r.bsb_right_;
      blockstream_iterator_ = r.blockstream_iterator_;
      buffer_iterator_ = r.buffer_iterator_;
      buffer_stream_iterator_ = r.buffer_stream_iterator_;
    }
    BlockStreamBase *bsb_right_;
    BlockStreamBase::BlockStreamTraverseIterator *blockstream_iterator_;
    DynamicBlockBuffer::Iterator *buffer_iterator_;
    BlockStreamBase::BlockStreamTraverseIterator *buffer_stream_iterator_;
  };

 public:
  class State {
    friend class PhysicalNestLoopJoin;

   public:
    State(PhysicalOperatorBase *child_left, PhysicalOperatorBase *child_right,
          Schema *input_schema_left, Schema *input_schema_right,
          Schema *output_schema, unsigned block_size);
    State() {}
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(const Archive &ar, const unsigned int version) {
      ar &child_left_ &child_right_ &input_schema_left_ &input_schema_right_ &
          output_schema_ &block_size_;
    }

   public:
    PhysicalOperatorBase *child_left_, *child_right_;
    Schema *input_schema_left_, *input_schema_right_;
    Schema *output_schema_;
    unsigned block_size_;
  };

 public:
  PhysicalNestLoopJoin();
  virtual ~PhysicalNestLoopJoin();
  PhysicalNestLoopJoin(State state);
  bool Open(const PartitionOffset &partition_offset = 0);
  bool Next(BlockStreamBase *block);
  bool Close();
  void Print();

 private:
  bool CreateBlockStream(BlockStreamBase *&, Schema *&schema) const;
  bool AtomicPopRemainingBlock(RemainingBlock &rb);
  void AtomicPushRemainingBlock(RemainingBlock rb);
  BlockStreamBase *AtomicPopFreeBlockStream();
  void AtomicPushFreeBlockStream(BlockStreamBase *block);
  BlockStreamBase *AtomicPopFreeHtBlockStream();
  void AtomicPushFreeHtBlockStream(BlockStreamBase *block);

  DynamicBlockBuffer *block_buffer_;
  std::map<unsigned, unsigned> joinIndex_left_to_output_;
  /* payload_left map to the output*/
  std::map<unsigned, unsigned> payload_left_to_output_;
  /* payload_right map to the output*/
  std::map<unsigned, unsigned> payload_right_to_output_;

  State state_;
  Lock lock_;
  unsigned produced_tuples_;
  unsigned consumed_tuples_from_right_;
  unsigned consumed_tuples_from_left_;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(const Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

}  // namespace physical_operator
}  // namespace claims
#endif  //  PHYSICAL_OPERATOR_PHYSICAL_NEST_LOOP_JOIN_H__
