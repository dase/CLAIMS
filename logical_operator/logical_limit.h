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
 * /CLAIMS/logical_operator/logical_limit.h
 *
 *  Created on: Sep 18, 2015
 *  Modified on: Nov 16, 2015
 *      Author: wangli，HanZhang, tonglanxuan
 *       Email: wangli1426@gmail.com
 *
 * Description: Implementation of Limit operator in logical layer
 */

#ifndef LOGICAL_OPERATOR_LOGICAL_LIMIT_H_
#define LOGICAL_OPERATOR_LOGICAL_LIMIT_H_

#include "../logical_operator/logical_operator.h"
namespace claims {
namespace logical_operator {
/**
 * @brief Method description:
 * start_position_:the default value is 0,it means the position where we should
 * acquire data.
 * return_tuples_:the default value was -1, it means that all rows are needed,
 * but in the current version, return_tuples_ can't get -1, we use another
 * method to control the value of it.
 */
class LogicalLimit : public LogicalOperator {
 public:
  /**
   * @brief Method description: Construction.
   * @param child : Its child logical operator.
   * @param returned_tuples: The number of rows returned, started from position
   * . We should notice that if the returned_tuples + position > real row
   * number, we only return tuples till the last row, that means in that case,
   * the number of rows returned is smaller than the value of returned_tuples.
   * @param position: The skipped rows number, and it can be omitted, if we
   * write limit 0, 5; this is the same as limit 5;, if we write limit 1, 5;
   * we get 5 rows from the second row.
   */
  LogicalLimit(LogicalOperator* child, int64_t returned_tuples,
               int64_t position);
  /**
   * @brief Method description: Destruction.
   */
  virtual ~LogicalLimit();
  /**
   * @brief Method description: To judge if the limit operator can be omitted,
   *                            currently, we judge it on the analysis level,
   *                            so here we return false in every cases.
   */
  bool CanBeOmitted() const;
  /**
   * @brief Method description: To get the plan context from its child logical
   *                            operator, operates on it, thus give it to
   *                            its father logical operator.
   * @return PlanContext
   */
  PlanContext GetPlanContext();
  /**
   * @brief Method description: To get the Iterator Tree from its child logical
   *                            operator, generates a state_ for establishing a
   *                            physical execution plan.
   * @param blocksize: The size of block in the CLAIMS project.
   * @return BlockStreamIteratorBase*: A block stream iterator pointer,
   *         pointing to the root node of limit's iterator tree.
   */
  PhysicalOperatorBase* GetPhysicalPlan(const unsigned& blocksize);
  PhysicalOperatorBase* GetPhysicalPlan(const unsigned& blocksize,
                                        PhysicalOperatorBase* child_iterator);

  /**
   * @brief Method description: To print information of returned_tuples_ and
   * start_position_.
   * @param level: As an index.
   */
  virtual void Print(int level = 0) const;

  LogicalOperator* child_;

 private:
  const unsigned PredictCardinality(unsigned i,
                                    const PlanContext& plan_context);
  PlanContext* plan_context_;
  int64_t returned_tuples_;
  int64_t start_position_;
};
}  // namespace logical_operator
}  // namespace claims

#endif  //  LOGICAL_OPERATOR_LOGICAL_LIMIT_H_
