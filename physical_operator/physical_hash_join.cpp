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
 * /CLAIMS/physical_operator/physical_hash_join.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: zhanglei
 *       Email:
 *
 *  Modified on: Oct 20, 2015
 *       Author: tonglanxuan
 *        Email: lxtong0526@163.com
 *
 * Description: Detailedly describes how the PhysicalJoin class functions.
 *
 */

#include "../physical_operator/physical_hash_join.h"

#include <glog/logging.h>
#include "../codegen/ExpressionGenerator.h"
#include "../Config.h"
#include "../Executor/expander_tracker.h"
#include "../utility/rdtsc.h"
// #define _DEBUG_

namespace claims {
namespace physical_operator {

PhysicalHashJoin::PhysicalHashJoin(State state)
    : state_(state),
      hash_(0),
      hashtable_(0),
      PhysicalOperator(barrier_number(2), serialized_section_number(1)),
      eftt_(0),
      memcpy_(0),
      memcat_(0) {
  // sema_open_.set_value(1);
  InitExpandedStatus();
}

PhysicalHashJoin::PhysicalHashJoin()
    : hash_(0),
      hashtable_(0),
      PhysicalOperator(barrier_number(2), serialized_section_number(1)),
      eftt_(0),
      memcpy_(0),
      memcat_(0) {
  // sema_open_.set_value(1);
  InitExpandedStatus();
}

PhysicalHashJoin::~PhysicalHashJoin() {}

PhysicalHashJoin::State::State(
    PhysicalOperatorBase* child_left, PhysicalOperatorBase* child_right,
    Schema* input_schema_left, Schema* input_schema_right,
    Schema* output_schema, Schema* ht_schema,
    std::vector<unsigned> joinIndex_left, std::vector<unsigned> joinIndex_right,
    std::vector<unsigned> payload_left, std::vector<unsigned> payload_right,
    unsigned ht_nbuckets, unsigned ht_bucketsize, unsigned block_size)
    : child_left_(child_left),
      child_right_(child_right),
      input_schema_left_(input_schema_left),
      input_schema_right_(input_schema_right),
      output_schema_(output_schema),
      hashtable_schema_(ht_schema),
      join_index_left_(joinIndex_left),
      join_index_right_(joinIndex_right),
      payload_left_(payload_left),
      payload_right_(payload_right),
      hashtable_bucket_num_(ht_nbuckets),
      hashtable_bucket_size_(ht_bucketsize),
      block_size_(block_size) {}

bool PhysicalHashJoin::Open(const PartitionOffset& partition_offset) {
#ifdef TIME
  startTimer(&timer);
#endif

  RegisterExpandedThreadToAllBarriers();

  unsigned long long int timer;
  bool winning_thread = false;
  if (TryEntryIntoSerializedSection(0)) {
    winning_thread = true;
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_desc, "Hash join build", 0));
    unsigned output_index = 0;
    for (unsigned i = 0; i < state_.join_index_left_.size(); i++) {
      join_index_left_to_output_[i] = output_index;
      output_index++;
    }
    for (unsigned i = 0; i < state_.payload_left_.size(); i++) {
      payload_left_to_output_[i] = output_index;
      output_index++;
    }
    for (unsigned i = 0; i < state_.payload_right_.size(); i++) {
      payload_right_to_output_[i] = output_index;
      output_index++;
    }

    hash_ = PartitionFunctionFactory::createBoostHashFunction(
        state_.hashtable_bucket_num_);
    unsigned long long hash_table_build = curtick();
    hashtable_ = new BasicHashTable(
        state_.hashtable_bucket_num_, state_.hashtable_bucket_size_,
        state_.input_schema_left_->getTupleMaxSize());
#ifdef _DEBUG_
    consumed_tuples_from_left = 0;
#endif
    QNode* expr = createEqualJoinExpression(
        state_.hashtable_schema_, state_.input_schema_right_,
        state_.join_index_left_, state_.join_index_right_);
    ticks start = curtick();
    if (Config::enable_codegen) {
      eftt_ = getExprFuncTwoTuples(expr, state_.hashtable_schema_,
                                   state_.input_schema_right_);
      memcpy_ = getMemcpy(state_.hashtable_schema_->getTupleMaxSize());
      memcat_ = getMemcat(state_.hashtable_schema_->getTupleMaxSize(),
                          state_.input_schema_right_->getTupleMaxSize());
    }
    if (eftt_) {
      cff_ = PhysicalHashJoin::IsMatchCodegen;
      LOG(INFO) << "Codegen(Join) succeed(" << setw(8) << fixed
                << setprecision(3) << getMilliSecond(start) << endl;
    } else {
      cff_ = PhysicalHashJoin::IsMatch;
      LOG(INFO) << "Codegen(Join) failed!" << endl;
    }
    delete expr;
  }

