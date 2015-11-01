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
 * physical_operator/expander.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <set>
#include <iostream>
#include "../common/Logging.h"
#include "../physical_operator/expander.h"
#include "../Executor/expander_tracker.h"
namespace claims {
namespace physical_operator {
struct ExpanderContext {
  Expander* pthis_;
  semaphore sem_;
};

Expander::Expander(State state)
    : state_(state),
      block_stream_buffer_(0),
      finished_thread_count_(0),
      thread_count_(0),
      coordinate_pid_(0) {}

Expander::Expander()
    : block_stream_buffer_(0),
      finished_thread_count_(0),
      thread_count_(0),
      coordinate_pid_(0) {}

Expander::~Expander() {
  if (NULL != state_.child_) {
    delete state_.child_;
    state_.child_ = NULL;
  }
  if (NULL != state_.schema_) {
    delete state_.schema_;
    state_.schema_ = NULL;
  }
}

Expander::State::State(Schema* schema, PhysicalOperatorBase* child,
                       unsigned thread_count, unsigned block_size,
                       unsigned block_count_in_buffer)
    : schema_(schema),
      child_(child),
      init_thread_count_(thread_count),
      block_size_(block_size),
      block_count_in_buffer_(block_count_in_buffer) {}
/**
 * @param partitoin_offset means to solve corresponding partition
 * every Expander should register to ExpanderTracker
 */
bool Expander::Open(const PartitionOffset& partitoin_offset) {
  received_tuples_ = 0;
  state_.partition_offset_ = partitoin_offset;
  input_data_complete_ = false;
  one_thread_finished_ = false;
  finished_thread_count_ = 0;
  block_stream_buffer_ = new BlockStreamBuffer(
      state_.block_size_, state_.block_count_in_buffer_ * 10, state_.schema_);

  in_work_expanded_thread_list_.clear();
  expander_id_ = ExpanderTracker::getInstance()->registerNewExpander(
      block_stream_buffer_, this);
  LOG(INFO) << expander_id_
            << "Expander open, thread count= " << state_.init_thread_count_
            << std::endl;

  for (unsigned i = 0; i < state_.init_thread_count_; i++) {
    if (CreateWorkingThread() == false) {
      LOG(INFO) << "expander_id_ = " << expander_id_
                << " Failed to create initial expanded thread*" << std::endl;
      return false;
    }
  }
  return true;
}
/**
 * fetch one block from buffer and return, until it is exhausted.
 */
bool Expander::Next(BlockStreamBase* block) {
  while (!block_stream_buffer_->getBlock(*block)) {
    if (ChildExhausted()) {
      return false;
    } else {
      usleep(1);
    }
  }
  return true;
}

bool Expander::Close() {
  LOG(INFO) << "Expander: " << expander_id_ << " received "
            << block_stream_buffer_->getReceivedDataSizeInKbytes() << " kByte "
            << received_tuples_ << " tuples!" << std::endl;
  ExpanderTracker::getInstance()->unregisterExpander(expander_id_);
  if (true == g_thread_pool_used) {
    // do nothing
  } else {
    for (std::set<pthread_t>::iterator it =
             in_work_expanded_thread_list_.begin();
         it != in_work_expanded_thread_list_.end(); it++) {
      void* res;
      pthread_join(*it, &res);
      assert(res == 0);
      LOG(WARNING) << expander_id_
                   << " A expander thread is killed before close!" << std::endl;
    }
  }

  assert(input_data_complete_);
  input_data_complete_ = false;
  one_thread_finished_ = false;
  assert(in_work_expanded_thread_list_.empty());
  assert(being_called_bacl_expanded_thread_list_.empty());
  finished_thread_count_ = 0;

  /*
   * check if all the information in ExpanderTrack has properly removed
   */
  assert(!ExpanderTracker::getInstance()->trackExpander(expander_id_));
  if (NULL != block_stream_buffer_) {
    delete block_stream_buffer_;
    block_stream_buffer_ = NULL;
  }
  LOG(INFO) << expander_id_ << " Buffer is freed in Expander!" << std::endl;
  state_.child_->Close();
  thread_count_ = 0;
  LOG(INFO) << expander_id_ << "<<<<<<<Expander closed!>>>>>>>>>>" << std::endl;
  return true;
}
void Expander::Print() {
  printf("Expander: thread num:%d\n", state_.init_thread_count_);
  state_.child_->Print();
}
/**
 * one expander may have many threads, so ExpanderTracker should track each
 * thread's status. call open() and next() of child to start the woke flow,
 * because the close() should called once, so can ignore to call it here. If the
 * work flow is exhausted, the delete some context and return
 */
void* Expander::ExpandedWork(void* arg) {
  Expander* Pthis = (reinterpret_cast<ExpanderContext*>(arg))->pthis_;
  const pthread_t pid = pthread_self();
  LOG(INFO) << Pthis->expander_id_ << " thread " << pid
            << " is created!  BlockStreamExpander address is  " << Pthis
            << std::endl;

  bool expanding = true;
  ticks start = curtick();

  Pthis->AddIntoWorkingThreadList(pid);
  ExpanderTracker::getInstance()->registerNewExpandedThreadStatus(
      pid, Pthis->expander_id_);
  unsigned block_count = 0;
  (reinterpret_cast<ExpanderContext*>(arg))->sem_.post();

  if (Pthis->ChildExhausted()) {
    ExpanderTracker::getInstance()->deleteExpandedThreadStatus(pthread_self());
    return 0;
  }

  LOG(INFO) << Pthis->expander_id_ << ", pid= " << pid
            << " begins to open child!" << std::endl;
  ticks start_open = curtick();

  Pthis->state_.child_->Open(Pthis->state_.partition_offset_);

  LOG(INFO) << Pthis->expander_id_ << ", pid= " << pid
            << " finished opening child" << std::endl;

  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(pid)) {
    LOG(INFO) << Pthis->expander_id_ << " <<<<<<<<<<<<<<<<Expander detected "
                                        "call back signal after open!>>>>>>>"
              << pthread_self() << std::endl;

    Pthis->RemoveFromCalledBackThreadList(pid);
    Pthis->tid_to_shrink_semaphore_[pid]->post();
  } else {
    if (expanding == true) {
      expanding = false;
    }
    BlockStreamBase* block_for_asking = BlockStreamBase::createBlock(
        Pthis->state_.schema_, Pthis->state_.block_size_);
    block_for_asking->setEmpty();

    while (Pthis->state_.child_->Next(block_for_asking)) {
      if (!block_for_asking->Empty()) {
        Pthis->lock_.acquire();
        Pthis->received_tuples_ += block_for_asking->getTuplesInBlock();
        Pthis->lock_.release();
        Pthis->block_stream_buffer_->insertBlock(block_for_asking);
        block_for_asking->setEmpty();
        block_count++;
      }
    }
    /*
     * When the above loop exits, it means that either the stage beginner has
     * exhausted, or it received termination request.
     */
    if (NULL != block_for_asking) {
      delete block_for_asking;
      block_for_asking = NULL;
    }

    if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
            pthread_self())) {
      LOG(INFO) << Pthis->expander_id_ << " <<<<<<<<<<<<<<<<Expander detected "
                                          "call back signal after open!>>>>>>>"
                << pthread_self() << std::endl;
      Pthis->lock_.acquire();
      Pthis->input_data_complete_ = false;
      Pthis->lock_.release();
      Pthis->RemoveFromCalledBackThreadList(pthread_self());
      LOG(INFO) << pthread_self() << " Produced " << block_count
                << " block before called-back" << std::endl;
      Pthis->tid_to_shrink_semaphore_[pid]->post();
    } else {
      LOG(INFO) << pthread_self() << " Produced " << block_count
                << " block before finished" << std::endl;

      Pthis->lock_.acquire();
      Pthis->finished_thread_count_++;
      Pthis->input_data_complete_ = true;

      /**
       * The finish of one expanded thread does not always mean the complete of
       *input data.
       *
       */
      Pthis->block_stream_buffer_->setInputComplete();
      LOG(INFO) << pthread_self() << " Produced " << block_count << "blocks"
                << std::endl;
      Pthis->lock_.release();

      if (!Pthis->RemoveFromWorkingThreadList(pthread_self())) {
        /* current thread has been called back*/
        Pthis->RemoveFromCalledBackThreadList(pthread_self());
        Pthis->tid_to_shrink_semaphore_[pid]->post();
      }
    }
  }

  /* delete its stauts from expander tracker before exit*/
  ExpanderTracker::getInstance()->deleteExpandedThreadStatus(pthread_self());
  LOG(INFO) << Pthis->expander_id_ << ", pid= " << pid
            << " expande thread finished!" << std::endl;
  return 0;
}

