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
 * /Claims/logical_operator/logical_limit.cpp
 *  Created on: Sep 21, 2015
 *  Modified on: Nov 16, 2015
 *      Author: wangli，HanZhang, tonglanxuan
 *       Email: wangli1426@gmail.com
 *
 * Description: Implementation of Limit operator in logical layer
 *
 */

#include "../logical_operator/logical_limit.h"
#include <stdio.h>
#include "../physical_operator/physical_limit.h"
using claims::physical_operator::PhysicalLimit;
namespace claims {
namespace logical_operator {
LogicalLimit::LogicalLimit(LogicalOperator* child, int64_t returned_tuples,
                           int64_t position)
    : LogicalOperator(kLogicalLimit),
      child_(child),
      plan_context_(NULL),
      returned_tuples_(returned_tuples),
      start_position_(position) {}

LogicalLimit::~LogicalLimit() {
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}

bool LogicalLimit::CanBeOmitted() const {
  /**
   * TODO(Anyone): A method to judge whether Limit can be omitted, that is to
   * judge if returned_tuples_ equals the row number of table.
   */
  return (returned_tuples_ == -1 && start_position_ == 0);
}

PlanContext LogicalLimit::GetPlanContext() {
  lock_->acquire();
  if (NULL != plan_context_) {
    lock_->release();
    return *plan_context_;
  }
  PlanContext plan_context = child_->GetPlanContext();
  if (plan_context.IsHashPartitioned()) {
    if (!CanBeOmitted()) {  // Call predictSelectivity to alter cardinality
                            // of each part
                            /**
                             * TODO(Anyone): A specified method to set each partition a proper
                             * cardinality.
                             * In this version, we maintain them unchanged.
                             */
      for (unsigned i = 0;
           i < plan_context.plan_partitioner_.GetNumberOfPartitions(); ++i) {
        plan_context.plan_partitioner_.GetPartition(i)
            ->set_cardinality(PredictCardinality(i, plan_context));
      }
    }
  }
  plan_context_ = new PlanContext();
  *plan_context_ = plan_context;
  lock_->release();
  return plan_context;
}

PhysicalOperatorBase* LogicalLimit::GetPhysicalPlan(const unsigned& blocksize) {
  PlanContext plan_context = GetPlanContext();
  PhysicalOperatorBase* child_iterator = child_->GetPhysicalPlan(blocksize);
  PhysicalLimit::State state(GetSchema(plan_context.attribute_list_),
                             child_iterator, returned_tuples_, blocksize,
                             start_position_);
  PhysicalOperatorBase* limit = new PhysicalLimit(state);
  return limit;
}

void LogicalLimit::Print(int level) const {
  if (!CanBeOmitted()) {
    printf("With limit constraint: %I64d, %I64d\n", returned_tuples_,
           start_position_);
  }
}

const unsigned LogicalLimit::PredictCardinality(
    unsigned i, const PlanContext& plan_context) {
  /**
   * TODO(Anyone): A specified method to set each partition a proper
   * cardinality.
   * In this version, we maintain them unchanged.
   */
  return plan_context.plan_partitioner_.GetPartition(i)->get_cardinality();
}

}  // namespace logical_operator
}  // namespace claims
