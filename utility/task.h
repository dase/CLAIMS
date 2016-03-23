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
 * /Claims/utility/task.h
 *
 *  Created on: Jan 8, 2016
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef UTILITY_TASK_H_
#define UTILITY_TASK_H_

#include <sys/syscall.h>
#include <vector>
#include "./cpu_scheduler.h"
#include "../common/Logging.h"

using std::vector;

class Task {
 public:
  typedef void *(*void_function)(void *);

 public:
  // Task() = default;
  Task(void_function f, void *a) : func_(f), arg_(a) {}
  virtual ~Task() {}
  virtual void Run() { (*func_)(arg_); }

  static inline void DestroyTask(Task *task) { delete task; }

 protected:
  void_function func_;  // pointer to function
  void *arg_;           // the parameter of function
};

class NumaSensitiveTask : public Task {
 public:
  NumaSensitiveTask(void_function f, void *a, int node_index) : Task(f, a) {
    if (node_index < 0 || node_index >= GetNumberOfSockets()) {
      ThreadPoolLogging::elog("Socket index %d is over range, expect 0 to %d ",
                              node_index, GetNumberOfSockets() - 1);
      assert(false);
    }
    node_index_.push_back(node_index);
  }
  NumaSensitiveTask(void_function f, void *a, vector<int> nodes)
      : Task(f, a), node_index_(nodes) {
    for (int i = 0; i < node_index_.size(); ++i) {
      if (node_index_[i] < 0 || node_index_[i] >= GetNumberOfSockets()) {
        ThreadPoolLogging::elog(
            "Socket index %d is over range, expect 0 to %d ", node_index_[i],
            GetNumberOfSockets() - 1);
        assert(false);
      }
    }
  }
  virtual ~NumaSensitiveTask() throw() {}

  virtual void Run();
  void BindSocket(int node_index);

 private:
  vector<int> node_index_;
};

class CpuSensitiveTask : public Task {
 public:
  CpuSensitiveTask(void_function f, void *a, vector<int> cpu_index)
      : Task(f, a), cpu_index_(cpu_index) {
    for (int i = 0; i < cpu_index_.size(); ++i) {
      if (cpu_index_[i] < 0 || cpu_index_[i] >= GetNumberOfCpus()) {
        ThreadPoolLogging::elog("CPU index %d is over range, expect 0 to %d ",
                                cpu_index_[i], GetNumberOfCpus() - 1);
        assert(false);
      }
    }
  }
  CpuSensitiveTask(void_function f, void *a, int cpu_index) : Task(f, a) {
    if (cpu_index < 0 || cpu_index >= GetNumberOfCpus()) {
      ThreadPoolLogging::elog("CPU index %d is over range, expect 0 to %d ",
                              cpu_index, GetNumberOfCpus() - 1);
      assert(false);
    }
    cpu_index_.push_back(cpu_index);
  }
  virtual void Run();

 private:
  vector<int> cpu_index_;
};

class DestroyTask : public Task {
 public:
  DestroyTask() : Task(NULL, NULL) {}
  ~DestroyTask() throw() {}
  void Run() {
    ThreadPoolLogging::log("thread(id=%ld,offset=%lx) exit...\n",
                           syscall(__NR_gettid), pthread_self());
    pthread_exit(NULL);
  }
};

#endif  // UTILITY_TASK_H_