/**
 * first acquire the exclusive lock to prevent creating expanded thread,
 * Otherwise, newly created thread may not be detected by ChildExhausted().
 * but what's coordinate_pid_?(fzh)
 */
bool Expander::ChildExhausted() {
  exclusive_expanding_.acquire();
  lock_.acquire();
  bool ret = input_data_complete_ == true &&
             in_work_expanded_thread_list_.empty() &&
             being_called_bacl_expanded_thread_list_.empty() &&
             this->block_stream_buffer_->Empty();
  lock_.release();
  exclusive_expanding_.release();
  if (ret == true && coordinate_pid_ != 0) {
    void* res;
    pthread_join(coordinate_pid_, &res);
    coordinate_pid_ = 0;
    return ChildExhausted();
  }
  if (ret) {
    LOG(INFO) << expander_id_ << " child iterator is exhausted!" << std::endl;
  }
  return ret;
}
/**
 * use thread-pool or pthread_create to create new working thread, the
 * thread-pool way can avoid the cost to create and destroy one new thread
 */
bool Expander::CreateWorkingThread() {
  pthread_t tid = 0;

  ExpanderContext para;
  para.pthis_ = this;
  ticks start = curtick();
  if (exclusive_expanding_.try_acquire()) {
    if (true == g_thread_pool_used) {
      Environment::getInstance()->getThreadPool()->add_task(ExpandedWork,
                                                            &para);
    } else {
      const int error = pthread_create(&tid, NULL, ExpandedWork, &para);
      if (error != 0) {
        LOG(ERROR) << "cannot create thread!!!!!!!!!!!!!!!" << std::endl;
        return false;
      }
    }
    para.sem_.wait();
    exclusive_expanding_.release();
    if (true == g_thread_pool_used) {
    } else {
      LOG(INFO) << " expande_id = " << expander_id_ << " New expanded thread "
                << tid << " created!" << std::endl;
    }

    lock_.acquire();
    thread_count_++;
    lock_.release();
    return true;
  } else {
    LOG(ERROR) << "Fails to obtain the exclusive lock to expanding!"
               << std::endl;
    return false;
  }
}
/**
 * in order to guarantee terminating one working thread fast and correctly, just
 * set the status of corresponding call_backed, then try wait(), waiting
 * somewhere at some PhysicalOperator can exit safely and set post()
 */
