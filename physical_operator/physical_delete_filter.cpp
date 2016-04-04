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
 * /CLAIMS/physical_query_plan/DeleteFilter.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: minqi
 *		   Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description:
 *
 */

#include <glog/logging.h>
#include "../Executor/expander_tracker.h"
#include "../../codegen/ExpressionGenerator.h"
#include "../../Config.h"
#include "../../utility/rdtsc.h"
#include "../../common/error_define.h"
#include "../../common/error_no.h"
#include "./physical_delete_filter.h"

#include <stack>
using namespace claims::common;
namespace claims {
namespace physical_operator {

PhysicalDeleteFilter::PhysicalDeleteFilter()
    : PhysicalOperator(barrier_number(2), serialized_section_number(1)),
      hash_(NULL),
      hashtable_(NULL),
      hashtable_schema_(NULL),
      eftt_(NULL),
      memcpy_(NULL),
      memcat_(NULL) {
  set_phy_oper_type(kPhysicalDeleteFilter);
  InitExpandedStatus();
}

PhysicalDeleteFilter::PhysicalDeleteFilter(State state)
    : PhysicalOperator(barrier_number(2), serialized_section_number(1)),
      state_(state),
      hash_(NULL),
      hashtable_(NULL),
      hashtable_schema_(NULL),
      eftt_(NULL),
      memcpy_(NULL),
      memcat_(NULL) {
  set_phy_oper_type(kPhysicalDeleteFilter);
  InitExpandedStatus();
}

PhysicalDeleteFilter::~PhysicalDeleteFilter() {}

PhysicalDeleteFilter::State::State(PhysicalOperatorBase* child_left,
                                   PhysicalOperatorBase* child_right,
                                   Schema* input_schema_left,
                                   Schema* input_schema_right,
                                   Schema* output_schema, Schema* ht_schema,
                                   unsigned block_size)
    : child_left_(child_left),
      child_right_(child_right),
      input_schema_left_(input_schema_left),
      input_schema_right_(input_schema_right),
      output_schema_(output_schema),
      hashtable_schema_(ht_schema),
      block_size_(block_size) {}

/**
 * build a hash table first, which stores the tuple needed to be deleted in a
 *hash manner and accelerate the probe phase
 *
 */
bool PhysicalDeleteFilter::Open(SegmentExecStatus* const exec_status,
                                const PartitionOffset& partition_offset) {
#ifdef TIME
  startTimer(&timer);
#endif
  RETURN_IF_CANCELLED(exec_status);

  RegisterExpandedThreadToAllBarriers();
  int ret = rSuccess;
  int64_t timer;
  bool winning_thread = false;
  if (TryEntryIntoSerializedSection(0)) {
    winning_thread = true;
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(),
        LocalStageEndPoint(stage_desc, "delete filter build", 0));
    unsigned output_index = 0;
    for (unsigned i = 0; i < state_.filter_key_deleted_.size(); i++) {
      joinIndex_table_to_output_[i] = output_index;
      output_index++;
    }
    for (unsigned i = 0; i < state_.payload_base_.size(); i++) {
      payload_table_to_output_[i] = output_index;
      output_index++;
    }
    // start to create the hash table, including the used hash function, hash
    // table structure
    hash_ = PartitionFunctionFactory::createBoostHashFunction(
        state_.hashtable_bucket_num_);
    int64_t hash_table_build = curtick();
    hashtable_ = new BasicHashTable(
        state_.hashtable_bucket_num_, state_.hashtable_bucket_size_,
        state_.input_schema_left_->getTupleMaxSize());
    if (NULL == hashtable_) {
      return ret = rMemoryAllocationFailed;
      LOG(ERROR) << "hashtable allocation failed"
                 << "[" << rMemoryAllocationFailed << "]" << endl;
    }
#ifdef _DEBUG_
    consumed_tuples_from_left = 0;
#endif

    // start to create the join expression, based on which it is able to the
    // probe the deleted tuples
    //    QNode* expr = createEqualJoinExpression(
    //        state_.hashtable_schema_, state_.input_schema_right_,
    //        state_.filter_key_deleted_, state_.filter_key_base_);
    //    if (NULL == expr) {
    //      ret = rSuccess;
    //      LOG(ERROR) << "The generation of the enqual join expression for
    //      delete "
    //                    "filter is failed" << endl;
    //    }
    //    ticks start = curtick();
    //
    //    // start to generate the dedicated function, based on which the probe
    //    is
    //    // eventually acted, including using llvm and the function pointer
    //    if (Config::enable_codegen) {
    //      eftt_ = getExprFuncTwoTuples(expr, state_.hashtable_schema_,
    //                                   state_.input_schema_right_);
    //      memcpy_ = getMemcpy(state_.hashtable_schema_->getTupleMaxSize());
    //      memcat_ = getMemcat(state_.hashtable_schema_->getTupleMaxSize(),
    //                          state_.input_schema_right_->getTupleMaxSize());
    //    }
    //    if (eftt_) {
    //      cff_ = PhysicalDeleteFilter::isMatchCodegen;
    //      printf("Codegen(delete filter) succeed(%4.3fms)!\n",
    //             getMilliSecond(start));
    //    } else {
    cff_ = PhysicalDeleteFilter::isMatch;
    //      printf("Codegen(delete filter) failed!\n");
    //    }
    //    delete expr;
  }

