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
 * /libeasy/include/afda.h
 *
 *  Created on: Jul 14, 2014
 *      Author: minqi, wangli1426, fzhedu
 *       Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description:
 *
 */

#ifndef LOGICAL_OPERATOR_LOGICAL_CROSS_JOIN_H_
#define LOGICAL_OPERATOR_LOGICAL_CROSS_JOIN_H_

#include "../physical_operator/physical_operator_base.h"
#include "../common/error_define.h"
#include "../logical_operator/logical_operator.h"

namespace claims {
namespace logical_operator {
/***
 * @brief cross join for generating Cartesian product
 */

class LogicalCrossJoin : public LogicalOperator {
 public:
  LogicalCrossJoin();
  LogicalCrossJoin(LogicalOperator* left_child, LogicalOperator* right_child);
  virtual ~LogicalCrossJoin();
  PlanContext GetPlanContext();
  PhysicalOperatorBase* GetPhysicalPlan(const unsigned& blocksize);
  void Print(int level = 0) const;

 protected:
  /**
   * The enum JoinPolicy is to enumerate the join policy used in the Cartesian
   * product, it includes:
   *  KLocalJoin:    local join which joined on the local node
   *  leftbroadcast: which broadcast the left table to the nodes which contains
   *                 the partitions of the right table
   *  rightbroadcast: which broadcast the right table to the nodes which
   *                contains the partitions of the left table
   */
  enum JoinPolicy {
    kUninitialized,
    kLeftBroadcast,
    kRightBroadcast,
    kLocalJoin
  };

  int get_join_policy_();

 private:
  /**
   * @brief  Method description : decide the join policy based on the plan
   * context of both of the left and right child
   * @param left, the plan_context of the left child
   * @param right, the plan_context of the right child
   * @ return the errorcode
   * @details   (additional)
   */

  int DecideJoinPolicy(const PlanContext& left, const PlanContext& right);
  /** return true if the nest-loop-join can be conducted locally**/
  bool CanLocalJoin(const PlanContext& left, const PlanContext& right) const;
  /**
   * @brief  Method description : generate the child sub physical plan for the
   * Cartesian product
   * @param left_child_iterator_tree  left child sub physical plan
   * @param right_child_iterator_tree  right child sub physical plan
   * @return error code
   * @details   (additional) */

  int GenerateChildPhysicalQueryPlan(
      PhysicalOperatorBase*& left_child_iterator_tree,
      PhysicalOperatorBase*& right_child_iterator_tree,
      const unsigned& blocksize);

  LogicalOperator* left_child_;
  LogicalOperator* right_child_;
  PlanContext* plan_context_;
  JoinPolicy join_policy_;
};
}  // namespace logical_operator
}  // namespace claims
#endif  // LOGICAL_OPERATOR_LOGICAL_CROSS_JOIN_H_
