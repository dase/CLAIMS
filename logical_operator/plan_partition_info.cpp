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
 * ./LogicalPlan/plan_partition_info.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */
#include "../logical_operator/plan_partition_info.h"
namespace claims {
namespace logical_operator {
PlanPartitionInfo::PlanPartitionInfo(unsigned partition_id, int datasize,
                                     NodeID location)
    : partition_id_(partition_id),
      cardinality_(datasize),
      location_(location),
      filtered_(false) {}

PlanPartitionInfo::~PlanPartitionInfo() {
  // TODO Auto-generated destructor stub
}
PlanPartitionInfo::PlanPartitionInfo(const PlanPartitionInfo& part) {
  this->partition_id_ = part.partition_id_;
  this->location_ = part.location_;
  this->cardinality_ = part.cardinality_;
  this->filtered_ = part.filtered_;
}
unsigned PlanPartitionInfo::get_cardinality() const {
  if (filtered_) return 0;
  return cardinality_;
}
void PlanPartitionInfo::set_cardinality(const unsigned& size) {
  cardinality_ = size;
}
bool PlanPartitionInfo::IsAvaliable() const { return !filtered_; }
void PlanPartitionInfo::set_filtered() {
  filtered_ = true;
  cardinality_ = 0;
}
NodeID PlanPartitionInfo::get_location() const { return location_; }
}  // namespace logical_operator
}  // namespace claims
