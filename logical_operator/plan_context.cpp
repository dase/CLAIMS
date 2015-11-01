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
 * ./LogicalPlan/plan_context.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#include "../logical_operator/plan_context.h"

#include <glog/logging.h>
#include <iosfwd>
#include <vector>
#include <iostream>
#include "../common/Schema/SchemaFix.h"
namespace claims {
namespace logical_operator {

PlanContext::PlanContext() {}

PlanContext::~PlanContext() {}
PlanContext::PlanContext(const PlanContext& plan_context) {
  attribute_list_ = plan_context.attribute_list_;
  commu_cost_ = plan_context.commu_cost_;
  plan_partitioner_ = plan_context.plan_partitioner_;
}
unsigned long PlanContext::GetAggregatedDatasize() const {
  const unsigned tuple_size = GetTupleSize();
  return plan_partitioner_.GetAggregatedDataCardinality() * tuple_size;
}
unsigned long PlanContext::GetAggregatedDataCardinality() const {
  return plan_partitioner_.GetAggregatedDataCardinality();
}
bool PlanContext::IsHashPartitioned() const {
  return plan_partitioner_.GetPartitionStyle() == PartitionFunction::hash_f;
}
Schema* PlanContext::GetSchema() const {
  /**
   * Only fixed schema is supported now.
   * TODO(wangli): support other schemas.
   */
  std::vector<column_type> columns;
  for (unsigned i = 0; i < attribute_list_.size(); i++) {
    columns.push_back(*attribute_list_[i].attrType);
  }
  return new SchemaFix(columns);
}
unsigned PlanContext::GetTupleSize() const {
  unsigned ret = 0;
  for (unsigned i = 0; i < attribute_list_.size(); i++) {
    ret += attribute_list_[i].attrType->get_length();
  }
  return ret;
}
Attribute PlanContext::GetAttribute(std::string name) const {
  for (unsigned i = 0; i < attribute_list_.size(); i++) {
    if (attribute_list_[i].attrName == name) {
      return attribute_list_[i];
    }
  }
  LOG(WARNING) << "Failed to find attribute " << name.c_str() << std::endl;
  return Attribute(ATTRIBUTE_NULL);
}

Attribute PlanContext::GetAttribute(std::string tbname,
                                    std::string colname) const {
  for (unsigned i = 0; i < attribute_list_.size(); i++) {
    if (attribute_list_[i].attrName == colname) {
      return attribute_list_[i];
    }
  }
  LOG(WARNING) << "Failed to find attribute " << colname.c_str() << std::endl;
  assert(false);
  return Attribute(ATTRIBUTE_NULL);
}
}  // namespace logical_operator
}  // namespace claims
