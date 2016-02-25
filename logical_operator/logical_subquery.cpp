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
 * /Claims/logical_operator/logical_subquery.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include "./logical_subquery.h"

#include <string>
#include <vector>

namespace claims {
namespace logical_operator {
LogicalSubquery::LogicalSubquery(LogicalOperator *child, string subquery_alias)
    : LogicalOperator(kLogicalSubquery),
      child_(child),
      subquery_alias_(subquery_alias),
      plan_context_(NULL) {}

LogicalSubquery::~LogicalSubquery() {
  if (NULL != plan_context_) {
    delete plan_context_;
    plan_context_ = NULL;
  }
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}

PlanContext LogicalSubquery::GetPlanContext() {
  lock_->acquire();
  if (NULL != plan_context_) {
    lock_->release();
    return *plan_context_;
  }
  PlanContext ret;
  // get the PlanContext of child
  const PlanContext child_plan_context = child_->GetPlanContext();
  /**
   * set commnication_cost and partitioner of the PlanContext to be returned
   * from PlanContext of child
   */
  ret.commu_cost_ = child_plan_context.commu_cost_;
  ret.plan_partitioner_ = child_plan_context.plan_partitioner_;
  ret.plan_partitioner_.UpdateTableNameOfPartitionKey(subquery_alias_);
  // construct an input schema from attribute list of child
  subquery_attrs_ = child_plan_context.attribute_list_;
  /**
   * select * from (select tb1.a, tb2.a from tb1,tb2) as tb0;
   * ERROR 1060 (42S21): Duplicate column name
   * from the result of MariaDB, so we can see to reconstruct attribute from
   * subquery need to change the table name of TB.COL in select list attribute.
   */
  for (int i = 0; i < subquery_attrs_.size(); ++i) {
    subquery_attrs_[i].attrName =
        subquery_alias_ + "." +
        subquery_attrs_[i].attrName.substr(
            subquery_attrs_[i].attrName.find('.') + 1);
  }
  ret.attribute_list_ = subquery_attrs_;
  plan_context_ = new PlanContext();
  // set the PlanContext to be returned
  *plan_context_ = ret;
  lock_->release();
  return ret;
}
PhysicalOperatorBase *LogicalSubquery::GetPhysicalPlan(
    const unsigned &blocksize) {
  return child_->GetPhysicalPlan(blocksize);
}
void LogicalSubquery::Print(int level) const {
  cout << setw(level * kTabSize) << " "
       << "Subquery: " << subquery_alias_ << endl;
  GetPlanContext();
  cout << setw(level * kTabSize) << " "
       << "[Partition info: "
       << plan_context_->plan_partitioner_.get_partition_key().attrName
       << " table_id= "
       << plan_context_->plan_partitioner_.get_partition_key().table_id_
       << " column_id= "
       << plan_context_->plan_partitioner_.get_partition_key().index << " ]"
       << endl;
  ++level;
  for (int i = 0; i < subquery_attrs_.size(); ++i) {
    cout << setw(level * kTabSize) << " " << subquery_attrs_[i].attrName
         << endl;
  }
  --level;
  cout << "--------------------------" << endl;
  child_->Print(level + 1);
}

}  // namespace logical_operator

} /* namespace claims */
