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
 * /Claims/utility/thread_pool.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./thread_pool.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <vector>
#include "./task.h"
#include "../common/memory_handle.h"
#include "../utility/thread_pool.h"

#define __USE_GNU  // 启用CPU_ZERO等相关的宏

ThreadPool::ThreadPool() {}

ThreadPool::~ThreadPool() {
  if (thread_list_ != NULL) Destroy(this);
}

bool ThreadPool::Init(int thread_count_in_pool) {
  is_destroy_ = false;
  current_thread_count_ = 0;
  base_thread_count_ = thread_count_in_pool;
  pthread_mutex_init(&task_queue_lock_, NULL);
  sem_init(&undo_task_sem_, 0, 0);  // init semaphore
  thread_list_ = static_cast<pthread_t *>(
      malloc(2 * base_thread_count_ * sizeof(pthread_t)));
  while (!task_queue_.empty()) {
    DELETE_PTR(task_queue_.back());
    task_queue_.pop();
  }

  if (pthread_create(&monitor_thread_, NULL, MonitorThreadExec, this) !=
      0) {  // if any failed, return false
    ThreadPoolLogging::elog("ERROR: create monitor thread failed!");
    return false;
  }
  for (int i = 0; i < base_thread_count_; ++i) {
    if (pthread_create(&thread_list_[i], NULL, ThreadExec, this) !=
        0) {  // if any failed, return false
      ThreadPoolLogging::elog("ERROR: create thread failed!");
      return false;
    }
    ++current_thread_count_;
  }
  assert(current_thread_count_ == base_thread_count_);
  return true;
}

void ThreadPool::AddTask(Task *t) {
  pthread_mutex_lock(&task_queue_lock_);
  task_queue_.push(t);
  pthread_mutex_unlock(&task_queue_lock_);

  sem_post(&undo_task_sem_);
}

void ThreadPool::AddTask(void_function f, void *a) {
  Task *task = new Task(f, a);
  AddTask(task);
}

void ThreadPool::AddDestroyTask() {
  DestroyTask *task = new DestroyTask();
  AddTask(task);
}

void ThreadPool::AddTaskInSocket(void_function f, void *a, int socket_index) {
  if (numa_available() < 0) {
    ThreadPoolLogging::elog("numa_* functions unavailable\n");
    assert(false);
  }
  NumaSensitiveTask *task = new NumaSensitiveTask(f, a, socket_index);
  AddTask(task);
}

void ThreadPool::AddTaskInSocket(void_function f, void *a,
                                 vector<int> socket_indexs) {
  if (numa_available() < 0) {
    ThreadPoolLogging::elog("numa_* functions unavailable\n");
    assert(false);
  }
  NumaSensitiveTask *task = new NumaSensitiveTask(f, a, socket_indexs);
  AddTask(task);
}

void ThreadPool::AddTaskInCpu(void_function f, void *arg, int cpu_index) {
  CpuSensitiveTask *task = new CpuSensitiveTask(f, arg, cpu_index);
  AddTask(task);
}

void ThreadPool::AddTaskInCpu(void_function f, void *arg,
                              vector<int> cpu_indexs) {
  CpuSensitiveTask *task = new CpuSensitiveTask(f, arg, cpu_indexs);
  AddTask(task);
}

void ThreadPool::HandleTask(ThreadPool *thread_pool) {
  sem_wait(&(thread_pool->undo_task_sem_));
  Task *task = NULL;

  pthread_mutex_lock(&(thread_pool->task_queue_lock_));
  if (!thread_pool->task_queue_.empty()) {
    task = thread_pool->task_queue_.front();
    thread_pool->task_queue_.pop();
  }
  pthread_mutex_unlock(&(thread_pool->task_queue_lock_));

  if (task != NULL) {
    ThreadPoolLogging::log(
        "thread (id=%ld,offset=%lx) in thread pool is executing..\n",
        syscall(__NR_gettid), pthread_self());
    task->Run();
    ThreadPoolLogging::log(
        "thread (id=%ld,offset=%lx) in thread pool finished executing..\n",
        syscall(__NR_gettid), pthread_self());
    DELETE_PTR(task);
  }
}