void Expander::TerminateWorkingThread(const pthread_t pid) {
  if (!ExpanderTracker::getInstance()->isExpandedThreadCallBack(pid)) {
    semaphore sem;
    tid_to_shrink_semaphore_[pid] = &sem;
    RemoveFromWorkingThreadList(pid);

    AddIntoCalledBackThreadList(pid);
    ExpanderTracker::getInstance()->callbackExpandedThread(pid);
    tid_to_shrink_semaphore_[pid]->wait();  // note waiting post() somewhere
    lock_.acquire();
    tid_to_shrink_semaphore_.erase(pid);
    lock_.release();

    lock_.acquire();
    thread_count_--;
    lock_.release();
    LOG(INFO) << expander_id_
              << " A thread is called back !******** working_thread_count= "
              << this->in_work_expanded_thread_list_.size()
              << " being_called_back_thread_count: "
              << this->being_called_bacl_expanded_thread_list_.size()
              << std::endl;
  } else {
    LOG(INFO) << expander_id_ << " This thread has already been called back!"
              << std::endl;
  }
}
void Expander::AddIntoWorkingThreadList(pthread_t pid) {
  lock_.acquire();
  in_work_expanded_thread_list_.insert(pid);
  LOG(INFO) << expander_id_ << " pid = " << pid
            << " is added into in working list, whose address is "
            << &in_work_expanded_thread_list_ << std::endl;
  assert(in_work_expanded_thread_list_.find(pid) !=
         in_work_expanded_thread_list_.end());
  lock_.release();
}
bool Expander::RemoveFromWorkingThreadList(pthread_t pid) {
  lock_.acquire();
  if (in_work_expanded_thread_list_.find(pid) !=
      in_work_expanded_thread_list_.end()) {
    in_work_expanded_thread_list_.erase(pid);
    LOG(INFO) << expander_id_ << " pid = " << pid
              << " is removed from in working list!" << std::endl;
    lock_.release();
    return true;
  } else {
    LOG(INFO) << expander_id_ << " pid = " << pid
              << " has already been removed from in working list!" << std::endl;
    lock_.release();
    return false;
  }
}
void Expander::AddIntoCalledBackThreadList(pthread_t pid) {
  lock_.acquire();
  being_called_bacl_expanded_thread_list_.insert(pid);
  LOG(INFO) << expander_id_ << " pid = " << pid
            << " is added into being called back list!" << std::endl;
  lock_.release();
}
// there should add some code to check that the pid exist in the list?
void Expander::RemoveFromCalledBackThreadList(pthread_t pid) {
  lock_.acquire();
  being_called_bacl_expanded_thread_list_.erase(pid);
  LOG(INFO) << expander_id_ << " pid = " << pid
            << " is removed from being called back list!" << std::endl;
  lock_.release();
}
unsigned Expander::GetDegreeOfParallelism() {
  unsigned ret;
  lock_.acquire();
  ret = in_work_expanded_thread_list_.size();
  lock_.release();
  return ret;
}
bool Expander::Expand() {
  if (input_data_complete_) {
    /*
     * Expander does not expand when at least one expanded thread has completely
     * processed the input data flow. Otherwise the newly created expanded
     * thread might not be able to work properly if the expander's close is
     * called before its creation.
     */
    return false;
  }
  return CreateWorkingThread();
}

bool Expander::Shrink() {
  ticks start = curtick();
  lock_.acquire();
  if (in_work_expanded_thread_list_.empty()) {
    lock_.release();
    return false;
  } else {
    pthread_t cencel_thread_id = *in_work_expanded_thread_list_.begin();
    lock_.release();
    this->TerminateWorkingThread(cencel_thread_id);

    return true;
  }
}
}  // namespace physical_operator
}  // namespace claims
