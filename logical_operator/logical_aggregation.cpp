
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
 * operator, main function includes getting plan context after executing this
 * operator and generating corresponding physical operator.
 */
#define GLOG_NO_ABBREVIATED_SEVERITIES  // avoid macro conflict
#include <glog/logging.h>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "../common/expression/expr_node.h"
#include "../common/expression/expr_column.h"
#include "../common/expression/expr_const.h"
#include "../common/expression/expr_unary.h"
#include "../logical_operator/logical_aggregation.h"
#include "../logical_operator/plan_context.h"
#include "../IDsGenerator.h"
#include "../Config.h"
#include "../catalog/stat/StatManager.h"
#include "../physical_operator/exchange_merger.h"
#include "../physical_operator/expander.h"
#include "../physical_operator/physical_aggregation.h"

using claims::physical_operator::ExchangeMerger;
using claims::physical_operator::Expander;
using std::vector;
using claims::common::ExprNode;
using claims::common::ExprNodeType;
using claims::common::ExprUnary;
using claims::common::OperType;
using claims::common::ExprConst;
using claims::common::ExprColumn;
using claims::common::LogicInitCnxt;

using claims::physical_operator::PhysicalAggregation;
namespace claims {
namespace logical_operator {
LogicalAggregation::LogicalAggregation(
    std::vector<Attribute> group_by_attribute_list,
    std::vector<Attribute> aggregation_attribute_list,
    std::vector<PhysicalAggregation::State::Aggregation>
        aggregation_function_list, LogicalOperator* child)
    : LogicalOperator(kLogicalAggregation) {
  assert(false);
}

LogicalAggregation::LogicalAggregation(vector<ExprNode*> group_by_attrs,
                                       vector<ExprUnary*> aggregation_attrs,
                                       LogicalOperator* child)
    : LogicalOperator(kLogicalAggregation),
      group_by_attrs_(group_by_attrs),
      aggregation_attrs_(aggregation_attrs),
      plan_context_(NULL),
      child_(child) {}

LogicalAggregation::~LogicalAggregation() {
  if (NULL != plan_context_) {
    delete plan_context_;
    plan_context_ = NULL;
  }
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}
// due to avg() column may be aliased, so judge the agg type by oper_type_
void LogicalAggregation::ChangeAggAttrsForAVG() {
  for (int i = 0; i < aggregation_attrs_.size(); ++i) {
    if (OperType::oper_agg_avg == aggregation_attrs_[i]->oper_type_) {
      // change the operation type avg to sum
      aggregation_attrs_[i]->oper_type_ = OperType::oper_agg_sum;
      avg_id_in_agg_.push_back(i);
    }
  }
  if (avg_id_in_agg_.size() > 0) {
    // if there isn't count() in agg, then add it and push the id into
    // avg_id_in_agg
    count_column_id_ = -1;
    for (int i = 0; i < aggregation_attrs_.size(); ++i) {
      if (OperType::oper_agg_count == aggregation_attrs_[i]->oper_type_) {
        count_column_id_ = i;
        break;
      }
    }
    // if there isn't count(), append count(1)
    if (count_column_id_ == -1) {
      count_column_id_ = aggregation_attrs_.size();
      aggregation_attrs_.push_back(new ExprUnary(
          ExprNodeType::t_qexpr_unary, t_u_long, "ACOUNT(1)",
          OperType::oper_agg_count,
          new ExprConst(ExprNodeType::t_qexpr, t_u_long, "COUNT(1)", "1")));
    }
  }
}

PlanContext LogicalAggregation::GetPlanContext() {
  lock_->acquire();
  if (NULL != plan_context_) {
    lock_->release();
    return *plan_context_;
  }
  PlanContext ret;
  const PlanContext child_context = child_->GetPlanContext();

  ChangeAggAttrsForAVG();
  // initialize expression of group_by_attrs and aggregation_attrs
  LogicInitCnxt licnxt;
  licnxt.schema0_ = child_context.GetSchema();
  GetColumnToId(child_context.attribute_list_, licnxt.column_id0_);

  for (int i = 0; i < group_by_attrs_.size(); ++i) {
    licnxt.return_type_ = group_by_attrs_[i]->actual_type_;
    group_by_attrs_[i]->InitExprAtLogicalPlan(licnxt);
  }
  for (int i = 0; i < aggregation_attrs_.size(); ++i) {
    licnxt.return_type_ = aggregation_attrs_[i]->actual_type_;
    aggregation_attrs_[i]->InitExprAtLogicalPlan(licnxt);
  }

  if (CanOmitHashRepartition(child_context)) {
    aggregation_style_ = kLocalAgg;
    LOG(INFO) << "Aggregation style: kLocalAgg" << std::endl;
  } else {  // as for the kLocalAggReparGlobalAgg style is optimal
            // to kReparAndGlobalAgg so it's set to be default.
    aggregation_style_ = kLocalAggReparGlobalAgg;
    LOG(INFO) << "Aggregation style: kLocalAggReparGlobalAgg" << std::endl;
  }
  switch (aggregation_style_) {
    case kLocalAgg: {
      ret.attribute_list_ = GetAttrsAfterAgg();
      ret.commu_cost_ = child_context.commu_cost_;
      ret.plan_partitioner_ = child_context.plan_partitioner_;
      Attribute partition_key =
          child_context.plan_partitioner_.get_partition_key();
      partition_key.table_id_ = INTERMEIDATE_TABLEID;
      ret.plan_partitioner_.set_partition_key(partition_key);
      for (unsigned i = 0; i < ret.plan_partitioner_.GetNumberOfPartitions();
           i++) {
        const unsigned cardinality =
            ret.plan_partitioner_.GetPartition(i)->get_cardinality();
        ret.plan_partitioner_.GetPartition(i)
            ->set_cardinality(EstimateGroupByCardinality(child_context) /
                              ret.plan_partitioner_.GetNumberOfPartitions());
      }
      break;
    }
    default: {
      /**
       * repartition aggregation is currently simplified.
       */

      // TODO(FZH): ideally, the partition properties (especially the the number
      // of partitions and partition style) after repartition aggregation should
      // be decided by the partition property enforcement.
      ret.attribute_list_ = GetAttrsAfterAgg();
      ret.commu_cost_ =
          child_context.commu_cost_ + child_context.GetAggregatedDatasize();
      ret.plan_partitioner_.set_partition_func(
          child_context.plan_partitioner_.get_partition_func());
      // set partition key
      if (group_by_attrs_.empty()) {
        ret.plan_partitioner_.set_partition_key(Attribute());
      } else {
        int id = 0;
        // if there is column in groupby attributes, so move it to the front, in
        // order to get partition by one column not one expression
        for (int i = 0; i < group_by_attrs_.size(); ++i) {
          if (group_by_attrs_[i]->expr_node_type_ == t_qcolcumns) {
            id = i;
            break;
          }
        }
        std::swap(group_by_attrs_[0], group_by_attrs_[id]);
        ret.plan_partitioner_.set_partition_key(
            group_by_attrs_[0]->ExprNodeToAttr(0));
      }
      /// set location of this coordinator, should invoke
      /// coordinator.get_location()
      NodeID location = 0;
      int64_t data_cardinality = EstimateGroupByCardinality(child_context);
      PartitionOffset offset = 0;
      PlanPartitionInfo par(offset, data_cardinality, location);
      std::vector<PlanPartitionInfo> partition_list;
      partition_list.push_back(par);
      ret.plan_partitioner_.set_partition_list(partition_list);
      break;
    }
  }
  plan_context_ = new PlanContext();
  *plan_context_ = ret;
  lock_->release();
  return ret;
}

bool LogicalAggregation::CanOmitHashRepartition(
    const PlanContext& child_plan_context) const {
  if (child_plan_context.plan_partitioner_.GetNumberOfPartitions() == 1 &&
      child_plan_context.plan_partitioner_.GetPartition(0)->get_location() == 0)
    return true;
  if (!child_plan_context.IsHashPartitioned()) return false;

  /**
   * the hash property of the input data can be leveraged in the
   * aggregation as long as the hash attribute is one of the group-by
   * attributes.
   */
  const Attribute partition_key =
      child_plan_context.plan_partitioner_.get_partition_key();
  for (int i = 0; i < group_by_attrs_.size(); ++i) {
    if (group_by_attrs_[i]->IsEqualAttr(partition_key)) {
      return true;
    }
  }
  return false;
}
/** replace each group by output attributes with one column, and change every
 * agg(expr) to agg(column[agg_expr_name]), for example: select sum(c)/2 from
 * TB group by a+b; => select sum(column["NULL_MID","sum(c)/2"]) from TB group
 * by column[ "NULL_MID", "a+b" ];
 * then initialize the new expression.
 */
void LogicalAggregation::SetGroupbyAndAggAttrsForGlobalAgg(
    vector<ExprNode*>& group_by_attrs, vector<ExprUnary*>& aggregation_attrs,
    Schema* input_schema) {
  ExprNode* group_by_node = NULL;
  ExprUnary* agg_node = NULL;
  LogicInitCnxt licnxt;
  licnxt.schema0_ = input_schema;
  int group_by_size = group_by_attrs_.size();
  // map column name to id
  for (int i = 0; i < group_by_size; ++i) {
    licnxt.column_id0_["NULL_MID." + group_by_attrs_[i]->alias_] = i;
  }
  for (int i = 0; i < aggregation_attrs_.size(); ++i) {
    licnxt.column_id0_["NULL_MID." + aggregation_attrs_[i]->alias_] =
        i + group_by_size;
  }
  // reconstruct group by attributes and initialize them
  for (int i = 0; i < group_by_attrs_.size(); ++i) {
    group_by_node = new ExprColumn(
        ExprNodeType::t_qcolcumns, group_by_attrs_[i]->actual_type_,
        group_by_attrs_[i]->alias_, "NULL_MID", group_by_attrs_[i]->alias_);
    licnxt.return_type_ = group_by_node->actual_type_;
    group_by_node->InitExprAtLogicalPlan(licnxt);
    group_by_attrs.push_back(group_by_node);
  }
  // reconstruct aggregation attributes and initialize them
  for (int i = 0; i < aggregation_attrs_.size(); ++i) {
    agg_node = new ExprUnary(
        ExprNodeType::t_qexpr_unary, aggregation_attrs_[i]->actual_type_,
        aggregation_attrs_[i]->alias_, aggregation_attrs_[i]->oper_type_,
        new ExprColumn(ExprNodeType::t_qexpr,
                       aggregation_attrs_[i]->actual_type_,
                       aggregation_attrs_[i]->alias_, "NULL_MID",
                       aggregation_attrs_[i]->alias_));
    licnxt.return_type_ = agg_node->actual_type_;
    agg_node->InitExprAtLogicalPlan(licnxt);
    aggregation_attrs.push_back(agg_node);
  }
}
/**
 * Note: if group_by_attribute_list_ is empty, the partition key is
 * ATTRIBUTE_NULL
 */
PhysicalOperatorBase* LogicalAggregation::GetPhysicalPlan(
    const unsigned& block_size) {
  if (NULL == plan_context_) {
    GetPlanContext();
  }
  PhysicalOperatorBase* ret;
  const PlanContext child_plan_context = child_->GetPlanContext();
  PhysicalAggregation::State local_agg_state;
  local_agg_state.group_by_attrs_ = group_by_attrs_;
  local_agg_state.aggregation_attrs_ = aggregation_attrs_;
  local_agg_state.block_size_ = block_size;
  local_agg_state.num_of_buckets_ =
      EstimateGroupByCardinality(child_plan_context);
  local_agg_state.bucket_size_ = 64;
  local_agg_state.input_schema_ = GetSchema(child_plan_context.attribute_list_);
  local_agg_state.output_schema_ = GetSchema(plan_context_->attribute_list_);
  local_agg_state.child_ = child_->GetPhysicalPlan(block_size);
  local_agg_state.avg_index_ = avg_id_in_agg_;
  local_agg_state.count_column_id_ = count_column_id_;
  local_agg_state.hash_schema_ =
      local_agg_state.output_schema_->duplicateSchema();
  switch (aggregation_style_) {
    case kLocalAgg: {
      local_agg_state.agg_node_type_ =
          PhysicalAggregation::State::kNotHybridAgg;
      ret = new PhysicalAggregation(local_agg_state);
      break;
    }
    case kLocalAggReparGlobalAgg: {
      local_agg_state.agg_node_type_ =
          PhysicalAggregation::State::kHybridAggLocal;
      PhysicalAggregation* local_aggregation =
          new PhysicalAggregation(local_agg_state);
      Expander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = block_size;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = local_aggregation;
      expander_state.schema_ = local_agg_state.hash_schema_->duplicateSchema();
      PhysicalOperatorBase* expander_lower = new Expander(expander_state);
      ExchangeMerger::State exchange_state;
      exchange_state.block_size_ = block_size;
      exchange_state.child_ = expander_lower;
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      exchange_state.lower_id_list_ =
          GetInvolvedNodeID(child_->GetPlanContext().plan_partitioner_);
      exchange_state.upper_id_list_ =
          GetInvolvedNodeID(plan_context_->plan_partitioner_);
      exchange_state.partition_schema_ =
          partition_schema::set_hash_partition(0);
      exchange_state.schema_ = local_agg_state.hash_schema_->duplicateSchema();
      PhysicalOperatorBase* exchange = new ExchangeMerger(exchange_state);
      PhysicalAggregation::State global_agg_state;
      global_agg_state.agg_node_type_ =
          PhysicalAggregation::State::kHybridAggGlobal;
      global_agg_state.input_schema_ =
          GetSchema(plan_context_->attribute_list_);
      global_agg_state.output_schema_ =
          GetSchema(plan_context_->attribute_list_);
      global_agg_state.hash_schema_ =
          global_agg_state.output_schema_->duplicateSchema();
      // change each aggregation expression and group by expression to one
      // attribute
      SetGroupbyAndAggAttrsForGlobalAgg(global_agg_state.group_by_attrs_,
                                        global_agg_state.aggregation_attrs_,
                                        global_agg_state.input_schema_);
      global_agg_state.block_size_ = block_size;
      global_agg_state.bucket_size_ = 64;
      global_agg_state.child_ = exchange;
      global_agg_state.num_of_buckets_ = local_agg_state.num_of_buckets_;
      global_agg_state.avg_index_ = avg_id_in_agg_;
      global_agg_state.count_column_id_ = count_column_id_;
      //      PhysicalOperatorBase* global_aggregation =
      //          new PhysicalAggregation(global_agg_state);
      ret = new PhysicalAggregation(global_agg_state);
      break;
    }
    case kReparGlobalAgg: {
      assert(false);
    }
  }
  return ret;
}

float LogicalAggregation::EstimateSelectivity() const { return 0.1; }

/**
 * In the current implementation, we assume that aggregation creates a new
 * table, i.e., intermediate table.
 * The id for the intermediate table is -1.
 */
std::vector<Attribute> LogicalAggregation::GetAttrsAfterAgg() const {
  std::vector<Attribute> ret;
  ret = GetGroupByAttrsAfterAgg();
  const std::vector<Attribute> aggregation_attributes = GetAggAttrsAfterAgg();
  ret.insert(ret.end(), aggregation_attributes.begin(),
             aggregation_attributes.end());
  return ret;
}
std::vector<Attribute> LogicalAggregation::GetGroupByAttrsAfterAgg() const {
  std::vector<Attribute> ret;
  for (int i = 0; i < group_by_attrs_.size(); ++i) {
    ret.push_back(group_by_attrs_[i]->ExprNodeToAttr(i));
  }
  return ret;
}
std::vector<Attribute> LogicalAggregation::GetAggAttrsAfterAgg() const {
  std::vector<Attribute> ret;
  int base_id = group_by_attrs_.size();
  for (int i = 0; i < aggregation_attrs_.size(); ++i) {
    ret.push_back(aggregation_attrs_[i]->ExprNodeToAttr(base_id + i));
  }
  return ret;
}
int64_t LogicalAggregation::EstimateGroupByCardinality(
    const PlanContext& plan_context) const {
  if (group_by_attrs_.size() == 0) {
    return 1;
  }
  const int64_t max_limits = 1024 * 1024;
  const int64_t min_limits = 1024 * 512;
  int64_t ret = plan_context.GetAggregatedDataCardinality();
#ifndef NEWCONDI
  for (unsigned i = 0; i < group_by_attribute_list_.size(); i++) {
    if (group_by_attribute_list_[i].isUnique()) {
      ret = ret < max_limits ? ret : max_limits;
      ret = ret > min_limits ? ret : min_limits;
      return ret;
    }
  }
  int64_t group_by_domain_size = 1;
  for (unsigned i = 0; i < group_by_attribute_list_.size(); i++) {
    AttributeStatistics* attr_stat =
        StatManager::getInstance()->getAttributeStatistic(
            group_by_attribute_list_[i]);
    if (attr_stat == 0) {
      group_by_domain_size *= 1000;
    } else {
      group_by_domain_size *= attr_stat->getDistinctCardinality();
    }
  }
  ret = group_by_domain_size;
// TODO(fzh): This is only the upper bound of group_by domain size;

#else
  ret = ret < max_limits ? ret : max_limits;
  ret = ret > min_limits ? ret : min_limits;
  return ret;
#endif
}
void LogicalAggregation::Print(int level) const {
  cout << setw(level * kTabSize) << " "
       << "Aggregation: ";
  ++level;
  switch (aggregation_style_) {
    case kLocalAgg: {
      cout << "kLocalAgg" << endl;
      break;
    }
    case kReparGlobalAgg: {
      cout << "kReparGlobalAgg" << endl;
      break;
    }
    case kLocalAggReparGlobalAgg: {
      cout << "kLocalAggReparGlobalAgg!" << endl;
      break;
    }
    default: { cout << "aggregation style is not given!" << endl; }
  }
  GetPlanContext();
  cout << setw(level * kTabSize) << " "
       << "[Partition info: "
       << plan_context_->plan_partitioner_.get_partition_key().attrName
       << " table_id= "
       << plan_context_->plan_partitioner_.get_partition_key().table_id_
       << " column_id= "
       << plan_context_->plan_partitioner_.get_partition_key().index << " ]"
       << endl;
  cout << setw((level - 1) * kTabSize) << " "
       << "## group by attributes:" << endl;
  for (int i = 0; i < group_by_attrs_.size(); ++i) {
    cout << "    " << group_by_attrs_[i]->alias_ << endl;
  }
  cout << setw((level - 1) * kTabSize) << " "
       << "## aggregation attributes:" << endl;
  for (int i = 0; i < aggregation_attrs_.size(); ++i) {
    cout << setw(level * kTabSize) << " " << aggregation_attrs_[i]->alias_
         << endl;
  }
  --level;
  child_->Print(level);
}
}  // namespace logical_operator
}  // namespace claims
