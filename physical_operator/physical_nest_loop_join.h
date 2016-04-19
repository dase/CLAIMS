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
#include <vector>

#include "../common/expression/expr_node.h"
#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/physical_operator.h"
#include "../Debug.h"

using claims::common::ExprEvalCnxt;
using claims::common::ExprNode;
namespace claims {
namespace physical_operator {

class PhysicalNestLoopJoin : public PhysicalOperator {
 protected:
  class NestLoopJoinContext : public ThreadContext {
   public:
    NestLoopJoinContext(const vector<ExprNode *> &join_condi,
                        const Schema *left_schema, const Schema *right_schema);
    ~NestLoopJoinContext();
    BlockStreamBase *block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator *block_stream_iterator_;
    DynamicBlockBuffer::Iterator buffer_iterator_;
    BlockStreamBase::BlockStreamTraverseIterator *buffer_stream_iterator_;
    vector<ExprNode *> join_condi_;
    ExprEvalCnxt expr_eval_cnxt_;
  };

 public:
  class State {
    friend class PhysicalNestLoopJoin;

   public:
    State(PhysicalOperatorBase *child_left, PhysicalOperatorBase *child_right,
          Schema *input_schema_left, Schema *input_schema_right,
          Schema *output_schema, unsigned block_size,
          std::vector<ExprNode *> join_condi);
    State() {}
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(const Archive &ar, const unsigned int version) {
      ar &child_left_ &child_right_ &input_schema_left_ &input_schema_right_ &
          output_schema_ &block_size_ &join_condi_;
    }

   public:
    PhysicalOperatorBase *child_left_, *child_right_;
    Schema *input_schema_left_, *input_schema_right_;
    Schema *output_schema_;
    std::vector<ExprNode *> join_condi_;
    unsigned block_size_;
  };
  typedef bool (*JoinCondiProcess)(void *tuple_left, void *tuple_right,
                                   NestLoopJoinContext *const nljcnxt);

 public:
  PhysicalNestLoopJoin();
  virtual ~PhysicalNestLoopJoin();
  PhysicalNestLoopJoin(State state);
  bool Open(const PartitionOffset &partition_offset = 0);
  bool Next(BlockStreamBase *block);
  bool Close();
  void Print();

 private:
  static bool WithJoinCondi(void *tuple_left, void *tuple_right,
                            NestLoopJoinContext *const nljcnxt);
  static bool WithoutJoinCondi(void *tuple_left, void *tuple_right,
                               NestLoopJoinContext *const nljcnxt);
  bool CreateBlockStream(BlockStreamBase *&, Schema *&schema) const;
  ThreadContext *CreateContext();

  DynamicBlockBuffer *block_buffer_;
  State state_;
  JoinCondiProcess join_condi_process_;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(const Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

}  // namespace physical_operator
}  // namespace claims
#endif  //  PHYSICAL_OPERATOR_PHYSICAL_NEST_LOOP_JOIN_H__
