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
 * /CLAIMS/physical_query_plan/physical_project.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: casa, Hanzhang
 *		   Email: zhangleicasa@gmail.com
 *
 * Description: Implementation of Project operator in physical layer.
 *
 */

#include "../physical_query_plan/physical_project.h"

// namespace claims{
// namespace physical_query_plan{
PhysicalProject::PhysicalProject() { InitExpandedStatus(); }

PhysicalProject::~PhysicalProject() {}

PhysicalProject::PhysicalProject(State state) : state_(state) {
  InitExpandedStatus();
}

PhysicalProject::State::State(Schema* input, Schema* output,
                              BlockStreamIteratorBase* children,
                              unsigned blocksize, Mapping map,
                              vector<QNode*> exprTree)
    : input_(input),
      output_(output),
      child_(children),
      block_size_(blocksize),
      map_(map),
      exprTree_(exprTree) {}
PhysicalProject::State::State(Schema* input, Schema* output,
                              BlockStreamIteratorBase* children,
                              unsigned blocksize, Mapping map)
    : input_(input),
      output_(output),
      child_(children),
      block_size_(blocksize),
      map_(map) {}
bool PhysicalProject::Open(const PartitionOffset& kPartitionOffset) {
  ProjectThreadContext* ptc = reinterpret_cast<ProjectThreadContext*>(
      CreateOrReuseContext(crm_core_sensitive));

  bool ret = state_.child_->Open(kPartitionOffset);
  SetReturnStatus(ret);
  BarrierArrive();
  return GetReturnStatus();
}

/**
 * @brief Method description: now the expressions computing speed is slow
 * because of the copy between among the expression.
 */

// TODO(casa): seek the pointer of data and LLVM will be solved by wangli.
bool PhysicalProject::Next(BlockStreamBase* block) {
  unsigned total_length_ = state_.output_->getTupleMaxSize();

  void* tuple_from_child;
  void* tuple_in_block;
  ProjectThreadContext* tc =
      reinterpret_cast<ProjectThreadContext*>(GetContext());
  while (true) {
    if (tc->block_stream_iterator_->currentTuple() == 0) {
      /* mark the block as processed by setting it empty*/
      tc->block_for_asking_->setEmpty();
      if (state_.child_->Next(tc->block_for_asking_)) {
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
    /**
     * @brief Method description: There are totally two reasons for the end of
     * the while loop.
     * (1) block is full of tuples satisfying filter (should return true to the
     * caller)
     * (2) block_for_asking_ is exhausted (should fetch a new block from child
     * and continue to process)
     */

    if (block->Full())
      // for case (1)
      return true;
  }
}

bool PhysicalProject::Close() {
  InitExpandedStatus();
  DestoryAllContext();
  return state_.child_->Close();
}

bool PhysicalProject::CopyNewValue(void* tuple, void* result, int length) {
  memcpy(tuple, result, length);
}
bool PhysicalProject::CopyColumn(void*& tuple, ExpressionItem& result,
                                 int length) {
  switch (result.return_type) {
    case t_int: {
      memcpy(tuple, &result.content.data.value._int, length);
      break;
    }
    case t_float: {
      memcpy(tuple, &result.content.data.value._float, length);
      break;
    }
    case t_u_long: {
      memcpy(tuple, &result.content.data.value._ulong, length);
      break;
    }
    case t_string: {
      memcpy(tuple, result._string.c_str(), length);
      break;
    }
    case t_decimal: {
      memcpy(tuple, &result._decimal, length);
      break;
    }
    case t_date: {
      memcpy(tuple, &result._date, length);
      break;
    }
    case t_time: {
      memcpy(tuple, &result._time, length);
      break;
    }
    case t_datetime: {
      memcpy(tuple, &result._datetime, length);
      break;
    }
    case t_double: {
      memcpy(tuple, &result.content.data.value._double, length);
      break;
    }
    case t_smallInt: {
      memcpy(tuple, &result.content.data.value._sint, length);
      break;
    }
    default: {
      std::cout << "missing the operator!!!" << endl;
      break;
    }
  }
}
void PhysicalProject::Print() {
  std::cout << "proj:" << endl;
  for (int i = 0; i < state_.exprTree_.size(); i++) {
    printf("  %s\n", state_.exprTree_[i]->alias.c_str());
  }
  state_.child_->Print();
}

/**
 * @brief Method description: By traversing block_steam_iterator, Copy
 * expression to thread and add the attribute in new tuples.
 * @param BlockStreamBase*, project_thread_context*
 * @details   (additional) The actual implementation of operations.
 */
void PhysicalProject::ProcessInLogic(BlockStreamBase* block,
                                     ProjectThreadContext* tc) {
  unsigned total_length = state_.output_->getTupleMaxSize();
  void* tuple_from_child;
  void* tuple;
  while ((tuple_from_child = tc->block_stream_iterator_->currentTuple()) > 0) {
    if ((tuple = block->allocateTuple(total_length)) > 0) {
      for (int i = 0; i < tc->thread_qual_.size(); i++) {
        void* result = tc->thread_qual_[i]->FuncId(
            tc->thread_qual_[i], tuple_from_child, state_.input_);
        CopyNewValue(tuple, result, state_.output_->getcolumn(i).get_length());
        tuple = reinterpret_cast<char*>(tuple) +
                state_.output_->getcolumn(i).get_length();
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
 * @brief Method description: Initialize project thread context with
 * state(Class)
 * @return a pointer(project_thread_context)
 */
ThreadContext* PhysicalProject::CreateContext() {
  ProjectThreadContext* ptc = new ProjectThreadContext();
  ptc->block_for_asking_ =
      BlockStreamBase::createBlock(state_.input_, state_.block_size_);
  ptc->temp_block_ =
      BlockStreamBase::createBlock(state_.output_, state_.block_size_);
  ptc->block_stream_iterator_ = ptc->block_for_asking_->createIterator();
  ptc->thread_qual_ = state_.exprTree_;
  for (int i = 0; i < state_.exprTree_.size(); i++) {
    Expr_copy(state_.exprTree_[i], ptc->thread_qual_[i]);
    InitExprAtPhysicalPlan(ptc->thread_qual_[i]);
  }
  return ptc;
}

// } // namespace physical_query_plan
// } // namespace claims
