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
#include <stack>

#include "../common/error_define.h"
#include "../common/expression/expr_node.h"
#include "../common/expression/expr_unary.h"
#include "../physical_operator/physical_operator_base.h"
#include "../common/hashtable.h"
#include "../common/hash.h"
#include "../common/ExpandedThreadTracker.h"
#include "../Debug.h"
#include "../utility/lock.h"
#include "../common/Schema/Schema.h"
#include "../common/Expression/queryfunc.h"
#include "../physical_operator/physical_operator.h"

using claims::common::ExprUnary;
using claims::common::ExprNode;

namespace claims {
namespace physical_operator {
#define NEWCONDI

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
    enum AggNodeType { kHybridAggGlobal, kHybridAggLocal, kNotHybridAgg };
    State(Schema *input, Schema *output, Schema *hash_schema,
          PhysicalOperatorBase *child, unsigned num_of_buckets,
          unsigned bucket_size, unsigned block_size,
          std::vector<unsigned> avg_index, AggNodeType agg_node_type,
          vector<ExprNode *> group_by_attrs,
          vector<ExprUnary *> aggregation_attrs, int count_column_id);
    State() : hash_schema_(0), input_schema_(0), output_schema_(0), child_(0){};
    ~State(){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &input_schema_ &output_schema_ &hash_schema_ &child_ &num_of_buckets_ &
          bucket_size_ &block_size_ &avg_index_ &agg_node_type_ &
              group_by_attrs_ &aggregation_attrs_ &count_column_id_;
    }

   public:
    Schema *input_schema_;
    Schema *output_schema_;
    Schema *hash_schema_;
    PhysicalOperatorBase *child_;
    unsigned num_of_buckets_;
    unsigned bucket_size_;
    unsigned block_size_;
    std::vector<unsigned> avg_index_;
    AggNodeType agg_node_type_;
    vector<ExprNode *> group_by_attrs_;
    vector<ExprUnary *> aggregation_attrs_;
    int count_column_id_;
  };
  PhysicalAggregation(State state);
  PhysicalAggregation();
  virtual ~PhysicalAggregation();

  bool Open(SegmentExecStatus *const exec_status,
            const PartitionOffset &partition_offset);
  bool Next(SegmentExecStatus *const exec_status, BlockStreamBase *block);
  bool Close(SegmentExecStatus *const exec_status);
  void Print();
  RetCode GetAllSegments(stack<Segment *> *all_segments);

 public:
  State state_;

 private:
  BasicHashTable *hashtable_;
  PartitionFunction *hash_;
  BlockStreamBase *block_for_asking;
  BasicHashTable *private_hashtable;
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
