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
 * /CLAIMS/physical_operator/physical_filter.h
 *
 *  Created on: Aug 28, 2013
 *      Author: wangli, Hanzhang
 *		   Email: wangli1426@gmail.com
 *
 * Description: Implementation of Filter operator in physical layer.
 *
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_FILTER_H_
#define PHYSICAL_OPERATOR_PHYSICAL_FILTER_H_

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <boost/serialization/map.hpp>
#include <glog/logging.h>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/physical_operator.h"
#include "../common/Schema/Schema.h"
#include "../common/Comparator.h"
#include "../common/Block/BlockStream.h"
#include "../utility/lock.h"
#include "../common/AttributeComparator.h"
#include "../common/ExpressionItem.h"
#include "../common/Mapping.h"
#include "../Catalog/Attribute.h"
#include "../physical_operator/physical_project.h"
#include "../common/Expression/qnode.h"
#include "../codegen/ExpressionGenerator.h"
#include "../common/error_no.h"

namespace claims {
namespace physical_operator {
/**
 * @brief Method description: According to expression function, we generate
 * filter function to remove tuple which does not match condition.
 */
class PhysicalFilter : public PhysicalOperator {
 public:
  class FilterThreadContext : public ThreadContext {
   public:
    BlockStreamBase* block_for_asking_;
    BlockStreamBase* temp_block_;
    BlockStreamBase::BlockStreamTraverseIterator* block_stream_iterator_;
    vector<QNode*> thread_qual_;
    ~FilterThreadContext();
  };

  /**
   * @brief Method description: struct to hold the remaining data when the
   * next is returned but the block from the child iterator is not exhausted.
   */
  struct RemainingBlock {
    RemainingBlock(BlockStreamBase* block,
                   BlockStreamBase::BlockStreamTraverseIterator* iterator)
        : block(block), iterator(iterator){};
    RemainingBlock() : block(NULL), iterator(NULL){};
    BlockStreamBase* block;
    BlockStreamBase::BlockStreamTraverseIterator* iterator;
  };

  /**
   * @brief Method description: Obtain information from logical layer
   */
  class State {
   public:
    friend class PhysicalFilter;
    State(Schema* schema, PhysicalOperatorBase* child, vector<QNode*> qual,
          map<string, int> column_id, unsigned block_size);
    State(Schema* s, PhysicalOperatorBase* child,
          std::vector<AttributeComparator> comparator_list,
          unsigned block_size);
    State(){};

   public:
    Schema* schema_;
    PhysicalOperatorBase* child_;
    unsigned block_size_;
    vector<QNode*> qual_;
    std::vector<AttributeComparator> comparator_list_;
    map<string, int> column_id_;

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& child_& block_size_& qual_& comparator_list_& column_id_;
    }
  };

  PhysicalFilter(State state);
  PhysicalFilter();
  virtual ~PhysicalFilter();

  /**
   * @brief: choose which way to generate filter function
   */
  bool Open(const PartitionOffset& kPartitionOffset);

  /**
   * @brief: fetch a block from child and execute ProcessInLogic
   */
  bool Next(BlockStreamBase* block);

  /**
   * @brief: revoke resource
   */
  bool Close();
  void Print();

 private:
  /**
   * @brief Method description: The actual implementation of operations.
   */
  void ProcessInLogic(BlockStreamBase* block, FilterThreadContext* tc);
  /**
   * @brief Method description:Initialize project thread context with
   * state(Class).
   * @return a pointer(FilterThreadContext)
  */
  ThreadContext* CreateContext();
  typedef void (*filter_func)(bool& ret, void* tuple, expr_func func_gen,
                              Schema* schema, vector<QNode*> thread_qual_);

  /**
   * @brief Method description: traditional optimized function to generate
   * filter function.
   * @return  a filter function
   */
  static void ComputeFilter(bool& ret, void* tuple, expr_func func_gen,
                            Schema* schema, vector<QNode*> thread_qual_);
  /**
   * @brief Method description:  llvm optimized function which be used to
   * tuples.
   * @return  a filter function
   */
  static void ComputeFilterWithGeneratedCode(bool& ret, void* tuple,
                                             expr_func func_gen, Schema* schema,
                                             vector<QNode*>);
  // return error_no.
  int DecideFilterFunction(expr_func const& generate_filter_function);

 private:
  State state_;

  unsigned long tuple_after_filter_;
  Lock lock_;

  filter_func filter_function_;
  // optimization of tuple:(llvm or none)
  expr_func generated_filter_function_;
  // optimization of block:(llvm or none)
  filter_process_func generated_filter_processing_fucntoin_;

  /* the following code is for boost serialization*/
 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};
}  // namespace physical_operator
}  // namespace claims

#endif  //  PHYSICAL_OPERATOR_PHYSICAL_FILTER_H_
