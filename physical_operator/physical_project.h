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
 * /CLAIMS/physical_operator/physical_project.h
 *
 *  Created on: Feb 17, 2014
 *      Author: casa,Hanzhang
 *		   Email:zhangleicasa@gmail.com
 *
 * Description: Implementation of Project operator in physical layer.
 *
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_PROJECT_H_
#define PHYSICAL_OPERATOR_PHYSICAL_PROJECT_H_

#include <iostream>
#include <vector>
#include <map>
#include <list>

#include "../common/ExpressionCalculator.h"
#include "../common/ExpressionItem.h"
#include "../common/Mapping.h"
#include "../configure.h"
#include "../common/Expression/qnode.h"
#include "../common/Expression/initquery.h"
#include "../common/Expression/execfunc.h"
#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/physical_operator.h"

namespace claims {
namespace physical_operator {

/**
 * @brief Method description: Implementation of Project operator in physical
 * layer. Projection operator corresponds to the select-clause of SQL, it
 * calculates each expression at the select-clause and construct one new tuple.
 */
class PhysicalProject : public PhysicalOperator {
 public:
  class ProjectThreadContext : public ThreadContext {
   public:
    BlockStreamBase *block_for_asking_;
    BlockStreamBase *temp_block_;
    BlockStreamBase::BlockStreamTraverseIterator *block_stream_iterator_;
    vector<QNode *> thread_qual_;
    ~ProjectThreadContext() {
      if (NULL != block_for_asking_) {
        delete block_for_asking_;
        block_for_asking_ = NULL;
      }
      if (NULL != temp_block_) {
        delete temp_block_;
        temp_block_ = NULL;
      }
      if (NULL != block_stream_iterator_) {
        delete block_stream_iterator_;
        block_stream_iterator_ = NULL;
      }
      for (int i = 0; i < thread_qual_.size(); i++) {
        if (NULL != thread_qual_[i]) {
          delete thread_qual_[i];
          thread_qual_[i] = NULL;
        }
      }
    }
  };

  class State {
    friend class PhysicalProject;

   public:
    State(Schema *schema_input, Schema *schema_output,
          PhysicalOperatorBase *children, unsigned block_size,
          vector<QNode *> expr_tree);
    State(){};

   public:
    Schema *schema_input_;
    Schema *schema_output_;

    /**
     * @brief Recently, the expression is supporting the reduce the number of
     * the input table!!!
     */

    // TODO(casa): Support the multi-to-multi between the input table and the
    // select list, this expr is the result of the getIteratorTree to construct
    // a schema. getDataflow() can generate a schema by using the SQLExpression
    // and Expression can be computed by SQLExpression

    vector<QNode *> expr_tree_;
    unsigned block_size_;
    PhysicalOperatorBase *child_;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &schema_input_ &schema_output_ &child_ &block_size_ &expr_tree_;
    }
  };
  PhysicalProject(State state);
  PhysicalProject();
  virtual ~PhysicalProject();
  /**
   * @brief: construct iterator of project operator
   */
  bool Open(const PartitionOffset &kPartitionOffset = 0);

  /**
   * @brief: fetch a block from child and ProcessInLogic().
   */
  bool Next(BlockStreamBase *block);

  /**
   * @brief: revoke resource.
   */
  bool Close();
  void Print();

 private:
  /**
   * @brief Method description: Initialize project thread context with
   * state(Class).
   * @return a pointer(ProjectThreadContext)
   */
  ThreadContext *CreateContext();
  /**
   * @brief Method description:// According to result,the function generate a
   * new attribute list(new schema:output).
   * @return  a new tuples.
   */
  bool CopyNewValue(void *tuple, void *result, int length);

  /**
   * @brief: The actual implementation of operations.
   * @param BlockStreamBase*, ProjectThreadContext*
   * @details   (additional) The actual implementation of operations.
   */
  void ProcessInLogic(BlockStreamBase *block, ProjectThreadContext *tc);

 private:
  State state_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};

}  // namespace physical_operator
}  // namespace claims

#endif  //  PHYSICAL_OPERATOR_PHYSICAL_PROJECT_H_
