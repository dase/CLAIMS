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
 * /CLAIMS/logical_operator/equal_join.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli,yuyang
 *       Email: wangli1426@gmail.com,youngfish2010@hotmail.com
 *
 * Description:
 *   This file mainly describe the EqualJoin Operator.
 *
 */
#include "../logical_operator/logical_equal_join.h"
#include <glog/logging.h>
#include <iostream>
#include <vector>

#include "../Config.h"
#include "../IDsGenerator.h"
#include "../common/Logging.h"
#include "../Catalog/stat/StatManager.h"
#include "../physical_operator/exchange_merger.h"
#include "../physical_operator/expander.h"
#include "../physical_operator/physical_hash_join.h"
#include "../physical_operator/physical_operator_base.h"

using claims::physical_operator::ExchangeMerger;
using claims::physical_operator::Expander;
using claims::physical_operator::PhysicalHashJoin;
using claims::physical_operator::PhysicalOperatorBase;

// using claims::physical_operator::PhysicalJoin;
namespace claims {
namespace logical_operator {
LogicalEqualJoin::LogicalEqualJoin(std::vector<JoinPair> joinpair_list,
                                   LogicalOperator* left_input,
                                   LogicalOperator* right_input)
    : joinkey_pair_list_(joinpair_list),
      left_child_(left_input),
      right_child_(right_input),
      join_policy_(kNull),
      dataflow_(NULL) {
  for (unsigned i = 0; i < joinpair_list.size(); ++i) {
    left_join_key_list_.push_back(joinpair_list[i].left_join_attr_);
    right_join_key_list_.push_back(joinpair_list[i].right_join_attr_);
  }
  set_operator_type(kLogicalEqualJoin);
}

LogicalEqualJoin::~LogicalEqualJoin() {
  if (NULL != dataflow_) {
    delete dataflow_;
    dataflow_ = NULL;
  }
  if (NULL != left_child_) {
    delete left_child_;
    left_child_ = NULL;
  }
  if (NULL != right_child_) {
    delete right_child_;
    right_child_ = NULL;
  }
}

PlanContext LogicalEqualJoin::GetPlanContext() {
  if (NULL != dataflow_) {
    // the data flow has been computed*/
    return *dataflow_;
  }

  /**
   *  in the current implementation, only the hash join is considered
   */
  PlanContext left_dataflow = left_child_->GetPlanContext();
  PlanContext right_dataflow = right_child_->GetPlanContext();
  PlanContext ret;

  const bool left_dataflow_key_partitioned = CanOmitHashRepartition(
      left_join_key_list_, left_dataflow.plan_partitioner_);
  const bool right_dataflow_key_partitioned = CanOmitHashRepartition(
      right_join_key_list_, right_dataflow.plan_partitioner_);

  const Attribute left_partition_key =
      left_dataflow.plan_partitioner_.get_partition_key();
  const Attribute right_partition_key =
      right_dataflow.plan_partitioner_.get_partition_key();
  if (left_dataflow_key_partitioned && right_dataflow_key_partitioned) {
    if (IsInOneJoinPair(left_partition_key, right_partition_key)) {
      // the best situation
      if (left_dataflow.plan_partitioner_.HasSamePartitionLocation(
              right_dataflow.plan_partitioner_)) {
        join_policy_ = kNoRepartition;
      } else {
        join_policy_ =
            DecideLeftOrRightRepartition(left_dataflow, right_dataflow);
      }
    } else {
      join_policy_ =
          DecideLeftOrRightRepartition(left_dataflow, right_dataflow);
    }
  } else {
    if (left_dataflow_key_partitioned && !right_dataflow_key_partitioned) {
      join_policy_ = kRightRepartition;
    }
    if (!left_dataflow_key_partitioned && right_dataflow_key_partitioned) {
      join_policy_ = kLeftRepartition;
    }
    if (!left_dataflow_key_partitioned && !right_dataflow_key_partitioned)
      join_policy_ = kCompleteRepartition;
  }

  /**finally, construct the output data flow according to the join police**/
  switch (join_policy_) {
    case kNoRepartition: {
      LOG(INFO) << "no_repartition" << std::endl;
      ret.attribute_list_.insert(ret.attribute_list_.end(),
                                 left_dataflow.attribute_list_.begin(),
                                 left_dataflow.attribute_list_.end());
      ret.attribute_list_.insert(ret.attribute_list_.end(),
                                 right_dataflow.attribute_list_.begin(),
                                 right_dataflow.attribute_list_.end());
      /**
       * Use the left partitioner as the output dataflow partitioner.
       * TODO(admin): In fact, the output dataflow partitioner should contains
       * both left partitioner and right partitioner.
       */
      //      ret.property_.partitioner=left_dataflow.property_.partitioner;
      ret.plan_partitioner_.set_partition_list(
          left_dataflow.plan_partitioner_.get_partition_list());
      ret.plan_partitioner_.set_partition_func(
          left_dataflow.plan_partitioner_.get_partition_func());
      ret.plan_partitioner_.set_partition_key(left_partition_key);
      ret.plan_partitioner_.AddShadowPartitionKey(right_partition_key);

      /**
       * Set the generated data size.
       * Currently, we assume the generated data size is the sum of input data
       * volume.
       * TODO(admin): Some reasonable output size estimation is needed.
       */
      for (unsigned i = 0; i < ret.plan_partitioner_.GetNumberOfPartitions();
           i++) {
        const unsigned l_cardinality =
            left_dataflow.plan_partitioner_.GetPartition(i)->get_cardinality();
        const unsigned r_cardinality =
            right_dataflow.plan_partitioner_.GetPartition(i)->get_cardinality();
        ret.plan_partitioner_.GetPartition(i)
            ->set_cardinality(l_cardinality + r_cardinality);
      }

      ret.commu_cost_ = left_dataflow.commu_cost_ + right_dataflow.commu_cost_;
      break;
    }
    case kLeftRepartition: {
      LOG(INFO) << "left_repartiotion" << std::endl;
      ret.attribute_list_.insert(ret.attribute_list_.end(),
                                 left_dataflow.attribute_list_.begin(),
                                 left_dataflow.attribute_list_.end());
      ret.attribute_list_.insert(ret.attribute_list_.end(),
                                 right_dataflow.attribute_list_.begin(),
                                 right_dataflow.attribute_list_.end());
      //     ret.property_.partitioner=right_dataflow.property_.partitioner;

      ret.plan_partitioner_.set_partition_list(
          right_dataflow.plan_partitioner_.get_partition_list());
      ret.plan_partitioner_.set_partition_func(
          right_dataflow.plan_partitioner_.get_partition_func());
      ret.plan_partitioner_.set_partition_key(
          right_dataflow.plan_partitioner_.get_partition_key());
      //  ret.property_.partitioner.addShadowPartitionKey(right_partition_key);
      /* set the generated data size*/
      const unsigned left_total_size =
          left_dataflow.plan_partitioner_.GetAggregatedDataSize();
      const unsigned right_partition_count =
          right_dataflow.plan_partitioner_.GetNumberOfPartitions();
      for (unsigned i = 0; i < ret.plan_partitioner_.GetNumberOfPartitions();
           i++) {
        const unsigned r_size =
            right_dataflow.plan_partitioner_.GetPartition(i)->get_cardinality();
        ret.plan_partitioner_.GetPartition(i)
            ->set_cardinality(r_size + left_total_size / right_partition_count);
      }

      ret.commu_cost_ = left_dataflow.commu_cost_ + right_dataflow.commu_cost_;
      ret.commu_cost_ +=
          left_dataflow.plan_partitioner_.GetAggregatedDataSize();
      break;
    }
    case kRightRepartition: {
      LOG(INFO) << "right_repartition" << std::endl;
      ret.attribute_list_.insert(ret.attribute_list_.end(),
                                 left_dataflow.attribute_list_.begin(),
                                 left_dataflow.attribute_list_.end());
      ret.attribute_list_.insert(ret.attribute_list_.end(),
                                 right_dataflow.attribute_list_.begin(),
                                 right_dataflow.attribute_list_.end());
      //  ret.property_.partitioner=left_dataflow.property_.partitioner;

      ret.plan_partitioner_.set_partition_list(
          left_dataflow.plan_partitioner_.get_partition_list());
      ret.plan_partitioner_.set_partition_func(
          left_dataflow.plan_partitioner_.get_partition_func());
      ret.plan_partitioner_.set_partition_key(
          left_dataflow.plan_partitioner_.get_partition_key());
      //   ret.property_.partitioner.addShadowPartitionKey(right_partition_key);
      /**
       *  set the generated data size
       */
      const unsigned right_total_size =
          right_dataflow.plan_partitioner_.GetAggregatedDataSize();
      const unsigned left_partition_count =
          left_dataflow.plan_partitioner_.GetNumberOfPartitions();
      for (unsigned i = 0; i < ret.plan_partitioner_.GetNumberOfPartitions();
           i++) {
        const unsigned l_size =
            left_dataflow.plan_partitioner_.GetPartition(i)->get_cardinality();
        ret.plan_partitioner_.GetPartition(i)
            ->set_cardinality(l_size + right_total_size / left_partition_count);
      }
      ret.commu_cost_ = left_dataflow.commu_cost_ + right_dataflow.commu_cost_;
      ret.commu_cost_ +=
          right_dataflow.plan_partitioner_.GetAggregatedDataSize();
      break;
    }
    case kCompleteRepartition: {
      /**
       * TODO(admin): the repartition strategy (e.g., the degree of parallelism
       * and the partition function) in such case is not decided by any child
       * data flow.
       * Additional optimization can be made by adopting the partition strategy
       * which benefits the remaining work.
       */
      LOG(INFO) << "complete_repartition" << std::endl;
      ret.attribute_list_.insert(ret.attribute_list_.end(),
                                 left_dataflow.attribute_list_.begin(),
                                 left_dataflow.attribute_list_.end());
      ret.attribute_list_.insert(ret.attribute_list_.end(),
                                 right_dataflow.attribute_list_.begin(),
                                 right_dataflow.attribute_list_.end());
      ret.commu_cost_ = left_dataflow.commu_cost_ + right_dataflow.commu_cost_;
      ret.commu_cost_ +=
          left_dataflow.plan_partitioner_.GetAggregatedDataSize();
      ret.commu_cost_ +=
          right_dataflow.plan_partitioner_.GetAggregatedDataSize();

      ret.plan_partitioner_ =
          DecideOutputDataflowProperty(left_dataflow, right_dataflow);
      //
      // QueryOptimizationLogging::log("[Complete_repartition
      // hash join] is not implemented, because I'm very lazy. -_- \n");
      // assert(false);
      break;
    }
    default: {
      LOG(ERROR) << "The join police has not been decided!" << std::endl;
      assert(false);
      break;
    }
  }

  dataflow_ = new PlanContext();
  *dataflow_ = ret;

  return ret;
}

bool LogicalEqualJoin::IsHashOnLeftKey(const Partitioner& part,
                                       const Attribute& key) const {
  if (part.getPartitionFashion() != PartitionFunction::hash_f) return false;
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
  }
  return part.getPartitionKey() == key;
}
bool LogicalEqualJoin::CanOmitHashRepartition(
    const std::vector<Attribute>& join_key_list,
    const PlanPartitioner& partitoiner) const {
  Attribute attribute = partitoiner.get_partition_key();
  for (unsigned i = 0; i < join_key_list.size(); i++) {
    if (attribute == join_key_list[i]) return true;
  }
  return false;
}

bool LogicalEqualJoin::IsInOneJoinPair(
    const Attribute& left_partition_key,
    const Attribute& right_partition_key) const {
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
    if (left_partition_key == joinkey_pair_list_[i].left_join_attr_ &&
        right_partition_key == joinkey_pair_list_[i].right_join_attr_) {
      return true;
    }
  }
  return false;
}
LogicalEqualJoin::JoinPolicy LogicalEqualJoin::DecideLeftOrRightRepartition(
    const PlanContext& left_dataflow, const PlanContext& right_dataflow) const {
  const unsigned left_data_size = left_dataflow.GetAggregatedDatasize();
  const unsigned right_data_size = right_dataflow.GetAggregatedDatasize();
  if (left_data_size > right_data_size) {
    return kRightRepartition;
  } else {
    return kLeftRepartition;
  }
}

