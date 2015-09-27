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
 * /libeasy/include/afda.h
 *
 *  Created on: Jul 14, 2014
 *      Author: minqi, wangli1426, fzhedu
 *       Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description:
 *
 */

#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamNestLoopJoinIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../IDsGenerator.h"
#include "../Config.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "../common/log/logging.h"
#include "../common/error_define.h"
#include "../logical_query_plan/logical_cross_join.h"

using namespace claims::common;

namespace claims {
namespace logical_query_plan {

LogicalCrossJoin::LogicalCrossJoin()
    : left_child_(NULL),
      right_child_(NULL),
      dataflow_(NULL),
      join_policy_(kUninitialized) {
  set_operator_type(kLogicalCrossJoin);
}

LogicalCrossJoin::LogicalCrossJoin(LogicalOperator* left_child,
                                   LogicalOperator* right_child)
    : left_child_(left_child),
      right_child_(right_child),
      dataflow_(NULL),
      join_policy_(kUninitialized) {
  set_operator_type(kLogicalCrossJoin);
}
LogicalCrossJoin::~LogicalCrossJoin() {
  if (NULL != dataflow_) {
      delete dataflow_;
      dataflow_ = NULL;
  }
  if (NULL != left_child_) {
    delete left_child_;
    left_child_ = NULL;
  }
  if (NULL != right_child_) {
    delete right_child_;
    right_child_ = NULL;
  }
}

/**
 * @brief Method description: get the decided join police
 * @param
 * @return   error no or the decided join police in the cross join
 * @details   (additional)
 */

int LogicalCrossJoin::get_join_policy_() {
  if (kUninitialized != join_policy_) {
    return join_policy_;
  } else {
    LOG(WARNING) << "[CrossJoin]: "
                 << "[" << kErrorMessage[kUninitializedJoinPolicy] << ","
                 << "]" << std::endl;
    return kUninitializedJoinPolicy;
  }
}
/**
 * @brief Method description: return the output of the dataflow of the dedicated
 * operator
 * @param
 * @return
 * @details   (additional)
 */

Dataflow LogicalCrossJoin::GetDataflow() {
  if (NULL != dataflow_) {
    /* the data flow has been computed already！*/
    return *dataflow_;
  }
  Dataflow left_dataflow = left_child_->GetDataflow();
  Dataflow right_dataflow = right_child_->GetDataflow();
  Dataflow ret;
  if (kSuccess == DecideJoinPolicy(left_dataflow, right_dataflow)) {
    const Attribute left_partition_key =
        left_dataflow.property_.partitioner.getPartitionKey();
    const Attribute right_partition_key =
        right_dataflow.property_.partitioner.getPartitionKey();
    ret.attribute_list_.insert(ret.attribute_list_.end(),
                               left_dataflow.attribute_list_.begin(),
                               left_dataflow.attribute_list_.end());
    ret.attribute_list_.insert(ret.attribute_list_.end(),
                               right_dataflow.attribute_list_.begin(),
                               right_dataflow.attribute_list_.end());
    switch (join_policy_) {
      case kLocalJoin: {
        ret.property_.partitioner.setPartitionList(
            left_dataflow.property_.partitioner.getPartitionList());
        ret.property_.partitioner.setPartitionFunction(
            left_dataflow.property_.partitioner.getPartitionFunction());
        ret.property_.partitioner.setPartitionKey(left_partition_key);
        ret.property_.partitioner.addShadowPartitionKey(right_partition_key);
        const unsigned long l_cardinality =
            left_dataflow.getAggregatedDataCardinality();
        const unsigned long r_cardinality =
            right_dataflow.getAggregatedDataCardinality();
        ret.property_.partitioner.getPartition(0)
            ->setDataCardinality(l_cardinality * r_cardinality);
        ret.property_.commnication_cost =
            left_dataflow.property_.commnication_cost +
            right_dataflow.property_.commnication_cost;
        break;
      }
      case kLeftBroadcast: {
        ret.property_.partitioner.setPartitionList(
            right_dataflow.property_.partitioner.getPartitionList());
        ret.property_.partitioner.setPartitionFunction(
            right_dataflow.property_.partitioner.getPartitionFunction());
        ret.property_.partitioner.setPartitionKey(right_partition_key);
        const unsigned long l_cardinality =
            left_dataflow.getAggregatedDataCardinality();
        for (unsigned i = 0;
             i < right_dataflow.property_.partitioner.getNumberOfPartitions();
             i++) {
          const unsigned r_cardinality =
              right_dataflow.property_.partitioner.getPartition(i)
                  ->getDataCardinality();
          ret.property_.partitioner.getPartition(i)
              ->setDataCardinality(l_cardinality * r_cardinality);
        }
        ret.property_.commnication_cost =
            left_dataflow.property_.commnication_cost +
            right_dataflow.property_.commnication_cost;
        ret.property_.commnication_cost +=
            left_dataflow.getAggregatedDatasize() *
            right_dataflow.property_.partitioner.getNumberOfPartitions();
        break;
      }
      case kRightBroadcast: {
        ret.property_.partitioner.setPartitionList(
            left_dataflow.property_.partitioner.getPartitionList());
        ret.property_.partitioner.setPartitionFunction(
            left_dataflow.property_.partitioner.getPartitionFunction());
        ret.property_.partitioner.setPartitionKey(left_partition_key);
        const unsigned long r_cardinality =
            left_dataflow.getAggregatedDataCardinality();
        for (unsigned i = 0;
             i < left_dataflow.property_.partitioner.getNumberOfPartitions();
             i++) {
          const unsigned l_cardinality =
              left_dataflow.property_.partitioner.getPartition(i)
                  ->getDataCardinality();
          ret.property_.partitioner.getPartition(i)
              ->setDataCardinality(l_cardinality * r_cardinality);
        }
        ret.property_.commnication_cost =
            right_dataflow.property_.commnication_cost +
            left_dataflow.property_.commnication_cost;
        ret.property_.commnication_cost +=
            right_dataflow.getAggregatedDatasize() *
            left_dataflow.property_.partitioner.getNumberOfPartitions();
        break;
      }
      default: { assert(false); }
    }
    dataflow_ = new Dataflow();
    *dataflow_ = ret;
    return ret;
  } else {
    LOG(WARNING) << "[CROSS JOIN]:"
                 << "[" << kErrorMessage[kGenerateDataflowFailed] << "],"
                 << std::endl;
    return ret;
  }
}

/**
 * @brief Method description: decide the join policy of the cross join based on
* the partition of the child operator
* @param
* @return
* @details   (additional)
*/

int LogicalCrossJoin::DecideJoinPolicy(const Dataflow& left_dataflow,
                                       const Dataflow& right_dataflow) {
  if (CanLocalJoin(left_dataflow, right_dataflow)) {
    join_policy_ = kLocalJoin;
  } else {
    if (left_dataflow.getAggregatedDatasize() <
        right_dataflow.getAggregatedDatasize()) {
      /** left input is the smaller one **/
      join_policy_ = kLeftBroadcast;
    } else {
      join_policy_ = kRightBroadcast;
    }
  }
  if (kUninitialized != join_policy_) {
    return kSuccess;
  } else {
    LOG(WARNING) << "[CROSS JOIN]:"
                 << "[" << kErrorMessage[kUninitializedJoinPolicy] << ",]"
                 << std::endl;
    return kUninitializedJoinPolicy;
  }
}

/**
 * @brief Method description: get the sub physical execution plan of the current
* local operator
* @param
* @return
* @details   (additional)
*/

BlockStreamIteratorBase* LogicalCrossJoin::GetIteratorTree(
    const unsigned& block_size) {
  if (NULL == dataflow_) {
    GetDataflow();
  }
  BlockStreamNestLoopJoinIterator* cross_join_iterator = NULL;
  BlockStreamIteratorBase* child_iterator_left = NULL;
  BlockStreamIteratorBase* child_iterator_right = NULL;
  GenerateChildPhysicalQueryPlan(child_iterator_left, child_iterator_right,
                                 block_size);
  Dataflow dataflow_left = left_child_->GetDataflow();
  Dataflow dataflow_right = right_child_->GetDataflow();
  BlockStreamNestLoopJoinIterator::State state;
  state.block_size_ = block_size;
  state.input_schema_left = GetSchema(dataflow_left.attribute_list_);
  state.input_schema_right = GetSchema(dataflow_right.attribute_list_);
  state.output_schema = GetSchema(dataflow_->attribute_list_);
  state.child_left = child_iterator_left;
  state.child_right = child_iterator_right;
  cross_join_iterator = new BlockStreamNestLoopJoinIterator(state);
  return cross_join_iterator;
}

int LogicalCrossJoin::GenerateChildPhysicalQueryPlan(
    const BlockStreamIteratorBase* left_child_iterator_tree,
    const BlockStreamIteratorBase* right_child_iterator_tree,
    const unsigned& blocksize) {
  int ret = kSuccess;
  Dataflow left_dataflow = left_child_->GetDataflow();
  Dataflow right_dataflow = right_child_->GetDataflow();
  switch (join_policy_) {
    case kLocalJoin: {
      left_child_iterator_tree = left_child_->GetIteratorTree(blocksize);
      right_child_iterator_tree = right_child_->GetIteratorTree(blocksize);
      break;
    }
    case kLeftBroadcast: {
      BlockStreamExpander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = blocksize;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = left_child_->GetIteratorTree(blocksize);
      expander_state.schema_ = left_dataflow.getSchema();
      BlockStreamIteratorBase* expander =
          new BlockStreamExpander(expander_state);
      ExpandableBlockStreamExchangeEpoll::State exchange_state;
      exchange_state.block_size_ = blocksize;
      exchange_state.child_ = expander;
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      std::vector<NodeID> upper_id_list =
          GetInvolvedNodeID(dataflow_->property_.partitioner);
      exchange_state.upper_id_list_ = upper_id_list;
      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(left_dataflow.property_.partitioner);
      exchange_state.lower_id_list_ = lower_id_list;
      exchange_state.partition_schema_ =
          partition_schema::set_broadcast_partition();
      exchange_state.schema_ = left_dataflow.getSchema();
      BlockStreamIteratorBase* exchange =
          new ExpandableBlockStreamExchangeEpoll(exchange_state);
      left_child_iterator_tree = exchange;
      right_child_iterator_tree = right_child_->GetIteratorTree(blocksize);
      break;
    }
    case kRightBroadcast: {
      left_child_iterator_tree = left_child_->GetIteratorTree(blocksize);
      BlockStreamExpander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = blocksize;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = right_child_->GetIteratorTree(blocksize);
      expander_state.schema_ = left_dataflow.getSchema();
      BlockStreamIteratorBase* expander =
          new BlockStreamExpander(expander_state);
      ExpandableBlockStreamExchangeEpoll::State exchange_state;
      exchange_state.block_size_ = blocksize;
      exchange_state.child_ = expander;
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      std::vector<NodeID> upper_id_list =
          GetInvolvedNodeID(dataflow_->property_.partitioner);
      exchange_state.upper_id_list_ = upper_id_list;
      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(right_dataflow.property_.partitioner);
      exchange_state.lower_id_list_ = lower_id_list;
      exchange_state.partition_schema_ =
          partition_schema::set_broadcast_partition();
      exchange_state.schema_ = right_dataflow.getSchema();
      BlockStreamIteratorBase* exchange =
          new ExpandableBlockStreamExchangeEpoll(exchange_state);
      right_child_iterator_tree = exchange;
      break;
    }
    default: { assert(false); }
      if (NULL == left_child_iterator_tree) {
        ret = kGenerateSubPhyPlanFailed;
        LOG(WARNING) << "[CrossJoin]: "
                     << "[" << kErrorMessage[kGenerateSubPhyPlanFailed] << ","
                     << "left child sub physical plan"
                     << "]" << std::endl;
      }
      if (NULL == right_child_iterator_tree) {
        ret = kGenerateSubPhyPlanFailed;
        LOG(WARNING) << "[CrossJoin]: "
                     << "[" << kErrorMessage[kGenerateSubPhyPlanFailed] << ","
                     << "right child sub physical plan"
                     << "]" << std::endl;
      }
  }
  return ret;
}

void LogicalCrossJoin::Print(int level) const {
  printf("CrossJoin:\n", level * 8, " ");
  switch (join_policy_) {
    case kLeftBroadcast: {
      printf(" left_broadcast\n");
      break;
    }
    case kRightBroadcast: {
      printf(" right_broadcast\n");
      break;
    }
    case kLocalJoin: {
      printf(" loca_join\n");
      break;
    }
  }
  left_child_->Print(level + 1);
  right_child_->Print(level + 1);
}

/**
 * @brief Method description: To test the join operator in this query weather
 * can be issued on local node or not?
 * The criterion is based on the number of partitions generated from the child
 * operator.
 * if the number of partitions from both of the left and right are 1, then it is
 * joined locally
 * @param left left child operator dataflow
 * @param ritht right child operator dataflow
 * @return
 * @details   (additional)
 */

bool LogicalCrossJoin::CanLocalJoin(const Dataflow& left,
                                    const Dataflow& right) const {
  if (left.property_.partitioner.getNumberOfPartitions() > 1) return false;
  if (right.property_.partitioner.getNumberOfPartitions() > 1) return false;
  return left.property_.partitioner.getPartitionList()[0].getLocation() ==
         right.property_.partitioner.getPartitionList()[0].getLocation();
}
}  // namespace logical_query_plan
}  // namespace claims
