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
#include <vector>

#include "../physical_operator/physical_operator_base.h"
#include "../common/Schema/Schema.h"
#include "../configure.h"
#include "../common/Block/BlockStream.h"
#include "../common/Block/DynamicBlockBuffer.h"
#include "../utility/rdtsc.h"
namespace claims {
namespace physical_operator {

/**
 * @brief Method description: Physical Operator "sort", is used to sort a
 *                            table due to current rules.
 */
class PhysicalSort : public PhysicalOperatorBase {
 public:
  class State {
    friend class PhysicalSort;

   public:
    State();
    /**
     * @brief Method description: Construct the State with paras, not used in
     *                            the current version.
     */
    State(Schema* input, vector<unsigned> orderbyKey,
          PhysicalOperatorBase* child, const unsigned block_size,
          vector<int> direction, const PartitionOffset partitoin_offset = 0);
    State(const State& r)
        : input_(r.input_),
          order_by_key_(r.order_by_key_),
          child_(r.child_),
          block_size_(r.block_size_),
          partition_offset_(r.partition_offset_),
          direction_(r.direction_) {}

   public:
    // TODO(anyone): fixed and varible schema both needed!
    Schema* input_;
    vector<unsigned> order_by_key_;
    PhysicalOperatorBase* child_;
    unsigned block_size_;
    PartitionOffset partition_offset_;
    vector<int> direction_;

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& input_& order_by_key_& child_& block_size_& partition_offset_&
          direction_;
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
  bool Open(const PartitionOffset& part_off = 0);
  /**
   * @brief Method description: Send the sorted data to father operator.
   * @param   BlockStreamBase *block, the info of block
   * @return  false if there's no tuple to function and the block is empty,
   *          otherwise true.
   */
  bool Next(BlockStreamBase* block);
  /**
   * @brief Method description: Close child opertor.
   * @return  true.
   */
  bool Close();
  void Print();

 private:
  void Swap(void*& desc, void*& src);
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
  static bool Compare(const void*, const void*);
  /**
   * @brief Method description: Order the data from the inferior to the prior
   *                            sort method.
   */
  void Order();
  /**
   * @brief Method description: Create a block according to state_.input_ and
   *                            state_.block_size_.
   */
  bool CreateBlockStream(BlockStreamBase*&) const;

 private:
  State state_;
  /* store the data in the buffer!*/
  DynamicBlockBuffer block_buffer_;
  /* create a iterator to traverse the buffer!*/
  DynamicBlockBuffer::Iterator block_buffer_iterator_;

  unsigned finished_thread_count_;
  unsigned registered_thread_count_;
  semaphore sema_open_;

  int swap_num_;
  unsigned temp_cur_;
  volatile bool open_finished_;
  semaphore sema_open_finished_;
  // TODO(anyone): use malloc to get the continuous memory
  vector<void*> tuple_vector_;
  unsigned order_by_key_pos_;
  /**
   *  Because function compare can only operates on static paras, we need those
   *  below.
   */
  static unsigned* sort_order_by_key_pos_;
  static State* sort_state_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};

}  // namespace physical_operator
}  // namespace claims

#endif  // PHYSICAL_OPERATOR_PHYSICAL_SORT_H_
