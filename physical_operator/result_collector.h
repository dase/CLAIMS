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
 * ./physical_operator/result_collector.h
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 *       Email: wangli1426@gmail.com
 */

#ifndef PHYSICAL_QUERY_PLAN_BLOCKSTREAMRESULTCOLLECTOR_H_
#define PHYSICAL_QUERY_PLAN_BLOCKSTREAMRESULTCOLLECTOR_H_
#include <string>
#include <vector>
#include "../utility/lock.h"
#include "../physical_operator/physical_operator_base.h"
#include "../common/Schema/Schema.h"
#include "../common/Block/DynamicBlockBuffer.h"
#include "../Environment.h"

using std::string;
using std::vector;

namespace claims {
namespace physical_operator {
/**
 * @brief ResultCollector is at the top of other physical operators.
 * Corresponding to logical operator-LogicalQueryPlanRoot.
 */
class ResultCollector : public PhysicalOperatorBase {
 public:
  struct State {
    friend class ResultCollector;

   public:
    State(Schema* input, PhysicalOperatorBase* child,
          const unsigned block_size,
          vector<string> column_header = vector<string>(),
          const PartitionOffset partitoin_offset = 0);
    State();
    ~State();

   private:
    Schema* input_;
    PhysicalOperatorBase* child_;
    unsigned block_size_;
    PartitionOffset partition_offset_;
    vector<string> column_header_;

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
      ar& input_& child_& block_size_& partition_offset_& column_header_;
    }
  };
  /**
   * @brief Method description: Usually construct the BlockStreamResultCollector
   * by State.
   */
  ResultCollector();
  ResultCollector(State);
  virtual ~ResultCollector();
  bool Open(const PartitionOffset& part_off = 0);
  bool Next(BlockStreamBase* block);
  bool Close();
  void Print();

  /**
   * @brief Get query result data set.
   * @details The resultset will be automatically freed along with the result
   * collector iterator.
   */
  ResultSet* GetResultSet();
  /**
   * @brief Method description: Get number of tuples in a block.
   * @return long int
   */
  unsigned long GetNumberOftuples() const;

 private:
  bool CreateBlockStream(BlockStreamBase*&) const;
  void DeallocateBlockStream(BlockStreamBase*&) const;
  /**
   * @brief Method description: After calling GetResultSet(), we should judge
   * whether all of child thread has accomplished.
   * @return bool
   */
  bool IsChildExhausted();
  /**
   * @brief Method description: Call child's open() and create a block buffer.
   */
  static void* CollectResult(void* arg);

 private:
  State state_;
  /**
   *  It is the resposibility of the user to free the resultset.
   */
  ResultSet* block_buffer_;
  ResultSet::Iterator block_buffer_iterator_;
  unsigned finished_thread_count_;
  unsigned registered_thread_count_;
  semaphore sema_open_;

  volatile bool open_finished_;
  semaphore sema_open_finished_;
  semaphore sema_input_complete_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this) &
        state_;
  }
};

#endif  // PHYSICAL_QUERY_PLAN_BLOCKSTREAMRESULTCOLLECTOR_H_
}  // namespace physical_operator
}  // namespace claims
