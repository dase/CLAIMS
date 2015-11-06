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
 * ./LogicalPlan/plan_partitioner.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#include "../logical_operator/plan_partitioner.h"

#include "../utility/print_tool.h"
namespace claims {
namespace logical_operator {
PlanPartitioner::PlanPartitioner(const Partitioner& partitioner) {
  partition_func_ = partitioner.getPartitionFunction();
  partition_key_ = partitioner.getPartitionKey();
  for (unsigned i = 0; i < partitioner.getNumberOfPartitions(); i++) {
    PlanPartitionInfo plan_partition_info;
    plan_partition_info.cardinality_ = partitioner.getPartitionCardinality(i);
    plan_partition_info.location_ = partitioner.getPartitionLocation(i);
    plan_partition_info.partition_id_ = i;
    partition_list_.push_back(plan_partition_info);
  }
}
PlanPartitioner::PlanPartitioner(const PlanPartitioner& r) {
  this->partition_func_ = r.partition_func_;
  this->partition_key_ = r.partition_key_;
  this->partition_list_ = r.partition_list_;
  this->shadow_partition_keys_ = r.shadow_partition_keys_;
}
PlanPartitioner::~PlanPartitioner() {}
Attribute PlanPartitioner::get_partition_key() const { return partition_key_; }

bool PlanPartitioner::HasSamePartitionLocation(
    const PlanPartitioner& target) const {
  if (partition_list_.size() != target.partition_list_.size()) return false;
  for (unsigned i = 0; i < partition_list_.size(); i++) {
    if (partition_list_[i].IsAvaliable() &&
        target.partition_list_[i].IsAvaliable()) {
      /**
       * if no party is filtered.
       */
      if (partition_list_[i].location_ != target.partition_list_[i].location_)
        return false;
    }
  }
  return true;
}

unsigned PlanPartitioner::GetAggregatedDataSize() const {
  unsigned ret = 0;
  for (unsigned i = 0; i < partition_list_.size(); i++) {
    ret += partition_list_[i].cardinality_;
  }
  return ret;
}
unsigned long PlanPartitioner::GetAggregatedDataCardinality() const {
  unsigned long ret = 0;
  for (unsigned i = 0; i < partition_list_.size(); i++) {
    ret += partition_list_[i].get_cardinality();
  }
  return ret;
}
PartitionFunction::PartitionStyle PlanPartitioner::GetPartitionStyle() const {
  return partition_func_->getPartitionFashion();
}
unsigned PlanPartitioner::GetNumberOfPartitions() const {
  return partition_list_.size();
}
PlanPartitionInfo* PlanPartitioner::GetPartition(unsigned index) const {
  assert(index < partition_list_.size());
  PlanPartitionInfo* ret = (PlanPartitionInfo*)&partition_list_[index];
  return ret;
}
PartitionFunction* PlanPartitioner::get_partition_func() const {
  return partition_func_;
}
void PlanPartitioner::set_partition_key(const Attribute& partitionkey) {
  partition_key_ = partitionkey;
}
void PlanPartitioner::AddShadowPartitionKey(const Attribute& partitionkey) {
  shadow_partition_keys_.push_back(partitionkey);
}

void PlanPartitioner::set_partition_func(
    PartitionFunction* partition_function) {
  partition_func_ = partition_function;
}
bool PlanPartitioner::HasShadowPartitionKey() const {
  return !shadow_partition_keys_.empty();
}
std::vector<Attribute> PlanPartitioner::get_shadow_partition_keys() const {
  return shadow_partition_keys_;
}

std::vector<PlanPartitionInfo> PlanPartitioner::get_partition_list() const {
  return partition_list_;
}
void PlanPartitioner::set_partition_list(
    const std::vector<PlanPartitionInfo>& list) {
  partition_list_ = list;
}

}  // namespace logical_operator
}  // namespace claims
