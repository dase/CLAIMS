#include <vector>

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
#define GLOG_NO_ABBREVIATED_SEVERITIES  // avoid macro conflict
#include <glog/logging.h>
#include "../IDsGenerator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../Catalog/stat/StatManager.h"
#include "../Config.h"
#include "./logical_aggregation.h"
// namespace claims {
// namespace logical_query_plan {
LogicalAggregation::LogicalAggregation(
    std::vector<Attribute> group_by_attribute_list,
    std::vector<Attribute> aggregation_attribute_list,
    std::vector<BlockStreamAggregationIterator::State::aggregation>
        aggregation_function_list, LogicalOperator* child)
    : group_by_attribute_list_(group_by_attribute_list),
      aggregation_attribute_list_(aggregation_attribute_list),
      aggregation_function_list_(aggregation_function_list),
      dataflow_(NULL),
      child_(child) {
  assert(aggregation_attribute_list_.size() ==
         aggregation_function_list_.size());
  setOperatortype(l_aggregation);
}

LogicalAggregation::~LogicalAggregation() {
  delete dataflow_;
  delete child_;
}
Dataflow LogicalAggregation::getDataflow() {
  if (NULL != dataflow_) return *dataflow_;
  Dataflow ret;
  const Dataflow child_dataflow = child_->getDataflow();
  if (CanOmitHashRepartition(child_dataflow)) {
    aggregation_style_ = kAgg;
    LOG(INFO) << "Aggregation style: kAgg" << std::endl;
  } else {  // as for the kLocalAggReparGlobalAgg style is optimal
            // to kReparAndGlobalAgg so it's set to be default.
    aggregation_style_ = kLocalAggReparGlobalAgg;
    LOG(INFO) << "Aggregation style: kLocalAggReparGlobalAgg" << std::endl;
  }
  switch (aggregation_style_) {
    case kAgg: {
      ret.attribute_list_ = GetAttrsAfterAgg();
      ret.property_.commnication_cost =
          child_dataflow.property_.commnication_cost;
      ret.property_.partitioner = child_dataflow.property_.partitioner;
      Attribute partition_key =
          child_dataflow.property_.partitioner.getPartitionKey();
      partition_key.table_id_ = INTERMEIDATE_TABLEID;
      ret.property_.partitioner.setPartitionKey(partition_key);
      for (unsigned i = 0;
           i < ret.property_.partitioner.getNumberOfPartitions(); i++) {
        const unsigned cardinality =
            ret.property_.partitioner.getPartition(i)->getDataCardinality();
        ret.property_.partitioner.getPartition(i)->setDataCardinality(
            EstimateGroupByCardinality(child_dataflow) /
            ret.property_.partitioner.getNumberOfPartitions());
      }
      break;
    }
    default: {
      /**
       * repartition aggregation is currently simplified.
       */

      // TODO(fzh): ideally, the partition properties (especially the the number
      // of partitions and partition style) after repartition aggregation should
      // be decided by the partition property enforcement.
      ret.attribute_list_ = GetAttrsAfterAgg();
      ret.property_.commnication_cost =
          child_dataflow.property_.commnication_cost +
          child_dataflow.getAggregatedDatasize();
      ret.property_.partitioner.setPartitionFunction(
          child_dataflow.property_.partitioner.getPartitionFunction());
      if (group_by_attribute_list_.empty()) {
        ret.property_.partitioner.setPartitionKey(Attribute());
      } else {
        const Attribute partition_key = group_by_attribute_list_[0];
        ret.property_.partitioner.setPartitionKey(partition_key);
      }
      NodeID location = 0;
      unsigned long data_cardinality =
          EstimateGroupByCardinality(child_dataflow);
      PartitionOffset offset = 0;
      DataflowPartition par(offset, data_cardinality, location);
      std::vector<DataflowPartition> partition_list;
      partition_list.push_back(par);
      ret.property_.partitioner.setPartitionList(partition_list);
      break;
    }
  }
  dataflow_ = new Dataflow();
  *dataflow_ = ret;
  return ret;
}

