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
 * /CLAIMS/physical_query_plan/physical_filter.h
 *
 *  Created on: Aug 28, 2013
 *      Author: wangli, Hanzhang
 *		   Email: wangli1426@gmail.com
 *
 * Description: Implementation of Filter operator in physical layer.
 *
 */

#ifndef PHYSICAL_QUERY_PLAN_PHYSICAL_FILTER_H_
#define PHYSICAL_QUERY_PLAN_PHYSICAL_FILTER_H_

#include <list>
#include <map>
#include <string>
#include <boost/serialization/map.hpp>

#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../physical_query_plan/physical_operator.h"
#include "../../common/Schema/Schema.h"
#include "../../common/Comparator.h"
#include "../../common/Block/BlockStream.h"
#include "../../utility/lock.h"
#include "../../common/AttributeComparator.h"
#include "../../common/ExpressionItem.h"
#include "../../common/Mapping.h"
#include "../../Catalog/Attribute.h"
#include "../physical_query_plan/physical_project.h"
#include "../../common/Expression/qnode.h"
#include "../../codegen/ExpressionGenerator.h"

// namespace claims {
// namespace physical_query_plan {
class ExpandableBlockStreamFilter : public PhysicalOperator {
 public:
  class filter_thread_context : public ThreadContext {
   public:
    BlockStreamBase* block_for_asking_;
    BlockStreamBase* temp_block_;
    BlockStreamBase::BlockStreamTraverseIterator* block_stream_iterator_;
    vector<QNode*> thread_qual_;
    ~filter_thread_context();
  };

  /**
   * @brief Method description: struct to hold the remaining data when the next
   * is returned but the block from the child iterator is not exhausted.
   */
  struct remaining_block {
    remaining_block(BlockStreamBase* block,
                    BlockStreamBase::BlockStreamTraverseIterator* iterator)
        : block(block), iterator(iterator){};
    remaining_block() : block(NULL), iterator(NULL){};
    BlockStreamBase* block;
    BlockStreamBase::BlockStreamTraverseIterator* iterator;
  };

  /**
   * @brief Method description: Obtain information from logical layer
   */
  class State {
   public:
    friend class ExpandableBlockStreamFilter;
    State(Schema* schema, BlockStreamIteratorBase* child, vector<QNode*> qual,
          map<string, int> colindex, unsigned block_size);
    State(Schema* s, BlockStreamIteratorBase* child,
          std::vector<AttributeComparator> comparator_list,
          unsigned block_size);
    State(){};

   public:
    Schema* schema_;
    BlockStreamIteratorBase* child_;
    unsigned block_size_;
    vector<QNode*> qual_;
    std::vector<AttributeComparator> comparator_list_;
    vector<ExpressItem_List> v_ei_;
    map<string, int> colindex_;

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& child_& block_size_& qual_& comparator_list_& colindex_;
    }
  };

  ExpandableBlockStreamFilter(State state);
  ExpandableBlockStreamFilter();
  virtual ~ExpandableBlockStreamFilter();
  bool Open(const PartitionOffset& part_off);
  bool Next(BlockStreamBase* block);
  bool Close();
  void Print();

 private:
  void process_logic(BlockStreamBase* block, filter_thread_context* tc);

 private:
  ThreadContext* CreateContext();
  typedef void (*filter_func)(bool& ret, void* tuple, expr_func func_gen,
                              Schema* schema, vector<QNode*> thread_qual_);

  // traditional optimized function to generate filter function.
  static void ComputeFilter(bool& ret, void* tuple, expr_func func_gen,
                            Schema* schema, vector<QNode*> thread_qual_);
  // llvm optimized function which be used to tuples.
  static void ComputeFilterwithGeneratedCode(bool& ret, void* tuple,
                                             expr_func func_gen, Schema* schema,
                                             vector<QNode*>);

 private:
  State state_;
  map<string, int> colindex;

  unsigned long tuple_after_filter_;
  Lock lock_;

  filter_func ff_;
  // optimization of tuple:(llvm or none)
  expr_func generated_filter_function_;
  // optimization of block:(llvm or none)
  filter_process_func generated_filter_processing_fucntoin_;

  /* the following code is for boost serialization*/
 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<BlockStreamIteratorBase>(*this) &
        state_;
  }
};
//}  // physical_query_plan
//}  // claims

#endif  //  PHYSICAL_QUERY_PLAN_PHYSICAL_FILTER_H_
