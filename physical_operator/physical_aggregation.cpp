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
 * /CLAIMS/physical_operator/physical_aggregation.cpp
 *
 *  Created on: Sep 9, 2013
 *      Author: casa cswang
 *       Email: cs_wang@infosys.com
 *
 * Description: Aggregation physical operator, implement interface of Open(),
 *Next(), Close().
 *  multiply threads to process data blocks in one Node by different node type.
 *  this file is about the class BlockStreamAggregationIterator implementation.
 *
 */

#include "../physical_operator/physical_aggregation.h"

#include <glog/logging.h>
#include "../Debug.h"
#include "../utility/rdtsc.h"
#include "../Executor/expander_tracker.h"
namespace claims {
namespace physical_operator {

PhysicalAggregation::PhysicalAggregation(State state)
    : state_(state),
      hashtable_(0),
      hash_(0),
      bucket_cur_(0),
      PhysicalOperator(4, 3) {
  InitExpandedStatus();
  assert(state_.hash_schema_);
  InitAvgDivide();
}

PhysicalAggregation::PhysicalAggregation()
    : hashtable_(0), hash_(0), bucket_cur_(0), PhysicalOperator(4, 3) {
  InitExpandedStatus();
  InitAvgDivide();
}

PhysicalAggregation::~PhysicalAggregation() {
  delete state_.input_;
  delete state_.hash_schema_;
  delete state_.output_;
  delete state_.child_;
}

PhysicalAggregation::State::State(
    Schema *input, Schema *output, Schema *hash_schema,
    PhysicalOperatorBase *child, std::vector<unsigned> groupby_index,
    std::vector<unsigned> aggregation_index,
    std::vector<State::Aggregation> aggregations, unsigned num_of_buckets,
    unsigned bucket_size, unsigned block_size, std::vector<unsigned> avg_index,
    AggNodeType agg_node_type)
    : input_(input),
      output_(output),
      hash_schema_(hash_schema),
      child_(child),
      index_of_group_by_(groupby_index),
      aggregation_index_(aggregation_index),
      aggregations_(aggregations),
      num_of_buckets_(num_of_buckets),
      bucket_size_(bucket_size),
      block_size_(block_size),
      avg_index_(avg_index),
      agg_node_type_(agg_node_type) {}

/**
 * while one thread starts Open(), the thread will be registered to all barriers
 * to synchronize other threads.
 * Open() aggregate tuples from child's block in private hashtable by several
 * threads.
 * One block of data get from child operator each time, then loop to get every
 * tuple,
 * firstly check it whether in this private hash table.
 * operate the prepared aggregation function to update new tuple into private
 * hash table if the tuple key exists.
 * otherwise, allocate new bucket and assign every value of column in the tuple
 * to the new bucket as the first tuple value in the new bucket.
 * after all block from child be processed. merge private hash table into shared
 * hash table thread by thread synchronized by the hash table lock.
 */
bool PhysicalAggregation::Open(const PartitionOffset &partition_offset) {
  RegisterExpandedThreadToAllBarriers();

  if (TryEntryIntoSerializedSection(0)) {
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_desc, "Aggregation", 0));
  }
  BarrierArrive(0);

  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers();
    return true;
  }
  state_.child_->Open(partition_offset);
  ticks start = curtick();
  if (TryEntryIntoSerializedSection(1)) {
    PrepareIndex();
    PrepareAggregateFunctions();
    hash_ = PartitionFunctionFactory::createGeneralModuloFunction(
        state_.num_of_buckets_);
    hashtable_ = new BasicHashTable(state_.num_of_buckets_, state_.bucket_size_,
                                    state_.hash_schema_->getTupleMaxSize());
  }

  start = curtick();
  /* A private hash table is allocated for each thread to buffer the local
   * results. All the private hash table should be merged
   * at the final phase to complete the aggregation. Aggregation using private
   * hash tables is called private aggregation. Although
   * consuming larger memory, private aggregation is more efficient than shared
   * aggregation for scalar aggregation or aggregation
   * with small groups, as private aggregation avoids the contention to the
   * shared hash table.
   */
  BasicHashTable *private_hashtable =
      new BasicHashTable(state_.num_of_buckets_, state_.bucket_size_,
                         state_.hash_schema_->getTupleMaxSize());

  start = curtick();
  BarrierArrive(1);
  void *cur = 0;
  unsigned bn;
  bool key_exist;
  void *tuple_in_hashtable;
  void *key_in_input_tuple;
  void *key_in_hash_table;
  void *value_in_input_tuple;
  void *value_in_hash_table;
  void *new_tuple_in_hash_table;
  unsigned allocated_tuples_in_hashtable = 0;
  BasicHashTable::Iterator ht_it = hashtable_->CreateIterator();
  BasicHashTable::Iterator pht_it = private_hashtable->CreateIterator();
  unsigned long long one = 1;
  BlockStreamBase *block_for_asking =
      BlockStreamBase::createBlock(state_.input_, state_.block_size_);
  block_for_asking->setEmpty();

  start = curtick();

  while (state_.child_->Next(
      block_for_asking)) {  // traverse every block from child
    BlockStreamBase::BlockStreamTraverseIterator *bsti =
        block_for_asking->createIterator();
    bsti->reset();
    while ((cur = bsti->currentTuple()) !=
           0) {  // traverse every tuple from block
      /* get the corresponding bucket index according to the first column in
       * group-by attributes.
       * Note that bn is always 0 for scalar aggregation.
       */
      bn = 0;
      if (state_.index_of_group_by_.size() > 0)
        bn = state_.input_->getcolumn(state_.index_of_group_by_[0])
                 .operate->getPartitionValue(
                     state_.input_->getColumnAddess(
                         state_.index_of_group_by_[0], cur),
                     state_.num_of_buckets_);

      private_hashtable->placeIterator(pht_it, bn);
      key_exist = false;
      while ((tuple_in_hashtable = pht_it.readCurrent()) != 0) {
        /* set key_exist flag to true such that the the case for scalar
         * aggregation (i.e., aggregation with no group-by attributes)
         * could be considered as passed the group by value verification.
         */
        key_exist = true;
        for (unsigned i = 0; i < state_.index_of_group_by_.size(); i++) {
          key_in_input_tuple =
              state_.input_->getColumnAddess(state_.index_of_group_by_[i], cur);
          key_in_hash_table = state_.hash_schema_->getColumnAddess(
              input_group_by_to_output_[i], tuple_in_hashtable);
          if (!state_.input_->getcolumn(state_.index_of_group_by_[i])
                   .operate->equal(key_in_input_tuple, key_in_hash_table)) {
            key_exist = false;
            break;
          }
        }
        if (key_exist) {  // hash table have the key (the value in group-by
                          // attribute)
          for (unsigned i = 0; i < state_.aggregation_index_.size(); i++) {
            value_in_input_tuple = state_.input_->getColumnAddess(
                state_.aggregation_index_[i], cur);
            value_in_hash_table = state_.hash_schema_->getColumnAddess(
                input_aggregation_to_output_[i], tuple_in_hashtable);
            private_hashtable->UpdateTuple(bn, value_in_hash_table,
                                           value_in_input_tuple,
                                           private_aggregation_functions_[i]);
          }
          bsti->increase_cur_();
          break;
        } else {
          pht_it.increase_cur_();
        }
      }
      if (key_exist) {
        continue;
      }
      new_tuple_in_hash_table = private_hashtable->allocate(bn);
      for (unsigned i = 0; i < state_.index_of_group_by_.size(); i++) {
        key_in_input_tuple =
            state_.input_->getColumnAddess(state_.index_of_group_by_[i], cur);
        key_in_hash_table = state_.hash_schema_->getColumnAddess(
            input_group_by_to_output_[i], new_tuple_in_hash_table);
        state_.input_->getcolumn(state_.index_of_group_by_[i])
            .operate->assignment(key_in_input_tuple, key_in_hash_table);
      }

      for (unsigned i = 0; i < state_.aggregation_index_.size(); i++) {
        /**
         * use if-else here is a kind of ugly.
         * TODO(Anyone): use a function which is initialized according to the
         * aggregation function.
         */
        if (state_.aggregations_[i] == State::kCount) {
          value_in_input_tuple = &one;
        } else {
          value_in_input_tuple =
              state_.input_->getColumnAddess(state_.aggregation_index_[i], cur);
        }
        value_in_hash_table = state_.hash_schema_->getColumnAddess(
            input_aggregation_to_output_[i], new_tuple_in_hash_table);
        state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
            .operate->assignment(value_in_input_tuple, value_in_hash_table);
      }
      bsti->increase_cur_();
    }
    block_for_asking->setEmpty();
  }

  for (int i = 0; i < state_.num_of_buckets_; i++) {
    private_hashtable->placeIterator(pht_it, i);
    while ((cur = pht_it.readCurrent()) !=
           0) {  // traverse every tuple from block
      /* get the corresponding bucket index according to the first column in
       * group-by attributes.
       * Note that bn is always 0 for scalar aggregation.
       */
      bn = 0;
      if (state_.index_of_group_by_.size() > 0)
        bn = state_.hash_schema_->getcolumn(0).operate->getPartitionValue(
            state_.hash_schema_->getColumnAddess(0, cur),
            state_.num_of_buckets_);

      hashtable_->lockBlock(bn);
      hashtable_->placeIterator(ht_it, bn);
      key_exist = false;
      while ((tuple_in_hashtable = ht_it.readCurrent()) != 0) {
        /* set key_exist flag to true such that the the case for scalar
         * aggregation (i.e., aggregation with no group-by attributes)
         * could be considered as passed the group by value verification.
         */
        key_exist = true;
        for (unsigned i = 0; i < state_.index_of_group_by_.size(); i++) {
          key_in_input_tuple = state_.hash_schema_->getColumnAddess(i, cur);
          key_in_hash_table = state_.hash_schema_->getColumnAddess(
              input_group_by_to_output_[i], tuple_in_hashtable);
          if (!state_.hash_schema_->getcolumn(i)
                   .operate->equal(key_in_input_tuple, key_in_hash_table)) {
            key_exist = false;
            break;
          }
        }
        if (key_exist) {  // hash table have the key (the value in group-by
                          // attribute)
          for (unsigned i = 0; i < state_.aggregation_index_.size(); i++) {
            value_in_input_tuple = state_.hash_schema_->getColumnAddess(
                i + state_.index_of_group_by_.size(), cur);
            value_in_hash_table = state_.hash_schema_->getColumnAddess(
                input_aggregation_to_output_[i], tuple_in_hashtable);
            hashtable_->UpdateTuple(bn, value_in_hash_table,
                                    value_in_input_tuple,
                                    global_aggregation_functions_[i]);
          }
          pht_it.increase_cur_();
          hashtable_->unlockBlock(bn);
          break;
        } else {
          ht_it.increase_cur_();
        }
      }
      if (key_exist) {
        continue;
      }
      new_tuple_in_hash_table = hashtable_->allocate(bn);
      allocated_tuples_in_hashtable++;
      for (unsigned i = 0; i < state_.index_of_group_by_.size(); i++) {
        key_in_input_tuple = state_.hash_schema_->getColumnAddess(i, cur);
        key_in_hash_table = state_.hash_schema_->getColumnAddess(
            input_group_by_to_output_[i], new_tuple_in_hash_table);
        state_.hash_schema_->getcolumn(i)
            .operate->assignment(key_in_input_tuple, key_in_hash_table);
      }

      for (unsigned i = 0; i < state_.aggregation_index_.size(); i++) {
        value_in_input_tuple = state_.hash_schema_->getColumnAddess(
            i + state_.index_of_group_by_.size(), cur);
        value_in_hash_table = state_.hash_schema_->getColumnAddess(
            input_aggregation_to_output_[i], new_tuple_in_hash_table);
        state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
            .operate->assignment(value_in_input_tuple, value_in_hash_table);
      }
      hashtable_->unlockBlock(bn);
      pht_it.increase_cur_();
    }
  }

  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(1);
    return true;
  }
  BarrierArrive(2);

  if (TryEntryIntoSerializedSection(2)) {
    // hashtable_->report_status();
    it_ = hashtable_->CreateIterator();
    bucket_cur_ = 0;
    hashtable_->placeIterator(it_, bucket_cur_);
    SetReturnStatus(true);
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_src, "Aggregation  ", 0));
    perf_info_ =
        ExpanderTracker::getInstance()->getPerformanceInfo(pthread_self());
    perf_info_->initialize();
  }
  BarrierArrive(3);

  delete block_for_asking;
  delete private_hashtable;
  return GetReturnStatus();
}

