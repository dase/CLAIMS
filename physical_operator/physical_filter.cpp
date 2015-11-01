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
 * /CLAIMS/physical_operator/physical_filter.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: wangli, Hanzhang
 *		   Email: wangli1426@gmail.com
 *
 * Description: Implementation of Filter operator in physical layer.
 *
 */

#include "../physical_operator/physical_filter.h"

#include <assert.h>
#include <limits>
#include "../utility/warmup.h"
#include "../utility/rdtsc.h"
#include "../common/ExpressionCalculator.h"
#include "../common/Expression/execfunc.h"
#include "../common/Expression/qnode.h"
#include "../common/Expression/initquery.h"
#include "../common/Expression/queryfunc.h"
#include "../common/data_type.h"
#include "../Config.h"
#include "../Parsetree/sql_node_struct.h"
#include "../codegen/ExpressionGenerator.h"
#include "../common/error_no.h"

using namespace claims::common;

namespace claims {
namespace physical_operator {
#define NEWCONDITION

PhysicalFilter::PhysicalFilter(State state)
    : state_(state),
      generated_filter_function_(NULL),
      generated_filter_processing_fucntoin_(NULL) {
  InitExpandedStatus();
}

PhysicalFilter::PhysicalFilter()
    : generated_filter_function_(NULL),
      generated_filter_processing_fucntoin_(NULL) {
  InitExpandedStatus();
}

PhysicalFilter::~PhysicalFilter() {}
PhysicalFilter::State::State(Schema* schema, PhysicalOperatorBase* child,
                             vector<QNode*> qual, map<string, int> column_id,
                             unsigned block_size)
    : schema_(schema),
      child_(child),
      qual_(qual),
      column_id_(column_id),
      block_size_(block_size) {}
PhysicalFilter::State::State(Schema* schema, PhysicalOperatorBase* child,
                             std::vector<AttributeComparator> comparator_list,
                             unsigned block_size)
    : schema_(schema),
      child_(child),
      comparator_list_(comparator_list),
      block_size_(block_size) {}

/**
 * To choose which optimization way of filter function.(if enable_codegen
 *isn't
 *open, we should execute the function: computerFilter ). traditional model of
 *iterator of physical plan.
 * 1)If a block can be optimized by llvm, we choose
 *generated_filter_processing_function_.
 * 2)If a tuple can be optimized by llvm, we choose
 *computerFilterwithGeneratedCode.
 * 3)If it can't be optimized by llvm , we still choose computerFilter.
 */
bool PhysicalFilter::Open(const PartitionOffset& kPartitiontOffset) {
  // set a Synchronization point.
  RegisterExpandedThreadToAllBarriers();
  FilterThreadContext* ftc = reinterpret_cast<FilterThreadContext*>(
      CreateOrReuseContext(crm_core_sensitive));

  if (TryEntryIntoSerializedSection()) {
    if (Config::enable_codegen) {
      ticks start = curtick();
      generated_filter_processing_fucntoin_ =
          getFilterProcessFunc(state_.qual_[0], state_.schema_);
      if (generated_filter_processing_fucntoin_) {
        LOG(INFO) << "CodeGen (full feature) succeeds!("
                  << getMilliSecond(start) << "ms)" << std::endl;
      } else {
        generated_filter_function_ =
            getExprFunc(state_.qual_[0], state_.schema_);

        if (kSuccess == DecideFilterFunction(generated_filter_function_)) {
          filter_function_ = ComputeFilterWithGeneratedCode;
          LOG(INFO) << "CodeGen (partial feature) succeeds!("
                    << getMilliSecond(start) << "ms)" << std::endl;
        } else {
          filter_function_ = ComputeFilter;
          LOG(ERROR) << "filter:" << kErrorMessage[kCodegenFailed] << std::endl;
        }
      }
    } else {
      filter_function_ = ComputeFilter;
      LOG(INFO) << "CodeGen closed!" << std::endl;
    }
  }
  bool ret = state_.child_->Open(kPartitiontOffset);
  SetReturnStatus(ret);
  BarrierArrive();
  return GetReturnStatus();
}

/**
 * There are totally two reasons for the end of the while loop.
 * (1) block is full of tuples satisfying filter (should return true to the
 * caller)
 * (2) block_for_asking_ is exhausted (should fetch a new block from child
 * and continue to process)
 */
bool PhysicalFilter::Next(BlockStreamBase* block) {
  void* tuple_from_child;
  void* tuple_in_block;
  FilterThreadContext* tc =
      reinterpret_cast<FilterThreadContext*>(GetContext());
  while (true) {
    if (NULL == (tc->block_stream_iterator_->currentTuple())) {
      /* mark the block as processed by setting it empty*/
      tc->block_for_asking_->setEmpty();
      if (state_.child_->Next(tc->block_for_asking_)) {
        delete tc->block_stream_iterator_;
        tc->block_stream_iterator_ = tc->block_for_asking_->createIterator();
      } else {
        if (!block->Empty())
          return true;
        else
          return false;
      }
    }
    ProcessInLogic(block, tc);
    if (block->Full())
      // for case (1)
      return true;
  }
}

/**
 * According to which optimization of generate filter function, execute
 * function
 * with related parameters. Different operator has different implementation in
 * process_logic().
 */
void PhysicalFilter::ProcessInLogic(BlockStreamBase* block,
                                    FilterThreadContext* tc) {
  if (generated_filter_processing_fucntoin_) {
    int b_cur = block->getTuplesInBlock();
    int c_cur = tc->block_stream_iterator_->get_cur();
    const int kBlockTulpeCount = block->getBlockCapacityInTuples();
    const int kContextTupleCount = tc->block_for_asking_->getTuplesInBlock();

    generated_filter_processing_fucntoin_(
        block->getBlock(), &b_cur, kBlockTulpeCount,
        tc->block_for_asking_->getBlock(), &c_cur, kContextTupleCount);
    (reinterpret_cast<BlockStreamFix*>(block))->setTuplesInBlock(b_cur);
    tc->block_stream_iterator_->set_cur(c_cur);
  } else {
    void* tuple_from_child;
    void* tuple_in_block;
    while ((tuple_from_child = tc->block_stream_iterator_->currentTuple()) >
           0) {
      bool pass_filter = true;
#ifdef NEWCONDITION
      filter_function_(pass_filter, tuple_from_child,
                       generated_filter_function_, state_.schema_,
                       tc->thread_qual_);
#else
      pass_filter = true;
      for (unsigned i = 0; i < state_.comparator_list_.size(); i++) {
        if (!state_.comparator_list_[i].filter(state_.schema_->getColumnAddess(
                state_.comparator_list_[i].get_index(), tuple_from_child))) {
          pass_filter = false;
          break;
        }
      }
#endif
      if (pass_filter) {
        const unsigned bytes =
            state_.schema_->getTupleActualSize(tuple_from_child);
        if (NULL != (tuple_in_block = block->allocateTuple(bytes))) {
          block->insert(tuple_in_block, tuple_from_child, bytes);
          tuple_after_filter_++;
        } else {
          /* we have got a block full of result tuples*/
          return;
        }
      }
      /* point the iterator to the next tuple */
      tc->block_stream_iterator_->increase_cur_();
    }
    /* mark the block as processed by setting it empty*/
    tc->block_for_asking_->setEmpty();
  }
}

bool PhysicalFilter::Close() {
  InitExpandedStatus();
  DestoryAllContext();
  state_.child_->Close();
  return true;
}

void PhysicalFilter::Print() {
  printf("filter: \n");
  for (int i = 0; i < state_.qual_.size(); i++) {
    printf("  %s\n", state_.qual_[i]->alias.c_str());
  }
  state_.child_->Print();
}

void PhysicalFilter::ComputeFilter(bool& ret, void* tuple, expr_func func_gen,
                                   Schema* schema,
                                   vector<QNode*> thread_qual_) {
  ret = ExecEvalQual(thread_qual_, tuple, schema);
}

void PhysicalFilter::ComputeFilterWithGeneratedCode(bool& ret, void* tuple,
                                                    expr_func func_gen,
                                                    Schema* schema,
                                                    vector<QNode*> allocator) {
  func_gen(tuple, &ret);
}

// Revoke the source of thread.
PhysicalFilter::FilterThreadContext::~FilterThreadContext() {
  if (NULL != block_for_asking_) {
    delete block_for_asking_;
    block_for_asking_ = NULL;
  }
  if (NULL != temp_block_) {
    delete temp_block_;
    temp_block_ = NULL;
  }
  if (NULL != block_stream_iterator_) {
    delete block_stream_iterator_;
    block_stream_iterator_ = NULL;
  }
  for (int i = 0; i < thread_qual_.size(); i++) {
    if (NULL != thread_qual_[i]) {
      delete thread_qual_[i];
      thread_qual_[i] = NULL;
    }
  }
}

/**
 * Copy expression tree to FilterThreadContext and initialize the expression
 * tree at physical plan.
 */
ThreadContext* PhysicalFilter::CreateContext() {
  FilterThreadContext* ftc = new FilterThreadContext();
  ftc->block_for_asking_ =
      BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
  ftc->temp_block_ =
      BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
  ftc->block_stream_iterator_ = ftc->block_for_asking_->createIterator();
  ftc->thread_qual_ = state_.qual_;
  for (int i = 0; i < state_.qual_.size(); i++) {
    Expr_copy(state_.qual_[i], ftc->thread_qual_[i]);
    InitExprAtPhysicalPlan(ftc->thread_qual_[i]);
  }
  return ftc;
}

/**
 * Because of the result of generate_filter_function, Decide whether return
 * error_no.
 */
int PhysicalFilter::DecideFilterFunction(
    expr_func const& generate_filter_function) {
  if (generate_filter_function) {
    return kSuccess;
  } else {
    return kCodegenFailed;
  }
}

}  // namespace claims
}  // namespace physical_operator
