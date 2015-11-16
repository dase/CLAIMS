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
 * /Claims/logical_operator/logical_subquery.h
 *
 *  Created on: Nov 11, 2015
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef LOGICAL_OPERATOR_LOGICAL_SUBQUERY_H_
#define LOGICAL_OPERATOR_LOGICAL_SUBQUERY_H_

#include "./logical_operator.h"
#include <string>
#include <vector>

#include "../Catalog/Attribute.h"
using std::string;
namespace claims {
namespace logical_operator {
/**
 * LogicalSubquery is needed for maintain the consistency of data logical
 * description, the table and attribute in subquery will be aliased.
 */
class LogicalSubquery : public LogicalOperator {
 public:
  LogicalSubquery(LogicalOperator *child, string subquery_alias);
  virtual ~LogicalSubquery();
  PlanContext GetPlanContext();
  PhysicalOperatorBase *GetPhysicalPlan(const unsigned &blocksize);
  void Print(int level = 0) const;

 private:
  vector<Attribute> subquery_attrs_;
  PlanContext *plan_context_;
  LogicalOperator *child_;
  string subquery_alias_;
};
}  // namespace logical_operator
} /* namespace claims */

#endif  //  LOGICAL_OPERATOR_LOGICAL_SUBQUERY_H_
