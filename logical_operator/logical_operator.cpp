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
 * /CLAIMS/logical_operator/logical_operator.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli, fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *
 */
#include "../logical_operator/logical_operator.h"
#include <limits.h>
#include <vector>

#include "../Resource/NodeTracker.h"
namespace claims {
namespace logical_operator {
// LogicalOperator::LogicalOperator() {
//	// TODO Auto-generated constructor stub
//
//}
//
// LogicalOperator::~LogicalOperator() {
//	// TODO Auto-generated destructor stub
//}
//
Schema* LogicalOperator::GetSchema(
    const std::vector<Attribute>& attributes) const {
  std::vector<column_type> columns;
  for (unsigned i = 0; i < attributes.size(); i++) {
    columns.push_back(*attributes[i].attrType);
  }
  return new SchemaFix(columns);
}
Schema* LogicalOperator::GetSchema(
    const std::vector<Attribute>& attributes_left,
    const std::vector<Attribute>& attributes_right) const {
  std::vector<column_type> columns;
  for (unsigned i = 0; i < attributes_left.size(); i++) {
    columns.push_back(*attributes_left[i].attrType);
  }
  for (unsigned i = 0; i < attributes_right.size(); i++) {
    columns.push_back(*attributes_right[i].attrType);
  }
  return new SchemaFix(columns);
}
std::vector<NodeID> LogicalOperator::GetInvolvedNodeID(
    const PlanPartitioner& part) const {
  std::vector<NodeID> ret;
  for (unsigned i = 0; i < part.GetNumberOfPartitions(); i++) {
    ret.push_back(part.GetPartition(i)->get_location());
  }
  return ret;
}
std::vector<NodeIP> LogicalOperator::ConvertNodeIDListToNodeIPList(
    const std::vector<NodeID>& node_id_list) const {
  std::vector<NodeIP> ret;
  NodeTracker* nodetracker = NodeTracker::GetInstance();
  for (unsigned i = 0; i < node_id_list.size(); i++) {
    ret.push_back(nodetracker->GetNodeIP(node_id_list[i]));
  }
  return ret;
}
PhysicalPlanDescriptor LogicalOperator::GetBestPhysicalPlanDescriptor(
    const std::vector<PhysicalPlanDescriptor> list) const {
  PhysicalPlanDescriptor ret;
  ret.cost = UINT_MAX;
  for (unsigned i = 0; i < list.size(); i++) {
    if (list[i].cost < ret.cost) ret = list[i];
  }
  return ret;
}
int LogicalOperator::GetIdInAttributeList(
    const std::vector<Attribute>& attributes,
    const Attribute& attribute) const {
  for (unsigned i = 0; i < attributes.size(); i++) {
    if (attributes[i] == attribute) {
      return i;
    }
  }
  return -1;
}

}  // namespace logical_operator
}  // namespace claims
