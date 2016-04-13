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
#include <vector>
#include "../common/expression/expr_node.h"
#include "../common/expression/data_type_oper.h"
#include "../common/expression/expr_unary.h"
#include "../common/Schema/Schema.h"
#include "../Debug.h"
#include "../utility/rdtsc.h"
#include "../Executor/expander_tracker.h"
using claims::common::DataTypeOper;
using claims::common::DataTypeOperFunc;
using claims::common::ExprEvalCnxt;
using claims::common::ExprNode;
using claims::common::ExprUnary;
using std::vector;
namespace claims {
namespace physical_operator {

PhysicalAggregation::PhysicalAggregation(State state)
    : PhysicalOperator(4, 3),
      state_(state),
      hashtable_(NULL),
      hash_(NULL),
      bucket_cur_(0) {
  InitExpandedStatus();
  assert(state_.hash_schema_);
}

PhysicalAggregation::PhysicalAggregation()
    : PhysicalOperator(4, 3), hashtable_(NULL), hash_(NULL), bucket_cur_(0) {
  InitExpandedStatus();
}

PhysicalAggregation::~PhysicalAggregation() {
  if (NULL != state_.input_schema_) {
    delete state_.input_schema_;
    state_.input_schema_ = NULL;
  }
  if (NULL != state_.hash_schema_) {
    delete state_.hash_schema_;
    state_.hash_schema_ = NULL;
  }
  if (NULL != state_.output_schema_) {
    delete state_.output_schema_;
    state_.output_schema_ = NULL;
  }
  if (NULL != state_.child_) {
    delete state_.child_;
    state_.child_ = NULL;
  }
  for (int i = 0; i < state_.group_by_attrs_.size(); ++i) {
    if (NULL != state_.group_by_attrs_[i]) {
      delete state_.group_by_attrs_[i];
      state_.group_by_attrs_[i] = NULL;
    }
  }
  state_.group_by_attrs_.clear();
  for (int i = 0; i < state_.aggregation_attrs_.size(); ++i) {
    if (NULL != state_.aggregation_attrs_[i]) {
      delete state_.aggregation_attrs_[i];
      state_.aggregation_attrs_[i] = NULL;
    }
  }
  state_.aggregation_attrs_.clear();
}

PhysicalAggregation::State::State(
    Schema *input, Schema *output, Schema *hash_schema,
    PhysicalOperatorBase *child, unsigned num_of_buckets, unsigned bucket_size,
    unsigned block_size, std::vector<unsigned> avg_index,
    AggNodeType agg_node_type, vector<ExprNode *> group_by_attrs,
    vector<ExprUnary *> aggregation_attrs, int count_column_id)
    : input_schema_(input),
      output_schema_(output),
      hash_schema_(hash_schema),
      child_(child),
      num_of_buckets_(num_of_buckets),
      bucket_size_(bucket_size),
      block_size_(block_size),
      avg_index_(avg_index),
      agg_node_type_(agg_node_type),
      group_by_attrs_(group_by_attrs),
      aggregation_attrs_(aggregation_attrs),
      count_column_id_(count_column_id) {}

/**
 * while one thread starts Open(), the thread will be registered to all
 * barriers
 * to synchronize other threads.
 * Open() aggregate tuples from child's block in private hashtable by several
 * threads.
 * One block of data get from child operator each time, then loop to get every
 * tuple,
 * firstly check it whether in this private hash table.
 * operate the prepared aggregation function to update new tuple into private
 * hash table if the tuple key exists.
 * otherwise, allocate new bucket and assign every value of column in the
 * tuple
 * to the new bucket as the first tuple value in the new bucket.
 * after all block from child be processed. merge private hash table into
 * shared
 * hash table thread by thread synchronized by the hash table lock.
 */
bool PhysicalAggregation::Open(const PartitionOffset &partition_offset) {
  RegisterExpandedThreadToAllBarriers();
  // copy expression and initialize them
  vector<ExprNode *> group_by_attrs;
  vector<ExprUnary *> agg_attrs;
  ExprNode *group_by_node = NULL;
  ExprUnary *agg_node = NULL;
  int group_by_size = state_.group_by_attrs_.size();
  for (int i = 0; i < state_.group_by_attrs_.size(); ++i) {
    group_by_node = state_.group_by_attrs_[i]->ExprCopy();
    group_by_node->InitExprAtPhysicalPlan();
    group_by_attrs.push_back(group_by_node);
  }
  for (int i = 0; i < state_.aggregation_attrs_.size(); ++i) {
    agg_node =
        reinterpret_cast<ExprUnary *>(state_.aggregation_attrs_[i]->ExprCopy());
    agg_node->InitExprAtPhysicalPlan();
    agg_attrs.push_back(agg_node);
  }

  if (TryEntryIntoSerializedSection(0)) {
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_desc, "Aggregation", 0));
  }
  BarrierArrive(0);

  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(1);
    return true;
  }
  state_.child_->Open(partition_offset);
  ticks start = curtick();
  if (TryEntryIntoSerializedSection(1)) {
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
   * consuming larger memory, private aggregation is more efficient than
   * shared
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
  void *group_by_expr_result;
  void *tuple_in_hashtable;
  void *key_in_input_tuple;
  void *key_in_hash_table;
  void *value_in_input_tuple;
  void *value_in_hash_table;
  void *new_tuple_in_hash_table;
  ExprEvalCnxt eecnxt;
  eecnxt.schema[0] = state_.input_schema_;

  unsigned allocated_tuples_in_hashtable = 0;
  BasicHashTable::Iterator ht_it = hashtable_->CreateIterator();
  BasicHashTable::Iterator pht_it = private_hashtable->CreateIterator();
  int64_t one = 1;
  BlockStreamBase *block_for_asking =
      BlockStreamBase::createBlock(state_.input_schema_, state_.block_size_);
  block_for_asking->setEmpty();

  start = curtick();
  // traverse every block from child
  while (state_.child_->Next(block_for_asking)) {
    BlockStreamBase::BlockStreamTraverseIterator *bsti =
        block_for_asking->createIterator();
    bsti->reset();
    // traverse every tuple from block
    while (NULL != (cur = bsti->currentTuple())) {
      /* get the corresponding bucket index according to the first column in
       * group-by attributes.
       * Note that bn is always 0 for scalar aggregation.
       */
      bn = 0;
      // execute group by attributes and get partition key
      eecnxt.tuple[0] = cur;
      if (state_.group_by_attrs_.size() > 0) {
        group_by_expr_result = group_by_attrs[0]->ExprEvaluate(eecnxt);
        bn = state_.hash_schema_->getcolumn(0).operate->getPartitionValue(
            group_by_expr_result, state_.num_of_buckets_);
      }
      private_hashtable->placeIterator(pht_it, bn);
      key_exist = false;
      while (NULL != (tuple_in_hashtable = pht_it.readCurrent())) {
        /* set key_exist flag to true such that the the case for scalar
         * aggregation (i.e., aggregation without group-by attributes)
         * could be considered as passed the group by value verification.
         */
        key_exist = true;
        for (int i = 0; i < group_by_attrs.size(); ++i) {
          group_by_expr_result = group_by_attrs[i]->ExprEvaluate(eecnxt);
          key_in_hash_table =
              state_.hash_schema_->getColumnAddess(i, tuple_in_hashtable);
          if (!state_.hash_schema_->getcolumn(i)
                   .operate->equal(group_by_expr_result, key_in_hash_table)) {
            key_exist = false;
            break;
          }
        }
        // hash table have the key (the value in group-by attribute)
        if (key_exist) {
          // value_in_input_tuple by expression
          // update function
          for (int i = 0; i < agg_attrs.size(); ++i) {
            agg_attrs[i]->ExprEvaluate(
                eecnxt, state_.hash_schema_->getColumnAddess(
                            i + group_by_size, tuple_in_hashtable));
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
      // set group-by's original value by expression
      for (int i = 0; i < group_by_attrs.size(); ++i) {
        key_in_input_tuple = group_by_attrs[i]->ExprEvaluate(eecnxt);
        key_in_hash_table =
            state_.hash_schema_->getColumnAddess(i, new_tuple_in_hash_table);
        state_.hash_schema_->getcolumn(i)
            .operate->assignment(key_in_input_tuple, key_in_hash_table);
      }
      // get value_in_input_tuple from expression
      for (int i = 0; i < agg_attrs.size(); ++i) {
        value_in_input_tuple = agg_attrs[i]->arg0_->ExprEvaluate(eecnxt);
        value_in_hash_table = state_.hash_schema_->getColumnAddess(
            group_by_size + i, new_tuple_in_hash_table);
        state_.hash_schema_->getcolumn(group_by_size + i)
            .operate->assignment(value_in_input_tuple, value_in_hash_table);
      }
      bsti->increase_cur_();
    }
    block_for_asking->setEmpty();
  }

  // merge private_hash_table into hash_table
  for (int i = 0; i < state_.num_of_buckets_; i++) {
    private_hashtable->placeIterator(pht_it, i);
    // traverse every tuple from block
    while (NULL != (cur = pht_it.readCurrent())) {
      /* get the corresponding bucket index according to the first column in
       * group-by attributes.
       * Note that bn is always 0 for scalar aggregation.
       */
      bn = 0;
      if (group_by_attrs.size() > 0) {
        bn = state_.hash_schema_->getcolumn(0).operate->getPartitionValue(
            state_.hash_schema_->getColumnAddess(0, cur),
            state_.num_of_buckets_);
      }
      // add a lock to guarantee operating the hash_table atomically
      hashtable_->lockBlock(bn);

      hashtable_->placeIterator(ht_it, bn);
      key_exist = false;
      while (NULL != (tuple_in_hashtable = ht_it.readCurrent())) {
        /* set key_exist flag to true such that the the case for scalar
         * aggregation (i.e., aggregation with no group-by attributes)
         * could be considered as passed the group by value verification.
         */
        key_exist = true;
        for (int i = 0; i < group_by_attrs.size(); ++i) {
          key_in_input_tuple = state_.hash_schema_->getColumnAddess(i, cur);
          key_in_hash_table =
              state_.hash_schema_->getColumnAddess(i, tuple_in_hashtable);
          if (!state_.hash_schema_->getcolumn(i)
                   .operate->equal(key_in_input_tuple, key_in_hash_table)) {
            key_exist = false;
            break;
          }
        }
        if (key_exist) {
          // hash table have the key (the value in group-by attribute)
          // global_aggregation update
          for (int i = 0; i < agg_attrs.size(); ++i) {
            agg_attrs[i]->ExprEvaluate(
                state_.hash_schema_->getColumnAddess(i + group_by_size, cur),
                state_.hash_schema_->getColumnAddess(i + group_by_size,
                                                     tuple_in_hashtable));
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
      // if the key doesn't exist, then set the initial value
      new_tuple_in_hash_table = hashtable_->allocate(bn);
      allocated_tuples_in_hashtable++;
      for (int i = 0; i < group_by_attrs.size(); ++i) {
        key_in_input_tuple = state_.hash_schema_->getColumnAddess(i, cur);
        key_in_hash_table =
            state_.hash_schema_->getColumnAddess(i, new_tuple_in_hash_table);
        state_.hash_schema_->getcolumn(i)
            .operate->assignment(key_in_input_tuple, key_in_hash_table);
      }
      for (int i = 0; i < agg_attrs.size(); ++i) {
        value_in_input_tuple =
            state_.hash_schema_->getColumnAddess(i + group_by_size, cur);
        value_in_hash_table = state_.hash_schema_->getColumnAddess(
            i + group_by_size, new_tuple_in_hash_table);
        state_.hash_schema_->getcolumn(i + group_by_size)
            .operate->assignment(value_in_input_tuple, value_in_hash_table);
      }
      hashtable_->unlockBlock(bn);
      pht_it.increase_cur_();
    }
  }

  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(2);
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
  if (NULL != block_for_asking) {
    delete block_for_asking;
    block_for_asking = NULL;
  }
  if (NULL != private_hashtable) {
    delete private_hashtable;
    private_hashtable = NULL;
  }
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
    while (NULL != (cur_in_ht = it_.readCurrent())) {
      if (NULL != (tuple = block->allocateTuple(
                       state_.output_schema_->getTupleMaxSize()))) {
        // copy the whole tuple, and update the corresponding column if there
        // is
        // avg()
        memcpy(tuple, cur_in_ht, state_.output_schema_->getTupleMaxSize());
        // update the sum=sum/count if there is avg()
        if (state_.avg_index_.size() > 0 &&
            (state_.agg_node_type_ == State::kHybridAggGlobal ||
             state_.agg_node_type_ == State::kNotHybridAgg)) {
          int id = 0;
          void *sum_value;
          int64_t count_value =
              *(int64_t *)(state_.hash_schema_->getColumnAddess(
                  state_.count_column_id_ + state_.group_by_attrs_.size(),
                  tuple));
          for (int i = 0; i < state_.avg_index_.size(); ++i) {
            id = state_.group_by_attrs_.size() + state_.avg_index_[i];
            sum_value = state_.hash_schema_->getColumnAddess(id, tuple);
            DataTypeOper::avg_divide_[state_.hash_schema_->columns[id].type](
                sum_value, count_value, sum_value);
          }
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
  if (NULL != hashtable_) {
    delete hashtable_;
    hashtable_ = NULL;
  }

  state_.child_->Close();
  return true;
}
void PhysicalAggregation::Print() {
  cout << "Aggregation:  " << state_.num_of_buckets_ << " buckets in hash table"
       << std::endl;
  cout << "group by attributes:" << endl;
  for (int i = 0; i < state_.group_by_attrs_.size(); ++i) {
    cout << "    " << state_.group_by_attrs_[i]->alias_ << endl;
  }
  cout << "aggregation attributes:" << endl;
  for (int i = 0; i < state_.aggregation_attrs_.size(); ++i) {
    cout << "    " << state_.aggregation_attrs_[i]->alias_ << endl;
  }
  cout << "---------------" << std::endl;
  state_.child_->Print();
}

}  // namespace physical_operator
}  // namespace claims
