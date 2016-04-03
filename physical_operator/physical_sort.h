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
 * /CLAIMS/physical_operator/physical_sort.h
 *
 *  Created on: Feb 17, 2014
 *      Author: zhanglei ,tonglanxuan
 *        Email: lxtong0526@163.com
 *
 * Description: Structures the PhysicalSort class, which functions as physical
 *              operator "Sort".
 *
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_SORT_H_
#define PHYSICAL_OPERATOR_PHYSICAL_SORT_H_

#include <algorithm>
#include <iterator>
#include <stack>
#include <utility>
#include <vector>

#include "../physical_operator/physical_operator.h"
#include "../common/Schema/Schema.h"
#include "../configure.h"
#include "../common/Block/BlockStream.h"
#include "../common/Block/DynamicBlockBuffer.h"
#include "../common/expression/expr_node.h"

#include "../utility/lock.h"
#include "../utility/rdtsc.h"
using claims::common::DataTypeOperFunc;
using std::vector;
using std::pair;
using claims::common::ExprNode;
using claims::common::OperFuncInfo;

namespace claims {
namespace physical_operator {
#define NEWCONDI
/**
 * @brief Method description: Physical Operator "sort", is used to sort a
 *                            table due to current rules.
 */
class PhysicalSort : public PhysicalOperator {
 public:
  class State {
    friend class PhysicalSort;

   public:
    State();
    /**
     * @brief Method description: Construct the State with paras, not used in
     *                            the current version.
     */
    State(Schema* input_schema, PhysicalOperatorBase* child,
          const unsigned block_size,
          vector<std::pair<ExprNode*, int>> order_by_attrs,
          const PartitionOffset partitoin_offset = 0);
    State(const State& r)
        : input_schema_(r.input_schema_),
          child_(r.child_),
          block_size_(r.block_size_),
          partition_offset_(r.partition_offset_),
          order_by_attrs_(r.order_by_attrs_) {}

   public:
    Schema* input_schema_;
    PhysicalOperatorBase* child_;
    unsigned block_size_;
    PartitionOffset partition_offset_;
    vector<std::pair<ExprNode*, int>> order_by_attrs_;
    vector<std::pair<ExprNode*, int>> order_by_attrs_copy_;

    DataTypeOperFunc (*compare_funcs_)[2];

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& input_schema_& child_& block_size_& partition_offset_&
          order_by_attrs_;
    }
  };

  PhysicalSort();
  PhysicalSort(State state);
  virtual ~PhysicalSort();
  /**
   * @brief Method description: Get tuples from child operator, store the data
   *                            in the buffer, and sort them with given method.
   * @param   const PartitionOffset& partition_offset, to identify which
   *          partiton the function operates on.
   * @return  true in all cases.
   */
  bool Open(SegmentExecStatus* const exec_status,
            const PartitionOffset& part_off = 0);
  /**
   * @brief Method description: Send the sorted data to father operator.
   * @param   BlockStreamBase *block, the info of block
   * @return  false if there's no tuple to function and the block is empty,
   *          otherwise true.
   */
  bool Next(SegmentExecStatus* const exec_status, BlockStreamBase* block);
  /**
   * @brief Method description: Close child opertor.
   * @return  true.
   */
  bool Close();
  void Print();

 private:
  /**
   * @brief Method description: To compare the two tuples by the column and the
   *                            compare type(asc/desc) of a sort method.
   * @param   two tuple pointers.
   * @return  static boolean values.
   * @details   (additional)    Since stable_sort()need a static bool function
   *                            compare, we should make it static, and it can
   *                            only visit static paras, so we introduce static
   *                            pointers sort_order_by_key_pos and sort_state_.
   */
  static bool Compare(void*, void*);
  /**
   * @brief Method description: Order the data from the inferior to the prior
   *                            sort method.
   */
  void Order();
  /**
   * @brief Method description: Create a block according to state_.input_ and
   *                            state_.block_size_.
   */
  bool CreateBlock(BlockStreamBase*&) const;
  RetCode GetAllSegments(stack<Segment*>* all_segments);

 private:
  State state_;
  /* store the data in the buffer!*/
  DynamicBlockBuffer block_buffer_;
  unsigned all_cur_;
  int64_t thread_id_;
  vector<void*> all_tuples_;
  // TODO(anyone): use malloc to get the continuous memory
  Lock* lock_;
  /**
   *  Because function compare can only operates on static paras, we need those
   *  below.
   */
  static State* cmp_state_;
  static OperFuncInfo fcinfo;
  static unsigned order_by_pos_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

}  // namespace physical_operator
}  // namespace claims

#endif  // PHYSICAL_OPERATOR_PHYSICAL_SORT_H_
