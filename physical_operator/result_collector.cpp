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
 * ./physical_operator/result_collector.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 *       Email: wangli1426@gmail.com
 */
#include "../physical_operator/result_collector.h"

#include <string>
#include <vector>
#include <iostream>  // NOLINT
#include "../utility/rdtsc.h"
using std::vector;
using std::string;
using std::cout;
using std::endl;

namespace claims {
namespace physical_operator {
ResultCollector::ResultCollector()
    : finished_thread_count_(0), registered_thread_count_(0) {
  sema_open_.set_value(1);
  sema_open_finished_.set_value(0);
  sema_input_complete_.set_value(0);
}
ResultCollector::ResultCollector(State state)
    : finished_thread_count_(0), registered_thread_count_(0), state_(state) {
  sema_open_.set_value(1);
  sema_open_finished_.set_value(0);
  sema_input_complete_.set_value(0);
}
ResultCollector::~ResultCollector() {
  if (NULL != state_.input_) {
    delete state_.input_;
    state_.input_ = NULL;
  }
  if (NULL != state_.child_) {
    delete state_.child_;
    state_.child_ = NULL;
  }
}
ResultCollector::State::State()
    : input_(NULL), child_(NULL), block_size_(0), partition_offset_(0) {}

ResultCollector::State::State(Schema* input, PhysicalOperatorBase* child,
                              const unsigned block_size,
                              vector<string> column_header,
                              const PartitionOffset partitoin_offset)
    : input_(input),
      child_(child),
      block_size_(block_size),
      partition_offset_(partitoin_offset),
      column_header_(column_header) {}

bool ResultCollector::Open(const PartitionOffset& part_offset) {
  state_.partition_offset_ = part_offset;

  if (sema_open_.try_wait()) {
    block_buffer_ = new ResultSet();
    block_buffer_iterator_ = block_buffer_->createIterator();
    open_finished_ = true;
  } else {
    while (!open_finished_) {
      usleep(1);
    }
  }
  registered_thread_count_++;
  if (true == g_thread_pool_used) {
    Environment::getInstance()->getThreadPool()->add_task(CollectResult, this);
  } else {
    pthread_t tid;
    pthread_create(&tid, NULL, CollectResult, this);
  }
  unsigned long long int start = curtick();
  sema_input_complete_.wait();
  block_buffer_->query_time_ = getSecond(start);
  return true;
}

bool ResultCollector::Next(BlockStreamBase* block) { return false; }

bool ResultCollector::Close() {
  state_.child_->Close();
  sema_input_complete_.set_value(0);
  return true;
}
void ResultCollector::Print() {
  cout << "------------" << endl;
  cout << "ResultCollector" << endl;
  cout << "------------" << endl;
  state_.child_->Print();
}
ResultSet* ResultCollector::GetResultSet() {
  while (!IsChildExhausted()) {
    usleep(1);
  }
  return block_buffer_;
}

bool ResultCollector::CreateBlockStream(BlockStreamBase*& target) const {
  /**
   * TODO(yuyang): the block allocation should apply for the memory budget from
   * the buffer manager first.
   */
  target = BlockStreamBase::createBlock(state_.input_, state_.block_size_);
  return target != NULL;
}

void ResultCollector::DeallocateBlockStream(BlockStreamBase*& target) const {
  /**
   * TODO(yuyang): return the allocated memory to the buffer manager, but this
   * is not used for now.
   */
  target->~BlockStreamBase();
}

void* ResultCollector::CollectResult(void* arg) {
  ResultCollector* Pthis = (ResultCollector*)arg;
  Pthis->state_.child_->Open(Pthis->state_.partition_offset_);
  BlockStreamBase* block_for_asking;
  if (false == Pthis->CreateBlockStream(block_for_asking)) {
    assert(false);
    return 0;
  }
  Pthis->block_buffer_->column_header_list_ = Pthis->state_.column_header_;

  unsigned long long start = 0;
  start = curtick();

  while (Pthis->state_.child_->Next(block_for_asking)) {
    Pthis->block_buffer_->atomicAppendNewBlock(block_for_asking);
    if (false == Pthis->CreateBlockStream(block_for_asking)) {
      assert(false);
      return 0;
    }
  }
  Pthis->sema_input_complete_.post();
  double eclipsed_seconds = getSecond(start);
  Pthis->block_buffer_->query_time_ = eclipsed_seconds;
  Pthis->block_buffer_->schema_ = Pthis->state_.input_->duplicateSchema();
  Pthis->finished_thread_count_++;

  return 0;
}

bool ResultCollector::IsChildExhausted() {
  return finished_thread_count_ == registered_thread_count_;
}

unsigned long ResultCollector::GetNumberOftuples() const {
  unsigned long ret = 0;
  ResultSet::Iterator block_buffer_iterator = block_buffer_->createIterator();
  BlockStreamBase* block;
  while (block = (BlockStreamBase*)block_buffer_iterator.nextBlock()) {
    ret += block->getTuplesInBlock();
  }
  return ret;
}

ResultCollector::State::~State() {
  //  delete input_;
  //  delete child_;
}
}  //  namespace physical_operator
}  //  namespace claims