  /**
   * For performance concern, the following line should place just after
   * "RegisterNewThreadToAllBarriers();"
   * in order to accelerate the open response time.
   */
  LOG(INFO) << "delete filter operator begin to open left child" << endl;
  state_.child_left_->Open(exec_status, partition_offset);
  LOG(INFO) << "delete filter operator finished opening left child" << endl;
  BarrierArrive(0);
  BasicHashTable::Iterator tmp_it = hashtable_->CreateIterator();

  void* cur;
  void* tuple_in_hashtable;
  unsigned bn;

  void* key_in_input;
  void* key_in_hashtable;
  void* value_in_input;
  void* value_in_hashtable;
  // create the context for the multi-thread to build the hash table
  DeleteFilterThreadContext* dftc = CreateOrReuseContext(crm_numa_sensitive);
  const Schema* input_schema = state_.input_schema_left_->duplicateSchema();
  //  we used the filter_key_deleted_[0] here, because the data is partitioned
  //  based on the first column in the join index
  const Operate* op = input_schema->getcolumn(state_.filter_key_deleted_[0])
                          .operate->duplicateOperator();
  const unsigned buckets = state_.hashtable_bucket_num_;

  int64_t start = curtick();
  int64_t processed_tuple_count = 0;

  LOG(INFO) << "delete filter operator begin to call left child's next()"
            << endl;
  RETURN_IF_CANCELLED(exec_status);

  while (state_.child_left_->Next(exec_status, dftc->l_block_for_asking_)) {
    RETURN_IF_CANCELLED(exec_status);
    delete dftc->l_block_stream_iterator_;
    dftc->l_block_stream_iterator_ =
        dftc->l_block_for_asking_->createIterator();
    while (cur = dftc->l_block_stream_iterator_->nextTuple()) {
#ifdef _DEBUG_
      processed_tuple_count++;
      lock_.acquire();
      consumed_tuples_from_left++;
      lock_.release();
#endif
      const void* key_addr =
          input_schema->getColumnAddess(state_.filter_key_deleted_[0], cur);
      bn = op->getPartitionValue(key_addr, buckets);
      tuple_in_hashtable = hashtable_->atomicAllocate(bn);
      if (memcpy_)
        memcpy_(tuple_in_hashtable, cur);
      else
        input_schema->copyTuple(cur, tuple_in_hashtable);
    }
    dftc->l_block_for_asking_->setEmpty();
  }
  //  printf("%d cycles per
  //  tuple!\n",(curtick()-start)/processed_tuple_count);
  unsigned tmp = 0;
#ifdef _DEBUG_
  tuples_in_hashtable = 0;

  produced_tuples = 0;
  consumed_tuples_from_right = 0;
#endif
  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(1);
    //    printf("<<<<<<<<<<<<<<<<<Join open detected call back
    //    signal!>>>>>>>>>>>>>>>>>\n");
    return true;
  }
  BarrierArrive(1);
  //  if(winning_thread){
  ////    hashtable->report_status();
  ////    printf("Hash Table Build time: %4.4f\n",getMilliSecond(timer));
  //  }

  //  hashtable->report_status();

  //  printf("join open consume %d tuples\n",consumed_tuples_from_left);
  RETURN_IF_CANCELLED(exec_status);

  state_.child_right_->Open(exec_status, partition_offset);
  RETURN_IF_CANCELLED(exec_status);

  LOG(INFO) << "delete filter operator finished opening right child" << endl;
  return true;
}

