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
#include <algorithm>
#include <stack>

#include "../codegen/ExpressionGenerator.h"
#include "../common/data_type.h"
#include "../common/expression/data_type_oper.h"
#include "../common/expression/expr_node.h"
#include "../common/hashtable.h"
#include "../Config.h"
#include "../Executor/expander_tracker.h"
#include "../utility/rdtsc.h"

using claims::common::DataTypeOper;
using claims::common::ExprNode;

// #define _DEBUG_

namespace claims {
namespace physical_operator {

PhysicalHashJoin::PhysicalHashJoin(State state)
    : state_(state),
      hash_func_(NULL),
      hashtable_(NULL),
      PhysicalOperator(barrier_number(2), serialized_section_number(1)),
      eftt_(NULL),
      memcpy_(NULL),
      memcat_(NULL),
      gpv_left_(NULL),
      gpv_right_(NULL) {
  set_phy_oper_type(kPhysicalHashJoin);
  // sema_open_.set_value(1);
  InitExpandedStatus();
}

PhysicalHashJoin::PhysicalHashJoin()
    : hash_func_(NULL),
      hashtable_(NULL),
      PhysicalOperator(barrier_number(2), serialized_section_number(1)),
      eftt_(NULL),
      memcpy_(NULL),
      memcat_(NULL),
      gpv_left_(NULL),
      gpv_right_(NULL) {
  set_phy_oper_type(kPhysicalHashJoin);

  // sema_open_.set_value(1);
  InitExpandedStatus();
}

PhysicalHashJoin::~PhysicalHashJoin() {
  for (int i = 0; i < state_.join_condi_.size(); ++i) {
    DELETE_PTR(state_.join_condi_[i]);
  }
  state_.join_condi_.clear();
}

PhysicalHashJoin::State::State(
    PhysicalOperatorBase* child_left, PhysicalOperatorBase* child_right,
    Schema* input_schema_left, Schema* input_schema_right,
    Schema* output_schema, Schema* ht_schema,
    std::vector<unsigned> joinIndex_left, std::vector<unsigned> joinIndex_right,
    unsigned ht_nbuckets, unsigned ht_bucketsize, unsigned block_size,
    vector<ExprNode*> join_condi)
    : child_left_(child_left),
      child_right_(child_right),
      input_schema_left_(input_schema_left),
      input_schema_right_(input_schema_right),
      output_schema_(output_schema),
      hashtable_schema_(ht_schema),
      join_index_left_(joinIndex_left),
      join_index_right_(joinIndex_right),
      hashtable_bucket_num_(ht_nbuckets),
      hashtable_bucket_size_(ht_bucketsize),
      block_size_(block_size),
      join_condi_(join_condi) {}

bool PhysicalHashJoin::Open(SegmentExecStatus* const exec_status,
                            const PartitionOffset& partition_offset) {
#ifdef TIME
  startTimer(&timer);
#endif

  RETURN_IF_CANCELLED(exec_status);

  RegisterExpandedThreadToAllBarriers();

  unsigned long long int timer;
  bool winning_thread = false;
  if (TryEntryIntoSerializedSection(0)) {
    winning_thread = true;
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_desc, "Hash join build", 0));
    hash_func_ = PartitionFunctionFactory::createBoostHashFunction(
        state_.hashtable_bucket_num_);
    unsigned long long hash_table_build = curtick();
    // optimal bucket size could contain 2 tuples
    hashtable_ = new BasicHashTable(
        state_.hashtable_bucket_num_, state_.hashtable_bucket_size_,
        state_.input_schema_left_->getTupleMaxSize());

    gpv_left_ = DataTypeOper::partition_value_
        [state_.input_schema_left_->getcolumn(state_.join_index_left_[0])
             .type][((state_.hashtable_bucket_num_ &
                      (state_.hashtable_bucket_num_ - 1)) == 0)];
    gpv_right_ = DataTypeOper::partition_value_
        [state_.input_schema_right_->getcolumn(state_.join_index_right_[0])
             .type][((state_.hashtable_bucket_num_ &
                      (state_.hashtable_bucket_num_ - 1)) == 0)];
    bucket_num_mod_ = state_.hashtable_bucket_num_;
    if (((state_.hashtable_bucket_num_ & (state_.hashtable_bucket_num_ - 1)) ==
         0)) {
      --bucket_num_mod_;
    }
#ifdef _DEBUG_
    consumed_tuples_from_left = 0;
#endif

