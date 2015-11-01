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
 * physical_operator/combine_tuple.h
 *
 *  Created on: 2013-8-27
 *      Author: casa, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#ifndef PHYSICAL_OPERATOR_COMBINE_TUPLE_H_
#define PHYSICAL_OPERATOR_COMBINE_TUPLE_H_

#include <malloc.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <list>
#include <vector>
#include <utility>
#include "../common/Schema/Schema.h"
#include "../configure.h"
#include "../physical_operator/physical_operator_base.h"
#include "../utility/lock.h"

namespace claims {
namespace physical_operator {
#define BLOCK_SIZE_CAO 4096
/**
 * combine each tuple from every child's block to a combined_tuple
 */
class CombineTuple : public PhysicalOperatorBase {
 public:
  struct RemainingBlock {
    RemainingBlock(
        std::vector<BlockStreamBase *> buffer,
        std::vector<BlockStreamBase::BlockStreamTraverseIterator *> bsti_list)
        : buffer_(buffer), bsti_list_(bsti_list){};
    RemainingBlock() : buffer_(0), bsti_list_(0){};
    //        void * combinedTuple_;
    std::vector<BlockStreamBase *> buffer_;
    std::vector<BlockStreamBase::BlockStreamTraverseIterator *> bsti_list_;
  };

  class State {
    friend class CombineTuple;

   public:
    State(std::vector<Schema *> input_schemas, Schema *output_schema,
          std::vector<PhysicalOperatorBase *> children);
    State(){};

   private:
    std::vector<Schema *> input_schemas_;
    Schema *output_schema_;
    std::vector<PhysicalOperatorBase *> children_;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &input_schemas_ &output_schema_ &children_;
    }
  };

  CombineTuple(State state);
  CombineTuple(){};
  virtual ~CombineTuple();

  bool Open(const PartitionOffset &partition_offset = 0);
  bool Next(BlockStreamBase *block);
  bool Close();

 private:
  bool AtomicPopRemainingBlock(RemainingBlock &rb);
  void AtomicPushRemainingBlock(RemainingBlock rb);

 private:
  State state_;

  std::list<RemainingBlock> remaining_block_list_;
  std::list<std::vector<BlockStreamBase *> > free_block_stream_list_;

  Lock lock_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};
}  // namespace physical_operator
}  // namespace claims

#endif  // PHYSICAL_OPERATOR_COMBINE_TUPLE_H_