bool LogicalAggregation::CanOmitHashRepartition(
    const Dataflow& child_dataflow) const {
  if (child_dataflow.property_.partitioner.getNumberOfPartitions() == 1 &&
      child_dataflow.property_.partitioner.getPartition(0)->getLocation() == 0)
    return true;
  if (!child_dataflow.isHashPartitioned()) return false;

  /**
   * the hash property of the input data flow can be leveraged in the
   * aggregation as long as the hash attribute is one of the group-by
   * attributes.
   */
  const Attribute partition_key =
      child_dataflow.property_.partitioner.getPartitionKey();
  for (unsigned i = 0; i < group_by_attribute_list_.size(); i++) {
    if (group_by_attribute_list_[i] == partition_key) return true;
  }
  return false;
}

void LogicalAggregation::ChangeSchemaForAVG(
    BlockStreamAggregationIterator::State& state_) {
  state_.avgIndex.clear();
  for (unsigned i = 0; i < state_.aggregations.size(); i++) {
    if (state_.aggregations[i] == BlockStreamAggregationIterator::State::avg) {
      state_.aggregations[i] = BlockStreamAggregationIterator::State::sum;
      state_.avgIndex.push_back(i);
    }
  }
  state_.hashSchema = state_.output->duplicateSchema();

  // if the agg style isn kLocalAggReparGlobalAgg
  // at local, output_schema = hash_schema, but for global,
  // input_schema = hash_schema
  // the hash_schema has to replace the column of avg() with sum() and append
  // one count() column at the end.

  if (state_.avgIndex.size() > 0) {
    column_type count_column_type = column_type(t_u_long, 8);
    state_.hashSchema->addColumn(count_column_type, 8);
    if (state_.agg_node_type ==
        BlockStreamAggregationIterator::State::Hybrid_Agg_Private) {
      state_.aggregations.push_back(
          BlockStreamAggregationIterator::State::count);
      state_.aggregationIndex.push_back(state_.aggregationIndex.size() +
                                        state_.groupByIndex.size());
      state_.output = state_.hashSchema->duplicateSchema();
    } else if (state_.agg_node_type ==
               BlockStreamAggregationIterator::State::Hybrid_Agg_Global) {
      state_.aggregations.push_back(BlockStreamAggregationIterator::State::sum);
      state_.aggregationIndex.push_back(state_.aggregationIndex.size() +
                                        state_.groupByIndex.size());
      state_.input = state_.hashSchema->duplicateSchema();
    } else if (state_.agg_node_type ==
               BlockStreamAggregationIterator::State::Not_Hybrid_Agg) {
      state_.aggregations.push_back(
          BlockStreamAggregationIterator::State::count);
      state_.aggregationIndex.push_back(state_.aggregationIndex.size() +
                                        state_.groupByIndex.size());
    }
  }
}
/**
 * Note: if group_by_attribute_list_ is empty, the partition key is
 * ATTRIBUTE_NULL
 */
