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
 * /CLAIMS/logical_operator/filter.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: tonglanxuan
 *       Email: lxtong0526@163.com
 *
 * Description: Detailedly describes how the LogicalFilter class functions.
 *
 */

#include "../logical_operator/logical_filter.h"
#include <string>
#include <map>
#include <vector>

#include "../catalog/stat/Estimation.h"
#include "../catalog/stat/StatManager.h"
#include "../IDsGenerator.h"
#include "../common/AttributeComparator.h"
#include "../common/TypePromotionMap.h"
#include "../common/TypeCast.h"
#include "../common/Expression/initquery.h"
#include "../physical_operator/exchange_merger.h"
#include "../physical_operator/physical_filter.h"

using claims::physical_operator::ExchangeMerger;
using claims::physical_operator::PhysicalFilter;
namespace claims {
namespace logical_operator {

LogicalFilter::LogicalFilter(LogicalOperator* child, vector<QNode*> qual)
    : LogicalOperator(kLogicalFilter),
      child_(child),
      condi_(qual),
      plan_context_(NULL) {}
LogicalFilter::LogicalFilter(LogicalOperator* child, vector<ExprNode*> condi)
    : LogicalOperator(kLogicalFilter),
      child_(child),
      condition_(condi),
      plan_context_(NULL) {}
LogicalFilter::~LogicalFilter() {
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}

PlanContext LogicalFilter::GetPlanContext() {
  /** In the currently implementation, we assume that the boolean operator
   * between each AttributeComparator is "AND".
   */
  lock_->acquire();
  if (NULL != plan_context_) {
    lock_->release();
    return *plan_context_;
  }
  PlanContext plan_context = child_->GetPlanContext();
  if (plan_context.IsHashPartitioned()) {
    for (unsigned i = 0;
         i < plan_context.plan_partitioner_.GetNumberOfPartitions(); ++i) {
      if (CanBeHashPruned(i, plan_context.plan_partitioner_)) {
        // Is filtered.
        plan_context.plan_partitioner_.GetPartition(i)->set_filtered();
      } else {  // Call predictSelectivilty() to alter cardinality.
                /**
                 * Should predict the volume of data that passes the filter.
                 * TODO(wangli): A precious prediction is needed based on the statistic
                 *               of the input data, which may be maintained in the
                 *               catalog module.
                 */

        const unsigned before_filter_cardinality =
            plan_context.plan_partitioner_.GetPartition(i)->get_cardinality();
        const unsigned after_filter_cardinality =
            before_filter_cardinality * PredictSelectivity();
        plan_context.plan_partitioner_.GetPartition(i)
            ->set_cardinality(after_filter_cardinality);
      }
    }
  }
  std::map<std::string, int> column_to_id;
  GetColumnToId(plan_context.attribute_list_, column_to_id);
  Schema* input_schema = GetSchema(plan_context.attribute_list_);
#ifdef NEWCONDI
  for (int i = 0; i < condi_.size(); ++i) {
    // Initialize expression of logical execution plan.
    InitExprAtLogicalPlan(condi_[i], t_boolean, column_to_id, input_schema);
  }
#else
  for (int i = 0; i < condition_.size(); ++i) {
    condition_[i]->InitExprAtLogicalPlan(t_boolean, column_to_id, input_schema);
  }
#endif
  plan_context_ = new PlanContext();
  *plan_context_ = plan_context;
  lock_->release();
  return plan_context;
}

PhysicalOperatorBase* LogicalFilter::GetPhysicalPlan(
    const unsigned& blocksize) {
  PlanContext plan_context = GetPlanContext();
  PhysicalOperatorBase* child_iterator = child_->GetPhysicalPlan(blocksize);
  PhysicalFilter::State state;  // Initial a state.
  state.block_size_ = blocksize;
  state.child_ = child_iterator;
  state.qual_ = condi_;
  state.condition_ = condition_;
  state.schema_ = GetSchema(plan_context.attribute_list_);
  PhysicalOperatorBase* filter = new PhysicalFilter(state);
  return filter;
}

bool LogicalFilter::GetOptimalPhysicalPlan(
    Requirement requirement, PhysicalPlanDescriptor& physical_plan_descriptor,
    const unsigned& block_size) {
  PhysicalPlanDescriptor physical_plan;
  std::vector<PhysicalPlanDescriptor> candidate_physical_plans;

  /* no requirement to the child*/
  if (child_->GetOptimalPhysicalPlan(Requirement(), physical_plan)) {
    NetworkTransfer transfer =
        requirement.requireNetworkTransfer(physical_plan.plan_context_);
    if (NONE == transfer) {
      PhysicalFilter::State state;
      state.block_size_ = block_size;
      state.child_ = physical_plan.plan;
      state.qual_ = condi_;
      state.condition_ = condition_;
      PlanContext plan_context = GetPlanContext();
      state.schema_ = GetSchema(plan_context.attribute_list_);
      PhysicalOperatorBase* filter = new PhysicalFilter(state);
      physical_plan.plan = filter;
      candidate_physical_plans.push_back(physical_plan);
    } else if ((OneToOne == transfer) || (Shuffle == transfer)) {
      /**
       * The input plan context should be transfered in the network to meet the
       * requirement.
       * TODO(wangli): Implement OneToOne Exchange
       * */
      PhysicalFilter::State state_f;
      state_f.block_size_ = block_size;
      state_f.child_ = physical_plan.plan;
      state_f.qual_ = condi_;
      state_f.condition_ = condition_;
      PlanContext plan_context = GetPlanContext();
      state_f.schema_ = GetSchema(plan_context.attribute_list_);
      PhysicalOperatorBase* filter = new PhysicalFilter(state_f);
      physical_plan.plan = filter;

      physical_plan.cost += physical_plan.plan_context_.GetAggregatedDatasize();

      ExchangeMerger::State state;
      state.block_size_ = block_size;
      state.child_ = physical_plan.plan;  // child_iterator;
      state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      state.schema_ = GetSchema(physical_plan.plan_context_.attribute_list_);

      std::vector<NodeID> upper_id_list;
      if (requirement.hasRequiredLocations()) {
        upper_id_list = requirement.getRequiredLocations();
      } else {
        if (requirement.hasRequiredPartitionFunction()) {
          // Partition function contains the number of partitions.
          PartitionFunction* partitoin_function =
              requirement.getPartitionFunction();
          upper_id_list = std::vector<NodeID>(
              NodeTracker::GetInstance()->GetNodeIDList().begin(),
              NodeTracker::GetInstance()->GetNodeIDList().begin() +
                  partitoin_function->getNumberOfPartitions() - 1);
        } else {
          // TODO(wangli): decide the degree of parallelism
          upper_id_list = NodeTracker::GetInstance()->GetNodeIDList();
        }
      }
      state.upper_id_list_ = upper_id_list;
      assert(requirement.hasReuiredPartitionKey());
      state.partition_schema_ =
          partition_schema::set_hash_partition(this->GetIdInAttributeList(
              physical_plan.plan_context_.attribute_list_,
              requirement.getPartitionKey()));
      assert(state.partition_schema_.partition_key_index >= 0);
      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(physical_plan.plan_context_.plan_partitioner_);
      state.lower_id_list_ = lower_id_list;
      PhysicalOperatorBase* exchange = new ExchangeMerger(state);
      physical_plan.plan = exchange;
    }
    candidate_physical_plans.push_back(physical_plan);
  }

  if (child_->GetOptimalPhysicalPlan(requirement, physical_plan)) {
    PhysicalFilter::State state;
    state.block_size_ = block_size;
    state.child_ = physical_plan.plan;
    PlanContext plan_context = GetPlanContext();
    state.schema_ = GetSchema(plan_context.attribute_list_);
    PhysicalOperatorBase* filter = new PhysicalFilter(state);
    physical_plan.plan = filter;
    candidate_physical_plans.push_back(physical_plan);
  }

  physical_plan_descriptor =
      GetBestPhysicalPlanDescriptor(candidate_physical_plans);

  if (requirement.passLimits(physical_plan_descriptor.cost))
    return true;
  else
    return false;
}

/**
 * TODO(tonglanxuan): CanBeHashPruned is remained to be rewrite， for condition_
 *                    is removed in the current version, and there no exists a
 *                    new method.
 * The upper method using "//" to commented out is one temptation to rewrite
 * this
 * function, but it doesn't work.
 * The lower using "/*" to comment out is the origin method using condition_.
 */
bool LogicalFilter::CanBeHashPruned(unsigned partition_id,
                                    const PlanPartitioner& part) const {
  //  for (unsigned i = 0; i < comparator_list_.size(); ++i) {
  //  const unsigned comparator_attribute_index =
  //  comparator_list_[i].get_index();
  //  if (part.getPartitionKey()
  //      == dataflow.attribute_list_[comparator_attribute_index]) {
  //    if (Comparator::EQ == comparator_list_[i].getCompareType()) {
  //      if (part.getPartitionKey().attrType->operate->getPartitionValue(
  //              comparator_list_[i].get_value(), part.getPartitionFunction())
  //          == partition_id) {
  //
  //        } else {/* the plan on this partition is filtered.*/
  //          return true;
  //        }
  //      }
  //    }
  //  }
  //  return false;
  /*
   for (unsigned i = 0; i < condition_.getCompaisonNumber(); ++i) {
   if (part.getPartitionKey() == condition_.attribute_list_[i]) {
   if (Comparator::EQ == comparator_list_[i].getCompareType()) {
   if (part.getPartitionKey().attrType->operate->getPartitionValue(
   comparator_list_[i].get_value(),
   part.getPartitionFunction()->getNumberOfPartitions())
   == partition_id) {
   } else {
   return true;
   }
   }
   }
   }
   */
  return false;
}

/**
 * TODO(tonglanxuan): predictSelectivity  is remained to be rewrite， for
 *                    condition_ is removed in the current version, and there
 *                    not exist a new method.
 * The method using "//" to commented out is the origin method using
 * condition_.
 */
float LogicalFilter::PredictSelectivity() const {
  float ret = 1;

  //  /**
  //   * In the current version, due to the lack of statistic information, we
  //   * only use a factor 0.5 for each comparison.
  //   * TODO(wangli): A more precious prediction is greatly needed.
  //   */
  //
  //  /**
  //   * TODO(wangli): Before predicting the selectivity, we should first check
  //   * whether there exist contradicted conditions (such as x=1 and x=4 is
  //   * contradicted to each other).
  //   */
  //
  //  for (unsigned i = 0; i < condition_.getCompaisonNumber(); ++i) {
  //    float selectivity = 1;
  //    const Attribute attr = condition_.attribute_list_[i];
  //    const TableStatistic* tab_stat = StatManager::getInstance()
  //        ->getTableStatistic(attr.table_id_);
  //    if (tab_stat > 0) {
  //      //Table statistics is available.
  //
  //      unsigned long cardinality = tab_stat->number_of_tuples_;
  //      const AttributeStatistics* attr_stat = StatManager::getInstance()
  //          ->getAttributeStatistic(attr);
  //      if (attr_stat > 0) {
  //        // Attribute statistics is available.
  //        if (attr_stat->getHistogram()) {
  //          /**
  //           * Histogram is available. Selectivity prediction is based on the
  //           * histogram.
  //           */
  //          const Histogram* histogram = attr_stat->getHistogram();
  //          /**
  //           * In the current implementation, only point estimation is
  //           * available, and hence we assume that thecomparator is equal.
  //           * TODO(wangli):
  //           */
  //          unsigned long filtered_cardinality = Estimation::estEqualOper(
  //              attr.getID(), condition_.const_value_list_[i]);
  //          selectivity = (float) filtered_cardinality / cardinality;
  //        } else {
  //          /**
  //           * No histogram is available. We just use the attribute
  //           cardinality
  //           * to predict the selectivity. In such case, we assume each
  //           * distinct value has the same frequency, i.e.,
  //           * cardinality/distinct_cardinality.
  //           */
  //          const unsigned int distinct_card =
  //              attr_stat->getDistinctCardinality();
  //          unsigned long filtered_cardinality = (double) cardinality
  //              / distinct_card;
  //          selectivity = (double) filtered_cardinality / cardinality;
  //        }
  //      } else {
  //        //Only Table statistic is available. We have to use the matic
  //        number.
  //        selectivity = 0.1;
  //      }
  //
  //    } else {
  //      // No statistic is available, so we use the magic number.
  //      selectivity = 0.1;
  //    }
  //
  //    ret *= selectivity;
  //  }
  return ret;
}
void LogicalFilter::Print(int level) const {
  cout << setw(level * kTabSize) << " "
       << "Filter:" << endl;
#ifdef NEWCONDI

  for (int i = 0; i < condi_.size(); ++i) {
    printf("  %s\n", condi_[i]->alias.c_str());
  }
#else
  ++level;
  for (int i = 0; i < condition_.size(); ++i) {
    cout << setw(level * kTabSize) << " " << condition_[i]->alias_ << endl;
  }
  --level;
#endif

  child_->Print(level);
}

}  // namespace logical_operator
}  // namespace claims
