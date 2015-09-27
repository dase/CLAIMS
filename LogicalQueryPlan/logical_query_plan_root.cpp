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
 * /Claims/LogicalQueryPlan/logical_query_plan_root.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: wangli, yukai
 *		 Email: yukai2014@gmail.com
 * 
 * Description:
 *
 */


#include "./logical_query_plan_root.h"

#include <vector>
#include <string>

#include "../Config.h"
#include "../IDsGenerator.h"
#include "../Resource/NodeTracker.h"

#include "../BlockStreamIterator/BlockStreamLimit.h"
#include "../BlockStreamIterator/BlockStreamPerformanceMonitorTop.h"
#include "../BlockStreamIterator/BlockStreamPrint.h"
#include "../BlockStreamIterator/BlockStreamResultCollector.h"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"

// namespace claims {
// namespace logical_query_plan {

LogicalQueryPlanRoot::LogicalQueryPlanRoot(NodeID collecter,
                                           LogicalOperator* child,
                                           const OutputStyle& style,
                                           LimitConstraint limit_constraint)
    : collecter_node(collecter),
      child_(child),
      style_(style),
      limit_constraint_(limit_constraint) {
  setOperatortype(l_root);
}

LogicalQueryPlanRoot::~LogicalQueryPlanRoot() {
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}

/**
 * get dataflow and child physical plan from child ,
 * consider dataflow's partitioner's location and collector,
 * decide whether add expander and exchange operator in physical plan.
 *
 * choose one of three top physical operators depend on fashion_,
 * return complete physical plan
 */
BlockStreamIteratorBase* LogicalQueryPlanRoot::GetIteratorTree(
    const unsigned& block_size) {
  Dataflow child_dataflow = GetDataflow();
  BlockStreamIteratorBase* child_iterator = child_->GetIteratorTree(block_size);
  NodeTracker* node_tracker = NodeTracker::GetInstance();

  bool is_exchange_need = false;
  /**
   * If the number of partitions in the child dataflow is 1 and the the location is right in the collector,
   * then exchange is not necessary.
   */
  if (!(1 == child_dataflow.property_.partitioner.getNumberOfPartitions()
      && child_dataflow.property_.partitioner.getPartitionList()[0].
        getLocation() == collecter_node)) {
    is_exchange_need = true;

    // add BlockStreamExpander iterator into physical plan
    BlockStreamExpander::State expander_state_lower;
    expander_state_lower.block_count_in_buffer_ = 10;
    expander_state_lower.block_size_ = block_size;
    expander_state_lower.init_thread_count_ =
        Config::initial_degree_of_parallelism;
    expander_state_lower.child_ = child_iterator;
    expander_state_lower.schema_ = GetSchema(child_dataflow.attribute_list_);
    BlockStreamIteratorBase* expander_lower = new BlockStreamExpander(
        expander_state_lower);

    // add ExchangeEpoll iterator into physical plan
    ExpandableBlockStreamExchangeEpoll::State state;
    state.block_size_ = block_size;
    state.child_ = expander_lower;  // child_iterator;
    state.exchange_id_ =
        IDsGenerator::getInstance()->generateUniqueExchangeID();
    state.schema_ = GetSchema(child_dataflow.attribute_list_);
    state.upper_id_list_.push_back(collecter_node);
    state.partition_schema_ = partition_schema::set_hash_partition(0);
    std::vector<NodeID> lower_id_list = GetInvolvedNodeID(
        child_dataflow.property_.partitioner);
    state.lower_id_list_ = lower_id_list;
    child_iterator = new ExpandableBlockStreamExchangeEpoll(state);
  }

  BlockStreamExpander::State expander_state;
  expander_state.block_count_in_buffer_ = 10;
  expander_state.block_size_ = block_size;
  if (is_exchange_need)
    // if data exchange is used, only one expanded thread is enough.
    expander_state.init_thread_count_ = 1;
  else
    expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
  expander_state.child_ = child_iterator;
  expander_state.schema_ = GetSchema(child_dataflow.attribute_list_);
  BlockStreamIteratorBase* expander = new BlockStreamExpander(expander_state);

  BlockStreamIteratorBase* middle_tier;
  if (!limit_constraint_.CanBeOmitted()) {
    // we should add a limit operator
    BlockStreamLimit::State limit_state(
        expander_state.schema_->duplicateSchema(),
        expander,
        limit_constraint_.returned_tuples_,
        block_size,
        limit_constraint_.start_position_);
    BlockStreamIteratorBase* limit = new BlockStreamLimit(limit_state);
    middle_tier = limit;
  } else {
    middle_tier = expander;
  }

  BlockStreamIteratorBase* ret;
  switch (style_) {
    case kPrint: {
      BlockStreamPrint::State print_state(
          GetSchema(child_dataflow.attribute_list_), middle_tier, block_size,
          GetAttributeName(child_dataflow));
      ret = new BlockStreamPrint(print_state);
      break;
    }
    case kPerformance: {
      BlockStreamPerformanceMonitorTop::State performance_state(
          GetSchema(child_dataflow.attribute_list_), middle_tier, block_size);
      ret = new BlockStreamPerformanceMonitorTop(performance_state);
      break;
    }
    case kResultCollector: {
      std::vector<std::string> column_header;
      for (unsigned i = 0; i < child_dataflow.attribute_list_.size(); i++) {
        column_header.push_back(child_dataflow.attribute_list_[i].getName());
      }
      BlockStreamResultCollector::State result_state(
          GetSchema(child_dataflow.attribute_list_), middle_tier, block_size,
          column_header);
      ret = new BlockStreamResultCollector(result_state);
      break;
    }
  }

  return ret;
}

