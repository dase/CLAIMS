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
 * /Claims/logical_operator/logical_query_plan_root.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: wangli, yukai
 *		 Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "../logical_operator/logical_query_plan_root.h"

#include <vector>
#include <string>

#include "../Config.h"
#include "../IDsGenerator.h"
#include "../logical_operator/logical_operator.h"
#include "../physical_operator/exchange_merger.h"
#include "../physical_operator/expander.h"
#include "../physical_operator/performance_monitor.h"
#include "../physical_operator/physical_limit.h"
#include "../physical_operator/result_collector.h"
#include "../physical_operator/result_printer.h"
#include "../physical_operator/physical_operator_base.h"
#include "../Resource/NodeTracker.h"

using claims::physical_operator::ExchangeMerger;
using claims::physical_operator::Expander;
using claims::physical_operator::PerformanceMonitor;
using claims::physical_operator::PhysicalLimit;
using claims::physical_operator::ResultPrinter;

namespace claims {
namespace logical_operator {

LogicalQueryPlanRoot::LogicalQueryPlanRoot(NodeID collecter,
                                           LogicalOperator* child,
                                           const OutputStyle& style,
                                           LimitConstraint limit_constraint)
    : collecter_node(collecter),
      child_(child),
      style_(style),
      limit_constraint_(limit_constraint) {
  set_operator_type(kLogicalQueryPlanRoot);
}

LogicalQueryPlanRoot::~LogicalQueryPlanRoot() {
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}

/**
 * get PlanContext and child physical plan from child ,
 * consider PlanContext's partitioner's location and collector,
 * decide whether add expander and exchange operator in physical plan.
 *
 * choose one of three top physical operators depend on fashion_,
 * return complete physical plan
 */
PhysicalOperatorBase* LogicalQueryPlanRoot::GetPhysicalPlan(
    const unsigned& block_size) {
  PlanContext child_plan_context = GetPlanContext();
  PhysicalOperatorBase* child_iterator = child_->GetPhysicalPlan(block_size);
  NodeTracker* node_tracker = NodeTracker::GetInstance();

  bool is_exchange_need = false;
  /**
   * If the number of partitions in the child PlanContext is 1 and the the
   * location is right in the collector,
   * then exchange is not necessary.
   */
  if (!(1 == child_plan_context.plan_partitioner_.GetNumberOfPartitions() &&
        child_plan_context.plan_partitioner_.get_partition_list()[0]
                .get_location() == collecter_node)) {
    is_exchange_need = true;

    // add BlockStreamExpander iterator into physical plan
    Expander::State expander_state_lower;
    expander_state_lower.block_count_in_buffer_ = 10;
    expander_state_lower.block_size_ = block_size;
    expander_state_lower.init_thread_count_ =
        Config::initial_degree_of_parallelism;
    expander_state_lower.child_ = child_iterator;
    expander_state_lower.schema_ =
        GetSchema(child_plan_context.attribute_list_);
    PhysicalOperatorBase* expander_lower = new Expander(expander_state_lower);

    // add ExchangeEpoll iterator into physical plan
    ExchangeMerger::State state;
    state.block_size_ = block_size;
    state.child_ = expander_lower;  // child_iterator;
    state.exchange_id_ =
        IDsGenerator::getInstance()->generateUniqueExchangeID();
    state.schema_ = GetSchema(child_plan_context.attribute_list_);
    state.upper_id_list_.push_back(collecter_node);
    state.partition_schema_ = partition_schema::set_hash_partition(0);
    std::vector<NodeID> lower_id_list =
        GetInvolvedNodeID(child_plan_context.plan_partitioner_);
    state.lower_id_list_ = lower_id_list;
    child_iterator = new ExchangeMerger(state);
  }

  Expander::State expander_state;
  expander_state.block_count_in_buffer_ = 10;
  expander_state.block_size_ = block_size;
  if (is_exchange_need)
    // if data exchange is used, only one expanded thread is enough.
    expander_state.init_thread_count_ = 1;
  else
    expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
  expander_state.child_ = child_iterator;
  expander_state.schema_ = GetSchema(child_plan_context.attribute_list_);
  PhysicalOperatorBase* expander = new Expander(expander_state);

  PhysicalOperatorBase* middle_tier;
  if (!limit_constraint_.CanBeOmitted()) {
    // we should add a limit operator
    PhysicalLimit::State limit_state(
        expander_state.schema_->duplicateSchema(), expander,
        limit_constraint_.returned_tuples_, block_size,
        limit_constraint_.start_position_);
    PhysicalOperatorBase* limit = new PhysicalLimit(limit_state);
    middle_tier = limit;
  } else {
    middle_tier = expander;
  }

  PhysicalOperatorBase* ret;
  switch (style_) {
    case kPrint: {
      ResultPrinter::State print_state(
          GetSchema(child_plan_context.attribute_list_), middle_tier,
          block_size, GetAttributeName(child_plan_context));
      ret = new ResultPrinter(print_state);
      break;
    }
    case kPerformance: {
      PerformanceMonitor::State performance_state(
          GetSchema(child_plan_context.attribute_list_), middle_tier,
          block_size);
      ret = new PerformanceMonitor(performance_state);
      break;
    }
    case kResultCollector: {
      std::vector<std::string> column_header;
      for (unsigned i = 0; i < child_plan_context.attribute_list_.size(); i++) {
        column_header.push_back(
            child_plan_context.attribute_list_[i].getName());
      }
      physical_operator::ResultCollector::State result_state(
          GetSchema(child_plan_context.attribute_list_), middle_tier,
          block_size, column_header);
      ret = new physical_operator::ResultCollector(result_state);
      break;
    }
  }

  return ret;
}

/**
 * get PlanContext from child and return
 */
PlanContext LogicalQueryPlanRoot::GetPlanContext() {
  PlanContext ret = child_->GetPlanContext();
  QueryOptimizationLogging::log(
      "Communication cost:%ld, predicted ouput size=%ld\n", ret.commu_cost_,
      ret.plan_partitioner_.GetAggregatedDataCardinality());
  return ret;
}

bool LogicalQueryPlanRoot::GetOptimalPhysicalPlan(
    Requirement requirement, PhysicalPlanDescriptor& final_physical_plan_desc,
    const unsigned& block_size) {
  std::vector<PhysicalPlanDescriptor> candidate_physical_plan;
  Requirement current_req;
  current_req.setRequiredLocations(std::vector<NodeID>(1, collecter_node));

  Requirement merged_req;
  bool requirement_merged = current_req.tryMerge(requirement, merged_req);
  if (requirement_merged) {
    current_req = merged_req;
  }

  PhysicalPlanDescriptor physical_plan;

  /** no requirement**/
  if (child_->GetOptimalPhysicalPlan(Requirement(), physical_plan,
                                     block_size)) {
    NetworkTransfer transfer =
        current_req.requireNetworkTransfer(physical_plan.plan_context_);

    if (transfer == NONE) {
      candidate_physical_plan.push_back(physical_plan);
    } else if ((transfer == OneToOne) || (transfer == Shuffle)) {
      // why transfer is compared with OneToOne, whose type is binding_mode?
      // ---Yu

      /* the input PlanContext should be transfered in the network to meet the
       * requirement
       * TODO: implement OneToOne Exchange
       * */
      physical_plan.cost += physical_plan.plan_context_.GetAggregatedDatasize();

      ExchangeMerger::State state;
      state.block_size_ = block_size;
      state.child_ = physical_plan.plan;  // child_iterator;
      state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      state.schema_ = GetSchema(physical_plan.plan_context_.attribute_list_);
      state.upper_id_list_.push_back(collecter_node);
      state.partition_schema_ = partition_schema::set_hash_partition(0);
      state.lower_id_list_ =
          GetInvolvedNodeID(physical_plan.plan_context_.plan_partitioner_);
      PhysicalOperatorBase* exchange = new ExchangeMerger(state);
      physical_plan.plan = exchange;
    }
  }

  /** with requirement**/
  if (child_->GetOptimalPhysicalPlan(current_req, physical_plan, block_size)) {
    candidate_physical_plan.push_back(physical_plan);
  }

  PhysicalPlanDescriptor best_plan =
      GetBestPhysicalPlanDescriptor(candidate_physical_plan);

  PhysicalPlan final_plan;
  switch (style_) {
    case kPrint: {
      ResultPrinter::State print_state(
          GetSchema(best_plan.plan_context_.attribute_list_), best_plan.plan,
          block_size, GetAttributeName(physical_plan.plan_context_));
      final_plan = new ResultPrinter(print_state);
      break;
    }
    case kPerformance: {
      PerformanceMonitor::State performance_state(
          GetSchema(best_plan.plan_context_.attribute_list_), best_plan.plan,
          block_size);
      final_plan = new PerformanceMonitor(performance_state);
    }
  }

  if (requirement_merged) {
    final_physical_plan_desc.cost = best_plan.cost;
    final_physical_plan_desc.plan_context_ = best_plan.plan_context_;
    final_physical_plan_desc.plan = final_plan;
  } else {
    NetworkTransfer transfer =
        current_req.requireNetworkTransfer(best_plan.plan_context_);

    if (transfer == NONE) {
      final_physical_plan_desc.cost = best_plan.cost;
      final_physical_plan_desc.plan_context_ = best_plan.plan_context_;
      final_physical_plan_desc.plan = final_plan;
    } else if ((transfer == OneToOne) || (transfer == Shuffle)) {
      /* the input PlanContext should be transfered in the network to meet the
       * requirement
       * TODO: implement OneToOne Exchange
       * */

      ExchangeMerger::State state;
      state.block_size_ = block_size;
      state.child_ = best_plan.plan;  // child_iterator;
      state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      state.schema_ = GetSchema(best_plan.plan_context_.attribute_list_);
      std::vector<NodeID> upper_id_list;
      if (requirement.hasRequiredLocations()) {
        upper_id_list = requirement.getRequiredLocations();
      } else {
        if (requirement.hasRequiredPartitionFunction()) {
          /* partition function contains the number of partitions*/
          PartitionFunction* partitoin_function =
              requirement.getPartitionFunction();
          upper_id_list = std::vector<NodeID>(
              NodeTracker::GetInstance()->GetNodeIDList().begin(),
              NodeTracker::GetInstance()->GetNodeIDList().begin() +
                  partitoin_function->getNumberOfPartitions() - 1);
        } else {
          // TODO(wangli): decide the degree of parallelism
          upper_id_list = NodeTracker::GetInstance()->GetNodeIDList();
        }
      }

      state.upper_id_list_ = upper_id_list;

      assert(requirement.hasReuiredPartitionKey());

      state.partition_schema_ = partition_schema::set_hash_partition(
          this->GetIdInAttributeList(best_plan.plan_context_.attribute_list_,
                                     requirement.getPartitionKey()));
      assert(state.partition_schema_.partition_key_index >= 0);

      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(best_plan.plan_context_.plan_partitioner_);

      state.lower_id_list_ = lower_id_list;

      PhysicalOperatorBase* exchange = new ExchangeMerger(state);
      best_plan.plan = exchange;
      best_plan.cost += best_plan.plan_context_.GetAggregatedDatasize();

      final_physical_plan_desc.cost = best_plan.cost;
      final_physical_plan_desc.plan_context_ = best_plan.plan_context_;
      final_physical_plan_desc.plan = exchange;
    }
  }

  if (requirement.passLimits(final_physical_plan_desc.cost))
    return true;
  else
    return false;
}

std::vector<std::string> LogicalQueryPlanRoot::GetAttributeName(
    const PlanContext& plan_context) const {
  std::vector<std::string> attribute_name_list;
  for (unsigned i = 0; i < plan_context.attribute_list_.size(); ++i) {
    attribute_name_list.push_back(plan_context.attribute_list_[i].attrName);
  }
  return attribute_name_list;
}
void LogicalQueryPlanRoot::Print(int level) const {
  printf("Root\n");
  if (!limit_constraint_.CanBeOmitted()) {
    printf("With limit constaint: %ld, %ld\n",
           limit_constraint_.start_position_,
           limit_constraint_.returned_tuples_);
  }
  child_->Print(level + 1);
}

}  // namespace logical_operator
}  // namespace claims