/**
 * In the current implementation, the lock is used based on the entire
 * hash table, which will definitely reduce the degree of parallelism.
 * But it is for now, assuming that the aggregated results are small.
 */
bool PhysicalAggregation::Next(BlockStreamBase *block) {
  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(3);
    LOG(INFO) << "<<<<<<<<<<<<<<<<<Aggregation next detected call back "
                 "signal!>>>>>>>>>>>>>>>>>" << std::endl;

    return false;
  }
  void *cur_in_ht;
  void *tuple;
  void *key_in_hash_tuple;
  void *key_in_output_tuple;
  hashtable_cur_lock_.acquire();
  while (it_.readCurrent() != 0 ||
         (hashtable_->placeIterator(it_, bucket_cur_)) != false) {
    while ((cur_in_ht = it_.readCurrent()) != 0) {
      if ((tuple = block->allocateTuple(state_.output_->getTupleMaxSize())) !=
          0) {  // the tuple is empty??
        if (state_.avg_index_.size() > 0 &&
            (state_.agg_node_type_ == State::kHybridAggGlobal ||
             state_.agg_node_type_ ==
                 State::kNotHybridAgg)) {  // avg=sum/tuple_size
          for (unsigned i = 0; i < state_.index_of_group_by_.size();
               i++) {  // in one tuple that are produced from aggregation
                       // statement, the groupby attributes is at the head, the
                       // rest attributes belong to the aggregation part.
            key_in_hash_tuple = state_.hash_schema_->getColumnAddess(
                input_group_by_to_output_[i], cur_in_ht);
            key_in_output_tuple = state_.output_->getColumnAddess(
                input_group_by_to_output_[i], tuple);
            state_.output_->getcolumn(input_group_by_to_output_[i])
                .operate->assignment(key_in_hash_tuple, key_in_output_tuple);
          }
          state_.avg_index_.push_back(-1);  // boundary point,
          int aggsize = state_.aggregation_index_.size() - 1;
          unsigned i = 0, j = 0;
          unsigned long count_value =
              (*(unsigned long *)state_.hash_schema_->getColumnAddess(
                  input_aggregation_to_output_[aggsize], cur_in_ht));
          for (; i < aggsize; i++) {
            if (state_.avg_index_[j] ==
                i) {  // avgIndex save the index of avg in
                      // aggregations,see
                      // logical_aggregation.cpp:116

              assert(state_.aggregations_[i] == State::kSum);
              j++;
              void *sum_value = state_.hash_schema_->getColumnAddess(
                  input_aggregation_to_output_[i],
                  cur_in_ht);  // get the value in hash table
              if (count_value ==
                  0) {  // how to report the error if divided by 0?
                key_in_hash_tuple = sum_value;
              } else {  // TODO(Anyone): precision of avg result is not enough
                key_in_hash_tuple = sum_value;  // the room is enough?
                ExectorFunction::avg_divide
                    [state_.hash_schema_
                         ->columns[input_aggregation_to_output_[i]]
                         .type](sum_value, count_value, key_in_hash_tuple);
              }
            } else {
              key_in_hash_tuple = state_.hash_schema_->getColumnAddess(
                  input_aggregation_to_output_[i], cur_in_ht);
            }
            key_in_output_tuple = state_.output_->getColumnAddess(
                input_aggregation_to_output_[i], tuple);
            state_.output_->getcolumn(input_aggregation_to_output_[i])
                .operate->assignment(key_in_hash_tuple, key_in_output_tuple);
          }
        } else {
          memcpy(tuple, cur_in_ht, state_.output_->getTupleMaxSize());
        }
        it_.increase_cur_();
      } else {
        hashtable_cur_lock_.release();
        perf_info_->processed_one_block();
        return true;
      }
    }
    bucket_cur_++;
  }
  hashtable_cur_lock_.release();
  if (block->Empty()) {
    return false;
  } else {
    perf_info_->processed_one_block();
    return true;
  }
}