    memcat_ = getMemcat(state_.hashtable_schema_->getTupleMaxSize(),
                        state_.input_schema_right_->getTupleMaxSize());
#ifdef CodeGen
    QNode* expr = createEqualJoinExpression(
        state_.hashtable_schema_, state_.input_schema_right_,
        state_.join_index_left_, state_.join_index_right_);
    ticks start = curtick();
    if (Config::enable_codegen) {
      eftt_ = getExprFuncTwoTuples(expr, state_.hashtable_schema_,
                                   state_.input_schema_right_);
      memcpy_ = getMemcpy(state_.hashtable_schema_->getTupleMaxSize());
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
#endif
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
  state_.child_left_->Open(exec_status, partition_offset);
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
  unsigned long long int start = curtick();
  unsigned long long int processed_tuple_count = 0;
  RETURN_IF_CANCELLED(exec_status);

  LOG(INFO) << "join operator begin to call left child's next()" << endl;
  while (state_.child_left_->Next(exec_status, jtc->l_block_for_asking_)) {
    RETURN_IF_CANCELLED(exec_status);
    // TODO(fzh) should reuse the block_iterator, instead of doing create/delete
    delete jtc->l_block_stream_iterator_;
    jtc->l_block_stream_iterator_ = jtc->l_block_for_asking_->createIterator();
    while (NULL != (cur = jtc->l_block_stream_iterator_->nextTuple())) {
#ifdef _DEBUG_
      processed_tuple_count++;
      lock_.acquire();
      consumed_tuples_from_left++;
      lock_.release();
#endif
      const void* key_addr =
          input_schema->getColumnAddess(state_.join_index_left_[0], cur);
      bn = gpv_left_(key_addr, bucket_num_mod_);
      tuple_in_hashtable = hashtable_->atomicAllocate(bn);
      /* copy join index columns*/
      input_schema->copyTuple(cur, tuple_in_hashtable);
    }
    jtc->l_block_for_asking_->setEmpty();
  }

#ifdef _DEBUG_
  tuples_in_hashtable = 0;

  produced_tuples = 0;
  consumed_tuples_from_right = 0;
#endif
  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(1);
    return true;
  }

  BarrierArrive(1);
  state_.child_right_->Open(exec_status, partition_offset);
  LOG(INFO) << "join operator finished opening right child" << endl;
  return true;
}

bool PhysicalHashJoin::Next(SegmentExecStatus* const exec_status,
                            BlockStreamBase* block) {
  RETURN_IF_CANCELLED(exec_status);

  void* result_tuple = NULL;
  void* tuple_from_right_child;
  void* tuple_in_hashtable;
  void* key_in_input;
  void* key_in_hashtable;
  void* column_in_joined_tuple;
  bool key_exit;
  int hash_tuple_size = state_.hashtable_schema_->getTupleMaxSize();

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
    RETURN_IF_CANCELLED(exec_status);

    while (NULL != (tuple_from_right_child =
                        jtc->r_block_stream_iterator_->currentTuple())) {
      while (NULL !=
             (tuple_in_hashtable = jtc->hashtable_iterator_.readCurrent())) {
#ifdef CodeGen
        cff_(tuple_in_hashtable, tuple_from_right_child, &key_exit,
             state_.join_index_left_, state_.join_index_right_,
             state_.hashtable_schema_, state_.input_schema_right_, eftt_);
#else
        key_exit =
            JoinCondiProcess(tuple_in_hashtable, tuple_from_right_child, jtc);
#endif
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
        unsigned bn =
            gpv_right_(state_.input_schema_right_->getColumnAddess(
                           state_.join_index_right_[0], tuple_from_right_child),
                       bucket_num_mod_);
        hashtable_->placeIterator(jtc->hashtable_iterator_, bn);
      }
    }
    jtc->r_block_for_asking_->setEmpty();
    jtc->hashtable_iterator_ = hashtable_->CreateIterator();
    if (state_.child_right_->Next(exec_status, jtc->r_block_for_asking_) ==
        false) {
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
          gpv_right_(state_.input_schema_right_->getColumnAddess(
                         state_.join_index_right_[0], tuple_from_right_child),
                     bucket_num_mod_);
      hashtable_->placeIterator(jtc->hashtable_iterator_, bn);
    }
  }
}