PhysicalOperatorBase* LogicalEqualJoin::GetPhysicalPlan(
    const unsigned& block_size) {
  if (NULL == dataflow_) {
    GetPlanContext();
  }
  PhysicalHashJoin* join_iterator;
  PhysicalOperatorBase* child_iterator_left =
      left_child_->GetPhysicalPlan(block_size);
  PhysicalOperatorBase* child_iterator_right =
      right_child_->GetPhysicalPlan(block_size);
  PlanContext dataflow_left = left_child_->GetPlanContext();
  PlanContext dataflow_right = right_child_->GetPlanContext();
  PhysicalHashJoin::State state;
  state.block_size_ = block_size;
  state.hashtable_bucket_num_ = 1024 * 1024;
  // state.ht_nbuckets=1024;
  state.input_schema_left_ = GetSchema(dataflow_left.attribute_list_);
  state.input_schema_right_ = GetSchema(dataflow_right.attribute_list_);
  state.hashtable_schema_ = GetSchema(dataflow_left.attribute_list_);
  // the bucket size is 64-byte-aligned
  // state_.ht_bucketsize =
  //  ((state_.input_schema_left->getTupleMaxSize()-1)/64+1)*64;
  /**
   * In the initial implementation, I set the bucket size to be up round to
   * cache line size, e.g., 64Bytes. Finally, I realized that different from
   * aggregation,the hash table bucket in the build phase of hash join is filled
   * very quickly and hence a * a relatively large bucket size could reduce the
   * number of overflowing buckets and avoid the random memory access caused by
   * acceesing overflowing buckets.
   */
  state.hashtable_bucket_size_ = 128;
  state.output_schema_ = GetSchema(dataflow_->attribute_list_);

  state.join_index_left_ = GetLeftJoinKeyIds();
  state.join_index_right_ = GetRightJoinKeyIds();

  state.payload_left_ = GetLeftPayloadIds();
  state.payload_right_ = GetRightPayloadIds();

  switch (join_policy_) {
    case kNoRepartition: {
      state.child_left_ = child_iterator_left;
      state.child_right_ = child_iterator_right;

      join_iterator = new PhysicalHashJoin(state);
      break;
    }
    case kLeftRepartition: {
      //    state_.child_left
      Expander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = block_size;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = child_iterator_left;
      expander_state.schema_ = GetSchema(dataflow_left.attribute_list_);
      PhysicalOperatorBase* expander = new Expander(expander_state);

      NodeTracker* node_tracker = NodeTracker::GetInstance();
      ExchangeMerger::State exchange_state;
      exchange_state.block_size_ = block_size;
      exchange_state.child_ = expander;  // child_iterator_left;
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();

      std::vector<NodeID> upper_id_list =
          GetInvolvedNodeID(dataflow_->plan_partitioner_);
      exchange_state.upper_id_list_ = upper_id_list;

      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(dataflow_left.plan_partitioner_);
      exchange_state.lower_id_list_ = lower_id_list;

      const Attribute right_partition_key =
          dataflow_->plan_partitioner_.get_partition_key();

      /* get the left attribute that is corresponding to the partition key.*/
      Attribute left_partition_key =
          joinkey_pair_list_[GetIdInRightJoinKeys(right_partition_key)]
              .left_join_attr_;

      exchange_state.partition_schema_ =
          partition_schema::set_hash_partition(GetIdInAttributeList(
              dataflow_left.attribute_list_, left_partition_key));

      // exchange_state.schema=getSchema(dataflow_left.attribute_list_,
      //                                 dataflow_right.attribute_list_);
      exchange_state.schema_ = GetSchema(dataflow_left.attribute_list_);
      PhysicalOperatorBase* exchange = new ExchangeMerger(exchange_state);
      state.child_left_ = exchange;
      state.child_right_ = child_iterator_right;
      join_iterator = new PhysicalHashJoin(state);
      break;
    }
    case kRightRepartition: {
      Expander::State expander_state;
      expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state.block_size_ = block_size;
      expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
      expander_state.child_ = child_iterator_right;
      expander_state.schema_ = GetSchema(dataflow_right.attribute_list_);
      PhysicalOperatorBase* expander = new Expander(expander_state);

      NodeTracker* node_tracker = NodeTracker::GetInstance();
      ExchangeMerger::State exchange_state;
      exchange_state.block_size_ = block_size;
      exchange_state.child_ = expander;
      exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();

      std::vector<NodeID> upper_id_list =
          GetInvolvedNodeID(dataflow_->plan_partitioner_);
      exchange_state.upper_id_list_ = upper_id_list;

      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(dataflow_right.plan_partitioner_);
      exchange_state.lower_id_list_ = lower_id_list;

      const Attribute output_partition_key =
          dataflow_->plan_partitioner_.get_partition_key();

      /* get the right attribute that is corresponding to the partition key.*/
      Attribute right_repartition_key;
      if (dataflow_->plan_partitioner_.HasShadowPartitionKey()) {
        right_repartition_key =
            joinkey_pair_list_[GetIdInLeftJoinKeys(
                                   output_partition_key,
                                   dataflow_->plan_partitioner_
                                       .get_shadow_partition_keys())]
                .right_join_attr_;
      } else {
        right_repartition_key =
            joinkey_pair_list_[GetIdInLeftJoinKeys(output_partition_key)]
                .right_join_attr_;
      }

      exchange_state.partition_schema_ =
          partition_schema::set_hash_partition(GetIdInAttributeList(
              dataflow_right.attribute_list_, right_repartition_key));

      exchange_state.schema_ = GetSchema(dataflow_right.attribute_list_);
      PhysicalOperatorBase* exchange = new ExchangeMerger(exchange_state);
      state.child_left_ = child_iterator_left;
      state.child_right_ = exchange;
      join_iterator = new PhysicalHashJoin(state);
      break;
    }
    case kCompleteRepartition: {
      /* build left input*/
      Expander::State expander_state_l;
      expander_state_l.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state_l.block_size_ = block_size;
      expander_state_l.init_thread_count_ =
          Config::initial_degree_of_parallelism;
      expander_state_l.child_ = child_iterator_left;
      expander_state_l.schema_ = GetSchema(dataflow_left.attribute_list_);
      PhysicalOperatorBase* expander_l = new Expander(expander_state_l);

      ExchangeMerger::State l_exchange_state;
      l_exchange_state.block_size_ = block_size;
      l_exchange_state.child_ = expander_l;
      l_exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();

      std::vector<NodeID> lower_id_list =
          GetInvolvedNodeID(dataflow_left.plan_partitioner_);
      l_exchange_state.lower_id_list_ = lower_id_list;

      std::vector<NodeID> upper_id_list =
          GetInvolvedNodeID(dataflow_->plan_partitioner_);
      l_exchange_state.upper_id_list_ = upper_id_list;

      const Attribute left_partition_key =
          dataflow_->plan_partitioner_.get_partition_key();
      l_exchange_state.partition_schema_ =
          partition_schema::set_hash_partition(GetIdInAttributeList(
              dataflow_left.attribute_list_, left_partition_key));
      l_exchange_state.schema_ = GetSchema(dataflow_left.attribute_list_);
      PhysicalOperatorBase* l_exchange = new ExchangeMerger(l_exchange_state);

      // build right input

      Expander::State expander_state_r;
      expander_state_r.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
      expander_state_r.block_size_ = block_size;
      expander_state_r.init_thread_count_ =
          Config::initial_degree_of_parallelism;
      expander_state_r.child_ = child_iterator_right;
      expander_state_r.schema_ = GetSchema(dataflow_right.attribute_list_);
      PhysicalOperatorBase* expander_r = new Expander(expander_state_r);

      ExchangeMerger::State r_exchange_state;
      r_exchange_state.block_size_ = block_size;
      r_exchange_state.child_ = expander_r;
      r_exchange_state.exchange_id_ =
          IDsGenerator::getInstance()->generateUniqueExchangeID();

      lower_id_list = GetInvolvedNodeID(dataflow_right.plan_partitioner_);
      r_exchange_state.lower_id_list_ = lower_id_list;

      upper_id_list = GetInvolvedNodeID(dataflow_->plan_partitioner_);
      r_exchange_state.upper_id_list_ = upper_id_list;

      const Attribute right_partition_key =
          joinkey_pair_list_[GetIdInLeftJoinKeys(left_partition_key)]
              .right_join_attr_;
      r_exchange_state.partition_schema_ =
          partition_schema::set_hash_partition(GetIdInAttributeList(
              dataflow_right.attribute_list_, right_partition_key));
      r_exchange_state.schema_ = GetSchema(dataflow_right.attribute_list_);
      PhysicalOperatorBase* r_exchange = new ExchangeMerger(r_exchange_state);

      // finally  build the join iterator itself
      state.child_left_ = l_exchange;
      state.child_right_ = r_exchange;
      join_iterator = new PhysicalHashJoin(state);
      break;
    }
    default: { break; }
  }
  return join_iterator;
}