bool PhysicalDeleteFilter::Next(SegmentExecStatus* const exec_status,
                                BlockStreamBase* block) {
  void* result_tuple;
  void* tuple_from_right_child;
  void* tuple_in_hashtable;
  void* key_in_input;
  void* key_in_hashtable;
  void* column_in_joinedTuple;
  void* joinedTuple =
      memalign(cacheline_size, state_.output_schema_->getTupleMaxSize());
  bool key_exist;

  DeleteFilterThreadContext* dftc =
      reinterpret_cast<DeleteFilterThreadContext*>(GetContext());

  while (true) {
    RETURN_IF_CANCELLED(exec_status);

    while ((tuple_from_right_child =
                dftc->r_block_stream_iterator_->currentTuple()) > 0) {
      unsigned bn =
          state_.input_schema_right_->getcolumn(state_.filter_key_base_[0])
              .operate->getPartitionValue(
                  state_.input_schema_right_->getColumnAddess(
                      state_.filter_key_base_[0], tuple_from_right_child),
                  state_.hashtable_bucket_num_);
      // hashtable_->placeIterator(dftc->hashtable_iterator_, bn);
      // if there is no tuple in the bn bucket of the hash table, then the
      // tuple
      // in the base table will be output
      if (NULL ==
          (tuple_in_hashtable = dftc->hashtable_iterator_.readCurrent())) {
        if ((result_tuple = block->allocateTuple(
                 state_.output_schema_->getTupleMaxSize())) > 0) {
          produced_tuples_++;
          if (memcat_) {
            memcat_(result_tuple, tuple_in_hashtable, tuple_from_right_child);
          } else {
            state_.input_schema_right_->copyTuple(
                tuple_from_right_child, reinterpret_cast<char*>(result_tuple));
          }
        } else {
          free(joinedTuple);
          return true;
        }
      } else {
        while ((tuple_in_hashtable = dftc->hashtable_iterator_.readCurrent()) >
               0) {
          cff_(tuple_in_hashtable, tuple_from_right_child, &key_exist,
               state_.filter_key_deleted_, state_.filter_key_base_,
               state_.hashtable_schema_, state_.input_schema_right_, eftt_);
          if (!key_exist) {
            if ((result_tuple = block->allocateTuple(
                     state_.output_schema_->getTupleMaxSize())) > 0) {
              produced_tuples_++;
              if (memcat_) {
                memcat_(result_tuple, tuple_in_hashtable,
                        tuple_from_right_child);
              } else {
                state_.input_schema_right_->copyTuple(
                    tuple_from_right_child,
                    reinterpret_cast<char*>(result_tuple));
              }
            } else {
              free(joinedTuple);
              return true;
            }
          }
          dftc->hashtable_iterator_.increase_cur_();
        }
      }
      dftc->r_block_stream_iterator_->increase_cur_();
#ifdef _DEBUG_
      consumed_tuples_from_right++;
#endif
      if ((tuple_from_right_child =
               dftc->r_block_stream_iterator_->currentTuple())) {
        bn = state_.input_schema_right_->getcolumn(state_.filter_key_base_[0])
                 .operate->getPartitionValue(
                     state_.input_schema_right_->getColumnAddess(
                         state_.filter_key_base_[0], tuple_from_right_child),
                     state_.hashtable_bucket_num_);
        hashtable_->placeIterator(dftc->hashtable_iterator_, bn);
      }
    }
    dftc->r_block_for_asking_->setEmpty();
    dftc->hashtable_iterator_ = hashtable_->CreateIterator();
    if (state_.child_right_->Next(exec_status, dftc->r_block_for_asking_) ==
        false) {
      if (block->Empty() == true) {
        free(joinedTuple);
        return false;
      } else {
        free(joinedTuple);
        return true;
      }
    }
    delete dftc->r_block_stream_iterator_;
    dftc->r_block_stream_iterator_ =
        dftc->r_block_for_asking_->createIterator();
    if ((tuple_from_right_child =
             dftc->r_block_stream_iterator_->currentTuple())) {
      unsigned bn =
          state_.input_schema_right_->getcolumn(state_.filter_key_base_[0])
              .operate->getPartitionValue(
                  state_.input_schema_right_->getColumnAddess(
                      state_.filter_key_base_[0], tuple_from_right_child),
                  state_.hashtable_bucket_num_);
      hashtable_->placeIterator(dftc->hashtable_iterator_, bn);
    }
  }
  RETURN_IF_CANCELLED(exec_status);

  return Next(exec_status, block);
}