  /**
   * For performance concern, the following line should place just after
   *"RegisterNewThreadToAllBarriers();"
   * in order to accelerate the open response time.
   *
   * I suppose not moving it before TryEntryIntoSerializedSection(0), in that
   * case, all the other threads must wait until the main thread finished
   * serialization, then continue processing. Tong
   */
  LOG(INFO) << "join operator begin to open left child" << endl;
  state_.child_left_->Open(partition_offset);
  LOG(INFO) << "join operator finished opening left child" << endl;
  BarrierArrive(0);
  BasicHashTable::Iterator tmp_it = hashtable_->CreateIterator();

  void* cur;
  void* tuple_in_hashtable;
  unsigned bn;

  void* key_in_input;
  void* key_in_hashtable;
  void* value_in_input;
  void* value_in_hashtable;

  JoinThreadContext* jtc = CreateOrReuseContext(crm_numa_sensitive);

  const Schema* input_schema = state_.input_schema_left_->duplicateSchema();
  const Operate* op = input_schema->getcolumn(state_.join_index_left_[0])
                          .operate->duplicateOperator();
  const unsigned buckets = state_.hashtable_bucket_num_;

  unsigned long long int start = curtick();
  unsigned long long int processed_tuple_count = 0;

  LOG(INFO) << "join operator begin to call left child's next()" << endl;
  while (state_.child_left_->Next(jtc->l_block_for_asking_)) {
    delete jtc->l_block_stream_iterator_;
    jtc->l_block_stream_iterator_ = jtc->l_block_for_asking_->createIterator();
    while (cur = jtc->l_block_stream_iterator_->nextTuple()) {
#ifdef _DEBUG_
      processed_tuple_count++;
      lock_.acquire();
      consumed_tuples_from_left++;
      lock_.release();
#endif
      const void* key_addr =
          input_schema->getColumnAddess(state_.join_index_left_[0], cur);
      bn = op->getPartitionValue(key_addr, buckets);
      tuple_in_hashtable = hashtable_->atomicAllocate(bn);
      /* copy join index columns*/
      if (memcpy_)
        memcpy_(tuple_in_hashtable, cur);
      else
        input_schema->copyTuple(cur, tuple_in_hashtable);
    }
    jtc->l_block_for_asking_->setEmpty();
  }

  //   printf("%d cycles per
  //   tuple!\n",(curtick()-start)/processed_tuple_count);
  unsigned tmp = 0;
#ifdef _DEBUG_
  tuples_in_hashtable = 0;

  produced_tuples = 0;
  consumed_tuples_from_right = 0;
#endif
  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(1);
    // printf("<<<<<<<<<<<<<<<<<Join open detected call back
    // signal!>>>>>>>>>>>>>>>>>\n");
    return true;
  }

  BarrierArrive(1);
  //  if(winning_thread){
  //  // hashtable->report_status();
  //  // printf("Hash Table Build time: %4.4f\n",getMilliSecond(timer));
  //  }
  //
  //  hashtable->report_status();
  //
  //  printf("join open consume %d tuples\n",consumed_tuples_from_left);

  state_.child_right_->Open(partition_offset);
  LOG(INFO) << "join operator finished opening right child" << endl;
  return true;
}

bool PhysicalHashJoin::Next(BlockStreamBase* block) {
  void* result_tuple;
  void* tuple_from_right_child;
  void* tuple_in_hashtable;
  void* key_in_input;
  void* key_in_hashtable;
  void* column_in_joined_tuple;
  bool key_exit;

  JoinThreadContext* jtc = (JoinThreadContext*)GetContext();

  /**
   * A specific method suitable for physical_join.
   * In this case, it preserves the r_block_stream_iterator_ and
   * hashtable_iterator_ status for physical_join's father to call Next() from
   * the previous call.
   * e.g.: Let's suppose that when physical_join's father first called Next(),
   * it returned true when the block for sending was full whereas half of the
   * tuples of a block from its right child was read. The next time
   * physical_join's father calls Next(), it should go on operates on that
   * right child block, so we need r_block_stream_iterator_ preserved, moreover
   * it should go on operates on last matched tuple not sent due to block for
   * send was full, so we need hashtable_iterator_ preserved.
   */
  while (true) {
    while (NULL != (tuple_from_right_child =
                        jtc->r_block_stream_iterator_->currentTuple())) {
      unsigned bn =
          state_.input_schema_right_->getcolumn(state_.join_index_right_[0])
              .operate->getPartitionValue(
                  state_.input_schema_right_->getColumnAddess(
                      state_.join_index_right_[0], tuple_from_right_child),
                  state_.hashtable_bucket_num_);

      while (NULL !=
             (tuple_in_hashtable = jtc->hashtable_iterator_.readCurrent())) {
        cff_(tuple_in_hashtable, tuple_from_right_child, &key_exit,
             state_.join_index_left_, state_.join_index_right_,
             state_.hashtable_schema_, state_.input_schema_right_, eftt_);
        if (key_exit) {
          if (NULL != (result_tuple = block->allocateTuple(
                           state_.output_schema_->getTupleMaxSize()))) {
            produced_tuples++;
            if (memcat_) {
              memcat_(result_tuple, tuple_in_hashtable, tuple_from_right_child);
            } else {
              const unsigned copyed_bytes =
                  state_.input_schema_left_->copyTuple(tuple_in_hashtable,
                                                       result_tuple);
              state_.input_schema_right_->copyTuple(
                  tuple_from_right_child, (char*)result_tuple + copyed_bytes);
            }
          } else {
            return true;
          }
        }
        jtc->hashtable_iterator_.increase_cur_();
      }
      jtc->r_block_stream_iterator_->increase_cur_();
#ifdef _DEBUG_
      consumed_tuples_from_right++;
#endif
      if (NULL != (tuple_from_right_child =
                       jtc->r_block_stream_iterator_->currentTuple())) {
        bn = state_.input_schema_right_->getcolumn(state_.join_index_right_[0])
                 .operate->getPartitionValue(
                     state_.input_schema_right_->getColumnAddess(
                         state_.join_index_right_[0], tuple_from_right_child),
                     state_.hashtable_bucket_num_);
        hashtable_->placeIterator(jtc->hashtable_iterator_, bn);
      }
    }
    jtc->r_block_for_asking_->setEmpty();
    jtc->hashtable_iterator_ = hashtable_->CreateIterator();
    if (state_.child_right_->Next(jtc->r_block_for_asking_) == false) {
      if (block->Empty() == true) {
        return false;
      } else {
        return true;
      }
    }
    delete jtc->r_block_stream_iterator_;
    jtc->r_block_stream_iterator_ = jtc->r_block_for_asking_->createIterator();
    if ((tuple_from_right_child =
             jtc->r_block_stream_iterator_->currentTuple())) {
      unsigned bn =
          state_.input_schema_right_->getcolumn(state_.join_index_right_[0])
              .operate->getPartitionValue(
                  state_.input_schema_right_->getColumnAddess(
                      state_.join_index_right_[0], tuple_from_right_child),
                  state_.hashtable_bucket_num_);
      hashtable_->placeIterator(jtc->hashtable_iterator_, bn);
    }
  }
}

