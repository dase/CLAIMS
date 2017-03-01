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
 *  Modified on: Nov 16, 2015
 *      Author: wangli, yukai, tonglanxuan
 *		 Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "../logical_operator/logical_query_plan_root.h"

#include <glog/logging.h>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

#include "./logical_limit.h"
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
                                           string raw_sql,
                                           const OutputStyle& style)
    : LogicalOperator(kLogicalQueryPlanRoot),
      collecter_node(collecter),
      child_(child),
      raw_sql_(raw_sql),
      style_(style),
      plan_context_(NULL) {}
LogicalQueryPlanRoot::LogicalQueryPlanRoot(NodeID collecter,
                                           LogicalOperator* child,
                                           const OutputStyle& style)
    : LogicalOperator(kLogicalQueryPlanRoot),
      collecter_node(collecter),
      child_(child),
      style_(style),
      plan_context_(NULL) {}
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
  ///////////
  LogicalLimit* limit = NULL;
  PhysicalOperatorBase* child_iterator = NULL;
  if (child_->get_operator_type() == OperatorType::kLogicalLimit) {
    limit = reinterpret_cast<LogicalLimit*>(child_);
    child_iterator = limit->child_->GetPhysicalPlan(block_size);
  } else {
    child_iterator = child_->GetPhysicalPlan(block_size);
  }
  /////////////
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
  if (child_->get_operator_type() == OperatorType::kLogicalLimit) {
    expander = limit->GetPhysicalPlan(block_size, expander);
  }
  PhysicalOperatorBase* ret;
  switch (style_) {
    case kPrint: {
      ResultPrinter::State print_state(
          GetSchema(child_plan_context.attribute_list_), expander, block_size,
          GetAttributeName(child_plan_context));
      ret = new ResultPrinter(print_state);
      break;
    }
    case kPerformance: {
      PerformanceMonitor::State performance_state(
          GetSchema(child_plan_context.attribute_list_), expander, block_size);
      ret = new PerformanceMonitor(performance_state);
      break;
    }
    case kResultCollector: {
      std::vector<std::string> column_header;
      GetColumnHeader(column_header, child_plan_context.attribute_list_);

      physical_operator::ResultCollector::State result_state(
          GetSchema(child_plan_context.attribute_list_), expander, block_size,
          column_header);
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
  lock_->acquire();
  if (NULL != plan_context_) {
    delete plan_context_;
    plan_context_ = NULL;
  }
  PlanContext ret = child_->GetPlanContext();
  LOG(INFO) << "Communication cost: " << ret.commu_cost_
            << " predicted ouput size= "
            << ret.plan_partitioner_.GetAggregatedDataCardinality() << endl;
  plan_context_ = new PlanContext();
  *plan_context_ = ret;
  lock_->release();
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
void LogicalQueryPlanRoot::GetColumnHeader(
    std::vector<std::string>& column_header,
    std::vector<Attribute>& attribute_list) {
  string str_upper = raw_sql_;
  for (int i = 0; i < str_upper.length(); i++) {
    if (isalpha(str_upper[i])) {
      str_upper[i] = toupper(str_upper[i]);
    } else if (str_upper[i] == '\n' || str_upper[i] == '\t') {
      raw_sql_[i] = ' ';
      str_upper[i] = ' ';
    }
  }
  int end = str_upper.find(" FROM ");
  int begin = 6;
  string word = "";
  string upper_word = "";
  vector<string> upper_list;
  for (int i = begin; i < end; i++) {
    if (str_upper[i] != ',') {
      word += raw_sql_[i];
      upper_word += str_upper[i];
    } else {
      if (word != "") {
        column_header.push_back(word);
        upper_list.push_back(upper_word);
        upper_word = "";
        word = "";
      } else {
        continue;
      }
    }
  }
  column_header.push_back(word);
  upper_list.push_back(upper_word);
  auto i = column_header.begin();
  auto j = upper_list.begin();
  for (; i != column_header.end(); i++, j++) {
    int pos = (*j).find(" AS ");
    if (pos != -1) {
      (*j) = (*j).substr(pos + 4, (*j).length() - pos - 4);
      (*i) = (*i).substr(pos + 4, (*j).length() - pos - 4);
    }
    boost::trim(*j);
    boost::trim(*i);
    //(*i) = FormmatAttrName((*i));
    //(*j) = FormmatAttrName((*j));
  }
  // select * from tb; the bug is when select * from tb group by col;
  if (column_header.size() == 1 && column_header[0] == "*") {
    column_header.clear();
    for (unsigned i = 0; i < attribute_list.size(); i++) {
      column_header.push_back(attribute_list[i].getName());
    }
  }
  // select tb.* from tb,ta; the bug is when tb only has one column
  if (column_header.size() != attribute_list.size()) {
    column_header.clear();
    for (unsigned i = 0; i < attribute_list.size(); i++) {
      column_header.push_back(attribute_list[i].getName());
    }
  }
  // romove "NULL_MID.", if above bug occur, but the recovered name may be not
  // match the raw expression
  for (int i = 0; i < column_header.size(); ++i) {
    if (column_header[i].size() > 9) {
      if (column_header[i].substr(0, 9) == "NULL_MID.") {
        column_header[i] = column_header[i].substr(9);
      }
    }
  }
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
  cout << setw(level * kTabSize) << " "
       << "Root" << endl;
  GetPlanContext();
  cout << setw(level * kTabSize) << " "
       << "[Partition info: "
       << plan_context_->plan_partitioner_.get_partition_key().attrName
       << " table_id= "
       << plan_context_->plan_partitioner_.get_partition_key().table_id_
       << " column_id= "
       << plan_context_->plan_partitioner_.get_partition_key().index << " ]"
       << endl;
  child_->Print(level);
}
void LogicalQueryPlanRoot::PruneProj(set<string>& above_attrs) {
  child_->PruneProj(above_attrs);
}

}  // namespace logical_operator
}  // namespace claims
