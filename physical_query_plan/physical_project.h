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
 * /CLAIMS/physical_query_plan/physical_project.h
 *
 *  Created on: Feb 17, 2014
 *      Author: casa,Hanzhang
 *		   Email:zhangleicasa@gmail.com
 *
 * Description: Implementation of Project operator in physical layer.
 *
 */

#ifndef PHYSICAL_QUERY_PLAN_PHYSICAL_PROJECT_H_
#define PHYSICAL_QUERY_PLAN_PHYSICAL_PROJECT_H_

#include <iostream>
#include <vector>
#include <map>
#include <list>

#include "../../common/ExpressionCalculator.h"
#include "../../common/ExpressionItem.h"
#include "../../common/Mapping.h"
#include "../../configure.h"
#include "../../common/Expression/qnode.h"
#include "../../common/Expression/initquery.h"
#include "../../common/Expression/execfunc.h"
#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../physical_query_plan/physical_operator.h"

using namespace std;

// namespace claims {
// namespace physical_query_plan {
typedef vector<ExpressionItem> ExpressItem_List;

class BlockStreamProjectIterator : public PhysicalOperator {
 public:
  class project_thread_context : public ThreadContext {
   public:
    BlockStreamBase *block_for_asking_;
    BlockStreamBase *temp_block_;
    BlockStreamBase::BlockStreamTraverseIterator *block_stream_iterator_;
    vector<QNode *> thread_qual_;
    ~project_thread_context() {
      delete block_for_asking_;
      delete temp_block_;
      delete block_stream_iterator_;
      for (int i = 0; i < thread_qual_.size(); i++) {
        delete thread_qual_[i];
      }
    }
  };

  class State {
    friend class BlockStreamProjectIterator;

   public:
    State(Schema *input, Schema *output, BlockStreamIteratorBase *children,
          unsigned blocksize, Mapping map, vector<ExpressItem_List> v_ei,
          vector<QNode *> exprTree);
    State(Schema *input, Schema *output, BlockStreamIteratorBase *children,
          unsigned blocksize, Mapping map, vector<ExpressItem_List> v_ei);
    State(){};

   public:
    Schema *input_;
    Schema *output_;

    /**
     * @brief Recently, the expression is supporting the reduce the number of
     * the input table!!!
     */

    // TODO(casa): Support the multi-to-multi between the input table and the
    // select list, this expr is the result of the getIteratorTree to construct
    // a schema. getDataflow() can generate a schema by using the SQLExpression
    // and Expression can be computed by SQLExpression
    vector<ExpressItem_List> v_ei_;
    Mapping map_;

    vector<QNode *> exprTree_;
    unsigned block_size_;
    BlockStreamIteratorBase *child_;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &input_ &output_ &child_ &map_ &block_size_ &v_ei_ &exprTree_;
    }
  };
  BlockStreamProjectIterator(State state);
  BlockStreamProjectIterator();
  virtual ~BlockStreamProjectIterator();

  bool Open(const PartitionOffset &partition_offset = 0);
  bool Next(BlockStreamBase *block);
  bool Close();
  void Print();

 private:
  ThreadContext *CreateContext();

  // According to result,the function generate a new attribute list(new
  // schema:output).
  bool copyNewValue(void *tuple, void *result, int length);

  // this function is not used. Because of ExpressionItem.
  bool copyColumn(void *&tuple, ExpressionItem &result, int length);

  // The actual implementation of operations.
  void process_logic(BlockStreamBase *block, project_thread_context *tc);

 private:
  State state_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<BlockStreamIteratorBase>(*this) &
        state_;
  }
};

// }  // namespace physical_query_plan
// }  // namespace claims

#endif  //  PHYSICAL_QUERY_PLAN_PHYSICAL_PROJECT_H_