bool PhysicalHashJoin::Close(SegmentExecStatus* const exec_status) {
#ifdef TIME
  stopTimer(&timer);
  LOG(INFO) << "time consuming: " << timer << ", "
            << timer / static_cast<double> CPU_FRE << endl;
#endif
  LOG(INFO) << "Consumes" << consumed_tuples_from_left
            << "tuples from left child!" << endl;
  InitExpandedStatus();
  DestoryAllContext();
  if (NULL != hashtable_) {
    delete hashtable_;
    hashtable_ = NULL;
  }
  state_.child_left_->Close(exec_status);
  state_.child_right_->Close(exec_status);
  return true;
}

void PhysicalHashJoin::Print() {
  LOG(INFO) << "Join: buckets: (num= " << state_.hashtable_bucket_num_
            << " , size= "
            << get_aligned_space(state_.input_schema_left_->getTupleMaxSize())
            << ")" << endl;
  cout << "Join: buckets: (num= " << state_.hashtable_bucket_num_ << " , size= "
       << get_aligned_space(state_.input_schema_left_->getTupleMaxSize()) << ")"
       << endl;

  LOG(INFO) << "------Join Left-------" << endl;
  cout << "------Join Left-------" << endl;

  state_.child_left_->Print();
  LOG(INFO) << "------Join Right-------" << endl;
  cout << "------Join Right-------" << endl;

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

inline void PhysicalHashJoin::IsMatchCodegen(
    void* l_tuple_addr, void* r_tuple_addr, void* return_addr,
    vector<unsigned>& l_join_index, vector<unsigned>& r_join_index,
    Schema* l_schema, Schema* r_schema, ExprFuncTwoTuples func) {
  func(l_tuple_addr, r_tuple_addr, return_addr);
}
inline bool PhysicalHashJoin::JoinCondiProcess(void* tuple_left,
                                               void* tuple_right,
                                               JoinThreadContext* const hjtc) {
  hjtc->expr_eval_cnxt_.tuple[0] = tuple_left;
  hjtc->expr_eval_cnxt_.tuple[1] = tuple_right;
  bool pass = false;
  for (int i = 0; i < hjtc->join_condi_.size(); ++i) {
    pass = *(bool*)(hjtc->join_condi_[i]->ExprEvaluate(hjtc->expr_eval_cnxt_));
    if (pass == false) {
      return false;
    }
  }
  return true;
}
PhysicalHashJoin::JoinThreadContext::~JoinThreadContext() {
  delete l_block_for_asking_;
  delete l_block_stream_iterator_;
  delete r_block_for_asking_;
  delete r_block_stream_iterator_;
  for (int i = 0; i < join_condi_.size(); ++i) {
    DELETE_PTR(join_condi_[i]);
  }
  join_condi_.clear();
}

ThreadContext* PhysicalHashJoin::CreateContext() {
  JoinThreadContext* jtc = new JoinThreadContext();
  jtc->l_block_for_asking_ = BlockStreamBase::createBlock(
      state_.input_schema_left_, state_.block_size_);
  jtc->l_block_stream_iterator_ = jtc->l_block_for_asking_->createIterator();
  jtc->r_block_for_asking_ = BlockStreamBase::createBlock(
      state_.input_schema_right_, state_.block_size_);
  jtc->r_block_stream_iterator_ = jtc->r_block_for_asking_->createIterator();
  ExprNode* new_node = NULL;
  for (int i = 0; i < state_.join_condi_.size(); ++i) {
    new_node = state_.join_condi_[i]->ExprCopy();
    new_node->InitExprAtPhysicalPlan();
    jtc->join_condi_.push_back(new_node);
  }
  jtc->expr_eval_cnxt_.schema[0] = state_.input_schema_left_;
  jtc->expr_eval_cnxt_.schema[1] = state_.input_schema_right_;
  return jtc;
}
RetCode PhysicalHashJoin::GetAllSegments(stack<Segment*>* all_segments) {
  RetCode ret = rSuccess;
  if (NULL != state_.child_right_) {
    ret = state_.child_right_->GetAllSegments(all_segments);
  }
  if (NULL != state_.child_left_) {
    ret = state_.child_left_->GetAllSegments(all_segments);
  }
  return ret;
}
}  // namespace physical_operator
}  // namespace claims
