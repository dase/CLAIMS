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
 * /CLAIMS/physical_operator/physical_aggregation.h
 *
 *  Created on: Sep 9, 2013
 *      Author: casa cswang
 *       Email: cs_wang@infosys.com
 *
 * Description: Aggregation physical operator, implement interface of Open(),
 *Next(), Close().
 *  multiply threads to process data blocks in one Node by different node type.
 *  this file is about the class BlockStreamAggregationIterator definition.
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_AGGREGATION_H_
#define PHYSICAL_OPERATOR_PHYSICAL_AGGREGATION_H_

#include <vector>
#include <map>
#include "../physical_operator/physical_operator_base.h"
#include "../common/hashtable.h"
#include "../common/hash.h"
#include "../common/ExpandedThreadTracker.h"
#include "../Debug.h"
#include "../utility/lock.h"
#include "../common/Schema/Schema.h"
#include "../common/Expression/queryfunc.h"
#include "../physical_operator/physical_operator.h"

namespace claims {
namespace physical_operator {

/**
 * @brief     Aggregation physical operator
 * @details   physical operator about group by and aggregation,
 *  data will be processed as stream within one block each time from its child
 * operator.
 *  several threads will be created dynamically to process data simultaneously.
 */
class PhysicalAggregation : public PhysicalOperator {
 public:
  class State {
    friend class PhysicalAggregation;

   public:
    enum Aggregation { kSum, kMin, kMax, kCount, kAvg };
    enum AggNodeType { kHybridAggGlobal, kHybridAggPrivate, kNotHybridAgg };
    State(Schema *input, Schema *output, Schema *hash_schema,
          PhysicalOperatorBase *child, std::vector<unsigned> groupby_index,
          std::vector<unsigned> aggregation_index,
          std::vector<Aggregation> aggregations, unsigned num_of_buckets,
          unsigned bucket_size, unsigned block_size,
          std::vector<unsigned> avg_index, AggNodeType agg_node_type);
    State() : hash_schema_(0), input_(0), output_(0), child_(0){};
    ~State(){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &input_ &output_ &hash_schema_ &child_ &index_of_group_by_ &
          aggregation_index_ &aggregations_ &num_of_buckets_ &bucket_size_ &
              block_size_ &avg_index_ &agg_node_type_;
    }

   public:
    Schema *input_;
    Schema *output_;
    Schema *hash_schema_;
    PhysicalOperatorBase *child_;
    std::vector<unsigned> index_of_group_by_;
    std::vector<unsigned> aggregation_index_;
    std::vector<Aggregation> aggregations_;
    unsigned num_of_buckets_;
    unsigned bucket_size_;
    unsigned block_size_;
    std::vector<unsigned> avg_index_;
    AggNodeType agg_node_type_;
  };
  PhysicalAggregation(State state);
  PhysicalAggregation();
  virtual ~PhysicalAggregation();

  bool Open(const PartitionOffset &partition_offset);
  bool Next(BlockStreamBase *block);
  bool Close();
  void Print();

 private:
  /* prepare all sorts of indices to facilitate aggregate*/
  void PrepareIndex();

  /* prepare the aggregation functions */
  void PrepareAggregateFunctions();

 public:
  State state_;

 private:
  BasicHashTable *hashtable_;
  PartitionFunction *hash_;
  std::map<unsigned, unsigned> input_group_by_to_output_;
  std::map<unsigned, unsigned> input_aggregation_to_output_;
  std::vector<fun> global_aggregation_functions_;
  std::vector<fun> private_aggregation_functions_;

  // hashtable traverse and in the next func
  Lock hashtable_cur_lock_;
  unsigned bucket_cur_;
  BasicHashTable::Iterator it_;

  PerformanceInfo *perf_info_;

// unsigned allocated_tuples_in_hashtable;
#ifdef TIME
  unsigned long long timer;
#endif

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

}  // namespace physical_operator
}  // namespace claims

#endif  // PHYSICAL_OPERATOR_PHYSICAL_AGGREGATION_H_
