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
 * /CLAIMS/physical_operator/physical_hash_join.h
 *
 *  Created on: Aug 27, 2013
 *      Author: zhanglei
 *       Email:
 *
 *  Modified on: Oct 20, 2015
 *       Author: tonglanxuan
 *        Email: lxtong0526@163.com
 *
 * Description: Structures the PhysicalJoin class, which functions as physical
 *              operator "Join".
 *
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_HASH_JOIN_H_
#define PHYSICAL_OPERATOR_PHYSICAL_HASH_JOIN_H_

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <stack>

#include "../Debug.h"
#include "../utility/rdtsc.h"
#include "../common/hash.h"
#include "../common/hashtable.h"
#include "../codegen/ExpressionGenerator.h"
#include "../common/error_define.h"
#include "../common/expression/expr_node.h"
#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/physical_operator.h"
using claims::common::ExprNode;
using claims::common::ExprEvalCnxt;
using claims::common::GetPartitionValue;

namespace claims {
namespace physical_operator {

/**
 * @brief Method description: Physical Operator "join", is used to join two
 *                            tables due to current rules.
 */
class PhysicalHashJoin : public PhysicalOperator {
 public:
  class JoinThreadContext : public ThreadContext {
   public:
    ~JoinThreadContext();
    BlockStreamBase* l_block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator* l_block_stream_iterator_;
    BlockStreamBase* r_block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator* r_block_stream_iterator_;
    BasicHashTable::Iterator hashtable_iterator_;
    std::vector<ExprNode*> join_condi_;
    ExprEvalCnxt expr_eval_cnxt_;
  };

  class State {
    friend class PhysicalHashJoin;

   public:
    /**
     * @brief Method description: Construct the State with paras, not used in
     *                            the current version.
     */
    State(PhysicalOperatorBase* child_left, PhysicalOperatorBase* child_right,
          Schema* input_schema_left, Schema* input_schema_right,
          Schema* output_schema, Schema* ht_schema,
          std::vector<unsigned> joinIndex_left,
          std::vector<unsigned> joinIndex_right, unsigned ht_nbuckets,
          unsigned ht_bucketsize, unsigned block_size,
          vector<ExprNode*> join_condi);
    State() {}
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& child_left_& child_right_& input_schema_left_& input_schema_right_&
          output_schema_& hashtable_schema_& join_index_left_&
              join_index_right_& hashtable_bucket_num_& hashtable_bucket_size_&
                  block_size_& join_condi_;
    }

   public:
    // input and output
    PhysicalOperatorBase* child_left_, *child_right_;
    Schema* input_schema_left_, *input_schema_right_;
    Schema* output_schema_, *hashtable_schema_;
    // how to join
    std::vector<unsigned> join_index_left_;
    std::vector<unsigned> join_index_right_;
    std::vector<ExprNode*> join_condi_;
    // hashtable
    unsigned hashtable_bucket_num_;
    unsigned hashtable_bucket_size_;
    unsigned block_size_;
  };
  PhysicalHashJoin(State state);
  PhysicalHashJoin();
  virtual ~PhysicalHashJoin();

  /**
   * @brief Method description: Map between join_index_left_, payload_left_,
   *                            payload_right_ and output_index_. Initialize
   *                            hash, hashtable, and the functions. Get tuples
   *                            from left child, and put them into the proper
   *                            buckets.
   * @param   const PartitionOffset& partition_offset, to identify which
   *          partiton the function operates on.
   * @return  true in all cases.
   */
  bool Open(SegmentExecStatus* const exec_status,
            const PartitionOffset& partition_offset = 0);
  /**
   * @brief Method description: Get tuples from right child, use algorithm to
   *                            find whether there's a left tuple that matches
   *                            on the key value, then combine them and put the
   *                            result into a block, and send the block to its
   *                            father.
   * @param   BlockStreamBase *block, the info of block
   * @return  false if there's no tuple to function and the block is empty,
   *          otherwise true.
   */
  bool Next(SegmentExecStatus* const exec_status, BlockStreamBase* block);
  /**
   * @brief Method description: Initialize thread status, destroy contexts,
   *                            delete hashtable, and close childs.
   * @return  true.
   */
  bool Close(SegmentExecStatus* const exec_status);
  void Print();
  RetCode GetAllSegments(stack<Segment*>* all_segments);

 private:
  /**
   * @brief Method description: Allocate memory for jtc.
   */
  ThreadContext* CreateContext();
  /**
   * @brief Method description: To see if right child tuple and left child
   *                            tuple matches on the key value. result stores
   *                            in key_exit.
   */
  static void IsMatch(void* l_tuple_addr, void* r_tuple_addr, void* return_addr,
                      vector<unsigned>& l_join_index,
                      vector<unsigned>& r_join_index, Schema* l_schema,
                      Schema* r_schema, ExprFuncTwoTuples func);
  /**
   * @brief Method description: Another way to see if right child tuple and left
   *                            child tuple matches on the key value by using
   *                            func result stores in key_exit.
   */
  static void IsMatchCodegen(void* l_tuple_addr, void* r_tuple_addr,
                             void* return_addr, vector<unsigned>& l_join_index,
                             vector<unsigned>& r_join_index, Schema* l_schema,
                             Schema* r_schema, ExprFuncTwoTuples func);
  // static void copy_to_hashtable(void* desc, void* src, Schema* );
  bool JoinCondiProcess(void* tuple_left, void* tuple_right,
                        JoinThreadContext* const jtc);

 private:
  State state_;

  PartitionFunction* hash_func_;
  BasicHashTable* hashtable_;
  Schema* hashtable_schema_;

  typedef void (*ConditionFilterFunc)(void*, void*, void*, vector<unsigned>&,
                                      vector<unsigned>&, Schema*, Schema*,
                                      ExprFuncTwoTuples);

  ConditionFilterFunc cff_;
  ExprFuncTwoTuples eftt_;
  GetPartitionValue gpv_right_, gpv_left_;
  LLVMMemcpy memcpy_;
  LLVMMemcat memcat_;
  unsigned bucket_num_mod_;

  // debug
  unsigned produced_tuples;
  unsigned consumed_tuples_from_right;
  unsigned consumed_tuples_from_left;
  unsigned tuples_in_hashtable;
  unsigned water_mark;

#ifdef TIME
  unsigned long long timer;
#endif

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

}  // namespace physical_operator
}  // namespace claims

#endif  // PHYSICAL_OPERATOR_PHYSICAL_HASH_JOIN_H_
