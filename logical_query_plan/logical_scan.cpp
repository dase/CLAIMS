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
 * /CLAIMS/logical_query_plan/logical_scan.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli, HanZhang
 *       Email:wangli1426@gmail.com
 *
 * Description: Implementation of Scan operator in logical layer
 *
 */

#include "../logical_query_plan/logical_scan.h"

#include <stdio.h>
#include <glog/logging.h>
#include <iostream>
#include "../Catalog/Catalog.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamProjectionScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../IDsGenerator.h"

// namespace claims {
// namespace logical_query_plan {
LogicalScan::LogicalScan(std::vector<Attribute> attribute_list)
    : scan_attribute_list_(attribute_list),
      target_projection_(NULL),
      dataflow_(NULL) {
  set_operator_type(kLogicalScan);
}

LogicalScan::LogicalScan(const TableID& table_id)
    : target_projection_(NULL), dataflow_(NULL) {
  TableDescriptor* table = Catalog::getInstance()->getTable(table_id);
  if (NULL == table) {
    LOG(WARNING) << "Table[id" << table_id << "] does not exists!" << std::endl;
  }
  scan_attribute_list_ = table->getAttributes();
  set_operator_type(kLogicalScan);
}
LogicalScan::LogicalScan(ProjectionDescriptor* projection,
                         const float sample_rate)
    : sample_rate_(sample_rate), dataflow_(NULL) {
  scan_attribute_list_ = projection->getAttributeList();
  target_projection_ = projection;
  set_operator_type(kLogicalScan);
}
LogicalScan::LogicalScan(
    const TableID& table_id,
    const std::vector<unsigned>& selected_attribute_index_list)
    : target_projection_(NULL), dataflow_(NULL) {
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
  if (NULL != dataflow_) {
    delete dataflow_;
    dataflow_ = NULL;
  }
}

/**
 * @brief It can generate many projection. We need the smallest cost of
 * projections, so we should choose the best one what we need with traversing
 * scan_attribute_list_.
 */
Dataflow LogicalScan::GetDataflow() {
  if (NULL != dataflow_) return *dataflow_;
  dataflow_ = new Dataflow();

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
   * @brief build the data flow
   */

  dataflow_->attribute_list_ = scan_attribute_list_;  // attribute_list_

  Partitioner* par = target_projection_->getPartitioner();
  dataflow_->property_.partitioner = DataflowPartitioningDescriptor(*par);
  dataflow_->property_.commnication_cost = 0;
  return *dataflow_;
}

/**
 * @brief Set the value of class state and get instantiation of physical
 * operator to transform logical operator.
 * In the current implementation, all the attributes within the involved
 * projection are read.
 */

// TODO: Ideally, the columns in one projection are stored separately
// and only the needed columns are touched for a given query.

BlockStreamIteratorBase* LogicalScan::GetIteratorTree(
    const unsigned& block_size) {
  ExpandableBlockStreamProjectionScan::State state;
  state.block_size_ = block_size;
  state.projection_id_ = target_projection_->getProjectionID();
  state.schema_ = GetSchema(dataflow_->attribute_list_);
  state.sample_rate_ = sample_rate_;
  return new ExpandableBlockStreamProjectionScan(state);
}

bool LogicalScan::GetOptimalPhysicalPlan(
    Requirement requirement, PhysicalPlanDescriptor& physical_plan_descriptor,
    const unsigned& block_size) {
  Dataflow dataflow = GetDataflow();
  NetworkTransfer transfer = requirement.requireNetworkTransfer(dataflow);

  ExpandableBlockStreamProjectionScan::State state;
  state.block_size_ = block_size;
  state.projection_id_ = target_projection_->getProjectionID();
  state.schema_ = GetSchema(dataflow_->attribute_list_);
  state.sample_rate_ = sample_rate_;

  PhysicalPlan scan = new ExpandableBlockStreamProjectionScan(state);

  if (transfer == NONE) {
    physical_plan_descriptor.plan = scan;
    physical_plan_descriptor.dataflow = dataflow;
    physical_plan_descriptor.cost += 0;
  } else {
    physical_plan_descriptor.cost += dataflow.getAggregatedDatasize();

    ExpandableBlockStreamExchangeEpoll::State state;
    state.block_size_ = block_size;
    state.child_ = scan;  // child_iterator;
    state.exchange_id_ =
        IDsGenerator::getInstance()->generateUniqueExchangeID();
    state.schema_ = GetSchema(dataflow.attribute_list_);

    std::vector<NodeID> lower_id_list =
        GetInvolvedNodeID(dataflow.property_.partitioner);
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
            dataflow.attribute_list_, requirement.getPartitionKey()));
    assert(state.partition_schema_.partition_key_index >= 0);

    BlockStreamIteratorBase* exchange =
        new ExpandableBlockStreamExchangeEpoll(state);

    Dataflow new_dataflow;
    new_dataflow.attribute_list_ = dataflow.attribute_list_;
    new_dataflow.property_.partitioner.setPartitionKey(
        requirement.getPartitionKey());
    new_dataflow.property_.partitioner.setPartitionFunction(
        PartitionFunctionFactory::createBoostHashFunction(
            state.upper_id_list_.size()));

    const unsigned total_size = dataflow.getAggregatedDatasize();
    const unsigned degree_of_parallelism = state.upper_id_list_.size();
    std::vector<DataflowPartition> dataflow_partition_list;
    for (unsigned i = 0; i < degree_of_parallelism; i++) {
      const NodeID location = upper_id_list[i];

      /**
       * @brief Currently, the join output size cannot be predicted due to the
       * absence of data statistics.
       * We just use the magic number as following
       */
      const unsigned kDatasize = total_size / degree_of_parallelism;
      DataflowPartition dfp(i, kDatasize, location);
      dataflow_partition_list.push_back(dfp);
    }
    new_dataflow.property_.partitioner.setPartitionList(
        dataflow_partition_list);

    physical_plan_descriptor.plan = exchange;
    physical_plan_descriptor.dataflow = new_dataflow;
    physical_plan_descriptor.cost += new_dataflow.getAggregatedDatasize();
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

//}   // namespace logical_query_plan
//}   // namespace claims
