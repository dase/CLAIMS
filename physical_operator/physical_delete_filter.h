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
 * /CLAIMS/physical_query_plan/DeleteFilter.h
 *
 *  Created on: Oct 26, 2015
 *      Author: minqi
 *		   Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description:
 *
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_DELETE_FILTER_H_
#define PHYSICAL_OPERATOR_PHYSICAL_DELETE_FILTER_H_

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "../../common/hash.h"
#include "../../common/hashtable.h"
#include "../utility/rdtsc.h"
#include "../codegen/ExpressionGenerator.h"
#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/physical_operator.h"

namespace claims {
namespace physical_operator {

/**
 * The physical operator to filter out the deleted tables from the base table.
 * In this version of the delete filter, it use a hash join manner to filter
 * those deleted tuples. The entire setup of the operator is very similar to
 * those used in the equal hash join physical operator.
 */
class PhysicalDeleteFilter : public PhysicalOperator {
 public:
  class DeleteFilterThreadContext : public ThreadContext {
   public:
    ~DeleteFilterThreadContext();

   public:
    BlockStreamBase* l_block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator* l_block_stream_iterator_;
    BlockStreamBase* r_block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator* r_block_stream_iterator_;
    BasicHashTable::Iterator hashtable_iterator_;
  };

  class State {
    friend class PhysicalDeleteFilter;

   public:
    State(PhysicalOperatorBase* child_left, PhysicalOperatorBase* child_right_,
          Schema* input_schema_left, Schema* input_schema_right,
          Schema* output_schema, Schema* hashtable_schema, unsigned block_size);
    State() {}
    ~State() {}
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& child_left_& child_right_& input_schema_left_& input_schema_right_&
          output_schema_& hashtable_schema_;
    }

   public:
    // input and output
    PhysicalOperatorBase* child_left_;
    PhysicalOperatorBase* child_right_;
    Schema* input_schema_left_;
    Schema* input_schema_right_;
    Schema* output_schema_;
    Schema* hashtable_schema_;

    unsigned block_size_;

    // how to filter the deleted tuples
    std::vector<unsigned>
        filter_key_deleted_;                 // the rowids in the deleted table
    std::vector<unsigned> filter_key_base_;  // the rowids in the base table
                                             // that needs to be filtered
    std::vector<unsigned>
        payload_deleted_;  // the payload columns of the deleted table
    std::vector<unsigned> payload_base_;  // the payload columns after filtered

    // hashtable
    unsigned hashtable_bucket_num_;
    unsigned hashtable_bucket_size_;
  };
  explicit PhysicalDeleteFilter(State state);
  PhysicalDeleteFilter();
  virtual ~PhysicalDeleteFilter();

  bool Open(const PartitionOffset& partition_offset = 0);
  bool Next(BlockStreamBase* block);
  bool Close();
  void Print();

 private:
  ThreadContext* CreateContext();

 private:
  State state_;
  PartitionFunction* hash_;
  BasicHashTable* hashtable_;
  Schema* hashtable_schema_;

  /* table row id map to the output*/
  std::map<unsigned, unsigned> joinIndex_table_to_output_;
  /* payload_right map to the output*/
  std::map<unsigned, unsigned> payload_table_to_output_;

  typedef void (*ConditionFilterFunc)(void*, void*, void*, vector<unsigned>&,
                                      vector<unsigned>&, Schema*, Schema*,
                                      ExprFuncTwoTuples);
  inline static void PhysicalDeleteFilter::isMatch(
      void* l_tuple_addr, void* r_tuple_addr, void* return_addr,
      vector<unsigned>& l_join_index, vector<unsigned>& r_join_index,
      Schema* l_schema, Schema* r_schema, ExprFuncTwoTuples func);
  inline static void PhysicalDeleteFilter::isMatchCodegen(
      void* l_tuple_addr, void* r_tuple_addr, void* return_addr,
      vector<unsigned>& l_join_index, vector<unsigned>& r_join_index,
      Schema* l_schema, Schema* r_schema, ExprFuncTwoTuples func);
  ConditionFilterFunc cff_;
  ExprFuncTwoTuples eftt_;
  LLVMMemcpy memcpy_;
  LLVMMemcat memcat_;

  // debug
  unsigned produced_tuples_;

  friend class boost::serialization::access;
  template <class Archieve>
  void serialize(Archieve& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

} /* namespace physical_query_plan */
} /* namespace claims */

#endif  //  PHYSICAL_OPERATOR_PHYSICAL_DELETE_FILTER_H_