BlockStreamIteratorBase* LogicalAggregation::getIteratorTree(
    const unsigned& block_size) {
  if (NULL == dataflow_) {
    getDataflow();
  }
  BlockStreamIteratorBase* ret;
  const Dataflow child_dataflow = child_->getDataflow();
  BlockStreamAggregationIterator::State aggregation_state;
  aggregation_state.groupByIndex =
      GetInvolvedAttrIdList(group_by_attribute_list_, child_dataflow);
  aggregation_state.aggregationIndex =
      GetInvolvedAttrIdList(aggregation_attribute_list_, child_dataflow);
  aggregation_state.aggregations = aggregation_function_list_;
  aggregation_state.block_size = block_size;
  aggregation_state.nbuckets = EstimateGroupByCardinality(child_dataflow);
  aggregation_state.bucketsize = 64;
  aggregation_state.input = getSchema(child_dataflow.attribute_list_);
  aggregation_state.output = getSchema(dataflow_->attribute_list_);
  aggregation_state.child = child_->getIteratorTree(block_size);

  switch (aggregation_style_) {
    case kAgg: {
      aggregation_state.agg_node_type =
          BlockStreamAggregationIterator::State::Not_Hybrid_Agg;
      ChangeSchemaForAVG(aggregation_state);
      ret = new BlockStreamAggregationIterator(aggregation_state);
      break;
    }
    case kLocalAggReparGlobalAgg: {
      aggregation_state.agg_node_type = BlockStreamAggregationIterator::State::
          Hybrid_Agg_Private;  // as regard to AVG(),for partition node and
                               // global node ,we should do change schema.
      ChangeSchemaForAVG(aggregation_state);
      BlockStreamAggregationIterator* local_aggregation =
          new BlockStreamAggregationIterator(aggregation_state);
      BlockStreamExpander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = block_size;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = local_aggregation;
      expander_state.schema_ = aggregation_state.hashSchema->duplicateSchema();
      BlockStreamIteratorBase* expander_lower =
          new BlockStreamExpander(expander_state);

      ExpandableBlockStreamExchangeEpoll::State exchange_state;
      exchange_state.block_size_ = block_size;
      exchange_state.child_ = expander_lower;
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      exchange_state.lower_id_list_ =
          getInvolvedNodeID(child_->getDataflow().property_.partitioner);
      exchange_state.upper_id_list_ =
          getInvolvedNodeID(dataflow_->property_.partitioner);
      if (group_by_attribute_list_.empty()) {
        exchange_state.partition_schema_ =
            partition_schema::set_hash_partition(0);
      } else {
        exchange_state.partition_schema_ = partition_schema::set_hash_partition(
            GetInvolvedAttrIdList(GetGroupByAttrsAfterAgg(), *dataflow_)[0]);
      }
      exchange_state.schema_ = aggregation_state.hashSchema->duplicateSchema();
      BlockStreamIteratorBase* exchange =
          new ExpandableBlockStreamExchangeEpoll(exchange_state);

      BlockStreamAggregationIterator::State global_aggregation_state;
      global_aggregation_state.aggregationIndex =
          GetInvolvedAttrIdList(GetAggAttrsAfterAgg(), *dataflow_);
      global_aggregation_state.aggregations =
          ChangeForGlobalAggregation(aggregation_function_list_);
      global_aggregation_state.block_size = block_size;
      global_aggregation_state.bucketsize = 64;
      global_aggregation_state.child = exchange;
      global_aggregation_state.groupByIndex =
          GetInvolvedAttrIdList(GetGroupByAttrsAfterAgg(), *dataflow_);
      global_aggregation_state.input = getSchema(dataflow_->attribute_list_);
      global_aggregation_state.nbuckets = aggregation_state.nbuckets;
      global_aggregation_state.output = getSchema(dataflow_->attribute_list_);
      global_aggregation_state.agg_node_type =
          BlockStreamAggregationIterator::State::Hybrid_Agg_Global;
      ChangeSchemaForAVG(global_aggregation_state);
      BlockStreamIteratorBase* global_aggregation =
          new BlockStreamAggregationIterator(global_aggregation_state);
      ret = global_aggregation;
      break;
    }
    case kReparGlobalAgg: {
      // the corresponding physical operation is't implemented
      BlockStreamExpander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = block_size;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = child_->getIteratorTree(block_size);
      expander_state.schema_ = getSchema(child_dataflow.attribute_list_);
      BlockStreamIteratorBase* expander =
          new BlockStreamExpander(expander_state);
      ExpandableBlockStreamExchangeEpoll::State exchange_state;
      exchange_state.block_size_ = block_size;
      exchange_state.child_ = expander;  // child_->getIteratorTree(block_size);
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();
      exchange_state.lower_id_list_ =
          getInvolvedNodeID(child_->getDataflow().property_.partitioner);
      exchange_state.upper_id_list_ =
          getInvolvedNodeID(dataflow_->property_.partitioner);
      if (group_by_attribute_list_.empty()) {
        /**
         * scalar aggregation allows parallel partitions to be partitioned in
         * any fashion.
         * In the current implementation, we use the first aggregation attribute
         * as the
         * partition attribute.
         */

        // TODO(fzh): select the proper partition attribute by considering the
        // cardinality and load balance.
        exchange_state.partition_schema_ =
            partition_schema::set_hash_partition(GetInvolvedAttrIdList(
                aggregation_attribute_list_, child_dataflow)[0]);
      } else {
        exchange_state.partition_schema_ = partition_schema::set_hash_partition(
            GetInvolvedAttrIdList(group_by_attribute_list_, child_dataflow)[0]);
      }
      exchange_state.schema_ = getSchema(child_dataflow.attribute_list_);
      BlockStreamIteratorBase* exchange =
          new ExpandableBlockStreamExchangeEpoll(exchange_state);
      aggregation_state.agg_node_type =
          BlockStreamAggregationIterator::State::Not_Hybrid_Agg;
      ChangeSchemaForAVG(aggregation_state);
      aggregation_state.child = exchange;
      ret = new BlockStreamAggregationIterator(aggregation_state);
      break;
    }
  }
  return ret;
}
std::vector<unsigned> LogicalAggregation::GetInvolvedAttrIdList(
    const std::vector<Attribute>& attribute_list,
    const Dataflow& dataflow) const {
  std::vector<unsigned> ret;
  for (unsigned i = 0; i < attribute_list.size(); i++) {
    bool found = false;
    for (unsigned j = 0; j < dataflow.attribute_list_.size(); j++) {
      /*
       * @brief: attribute_list[j].isANY()
       */
      if (attribute_list[i].isANY() ||
          (dataflow.attribute_list_[j] == attribute_list[i])) {
        found = true;
        ret.push_back(j);
        break;
      }
    }
    if (found == false) {
      LOG(ERROR) << "can't find attrbute in dataflow in "
                    "LogicalAggeration::GetInvolvedAttributeIdList"
                 << std::endl;
    }
  }
  return ret;
}
float LogicalAggregation::EstimateSelectivity() const { return 0.1; }