bool PhysicalDeleteFilter::Close() {
#ifdef TIME
  stopTimer(&timer);
  printf("time consuming: %lld, %f\n", timer,
         timer / reinterpret_cast<double>(CPU_FRE));
#endif
  LOG(INFO) << "Consumes %ld tuples from left child!" << endl;
  InitExpandedStatus();
  DestoryAllContext();
  if (NULL != hashtable_) {
    delete hashtable_;
    hashtable_ = NULL;
  }
  state_.child_left_->Close();
  state_.child_right_->Close();
  return true;
}

inline static void PhysicalDeleteFilter::isMatch(
    void* l_tuple_addr, void* r_tuple_addr, void* return_addr,
    vector<unsigned>& l_join_index, vector<unsigned>& r_join_index,
    Schema* l_schema, Schema* r_schema, ExprFuncTwoTuples func) {
  bool key_exist = true;
  for (unsigned i = 0; i < r_join_index.size(); i++) {
    void* key_in_input =
        r_schema->getColumnAddess(r_join_index[i], r_tuple_addr);
    void* key_in_hashtable =
        l_schema->getColumnAddess(l_join_index[i], l_tuple_addr);
    if (!r_schema->getcolumn(r_join_index[i])
             .operate->equal(key_in_input, key_in_hashtable)) {
      key_exist = false;
      break;
    }
  }
  *(bool*)return_addr = key_exist;
}

inline static void PhysicalDeleteFilter::isMatchCodegen(
    void* l_tuple_addr, void* r_tuple_addr, void* return_addr,
    vector<unsigned>& l_join_index, vector<unsigned>& r_join_index,
    Schema* l_schema, Schema* r_schema, ExprFuncTwoTuples func) {
  func(l_tuple_addr, r_tuple_addr, return_addr);
}

void PhysicalDeleteFilter::Print() {
  printf("Delete Filter: buckets:%d\n", state_.hashtable_bucket_num_);
  LOG(INFO) << "Delete Filter: buckets:%d\n" << state_.hashtable_bucket_num_
            << endl;
  printf("------Delete Filter Left-------\n");
  LOG(INFO) << "------Delete Filter Left-------" << endl;
  state_.child_left_->Print();
  printf("------Delete Filter Right-------\n");
  LOG(INFO) << "------Delete Filter Right-------" << endl;
  state_.child_right_->Print();
}

PhysicalDeleteFilter::DeleteFilterThreadContext::~DeleteFilterThreadContext() {
  delete l_block_for_asking_;
  delete l_block_stream_iterator_;
  delete r_block_for_asking_;
  delete r_block_stream_iterator_;
}

ThreadContext* PhysicalDeleteFilter::CreateContext() {
  DeleteFilterThreadContext* dftc = new DeleteFilterThreadContext();
  if (NULL == dftc) {
    LOG(ERROR) << "Memory allocation failed for Delete filter Thread context"
               << std::endl;
    return NULL;
  }

  dftc->l_block_for_asking_ = BlockStreamBase::createBlock(
      state_.input_schema_left_, state_.block_size_);
  if (NULL == dftc->l_block_for_asking_) {
    LOG(ERROR) << "Memory allocation failed when creating the buffer to store "
                  "intermediate results." << std::endl;
    return NULL;
  }
  dftc->l_block_stream_iterator_ = dftc->l_block_for_asking_->createIterator();
  dftc->r_block_for_asking_ = BlockStreamBase::createBlock(
      state_.input_schema_right_, state_.block_size_);
  if (NULL == dftc->r_block_for_asking_) {
    LOG(ERROR) << "Memory allocation failed when creating the buffer to store "
                  "intermediate results." << std::endl;
    return NULL;
  }
  dftc->r_block_stream_iterator_ = dftc->r_block_for_asking_->createIterator();
  return dftc;
}
RetCode PhysicalDeleteFilter::GetAllSegments(stack<Segment*>* all_segments) {
  RetCode ret = rSuccess;
  if (NULL != state_.child_right_) {
    ret = state_.child_right_->GetAllSegments(all_segments);
  }
  if (NULL != state_.child_left_) {
    ret = state_.child_left_->GetAllSegments(all_segments);
  }
  return ret;
}
} /* namespace physical_operator */
} /* namespace claims */
