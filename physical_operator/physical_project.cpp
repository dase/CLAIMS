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
 * /CLAIMS/physical_operator/physical_project.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: casa, Hanzhang
 *		   Email: zhangleicasa@gmail.com
 *
 * Description: Implementation of Project operator in physical layer.
 *
 */

#include "../physical_operator/physical_project.h"

#include <stack>
#include <vector>
using claims::common::ExprNode;

#include "../common/expression/expr_node.h"
namespace claims {
namespace physical_operator {
PhysicalProject::PhysicalProject() {
  set_phy_oper_type(kPhysicalProject);
  InitExpandedStatus();
}

PhysicalProject::~PhysicalProject() {}

PhysicalProject::PhysicalProject(State state) : state_(state) {
  set_phy_oper_type(kPhysicalProject);
  InitExpandedStatus();
}

PhysicalProject::State::State(Schema* schema_input, Schema* schema_output,
                              PhysicalOperatorBase* children,
                              unsigned block_size, vector<QNode*> expr_tree)
    : schema_input_(schema_input),
      schema_output_(schema_output),
      child_(children),
      block_size_(block_size),
      expr_tree_(expr_tree) {}

PhysicalProject::State::State(Schema* schema_input, Schema* schema_output,
                              PhysicalOperatorBase* children,
                              unsigned block_size, vector<ExprNode*> expr_list)
    : schema_input_(schema_input),
      schema_output_(schema_output),
      child_(children),
      block_size_(block_size),
      expr_list_(expr_list) {}

/**
 * Generate ProjectThreadContext and initialize the expression of this operator.
 * Call back child Open().
 */

bool PhysicalProject::Open(SegmentExecStatus* const exec_status,
                           const PartitionOffset& kPartitionOffset) {
  RETURN_IF_CANCELLED(exec_status);

  RegisterExpandedThreadToAllBarriers();
  ProjectThreadContext* ptc = reinterpret_cast<ProjectThreadContext*>(
      CreateOrReuseContext(crm_core_sensitive));
  bool ret = state_.child_->Open(exec_status, kPartitionOffset);
  SetReturnStatus(ret);
  BarrierArrive();  //  Synchronization point
  return GetReturnStatus();
}

/**
 * There are totally two reasons for the end of the while loop.
 * case(1): block is full of tuples satisfying filter (should return true to
 * the caller)
 * case(2): block_for_asking_ is exhausted (should fetch a new block from
 * child and continue to process)
 */
bool PhysicalProject::Next(SegmentExecStatus* const exec_status,
                           BlockStreamBase* block) {
  unsigned total_length_ = state_.schema_output_->getTupleMaxSize();

  void* tuple_from_child;
  void* tuple_in_block;
  ProjectThreadContext* tc =
      reinterpret_cast<ProjectThreadContext*>(GetContext());
  while (true) {
    RETURN_IF_CANCELLED(exec_status);

    if (tc->block_stream_iterator_->currentTuple() == 0) {
      /* mark the block as processed by setting it empty*/
      tc->block_for_asking_->setEmpty();
      if (state_.child_->Next(exec_status, tc->block_for_asking_)) {
        delete tc->block_stream_iterator_;
        tc->block_stream_iterator_ = tc->block_for_asking_->createIterator();
      } else {
        if (!block->Empty()) {
          return true;
        } else {
          return false;
        }
      }
    }
    ProcessInLogic(block, tc);
    if (block->Full())
      // for case (1)
      return true;
  }
  return false;
}

bool PhysicalProject::Close() {
  InitExpandedStatus();
  DestoryAllContext();
  return state_.child_->Close();
}

bool PhysicalProject::CopyNewValue(void* tuple, void* result, int length) {
  memcpy(tuple, result, length);
}

void PhysicalProject::Print() {
  std::cout << "proj:" << endl;
#ifdef NEWCONDI
  for (int i = 0; i < state_.expr_tree_.size(); i++) {
    printf("  %s\n", state_.expr_tree_[i]->alias.c_str());
  }
#else
  for (int i = 0; i < state_.expr_list_.size(); ++i) {
    cout << "    " << state_.expr_list_[i]->alias_ << endl;
  }
#endif
  state_.child_->Print();
}

/**
 * By traversing block_steam_iterator, generate a expression by project function
 * and add some columns in a new tuple.
 */
void PhysicalProject::ProcessInLogic(BlockStreamBase* block,
                                     ProjectThreadContext* tc) {
  unsigned total_length = state_.schema_output_->getTupleMaxSize();
  void* tuple_from_child;
  void* tuple;
  while (NULL !=
         (tuple_from_child = tc->block_stream_iterator_->currentTuple())) {
    if (NULL != (tuple = block->allocateTuple(total_length))) {
#ifdef NEWCONDI
      for (int i = 0; i < tc->thread_qual_.size(); i++) {
        void* result = tc->thread_qual_[i]->FuncId(
            tc->thread_qual_[i], tuple_from_child, state_.schema_input_);
#else
      for (int i = 0; i < tc->thread_expr_.size(); ++i) {
        void* result = tc->thread_expr_[i]->ExprEvaluate(tuple_from_child,
                                                         state_.schema_input_);

#endif
        CopyNewValue(tuple, result,
                     state_.schema_output_->getcolumn(i).get_length());
        tuple = reinterpret_cast<char*>(tuple) +
                state_.schema_output_->getcolumn(i).get_length();
      }
      tc->block_stream_iterator_->increase_cur_();
    } else {
      return;
    }
  }
  /* mark the block as processed by setting it empty*/
  tc->block_for_asking_->setEmpty();
}

/**
 * Copy exprtree to ProjectThreadContext and initialize the exprtree at physical
 * plan
 */
ThreadContext* PhysicalProject::CreateContext() {
  ProjectThreadContext* ptc = new ProjectThreadContext();
  ptc->block_for_asking_ =
      BlockStreamBase::createBlock(state_.schema_input_, state_.block_size_);
  ptc->temp_block_ =
      BlockStreamBase::createBlock(state_.schema_output_, state_.block_size_);
  ptc->block_stream_iterator_ = ptc->block_for_asking_->createIterator();
#ifdef NEWCONDI
  ptc->thread_qual_ = state_.expr_tree_;
  for (int i = 0; i < state_.expr_tree_.size(); i++) {
    Expr_copy(state_.expr_tree_[i], ptc->thread_qual_[i]);
    InitExprAtPhysicalPlan(ptc->thread_qual_[i]);
  }
#else
  ptc->thread_expr_ = state_.expr_list_;
  for (int i = 0; i < state_.expr_list_.size(); ++i) {
    ptc->thread_expr_[i] = state_.expr_list_[i]->ExprCopy();
    ptc->thread_expr_[i]->InitExprAtPhysicalPlan();
  }
#endif
  return ptc;
}
RetCode PhysicalProject::GetAllSegments(stack<Segment*>* all_segments) {
  RetCode ret = rSuccess;
  if (NULL != state_.child_) {
    ret = state_.child_->GetAllSegments(all_segments);
  }
  return ret;
}
}  // namespace physical_operator
}  // namespace claims
