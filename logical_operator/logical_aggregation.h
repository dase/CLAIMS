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
 * ./LogicalPlan/logical_aggregation.h
 *
 *  Created on: Nov 11,2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description: Aggregation operator is designed for executing group by and
 * aggregation function. LogicalAggregation is the logical type of Aggregation
 * operator, main function includes getting data context after executing this
 * operator and generating corresponding physical operator.
 */
#ifndef LOGICAL_OPERATOR_LOGICAL_AGGREGATION_H_
#define LOGICAL_OPERATOR_LOGICAL_AGGREGATION_H_
#include <string>
#include <map>
#include <vector>

#include "../common/expression/expr_node.h"
#include "../catalog/attribute.h"
#include "../logical_operator/logical_operator.h"
#include "../physical_operator/physical_aggregation.h"
#include "../physical_operator/physical_operator_base.h"
#include "../common/expression/expr_unary.h"

using claims::common::ExprNode;
using claims::common::ExprUnary;
using claims::physical_operator::PhysicalAggregation;
using claims::physical_operator::PhysicalOperatorBase;

namespace claims {
namespace logical_operator {
#define NEWCONDI
/**
 * @brief class description: maintain operator property that results from
 * executing aggregation operator, and generate corresponding physical operator.
 * @details as for the implementation of avg() based on sum()/count(), so it's
 * necessary to change the corresponding avg() to sum(), and find count() column
 * id, if there isn't count(), append additional count(*) at the end of
 * aggregation_attrs[]. so the schema is also changed, but it doesn't matter.
 * and all count() = count(1) = sum(1).
 * Note: kReparAndGlobalAgg node isn't implemented because it's inefficient by
 * comparing with kLocalAggAndGlobalAgg.
 * TODO(FZH) due to count(*) is different from count(expr) for NULL, and now the
 * implementation doesn't consider this.
 */

class LogicalAggregation : public LogicalOperator {
 public:
  /**
   * AggregationStyle describe the implementation of aggregation judged from
   * whether the data distribution agrees with the group by attributes or the
   * data is local.
   */
  enum AggregationStyle {
    kLocalAgg,  // the data is local or it's distribution agrees with the group
                // by attributes, one aggregation is enough.
    kReparGlobalAgg,         // otherwise first repartition raw data and then
                             // aggregation
    kLocalAggReparGlobalAgg  // additionally, first do local aggregation
    // based on raw data, second repartition the intermediate
    // result, and last do global aggregation.
  };

  /**
   * This is the default construct method of Aggregation.
   * This logical operator creates a new plan context.
   * group_by_attribute can be empty if it is scalar aggregation.
   * count(*) aggregation can be expressed as 'count' aggregation function along
   * with Attribute(ATTRIBUTE_ANY)
   */
  LogicalAggregation(std::vector<Attribute> group_by_attribute_list,
                     std::vector<Attribute> aggregation_attribute_list,
                     std::vector<PhysicalAggregation::State::Aggregation>
                         aggregation_function_list, LogicalOperator* child);
  LogicalAggregation(vector<ExprNode*> group_by_attrs,
                     vector<ExprUnary*> aggregation_attrs,
                     LogicalOperator* child);
  virtual ~LogicalAggregation();
  void PruneProj(set<string>& above_attrs);

 protected:
  /**
   * get plan context resulting from executing aggregation operator.
   * including corresponding attribute_list and property with partition info and
   * communication cost.
   */
  PlanContext GetPlanContext();
  /**
   * generate corresponding physical plan tree, mainly set necessary state_.
   */
  PhysicalOperatorBase* GetPhysicalPlan(const unsigned& block_size);

 private:
  // store the avg() column id, change avg() operation to sum(), and find one
  // count() column id.
  void ChangeAggAttrsForAVG();
  bool CanOmitHashRepartition(const PlanContext& child_plan_context) const;
  float EstimateSelectivity() const;
  void Print(int level = 0) const;
  // for global agg, reconstruct the group by attributes and aggregation
  // attributes
  void SetGroupbyAndAggAttrsForGlobalAgg(vector<ExprNode*>& group_by_attrs,
                                         vector<ExprUnary*>& agg_attrs,
                                         Schema* input_schema);
  /**
   * generate a list containing attributes that are generated by the aggregation
   * operator.
   */
  std::vector<Attribute> GetAttrsAfterAgg() const;
  std::vector<Attribute> GetGroupByAttrsAfterAgg() const;
  std::vector<Attribute> GetAggAttrsAfterAgg() const;

  int64_t EstimateGroupByCardinality(const PlanContext& plan_context) const;

  LogicalOperator* child_;
  vector<ExprNode*> group_by_attrs_;
  vector<ExprUnary*> aggregation_attrs_;
  vector<unsigned> avg_id_in_agg_;
  int count_column_id_;
  PlanContext* plan_context_;
  AggregationStyle aggregation_style_;
};
}  // namespace logical_operator
}  // namespace claims
#endif  // LOGICAL_OPERATOR_LOGICAL_AGGREGATION_H_