bool PhysicalHashJoin::Close() {
#ifdef TIME
  stopTimer(&timer);
  LOG(INFO) << "time consuming: " << timer << ", "
            << timer / static_cast<double> CPU_FRE << endl;
#endif
  LOG(INFO) << "Consumes" << consumed_tuples_from_left
            << "tuples from left child!" << endl;
  InitExpandedStatus();
  DestoryAllContext();
  delete hashtable_;
  state_.child_left_->Close();
  state_.child_right_->Close();
  return true;
}

void PhysicalHashJoin::Print() {
  LOG(INFO) << "Join: buckets:" << state_.hashtable_bucket_num_ << endl;
  LOG(INFO) << "------Join Left-------" << endl;
  state_.child_left_->Print();
  LOG(INFO) << "------Join Right-------" << endl;

  state_.child_right_->Print();
}

inline void PhysicalHashJoin::IsMatch(void* l_tuple_addr, void* r_tuple_addr,
                                  void* return_addr,
                                  vector<unsigned>& l_join_index,
                                  vector<unsigned>& r_join_index,
                                  Schema* l_schema, Schema* r_schema,
                                  ExprFuncTwoTuples func) {
  bool key_exit = true;
  for (unsigned i = 0; i < r_join_index.size(); i++) {
    void* key_in_input =
        r_schema->getColumnAddess(r_join_index[i], r_tuple_addr);
    void* key_in_hashtable =
        l_schema->getColumnAddess(l_join_index[i], l_tuple_addr);
    if (!r_schema->getcolumn(r_join_index[i])
             .operate->equal(key_in_input, key_in_hashtable)) {
      key_exit = false;
      break;
    }
  }
  *(bool*)return_addr = key_exit;
}

inline void PhysicalHashJoin::IsMatchCodegen(void* l_tuple_addr, void* r_tuple_addr,
                                         void* return_addr,
                                         vector<unsigned>& l_join_index,
                                         vector<unsigned>& r_join_index,
                                         Schema* l_schema, Schema* r_schema,
                                         ExprFuncTwoTuples func) {
  func(l_tuple_addr, r_tuple_addr, return_addr);
}

PhysicalHashJoin::JoinThreadContext::~JoinThreadContext() {
  delete l_block_for_asking_;
  delete l_block_stream_iterator_;
  delete r_block_for_asking_;
  delete r_block_stream_iterator_;
}

ThreadContext* PhysicalHashJoin::CreateContext() {
  JoinThreadContext* jtc = new JoinThreadContext();
  jtc->l_block_for_asking_ = BlockStreamBase::createBlock(
      state_.input_schema_left_, state_.block_size_);
  jtc->l_block_stream_iterator_ = jtc->l_block_for_asking_->createIterator();
  jtc->r_block_for_asking_ = BlockStreamBase::createBlock(
      state_.input_schema_right_, state_.block_size_);
  jtc->r_block_stream_iterator_ = jtc->r_block_for_asking_->createIterator();
  return jtc;
}

}  // namespace physical_operator
}  // namespace claims
