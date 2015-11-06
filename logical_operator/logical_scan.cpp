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
 * /CLAIMS/logical_operator/logical_scan.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli, HanZhang
 *       Email:wangli1426@gmail.com
 *
 * Description: Implementation of Scan operator in logical layer
 *
 */

#include "../logical_operator/logical_scan.h"
#include <stdio.h>
#include <glog/logging.h>
#include <iostream>
#include <vector>

#include "../Catalog/Catalog.h"
#include "../IDsGenerator.h"
#include "../logical_operator/plan_partition_info.h"
#include "../physical_operator/exchange_merger.h"
#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/physical_projection_scan.h"
#include "../Resource/NodeTracker.h"

using claims::physical_operator::ExchangeMerger;
using claims::physical_operator::PhysicalProjectionScan;
namespace claims {
namespace logical_operator {
LogicalScan::LogicalScan(std::vector<Attribute> attribute_list)
    : scan_attribute_list_(attribute_list),
      target_projection_(NULL),
      plan_context_(NULL) {
  set_operator_type(kLogicalScan);
}

LogicalScan::LogicalScan(const TableID& table_id)
    : target_projection_(NULL), plan_context_(NULL) {
  TableDescriptor* table = Catalog::getInstance()->getTable(table_id);
  if (NULL == table) {
    LOG(WARNING) << "Table[id" << table_id << "] does not exists!" << std::endl;
  }
  scan_attribute_list_ = table->getAttributes();
  set_operator_type(kLogicalScan);
}
LogicalScan::LogicalScan(ProjectionDescriptor* projection,
                         const float sample_rate)
    : sample_rate_(sample_rate), plan_context_(NULL) {
  scan_attribute_list_ = projection->getAttributeList();
  target_projection_ = projection;
  set_operator_type(kLogicalScan);
}
LogicalScan::LogicalScan(
    const TableID& table_id,
    const std::vector<unsigned>& selected_attribute_index_list)
    : target_projection_(NULL), plan_context_(NULL) {
  TableDescriptor* table = Catalog::getInstance()->getTable(table_id);
  if (NULL == table) {
    LOG(WARNING) << "Table[id" << table_id << "] does not exists!" << std::endl;
  }
  for (unsigned i = 0; i < selected_attribute_index_list.size(); i++) {
    scan_attribute_list_.push_back(
        table->getAttribute(selected_attribute_index_list[i]));
  }
  set_operator_type(kLogicalScan);
}

LogicalScan::~LogicalScan() {
  if (NULL != plan_context_) {
    delete plan_context_;
    plan_context_ = NULL;
  }
}

/**
 * @brief It can generate many projection. We need the smallest cost of
 * projections, so we should choose the best one what we need with traversing
 * scan_attribute_list_.
 */
PlanContext LogicalScan::GetPlanContext() {
  if (NULL != plan_context_) return *plan_context_;
  plan_context_ = new PlanContext();

  TableID table_id = scan_attribute_list_[0].table_id_;
  TableDescriptor* table = Catalog::getInstance()->getTable(table_id);

  if (target_projection_ == NULL) {
    ProjectionOffset target_projection_off = -1;
    unsigned int min_projection_cost = -1;
    // TODO(KaiYu): get real need column as scan_attribute_list_, otherwise,
    // optimization don't work
    for (ProjectionOffset projection_off = 0;
         projection_off < table->getNumberOfProjection(); projection_off++) {
      ProjectionDescriptor* projection = table->getProjectoin(projection_off);
      bool fail = false;
      for (std::vector<Attribute>::iterator it = scan_attribute_list_.begin();
           it != scan_attribute_list_.end(); it++) {
        if (!projection->hasAttribute(*it)) {
          /*the attribute *it is not in the projection*/
          fail = true;
          break;
        }
      }
      if (fail == true) {
        continue;
      }
      unsigned int projection_cost = projection->getProjectionCost();
      // get the projection with minimum cost
      if (min_projection_cost > projection_cost) {
        target_projection_off = projection_off;
        min_projection_cost = projection_cost;
        cout << "in " << table->getNumberOfProjection() << " projections, "
                                                           "projection "
             << projection_off << " has less cost:" << projection_cost << endl;
      }
    }
    if (target_projection_off == -1) {
      // fail to find a projection that contains all the scan attribute
      LOG(ERROR) << "The current implementation does not support the scanning "
                    "that involves more than one projection." << std::endl;
      assert(false);
    }
    target_projection_ = table->getProjectoin(target_projection_off);
    cout << "in " << table->getNumberOfProjection() << " projections, "
                                                       "projection "
         << target_projection_off << " has min cost:" << min_projection_cost
         << endl;
  }

  if (!target_projection_->AllPartitionBound()) {
    Catalog::getInstance()->getBindingModele()->BindingEntireProjection(
        target_projection_->getPartitioner(), DESIRIABLE_STORAGE_LEVEL);
  }

  /**
   * @brief build the PlanContext
   */

  plan_context_->attribute_list_ = scan_attribute_list_;  // attribute_list_

  Partitioner* par = target_projection_->getPartitioner();
  plan_context_->plan_partitioner_ = PlanPartitioner(*par);
  plan_context_->commu_cost_ = 0;
  return *plan_context_;
}

/**
 * @brief Set the value of class state_ and get instantiation of physical
 * operator to transform logical operator.
 * In the current implementation, all the attributes within the involved
 * projection are read.
 */

// TODO(wangli): Ideally, the columns in one projection are stored separately
// and only the needed columns are touched for a given query.

PhysicalOperatorBase* LogicalScan::GetPhysicalPlan(const unsigned& block_size) {
  PhysicalProjectionScan::State state;
  state.block_size_ = block_size;
  state.projection_id_ = target_projection_->getProjectionID();
  state.schema_ = GetSchema(plan_context_->attribute_list_);
  state.sample_rate_ = sample_rate_;
  return new PhysicalProjectionScan(state);
}

bool LogicalScan::GetOptimalPhysicalPlan(
    Requirement requirement, PhysicalPlanDescriptor& physical_plan_descriptor,
    const unsigned& block_size) {
  PlanContext plan_context = GetPlanContext();
  NetworkTransfer transfer = requirement.requireNetworkTransfer(plan_context);

  PhysicalProjectionScan::State state;
  state.block_size_ = block_size;
  state.projection_id_ = target_projection_->getProjectionID();
  state.schema_ = GetSchema(plan_context_->attribute_list_);
  state.sample_rate_ = sample_rate_;

  PhysicalPlan scan = new PhysicalProjectionScan(state);

  if (transfer == NONE) {
    physical_plan_descriptor.plan = scan;
    physical_plan_descriptor.plan_context_ = plan_context;
    physical_plan_descriptor.cost += 0;
  } else {
    physical_plan_descriptor.cost += plan_context.GetAggregatedDatasize();

    ExchangeMerger::State state;
    state.block_size_ = block_size;
    state.child_ = scan;  // child_iterator;
    state.exchange_id_ =
        IDsGenerator::getInstance()->generateUniqueExchangeID();
    state.schema_ = GetSchema(plan_context.attribute_list_);

    std::vector<NodeID> lower_id_list =
        GetInvolvedNodeID(plan_context.plan_partitioner_);
    state.lower_id_list_ = lower_id_list;

    std::vector<NodeID> upper_id_list;
    if (requirement.hasRequiredLocations()) {
      upper_id_list = requirement.getRequiredLocations();
    } else {
      if (requirement.hasRequiredPartitionFunction()) {
        // partition function contains the number of partitions
        PartitionFunction* partitoin_function =
            requirement.getPartitionFunction();
        upper_id_list = std::vector<NodeID>(
            NodeTracker::GetInstance()->GetNodeIDList().begin(),
            NodeTracker::GetInstance()->GetNodeIDList().begin() +
                partitoin_function->getNumberOfPartitions() - 1);
      } else {
        // TODO: decide the degree of parallelism
        upper_id_list = NodeTracker::GetInstance()->GetNodeIDList();
      }
    }

    state.upper_id_list_ = upper_id_list;

    state.partition_schema_ =
        partition_schema::set_hash_partition(GetIdInAttributeList(
            plan_context.attribute_list_, requirement.getPartitionKey()));
    assert(state.partition_schema_.partition_key_index >= 0);

    PhysicalOperatorBase* exchange = new ExchangeMerger(state);

    PlanContext new_plan_context;
    new_plan_context.attribute_list_ = plan_context.attribute_list_;
    new_plan_context.plan_partitioner_.set_partition_key(
        requirement.getPartitionKey());
    new_plan_context.plan_partitioner_.set_partition_func(
        PartitionFunctionFactory::createBoostHashFunction(
            state.upper_id_list_.size()));

    const unsigned total_size = plan_context.GetAggregatedDatasize();
    const unsigned degree_of_parallelism = state.upper_id_list_.size();
    std::vector<PlanPartitionInfo> plan_context_partition_list;
    for (unsigned i = 0; i < degree_of_parallelism; i++) {
      const NodeID location = upper_id_list[i];

      /**
       * @brief Currently, the join output size cannot be predicted due to the
       * absence of data statistics.
       * We just use the magic number as following
       */
      const unsigned kDatasize = total_size / degree_of_parallelism;
      PlanPartitionInfo dfp(i, kDatasize, location);
      plan_context_partition_list.push_back(dfp);
    }
    new_plan_context.plan_partitioner_.set_partition_list(
        plan_context_partition_list);

    physical_plan_descriptor.plan = exchange;
    physical_plan_descriptor.plan_context_ = new_plan_context;
    physical_plan_descriptor.cost += new_plan_context.GetAggregatedDatasize();
  }

  if (requirement.passLimits(physical_plan_descriptor.cost))
    return true;
  else
    return false;
}
void LogicalScan::Print(int level) const {
  printf("%*.sScan: %s\n", level * 8, " ",
         Catalog::getInstance()
             ->getTable(target_projection_->getProjectionID().table_id)
             ->getTableName()
             .c_str());
}

}  // namespace logical_operator
}  // namespace claims
