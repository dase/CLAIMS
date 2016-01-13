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
 * /Claims/utility/thread_pool.h
 *
 *  Created on: Jan 8, 2016
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef UTILITY_THREAD_POOL_H_
#define UTILITY_THREAD_POOL_H_

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <queue>
#include <vector>
#include "../common/Logging.h"

static bool g_thread_pool_used = true;
// static bool g_thread_pool_used = false;

class Task;
class ThreadPool {
 public:
  typedef void *(*void_function)(void *);

 public:
  ThreadPool();
  virtual ~ThreadPool();

  bool Init(int thread_count_in_pool);
  // parameter void* a can't be a class, because delete void * won't execute
  // destructor function
  void AddTask(void_function f, void *a);
  void AddTaskInSocket(void_function f, void *a, int socket_index);
  void AddTaskInSocket(void_function f, void *a,
                       std::vector<int> socket_indexs);
  void AddTaskInCpu(void_function f, void *arg, int cpu_index);
  void AddTaskInCpu(void_function f, void *arg, std::vector<int> cpu_indexs);
  void Destroy(ThreadPool *tp);

 private:
  void AddTask(Task *t);
  void AddDestroyTask();
  void HandleTask(ThreadPool *thread_pool);
  static void *MonitorThreadExec(void *arg);
  static void *ThreadExec(void *arg);
  static void *TempThreadExec(void *arg);

 private:
  //  pthread_cond_t free_thread_cond;
  //  pthread_mutex_t cond_lock;
  //
  //  attfinity_start_cpu;

  sem_t undo_task_sem_;  // undo task count

  pthread_t *thread_list_ = NULL;
  pthread_t monitor_thread_;

  std::queue<Task *> task_queue_;
  pthread_mutex_t task_queue_lock_;

  int base_thread_count_;
  int free_thread_count_;
  int max_thread_count_;
  volatile int current_thread_count_;
  //  pthread_mutex_t free_thread_count_lock_;

  int task_count_;
  int undo_task_count_;
  bool is_destroy_ = false;
  //  pthread_mutex_t undo_task_count_lock_;
};
#endif  // UTILITY_THREAD_POOL_H_
