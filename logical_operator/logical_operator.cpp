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
#include <map>
#include <vector>
#include <string>

#include "../common/expression/expr_column.h"
#include "../common/expression/expr_node.h"
#include "../logical_operator/logical_project.h"
#include "../Resource/NodeTracker.h"

using claims::common::ExprColumn;
using claims::common::ExprNode;
using claims::common::ExprNodeType;
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
void LogicalOperator::GetColumnToId(const std::vector<Attribute>& attributes,
                                    map<string, int>& column_to_id) {
  for (int i = 0; i < attributes.size(); ++i) {
    column_to_id[attributes[i].attrName] = i;
  }
}
LogicalOperator* LogicalOperator::DecideAndCreateProject(
    set<string>& attrs, LogicalOperator* child) {
  LogicalOperator* ret = child;
  auto child_attr_list = child->GetPlanContext().attribute_list_;
  // get the position where the attribute from child should be pruned
  vector<int> keep_id;
  for (int i = 0, size = child_attr_list.size(); i < size; ++i) {
    bool need_prune = true;
    for (auto it = attrs.begin(); it != attrs.end() && need_prune; ++it) {
      if (*it == child_attr_list[i].attrName) {
        need_prune = false;
      }
    }
    if (!need_prune) {
      keep_id.push_back(i);
    }
  }
  // if there are attributes should be pruned, then create project
  if (keep_id.size() < child_attr_list.size()) {
    vector<ExprNode*> expression_list;
    for (int i = 0, size = keep_id.size(); i < size; ++i) {
      int pos = child_attr_list[keep_id[i]].attrName.find('.');
      int len = child_attr_list[keep_id[i]].attrName.length();
      expression_list.push_back(new ExprColumn(
          ExprNodeType::t_qcolcumns, child_attr_list[keep_id[i]].attrType->type,
          child_attr_list[keep_id[i]].attrName,
          child_attr_list[keep_id[i]].attrName.substr(0, pos),
          child_attr_list[keep_id[i]].attrName.substr(pos + 1, len)));
    }
    // if no need to provide one column, then choose the first column or don't
    // add project
    if (keep_id.size() == 0 && child_attr_list.size() > 3) {
      int pos = child_attr_list[0].attrName.find('.');
      int len = child_attr_list[0].attrName.length();
      expression_list.push_back(new ExprColumn(
          ExprNodeType::t_qcolcumns, child_attr_list[0].attrType->type,
          child_attr_list[0].attrName,
          child_attr_list[0].attrName.substr(0, pos),
          child_attr_list[0].attrName.substr(pos + 1, len)));
    }
    if (expression_list.size() > 0) {
      ret = new LogicalProject(child, expression_list);
    }
  }
  return ret;
}

}  // namespace logical_operator
}  // namespace claims
