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
 * /CLAIMS/logical_operator/logical_sort.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: yuyang
 *       Email: youngfish93@hotmail.com
 *
 * Description:
 *   This file mainly describe the Sort Operater.
 *
 */

#include "../logical_operator/logical_sort.h"

#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "../common/expression/expr_node.h"
#include "../catalog/catalog.h"
#include "../common/ids.h"
#include "../Config.h"
#include "../IDsGenerator.h"
#include "../logical_operator/logical_operator.h"
#include "../physical_operator/exchange_merger.h"
#include "../physical_operator/expander.h"
#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/physical_sort.h"
using claims::common::ExprNode;
using claims::physical_operator::ExchangeMerger;
using claims::physical_operator::Expander;
using claims::physical_operator::PhysicalSort;
using std::endl;
namespace claims {
namespace logical_operator {
LogicalSort::LogicalSort(
    LogicalOperator *child,
    std::vector<LogicalSort::OrderByAttr *> order_by_attr) {
  assert(false);
}

LogicalSort::LogicalSort(LogicalOperator *child,
                         vector<pair<ExprNode *, int>> order_by_attrs)
    : LogicalOperator(kLogicalSort),
      child_(child),
      order_by_attrs_(order_by_attrs),
      plan_context_(NULL) {}
LogicalSort::~LogicalSort() {
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
  if (NULL != plan_context_) {
    delete plan_context_;
    plan_context_ = NULL;
  }
}

PlanContext LogicalSort::GetPlanContext() {
  lock_->acquire();
  if (NULL != plan_context_) {
    lock_->release();
    return *plan_context_;
  }
  // Get the information from its child
  PlanContext child_plan_context_ = child_->GetPlanContext();
  PlanContext ret;
  ret.attribute_list_ = child_plan_context_.attribute_list_;
  ret.commu_cost_ = child_plan_context_.commu_cost_;
  ret.plan_partitioner_.set_partition_func(
      child_plan_context_.plan_partitioner_.get_partition_func());
  ret.plan_partitioner_.set_partition_key(Attribute());

  NodeID location = 0;
  unsigned long data_cardinality = 0;
  PartitionOffset offset = 0;
  PlanPartitionInfo par(offset, data_cardinality, location);
  vector<PlanPartitionInfo> partition_list;
  partition_list.push_back(par);
  ret.plan_partitioner_.set_partition_list(partition_list);
  SetColumnId(child_plan_context_);
  Schema *input_schema = GetSchema(child_plan_context_.attribute_list_);
  for (int i = 0; i < order_by_attrs_.size(); ++i) {
    order_by_attrs_[i].first->InitExprAtLogicalPlan(
        order_by_attrs_[i].first->actual_type_, column_to_id_, input_schema);
  }
  plan_context_ = new PlanContext();
  *plan_context_ = ret;
  lock_->release();
  return ret;
}
void LogicalSort::SetColumnId(const PlanContext &plan_context) {
  for (int i = 0; i < plan_context.attribute_list_.size(); ++i) {
    /**
     * Traverse the attribute_list_，store the attribute name and index into
     * column_id.
     */
    column_to_id_[plan_context.attribute_list_[i].attrName] = i;
  }
}

PhysicalOperatorBase *LogicalSort::GetPhysicalPlan(const unsigned &blocksize) {
  PlanContext child_plan_context_ = child_->GetPlanContext();

  // Get all of the data from other nodes if needed.
  Expander::State expander_state;
  expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
  expander_state.block_size_ = blocksize;
  expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
  expander_state.child_ = child_->GetPhysicalPlan(blocksize);
  expander_state.schema_ = GetSchema(child_plan_context_.attribute_list_);
  PhysicalOperatorBase *expander_lower = new Expander(expander_state);

  ExchangeMerger::State exchange_state;
  exchange_state.block_size_ = blocksize;
  exchange_state.child_ = expander_lower;
  exchange_state.exchange_id_ =
      IDsGenerator::getInstance()->generateUniqueExchangeID();
  exchange_state.schema_ = GetSchema(child_plan_context_.attribute_list_);
  vector<NodeID> lower_id_list =
      GetInvolvedNodeID(child_plan_context_.plan_partitioner_);
  exchange_state.lower_id_list_ = lower_id_list;  // upper
  exchange_state.partition_schema_ = partition_schema::set_hash_partition(0);
  // TODO(admin): compute the upper_ip_list to do reduce side sort
  vector<NodeID> upper_ip_list;
  upper_ip_list.push_back(0);
  exchange_state.upper_id_list_ = upper_ip_list;  // lower
  PhysicalOperatorBase *exchange = new ExchangeMerger(exchange_state);

  PhysicalSort::State reducer_state;
  reducer_state.block_size_ = blocksize;
  reducer_state.child_ = exchange;
#ifndef NEWCONDI
  // Actually we just need the column number in the end.
  for (unsigned i = 0; i < order_by_attr_.size(); i++) {
    reducer_state.order_by_key_.push_back(
        GetOrderByKey(order_by_attr_[i]->table_name_));
    reducer_state.direction_.push_back(order_by_attr_[i]->direction_);
  }
#else
  reducer_state.order_by_attrs_ = order_by_attrs_;
#endif
  reducer_state.input_schema_ = GetSchema(child_plan_context_.attribute_list_);
  PhysicalOperatorBase *reducer_sort = new PhysicalSort(reducer_state);

  return reducer_sort;
}

int LogicalSort::GetOrderByKey(const char *table_name) {
  PlanContext child_plan_context_ = child_->GetPlanContext();

  for (unsigned attr_id = 0;
       attr_id < child_plan_context_.attribute_list_.size(); attr_id++) {
    string _tablename(table_name);
    if (_tablename.compare(
            child_plan_context_.attribute_list_[attr_id].attrName) == 0) {
      return attr_id;
    }
  }
}
void LogicalSort::PrintOrderByAttr(int level) const {
  cout << setw(level * kTabSize) << " "
       << "OrderByAttr:" << endl;
#ifndef NEWCONDI
  for (int i = 0; i < order_by_attr_.size(); i++) {
    printf("%s  %s\n", (const char *)order_by_attr_[i]->table_name_,
           order_by_attr_[i]->direction_ == 0 ? "ASC" : "DESC");
  }
#else
  level++;
  for (int i = 0; i < order_by_attrs_.size(); ++i) {
    cout << setw(level * kTabSize) << " " << order_by_attrs_[i].first->alias_
         << "    " << ((order_by_attrs_[i].second == 0) ? "ASC" : "DESC")
         << endl;
  }
#endif
}
void LogicalSort::Print(int level) const {
  PrintOrderByAttr(level);
  child_->Print(level);
}
}  // namespace logical_operator
}  // namespace claims
