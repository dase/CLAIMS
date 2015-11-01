
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
#define GLOG_NO_ABBREVIATED_SEVERITIES

#include "../Config.h"
#include "../IDsGenerator.h"
#include "../Resource/NodeTracker.h"
#include "../common/log/logging.h"
#include "../physical_operator/expander.h"
#include "../common/error_define.h"
#include "../physical_operator/exchange_merger.h"
#include "../logical_operator/logical_cross_join.h"
#include "../physical_operator/physical_nest_loop_join.h"
using claims::physical_operator::ExchangeMerger;
using claims::physical_operator::Expander;
using claims::physical_operator::PhysicalNestLoopJoin;
using namespace claims::common;

namespace claims {
namespace logical_operator {

LogicalCrossJoin::LogicalCrossJoin()
    : left_child_(NULL),
      right_child_(NULL),
      plan_context_(NULL),
      join_policy_(kUninitialized) {
  set_operator_type(kLogicalCrossJoin);
}

LogicalCrossJoin::LogicalCrossJoin(LogicalOperator* left_child,
                                   LogicalOperator* right_child)
    : left_child_(left_child),
      right_child_(right_child),
      plan_context_(NULL),
      join_policy_(kUninitialized) {
  set_operator_type(kLogicalCrossJoin);
}
LogicalCrossJoin::~LogicalCrossJoin() {
  if (NULL != plan_context_) {
    delete plan_context_;
    plan_context_ = NULL;
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
 * @brief Method description: return the output of the plan context of the
 * dedicated operator
 * @param
 * @return
 * @details   (additional)
 */

PlanContext LogicalCrossJoin::GetPlanContext() {
  if (NULL != plan_context_) {
    /* the plan context has been computed already！*/
    return *plan_context_;
  }
  PlanContext left_plan_context = left_child_->GetPlanContext();
  PlanContext right_plan_context = right_child_->GetPlanContext();
  PlanContext ret;
  if (kSuccess == DecideJoinPolicy(left_plan_context, right_plan_context)) {
    const Attribute left_partition_key =
        left_plan_context.plan_partitioner_.get_partition_key();
    const Attribute right_partition_key =
        right_plan_context.plan_partitioner_.get_partition_key();
    ret.attribute_list_.insert(ret.attribute_list_.end(),
                               left_plan_context.attribute_list_.begin(),
                               left_plan_context.attribute_list_.end());
    ret.attribute_list_.insert(ret.attribute_list_.end(),
                               right_plan_context.attribute_list_.begin(),
                               right_plan_context.attribute_list_.end());
    switch (join_policy_) {
      case kLocalJoin: {
        ret.plan_partitioner_.set_partition_list(
            left_plan_context.plan_partitioner_.get_partition_list());
        ret.plan_partitioner_.set_partition_func(
            left_plan_context.plan_partitioner_.get_partition_func());
        ret.plan_partitioner_.set_partition_key(left_partition_key);
        ret.plan_partitioner_.AddShadowPartitionKey(right_partition_key);
        const unsigned long l_cardinality =
            left_plan_context.GetAggregatedDataCardinality();
        const unsigned long r_cardinality =
            right_plan_context.GetAggregatedDataCardinality();
        ret.plan_partitioner_.GetPartition(0)
            ->set_cardinality(l_cardinality * r_cardinality);
        ret.commu_cost_ =
            left_plan_context.commu_cost_ + right_plan_context.commu_cost_;
        break;
      }
      case kLeftBroadcast: {
        ret.plan_partitioner_.set_partition_list(
            right_plan_context.plan_partitioner_.get_partition_list());
        ret.plan_partitioner_.set_partition_func(
            right_plan_context.plan_partitioner_.get_partition_func());
        ret.plan_partitioner_.set_partition_key(right_partition_key);
        const unsigned long l_cardinality =
            left_plan_context.GetAggregatedDataCardinality();
        for (unsigned i = 0;
             i < right_plan_context.plan_partitioner_.GetNumberOfPartitions();
             i++) {
          const unsigned r_cardinality =
              right_plan_context.plan_partitioner_.GetPartition(i)
                  ->get_cardinality();
          ret.plan_partitioner_.GetPartition(i)
              ->set_cardinality(l_cardinality * r_cardinality);
        }
        ret.commu_cost_ =
            left_plan_context.commu_cost_ + right_plan_context.commu_cost_;
        ret.commu_cost_ +=
            left_plan_context.GetAggregatedDatasize() *
            right_plan_context.plan_partitioner_.GetNumberOfPartitions();
        break;
      }
      case kRightBroadcast: {
        ret.plan_partitioner_.set_partition_list(
            left_plan_context.plan_partitioner_.get_partition_list());
        ret.plan_partitioner_.set_partition_func(
            left_plan_context.plan_partitioner_.get_partition_func());
        ret.plan_partitioner_.set_partition_key(left_partition_key);
        const unsigned long r_cardinality =
            left_plan_context.GetAggregatedDataCardinality();
        for (unsigned i = 0;
             i < left_plan_context.plan_partitioner_.GetNumberOfPartitions();
             i++) {
          const unsigned l_cardinality =
              left_plan_context.plan_partitioner_.GetPartition(i)
                  ->get_cardinality();
          ret.plan_partitioner_.GetPartition(i)
              ->set_cardinality(l_cardinality * r_cardinality);
        }
        ret.commu_cost_ =
            right_plan_context.commu_cost_ + left_plan_context.commu_cost_;
        ret.commu_cost_ +=
            right_plan_context.GetAggregatedDatasize() *
            left_plan_context.plan_partitioner_.GetNumberOfPartitions();
        break;
      }
      default: { assert(false); }
    }
    plan_context_ = new PlanContext();
    *plan_context_ = ret;
    return ret;
  } else {
    LOG(WARNING) << "[CROSS JOIN]:"
                 << "[" << kErrorMessage[kGeneratePlanContextFailed] << "],"
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

int LogicalCrossJoin::DecideJoinPolicy(const PlanContext& left_plan_context,
                                       const PlanContext& right_plan_context) {
  if (CanLocalJoin(left_plan_context, right_plan_context)) {
    join_policy_ = kLocalJoin;
  } else {
    if (left_plan_context.GetAggregatedDatasize() <
        right_plan_context.GetAggregatedDatasize()) {
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

PhysicalOperatorBase* LogicalCrossJoin::GetPhysicalPlan(
    const unsigned& block_size) {
  if (NULL == plan_context_) {
    GetPlanContext();
  }
  PhysicalNestLoopJoin* cross_join_iterator = NULL;
  PhysicalOperatorBase* child_iterator_left = NULL;
  PhysicalOperatorBase* child_iterator_right = NULL;
  GenerateChildPhysicalQueryPlan(child_iterator_left, child_iterator_right,
                                 block_size);
  PlanContext left_plan_context = left_child_->GetPlanContext();
  PlanContext right_plan_context = right_child_->GetPlanContext();
  PhysicalNestLoopJoin::State state;
  state.block_size_ = block_size;
  state.input_schema_left_ = GetSchema(left_plan_context.attribute_list_);
  state.input_schema_right_ = GetSchema(right_plan_context.attribute_list_);
  state.output_schema_ = GetSchema(plan_context_->attribute_list_);
  state.child_left_ = child_iterator_left;
  state.child_right_ = child_iterator_right;
  cross_join_iterator = new PhysicalNestLoopJoin(state);
  return cross_join_iterator;
}

int LogicalCrossJoin::GenerateChildPhysicalQueryPlan(
    PhysicalOperatorBase*& left_child_iterator_tree,
    PhysicalOperatorBase*& right_child_iterator_tree,
    const unsigned& blocksize) {
  int ret = kSuccess;
  PlanContext left_plan_context = left_child_->GetPlanContext();
  PlanContext right_plan_context = right_child_->GetPlanContext();
  switch (join_policy_) {
    case kLocalJoin: {
      left_child_iterator_tree = left_child_->GetPhysicalPlan(blocksize);
      right_child_iterator_tree = right_child_->GetPhysicalPlan(blocksize);
      break;
    }
    case kLeftBroadcast: {
      Expander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = blocksize;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = left_child_->GetPhysicalPlan(blocksize);
      expander_state.schema_ = left_plan_context.GetSchema();
      PhysicalOperatorBase* expander = new Expander(expander_state);
      ExchangeMerger::State exchange_state;
      exchange_state.block_size_ = blocksize;
      exchange_state.child_ = expander;
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      std::vector<NodeID> upper_id_list =
          GetInvolvedNodeID(plan_context_->plan_partitioner_);
      exchange_state.upper_id_list_ = upper_id_list;
      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(left_plan_context.plan_partitioner_);
      exchange_state.lower_id_list_ = lower_id_list;
      exchange_state.partition_schema_ =
          partition_schema::set_broadcast_partition();
      exchange_state.schema_ = left_plan_context.GetSchema();
      PhysicalOperatorBase* exchange = new ExchangeMerger(exchange_state);
      left_child_iterator_tree = exchange;
      right_child_iterator_tree = right_child_->GetPhysicalPlan(blocksize);
      break;
    }
    case kRightBroadcast: {
      left_child_iterator_tree = left_child_->GetPhysicalPlan(blocksize);
      Expander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = blocksize;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = right_child_->GetPhysicalPlan(blocksize);
      expander_state.schema_ = left_plan_context.GetSchema();
      PhysicalOperatorBase* expander = new Expander(expander_state);
      ExchangeMerger::State exchange_state;
      exchange_state.block_size_ = blocksize;
      exchange_state.child_ = expander;
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      std::vector<NodeID> upper_id_list =
          GetInvolvedNodeID(plan_context_->plan_partitioner_);
      exchange_state.upper_id_list_ = upper_id_list;
      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(right_plan_context.plan_partitioner_);
      exchange_state.lower_id_list_ = lower_id_list;
      exchange_state.partition_schema_ =
          partition_schema::set_broadcast_partition();
      exchange_state.schema_ = right_plan_context.GetSchema();
      PhysicalOperatorBase* exchange = new ExchangeMerger(exchange_state);
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
 * @param  left child operator plan context
 * @param  right child operator plan context
 * @return
 * @details   (additional)
 */

bool LogicalCrossJoin::CanLocalJoin(
    const PlanContext& left_plan_context,
    const PlanContext& right_plan_context) const {
  if (left_plan_context.plan_partitioner_.GetNumberOfPartitions() > 1)
    return false;
  if (right_plan_context.plan_partitioner_.GetNumberOfPartitions() > 1)
    return false;
  return left_plan_context.plan_partitioner_.get_partition_list()[0]
             .get_location() ==
         right_plan_context.plan_partitioner_.get_partition_list()[0]
             .get_location();
}
}  // namespace logical_operator
}  // namespace claims
