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
 * ./LogicalPlan/plan_context.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#ifndef LOGICAL_OPERATOR_PLAN_CONTEXT_H_
#define LOGICAL_OPERATOR_PLAN_CONTEXT_H_
#include <vector>
#include "../Catalog/Column.h"
#include "../Catalog/Partitioner.h"
#include "../common/Schema/Schema.h"
#include "../logical_operator/plan_partitioner.h"
namespace claims {
namespace logical_operator {
/**
 * maintain the data property of the plan after having completed current
 * operator, mainly include the attribute_list_, commu_cost_ and
 * plan_partitioner_. the PlanContext originates the leaf of the plan (i.e. scan
 * operator), the PlanContext of current operator generates from it's children.
 */
class PlanContext {
  friend class LogcalOperator;
 public:
  PlanContext();
  PlanContext(const PlanContext& plan_context);
  virtual ~PlanContext();
  /**
   * after having executing current operator, the data size could be changed,
   * the data size = total tuples * tuple size.
   */
  unsigned long GetAggregatedDatasize() const;
  // cardinality = tuple number
  unsigned long GetAggregatedDataCardinality() const;
  bool IsHashPartitioned() const;
  Schema* GetSchema() const;
  unsigned GetTupleSize() const;
  Attribute GetAttribute(std::string name) const;
  Attribute GetAttribute(std::string tbname, std::string colname) const;

 public:
  std::vector<Attribute> attribute_list_;  // maintain the output attributes
                                           // after having completed current
                                           // operator.
  unsigned long commu_cost_;               // communication cost
  // describe the information of the data partition
  PlanPartitioner plan_partitioner_;
};

}  // namespace logical_operator
}  // namespace claims
#endif  // LOGICAL_OPERATOR_PLAN_CONTEXT_H_
