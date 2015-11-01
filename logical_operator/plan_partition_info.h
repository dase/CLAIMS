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
 * ./LogicalPlan/plan_partition_info.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#ifndef LOGICAL_OPERATOR_PLAN_PARTITION_INFO_H_
#define LOGICAL_OPERATOR_PLAN_PARTITION_INFO_H_
#include "../common/ids.h"
namespace claims {
namespace logical_operator {
/**
 * the partition info of every partition is changing during executing them.
 */
class PlanPartitionInfo {
 public:
  friend class PlanPartitioner;
  PlanPartitionInfo() : filtered_(false){};
  PlanPartitionInfo(unsigned partitoin_id, int cardinality, NodeID location);
  PlanPartitionInfo(const PlanPartitionInfo& part);
  virtual ~PlanPartitionInfo();
  unsigned get_cardinality() const;
  void set_cardinality(const unsigned&);
  bool IsAvaliable() const;
  void set_filtered();
  NodeID get_location() const;

 protected:
  unsigned partition_id_;
  int cardinality_;
  NodeID location_;
  bool filtered_;
};

}  // namespace logical_operator
}  // namespace claims
#endif  //  LOGICAL_OPERATOR_PLAN_PARTITION_INFO_H_