bool LogicalEqualJoin::GetOptimalPhysicalPlan(
    Requirement requirement, PhysicalPlanDescriptor& physical_plan_descriptor,
    const unsigned& block_size) {}

std::vector<unsigned> LogicalEqualJoin::GetLeftJoinKeyIds() const {
  std::vector<unsigned> ret;
  const PlanContext dataflow = left_child_->GetPlanContext();
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
    for (unsigned j = 0; j < dataflow.attribute_list_.size(); j++) {
      if (joinkey_pair_list_[i].left_join_attr_ ==
          dataflow.attribute_list_[j]) {
        ret.push_back(j);
      }
    }
  }
  return ret;
}

std::vector<unsigned> LogicalEqualJoin::GetRightJoinKeyIds() const {
  std::vector<unsigned> ret;
  const PlanContext dataflow = right_child_->GetPlanContext();
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
    for (unsigned j = 0; j < dataflow.attribute_list_.size(); j++) {
      if (joinkey_pair_list_[i].right_join_attr_ ==
          dataflow.attribute_list_[j]) {
        ret.push_back(j);
      }
    }
  }
  return ret;
}
std::vector<unsigned> LogicalEqualJoin::GetLeftPayloadIds() const {
  std::vector<unsigned> ret;
  const PlanContext dataflow = left_child_->GetPlanContext();
  const std::vector<unsigned> left_join_key_index_list = GetLeftJoinKeyIds();

  for (unsigned i = 0; i < dataflow.attribute_list_.size(); i++) {
    bool found_equal = false;
    for (unsigned j = 0; j < left_join_key_index_list.size(); j++) {
      if (i == left_join_key_index_list[j]) {
        found_equal = true;
        break;
      }
    }
    if (!found_equal) {
      ret.push_back(i);
    }
  }
  return ret;
}