/**
 * get dataflow from child and return
 */
Dataflow LogicalQueryPlanRoot::GetDataflow() {
  Dataflow ret = child_->GetDataflow();
  QueryOptimizationLogging::log(
      "Communication cost:%ld, predicted ouput size=%ld\n",
      ret.property_.commnication_cost,
      ret.property_.partitioner.getAggregatedDataCardinality());
  return ret;
}

bool LogicalQueryPlanRoot::GetOptimalPhysicalPlan(
    Requirement requirement, PhysicalPlanDescriptor& final_physical_plan_desc,
    const unsigned & block_size) {
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
    NetworkTransfer transfer = current_req.requireNetworkTransfer(
        physical_plan.dataflow);

    if (transfer == NONE) {
      candidate_physical_plan.push_back(physical_plan);
    } else if ((transfer == OneToOne) || (transfer == Shuffle)) {
    // why transfer is compared with OneToOne, whose type is binding_mode? ---Yu

      /* the input data flow should be transfered in the network to meet the requirement
       * TODO: implement OneToOne Exchange
       * */
      physical_plan.cost += physical_plan.dataflow.getAggregatedDatasize();

      ExpandableBlockStreamExchangeEpoll::State state;
      state.block_size_ = block_size;
      state.child_ = physical_plan.plan;  // child_iterator;
      state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      state.schema_ = GetSchema(physical_plan.dataflow.attribute_list_);
      state.upper_id_list_.push_back(collecter_node);
      state.partition_schema_ = partition_schema::set_hash_partition(0);
      state.lower_id_list_ = GetInvolvedNodeID(
          physical_plan.dataflow.property_.partitioner);
      BlockStreamIteratorBase* exchange =
          new ExpandableBlockStreamExchangeEpoll(state);
      physical_plan.plan = exchange;
    }
  }

  /** with requirement**/
  if (child_->GetOptimalPhysicalPlan(current_req, physical_plan, block_size)) {
    candidate_physical_plan.push_back(physical_plan);
  }

  PhysicalPlanDescriptor best_plan = getBestPhysicalPlanDescriptor(
      candidate_physical_plan);

  PhysicalPlan final_plan;
  switch (style_) {
    case kPrint: {
      BlockStreamPrint::State print_state(
          GetSchema(best_plan.dataflow.attribute_list_), best_plan.plan,
          block_size, GetAttributeName(physical_plan.dataflow));
      final_plan = new BlockStreamPrint(print_state);
      break;
    }
    case kPerformance: {
      BlockStreamPerformanceMonitorTop::State performance_state(
          GetSchema(best_plan.dataflow.attribute_list_), best_plan.plan,
          block_size);
      final_plan = new BlockStreamPerformanceMonitorTop(performance_state);
    }
  }

  if (requirement_merged) {
    final_physical_plan_desc.cost = best_plan.cost;
    final_physical_plan_desc.dataflow = best_plan.dataflow;
    final_physical_plan_desc.plan = final_plan;
  } else {
    NetworkTransfer transfer = current_req.requireNetworkTransfer(
        best_plan.dataflow);

    if (transfer == NONE) {
      final_physical_plan_desc.cost = best_plan.cost;
      final_physical_plan_desc.dataflow = best_plan.dataflow;
      final_physical_plan_desc.plan = final_plan;
    } else if ((transfer == OneToOne) || (transfer == Shuffle)) {
      /* the input data flow should be transfered in the network to meet the requirement
       * TODO: implement OneToOne Exchange
       * */

      ExpandableBlockStreamExchangeEpoll::State state;
      state.block_size_ = block_size;
      state.child_ = best_plan.plan;  // child_iterator;
      state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      state.schema_ = GetSchema(best_plan.dataflow.attribute_list_);
      std::vector<NodeID> upper_id_list;
      if (requirement.hasRequiredLocations()) {
        upper_id_list = requirement.getRequiredLocations();
      } else {
        if (requirement.hasRequiredPartitionFunction()) {
          /* partition function contains the number of partitions*/
          PartitionFunction* partitoin_function = requirement
              .getPartitionFunction();
          upper_id_list = std::vector<NodeID>(
              NodeTracker::GetInstance()->GetNodeIDList().begin(),
              NodeTracker::GetInstance()->GetNodeIDList().begin()
                  + partitoin_function->getNumberOfPartitions() - 1);
        } else {
          // TODO(wangli): decide the degree of parallelism
          upper_id_list = NodeTracker::GetInstance()->GetNodeIDList();
        }
      }

      state.upper_id_list_ = upper_id_list;

      assert(requirement.hasReuiredPartitionKey());

      state.partition_schema_ = partition_schema::set_hash_partition(
          this->getIndexInAttributeList(best_plan.dataflow.attribute_list_,
                                        requirement.getPartitionKey()));
      assert(state.partition_schema_.partition_key_index >= 0);

      std::vector<NodeID> lower_id_list = GetInvolvedNodeID(
          best_plan.dataflow.property_.partitioner);

      state.lower_id_list_ = lower_id_list;

      BlockStreamIteratorBase* exchange =
          new ExpandableBlockStreamExchangeEpoll(state);
      best_plan.plan = exchange;
      best_plan.cost += best_plan.dataflow.getAggregatedDatasize();

      final_physical_plan_desc.cost = best_plan.cost;
      final_physical_plan_desc.dataflow = best_plan.dataflow;
      final_physical_plan_desc.plan = exchange;
    }
  }

  if (requirement.passLimits(final_physical_plan_desc.cost))
    return true;
  else
    return false;
}

std::vector<std::string> LogicalQueryPlanRoot::GetAttributeName(
    const Dataflow& dataflow) const {
  std::vector<std::string> attribute_name_list;
  for (unsigned i = 0; i < dataflow.attribute_list_.size(); i++) {
    attribute_name_list.push_back(dataflow.attribute_list_[i].attrName);
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

//}  // namespace logical_query_plan
//}  // namespace claims