void *ThreadPool::ThreadExec(void *arg) {
  ThreadPoolLogging::log(
      "a new thread(id=%ld,offset=%lx) is added into pool.\n",
      syscall(__NR_gettid), pthread_self());
  ThreadPool *thread_pool = static_cast<ThreadPool *>(arg);
  Task *task = NULL;

  // every thread execute a endless loop, waiting for task, and exit when
  // receive a task with end member of 'true'
  while (1) {
    thread_pool->HandleTask(thread_pool);
  }
  pthread_exit(NULL);
  return NULL;
}

void *ThreadPool::TempThreadExec(void *arg) {
  ThreadPoolLogging::log("a temporary thread(id=%ld,offset=%lx) is created.\n",
                         syscall(__NR_gettid), pthread_self());
  ThreadPool *thread_pool = static_cast<ThreadPool *>(arg);
  Task *task = NULL;

  thread_pool->HandleTask(thread_pool);
  return NULL;
}

void *ThreadPool::MonitorThreadExec(void *arg) {
  while (true) {
    usleep(500 * 1000);
    ThreadPool *tp = static_cast<ThreadPool *>(arg);
    if (tp->is_destroy_) return NULL;  // which means pool is destroyed
    static int too_many_task_times = 0;
    static int expand_thread_times = 0;
    int undo_task_count;
    sem_getvalue(&(tp->undo_task_sem_), &undo_task_count);
    ThreadPoolLogging::log("undo task count: ----- %d-------%d ------%d\n",
                           undo_task_count, too_many_task_times,
                           expand_thread_times);

    if (undo_task_count > 0) {
      if (++too_many_task_times > 3) {
        int to_expand_thread_count = (undo_task_count + 1) / 2;
        if (++expand_thread_times >= 3 &&
            tp->current_thread_count_ < tp->base_thread_count_ * 2) {
          if (tp->is_destroy_) return NULL;  // which means pool is destroyed
          pthread_create(&tp->thread_list_[tp->current_thread_count_++], NULL,
                         ThreadExec, tp);
          --to_expand_thread_count;  // because of adding one thread perpetually
        }
        expand_thread_times %= 3;
        for (int i = 0; i < to_expand_thread_count && i < GetNumberOfCpus() / 2;
             ++i) {
          if (tp->is_destroy_) return NULL;  // which means pool is destroyed
          pthread_t temp;
          pthread_create(&temp, NULL, TempThreadExec, tp);
        }
        too_many_task_times = 0;
      }
    } else {
      too_many_task_times = 0;
      expand_thread_times = 0;
    }
  }
}

void ThreadPool::Destroy(ThreadPool *tp) {
  is_destroy_ = true;
  while (!tp->task_queue_.empty()) {
    Task *temp = tp->task_queue_.front();
    tp->task_queue_.pop();
    DELETE_PTR(temp);
  }
  // destroy every thread by sending destroy task to everyone
  for (int i = 0; i < 2 * tp->current_thread_count_; ++i) {
    tp->AddDestroyTask();
  }
  for (int i = 0; i < tp->current_thread_count_; ++i) {
    pthread_cancel(tp->thread_list_[i]);
  }
#ifndef UNBLOCKED_JOIN
  for (int i = 0; i < tp->current_thread_count_; ++i) {
    pthread_join(tp->thread_list_[i], NULL);
  }
#endif
  DELETE_PTR(tp->thread_list_);
  while (!tp->task_queue_.empty()) {
    Task *temp = tp->task_queue_.front();
    tp->task_queue_.pop();
    DELETE_PTR(temp);
  }
  sem_destroy(&tp->undo_task_sem_);
  pthread_mutex_destroy(&tp->task_queue_lock_);
  ThreadPoolLogging::log("waiting monitor thread exiting...\n");
  pthread_join(tp->monitor_thread_, NULL);
}