std::vector<BlockStreamAggregationIterator::State::aggregation>
LogicalAggregation::ChangeForGlobalAggregation(const std::vector<
    BlockStreamAggregationIterator::State::aggregation> list) const {
  std::vector<BlockStreamAggregationIterator::State::aggregation> ret;
  for (unsigned i = 0; i < list.size(); i++) {
    if (list[i] == BlockStreamAggregationIterator::State::count) {
      ret.push_back(BlockStreamAggregationIterator::State::sum);
    } else {
      ret.push_back(list[i]);
    }
  }
  return ret;
}
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

  for (unsigned i = 0; i < group_by_attribute_list_.size(); i++) {
    Attribute temp = group_by_attribute_list_[i];
    temp.index = i;
    temp.table_id_ = INTERMEIDATE_TABLEID;
    ret.push_back(temp);
  }
  return ret;
}
std::vector<Attribute> LogicalAggregation::GetAggAttrsAfterAgg() const {
  std::vector<Attribute> ret;

  unsigned aggregation_start_index = group_by_attribute_list_.size();
  for (unsigned i = 0; i < aggregation_attribute_list_.size(); i++) {
    Attribute temp = aggregation_attribute_list_[i];

    switch (aggregation_function_list_[i]) {
      case BlockStreamAggregationIterator::State::count: {
        if (!(temp.isNULL() || temp.isANY())) temp.attrType->~column_type();
        temp.attrType = new column_type(t_u_long, 8);
        temp.attrName = "count(" + temp.getName() + ")";
        temp.index = aggregation_start_index++;
        temp.table_id_ = INTERMEIDATE_TABLEID;
        break;
      }
      case BlockStreamAggregationIterator::State::max: {
        temp.attrName = "max(" + temp.getName() + ")";
        temp.index = aggregation_start_index++;
        temp.table_id_ = INTERMEIDATE_TABLEID;
        break;
      }
      case BlockStreamAggregationIterator::State::min: {
        temp.attrName = "min(" + temp.getName() + ")";
        temp.index = aggregation_start_index++;
        temp.table_id_ = INTERMEIDATE_TABLEID;
        break;
      }
      case BlockStreamAggregationIterator::State::sum: {
        temp.attrName = "sum(" + temp.getName() + ")";
        temp.index = aggregation_start_index++;
        temp.table_id_ = INTERMEIDATE_TABLEID;
        break;
      }
      case BlockStreamAggregationIterator::State::avg: {
        temp.attrName = "avg(" + temp.getName() + ")";
        temp.index = aggregation_start_index++;
        temp.table_id_ = INTERMEIDATE_TABLEID;
      } break;
      default: { assert(false); }
    }
    ret.push_back(temp);
  }
  return ret;
}
unsigned long LogicalAggregation::EstimateGroupByCardinality(
    const Dataflow& dataflow) const {
  if (group_by_attribute_list_.size() == 0) {
    return 1;
  }
  const unsigned long max_limits = 1024 * 1024;
  const unsigned long min_limits = 1024 * 512;
  unsigned long data_card = dataflow.getAggregatedDataCardinality();
  unsigned long ret;
  for (unsigned i = 0; i < group_by_attribute_list_.size(); i++) {
    if (group_by_attribute_list_[i].isUnique()) {
      ret = ret < max_limits ? ret : max_limits;
      ret = ret > min_limits ? ret : min_limits;
      return ret;
    }
  }
  unsigned long group_by_domain_size = 1;
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
  ret = group_by_domain_size;  // TODO(fzh): This is only the upper bound of group_by
                               // domain size;

  ret = ret < max_limits ? ret : max_limits;
  ret = ret > min_limits ? ret : min_limits;
  return ret;
}
void LogicalAggregation::print(int level) const {
  printf("%*.sAggregation:", level * 8, " ");
  switch (aggregation_style_) {
    case kAgg: {
      printf("kAgg\n");
      break;
    }
    case kReparGlobalAgg: {
      printf("kReparGlobalAgg\n");
      break;
    }
    case kLocalAggReparGlobalAgg: {
      printf("kLocalAggReparGlobalAgg!\n");
      break;
    }
    default: { printf("aggregation style is not given!\n"); }
  }
  printf("group-by attributes:\n");
  for (unsigned i = 0; i < this->group_by_attribute_list_.size(); i++) {
    printf("%*.s", level * 8, " ");
    printf("%s\n", group_by_attribute_list_[i].attrName.c_str());
  }
  printf("%*.saggregation attributes:\n", level * 8, " ");
  for (unsigned i = 0; i < aggregation_attribute_list_.size(); i++) {
    printf("%*.s", level * 8, " ");
    switch (aggregation_function_list_[i]) {
      case BlockStreamAggregationIterator::State::count: {
        printf("Count: %s\n", aggregation_attribute_list_[i].attrName.c_str());
        break;
      }
      case BlockStreamAggregationIterator::State::max: {
        printf("Max: %s\n", aggregation_attribute_list_[i].attrName.c_str());
        break;
      }
      case BlockStreamAggregationIterator::State::min: {
        printf("Min: %s\n", aggregation_attribute_list_[i].attrName.c_str());
        break;
      }
      case BlockStreamAggregationIterator::State::sum: {
        printf("Sum: %s\n", aggregation_attribute_list_[i].attrName.c_str());
        break;
      }
      case BlockStreamAggregationIterator::State::avg: {
        printf("Avg: %s\n", aggregation_attribute_list_[i].attrName.c_str());
        break;
      }
      default: { break; }
    }
  }
  child_->print(level + 1);
}
//} // namespace logical_query_plan
//}  // namespace claims
