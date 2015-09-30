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
 * /CLAIMS/logical_query_plan/logical_operator.h
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli, fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *
 */
#ifndef LOGICAL_QUERY_PLAN_LOGICAL_OPERATOR_H_
#define LOGICAL_QUERY_PLAN_LOGICAL_OPERATOR_H_
#include <stdio.h>
#include <vector>
#include "./plan_context.h"
#include "../common/Schema/SchemaFix.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../logical_query_plan/Requirement.h"

namespace claims {
namespace logical_query_plan {

enum OperatorType {
  kLogicalScan,
  kLogicalFilter,
  kLogicalAggregation,
  kLogicalEqualJoin,
  kLogicalProject,
  kLogicalSort,
  kLogicalQueryPlanRoot,
  kLogicalCrossJoin
};

typedef BlockStreamIteratorBase* PhysicalPlan;
typedef struct PhysicalPlanDescriptor {
  PhysicalPlan plan;
  PlanContext plan_context_;
  unsigned cost;
};
class LogicalOperator {
 public:
  LogicalOperator(){};
  virtual ~LogicalOperator(){};

  /*get the data flow which is optimal in the local view.*
   * TODO: leverage recursive search for the global optimal. */
  virtual PlanContext GetPlanContext() = 0;

  /*
   * generate the iterator tree based on the computed dataflow. Hence,
   * this function can only be called after the calling of getDataflow()
   */
  virtual BlockStreamIteratorBase* GetPhysicalPlan(const unsigned&) = 0;

  /**
   * get the optimal Physical plan that meets the requirement.
   * @return true if find physical plan that meets the requirement and store the
   * physical plan and
   * its corresponding information in physical_plan_descriptor.
   */
  virtual bool GetOptimalPhysicalPlan(
      Requirement requirement, PhysicalPlanDescriptor& physical_plan_descriptor,
      const unsigned& block_size = 4096 * 1024){};

  virtual void Print(int level = 0) const = 0;

  inline OperatorType GetOperatorType() { return operator_type_; }

 protected:
  Schema* GetSchema(const std::vector<Attribute>&) const;
  Schema* GetSchema(const std::vector<Attribute>&,
                    const std::vector<Attribute>&) const;
  std::vector<NodeID> GetInvolvedNodeID(const PlanPartitioner&) const;
  std::vector<NodeIP> ConvertNodeIDListToNodeIPList(
      const std::vector<NodeID>&) const;
  PhysicalPlanDescriptor GetBestPhysicalPlanDescriptor(
      const std::vector<PhysicalPlanDescriptor>) const;
  int GetIdInAttributeList(const std::vector<Attribute>& attributes,
                           const Attribute& attribute) const;
  void Align(int space) const;

  inline void set_operator_type(OperatorType node_operator) {
    operator_type_ = node_operator;
  }

 private:
  OperatorType operator_type_;
};

}  // namespace logical_query_plan
}  // namespace claims

#endif  // LOGICAL_QUERY_PLAN_LOGICAL_OPERATOR_H_