std::vector<unsigned> LogicalEqualJoin::GetRightPayloadIds() const {
  std::vector<unsigned> ret;
  const PlanContext dataflow = right_child_->GetPlanContext();
  const std::vector<unsigned> right_join_key_index_list = GetRightJoinKeyIds();

  for (unsigned i = 0; i < dataflow.attribute_list_.size(); i++) {
    for (unsigned j = 0; j < right_join_key_index_list.size(); j++) {
      if (i == right_join_key_index_list[j]) {
        break;
      }
    }
    ret.push_back(i);
  }
  return ret;
}
int LogicalEqualJoin::GetIdInLeftJoinKeys(const Attribute& attribute) const {
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
    if (joinkey_pair_list_[i].left_join_attr_ == attribute) {
      return i;
    }
  }
  assert(false);
  return -1;
}
int LogicalEqualJoin::GetIdInLeftJoinKeys(
    const Attribute& attribute,
    const std::vector<Attribute> shadow_attribute_list) const {
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
    if (joinkey_pair_list_[i].left_join_attr_ == attribute) {
      return i;
    }
  }
  /**
   * The attribute fails to match any join key.
   * Now we try to match the shadow partition attribute(s)
   */
  for (unsigned s = 0; s < shadow_attribute_list.size(); s++) {
    for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
      if (joinkey_pair_list_[i].left_join_attr_ == shadow_attribute_list[s]) {
        return i;
      }
    }
  }

  /**
   * Neither the partition attribute nor the shadow partition attribute could
   * match any join key.
   */
  assert(false);
  return -1;
}
int LogicalEqualJoin::GetIdInRightJoinKeys(const Attribute& attribute) const {
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
    if (joinkey_pair_list_[i].right_join_attr_ == attribute) {
      return i;
    }
  }
  assert(false);
  return -1;
}
int LogicalEqualJoin::GetIdInRightJoinKeys(
    const Attribute& attribute,
    const std::vector<Attribute> shadow_attribute_list) const {
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
    if (joinkey_pair_list_[i].right_join_attr_ == attribute) {
      return i;
    }
  }
  /**
   * The attribute fails to match any join key.
   * Now we try to match the shadow partition attribute(s)
   */
  for (unsigned s = 0; s < shadow_attribute_list.size(); s++) {
    for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
      if (joinkey_pair_list_[i].right_join_attr_ == shadow_attribute_list[s]) {
        return i;
      }
    }
  }

  /**
   * neither the partition attribute nor the shadow partition attribute could
   * match any join key.
   */
  assert(false);
  return -1;
}
int LogicalEqualJoin::GetIdInAttributeList(
    const std::vector<Attribute>& attributes,
    const Attribute& attribute) const {
  for (unsigned i = 0; i < attributes.size(); i++) {
    if (attributes[i] == attribute) {
      return i;
    }
  }
  assert(false);
  return -1;
}
PlanPartitioner LogicalEqualJoin::DecideOutputDataflowProperty(
    const PlanContext& left_dataflow, const PlanContext& right_dataflow) const {
  PlanPartitioner ret;

  //	const unsigned l_data_cardinality=left_dataflow.getAggregatedDatasize();
  //	const unsigned r_datasize=right_dataflow.getAggregatedDatasize();
  const unsigned long l_data_cardinality =
      left_dataflow.GetAggregatedDataCardinality();
  const unsigned long r_data_cardinality =
      right_dataflow.GetAggregatedDataCardinality();

  std::vector<NodeID> all_node_id_list =
      NodeTracker::GetInstance()->GetNodeIDList();
  /**
   * In the current implementation, all the nodes are involved in the
   * complete_repartition method.
   * TODO: decide the degree of parallelism
   */
  const unsigned degree_of_parallelism = all_node_id_list.size();

  std::vector<PlanPartitionInfo> dataflow_partition_list;
  for (unsigned i = 0; i < degree_of_parallelism; i++) {
    const NodeID location = all_node_id_list[i];

    /**
     * Currently, the join output size cannot be predicted due to the absence of
     * data statistics.
     * We just use the magic number as following
     */
    //    const unsigned
    // cardinality=l_data_cardinality/degree_of_parallelism+r_data_cardinality/degree_of_parallelism;
    const unsigned long cardinality =
        l_data_cardinality * r_data_cardinality *
        PredictEqualJoinSelectivity(left_dataflow, right_dataflow) /
        degree_of_parallelism;
    PlanPartitionInfo dfp(i, cardinality, location);
    dataflow_partition_list.push_back(dfp);
  }
  ret.set_partition_list(dataflow_partition_list);
  ret.set_partition_key(joinkey_pair_list_[0].left_join_attr_);
  ret.AddShadowPartitionKey(joinkey_pair_list_[0].right_join_attr_);
  PartitionFunction* partition_function =
      PartitionFunctionFactory::createBoostHashFunction(degree_of_parallelism);
  ret.set_partition_func(partition_function);
  return ret;
}
void LogicalEqualJoin::Print(int level) const {
  printf("%*.sEqualJoin:", level * 8, " ");
  switch (join_policy_) {
    case kNoRepartition: {
      printf("no_repartition\n");
      break;
    }
    case kLeftRepartition: {
      printf("left_repartition\n");
      break;
    }
    case kRightRepartition: {
      printf("right_repartition!\n");
      break;
    }
    case kCompleteRepartition: {
      printf("complete_repartition!\n");
      break;
    }
    default: { printf("not given!\n"); }
  }
  for (unsigned i = 0; i < this->joinkey_pair_list_.size(); i++) {
    printf("%*.s", level * 8, " ");
    printf("%s=%s\n", joinkey_pair_list_[i].left_join_attr_.attrName.c_str(),
           joinkey_pair_list_[i].right_join_attr_.attrName.c_str());
  }
  left_child_->Print(level + 1);
  right_child_->Print(level + 1);
}
double LogicalEqualJoin::PredictEqualJoinSelectivity(
    const PlanContext& left_dataflow, const PlanContext& right_dataflow) const {
  /**
   * Currently, we assume that we do not know the joint distribution of join
   * attributes.
   * Consequently, we predict the selectivity for each join attribute pair and
   * finally combine them.
   */
  double ret = 1;
  for (unsigned i = 0; i < joinkey_pair_list_.size(); i++) {
    ret *= PredictEqualJoinSelectivityOnSingleJoinAttributePair(
        joinkey_pair_list_[i].left_join_attr_,
        joinkey_pair_list_[i].right_join_attr_);
  }
  return ret;
}
double LogicalEqualJoin::PredictEqualJoinSelectivityOnSingleJoinAttributePair(
    const Attribute& attr_left, const Attribute& attr_right) const {
  double ret;
  TableStatistic* t_l_stat =
      StatManager::getInstance()->getTableStatistic(attr_left.table_id_);
  TableStatistic* t_r_stat =
      StatManager::getInstance()->getTableStatistic(attr_right.table_id_);
  if (t_r_stat && t_l_stat) {
    unsigned long t_l_card = t_l_stat->getCardinality();
    unsigned long t_r_card = t_r_stat->getCardinality();

    AttributeStatistics* a_l_stat =
        StatManager::getInstance()->getAttributeStatistic(attr_left);
    AttributeStatistics* a_r_stat =
        StatManager::getInstance()->getAttributeStatistic(attr_right);
    if (a_l_stat && a_r_stat) {
      /**
       * both tables have the attribute level statistics.
       */
      Histogram* attr_left_hist = a_l_stat->getHistogram();
      Histogram* attr_right_hist = a_r_stat->getHistogram();
      if (attr_left_hist && attr_right_hist) {
        /**
         * Both tables have histogram, so we predict the selectivity based on
         * histogram.
         */
        ;  // Waiting for Zhutao's implementation

        const unsigned long a_l_dist_card = a_l_stat->getDistinctCardinality();
        const unsigned long a_r_dist_card = a_r_stat->getDistinctCardinality();
        double min_card =
            a_l_dist_card < a_r_dist_card ? a_l_dist_card : a_r_dist_card;
        min_card *= 1;  // 0.8 is the magic number
        const double output_card = min_card * t_l_card / (double)a_l_dist_card *
                                   t_r_card / (double)a_r_dist_card;
        ret = output_card / t_l_card / t_r_card;
        double max_card =
            a_l_dist_card > a_r_dist_card ? a_l_dist_card : a_r_dist_card;
        ret = 1 / max_card;
      } else {
        /**
         * predict based on the cardinality and distinct cardinality of the two
         * attribute.
         */
        const unsigned long a_l_dist_card = a_l_stat->getDistinctCardinality();
        const unsigned long a_r_dist_card = a_r_stat->getDistinctCardinality();
        double min_card =
            a_l_dist_card < a_r_dist_card ? a_l_dist_card : a_r_dist_card;
        min_card *= 1;  // 0.8 is the magic number
        const double output_card = min_card * t_l_card / (double)a_l_dist_card *
                                   t_r_card / (double)a_r_dist_card;

        double max_card =
            a_l_dist_card > a_r_dist_card ? a_l_dist_card : a_r_dist_card;
        ret = 1 / max_card;
      }
    } else {
      /**
       * Not both a_l and a_r have the attribute level statistics, so we predict
       * the join size based
       * on magic number.
       */
      ret = 0.1;
    }
  } else {
    /**
     * No table statistic is available, so we use the the magic number.
     */
    ret = 0.1;
  }
  LOG(INFO) << "Predicted selectivity for " << attr_left.attrName.c_str()
            << " and " << attr_right.attrName.c_str() << " is " << ret
            << std::endl;
  return ret;
}

}  // namespace logical_operator
}  // namespace claims