bool PhysicalAggregation::Close() {
  InitExpandedStatus();

  delete hashtable_;
  global_aggregation_functions_.clear();
  input_aggregation_to_output_.clear();
  input_group_by_to_output_.clear();

  state_.child_->Close();
  return true;
}
void PhysicalAggregation::Print() {
  LOG(INFO) << "Aggregation:  " << state_.num_of_buckets_
            << " buckets in hash table" << std::endl;
  LOG(INFO) << "---------------" << std::endl;
  state_.child_->Print();
}

void PhysicalAggregation::PrepareIndex() {
  unsigned outputindex = 0;
  for (unsigned i = 0; i < state_.index_of_group_by_.size(); i++) {
    input_group_by_to_output_[i] = outputindex++;  // index of group by
                                                   // attributes from input To
                                                   // output index
  }
  for (unsigned i = 0; i < state_.aggregation_index_.size(); i++) {
    input_aggregation_to_output_[i] = outputindex++;  // index of aggregation
                                                      // attributes from input
                                                      // To output index
  }
}

void PhysicalAggregation::PrepareAggregateFunctions() {
  for (unsigned i = 0; i < state_.aggregations_.size(); i++) {
    switch (state_.aggregations_[i]) {
      case PhysicalAggregation::State::kCount:
        private_aggregation_functions_.push_back(
            state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
                .operate->GetIncreateByOneFunction());
        global_aggregation_functions_.push_back(
            state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
                .operate->GetADDFunction());
        break;
      case PhysicalAggregation::State::kMin:
        private_aggregation_functions_.push_back(
            state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
                .operate->GetMINFunction());
        global_aggregation_functions_.push_back(
            state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
                .operate->GetMINFunction());
        break;
      case PhysicalAggregation::State::kMax:
        private_aggregation_functions_.push_back(
            state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
                .operate->GetMAXFunction());
        global_aggregation_functions_.push_back(
            state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
                .operate->GetMAXFunction());
        break;
      case PhysicalAggregation::State::kSum:
        private_aggregation_functions_.push_back(
            state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
                .operate->GetADDFunction());
        global_aggregation_functions_.push_back(
            state_.hash_schema_->getcolumn(input_aggregation_to_output_[i])
                .operate->GetADDFunction());
        break;
      default:  // for avg has changed to sum and count
        printf("invalid aggregation function!\n");
    }
  }
}

}  // namespace physical_operator
}  // namespace claims
