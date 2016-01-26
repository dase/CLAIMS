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
 * /Claims/utility/task.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./task.h"
#include <glog/logging.h>
#include <vector>
#include "../common/error_define.h"

// #define SPECIFY_CPU

/* switch to open debug log ouput */
#define THREAD_POOL_DEBUG
//#define THREAD_POOL_PREF

#ifdef CLAIMS_DEBUG_LOG
#ifdef THREAD_POOL_DEBUG
#define DLOG_TP(info) DLOG(INFO) << info
#else
#define DLOG_TP(info)
#endif
#ifdef THREAD_POOL_PREF
#define PLOG_TP(info) DLOG(INFO) << info
#else
#define PLOG_TP(info)
#endif
#else
#define DLOG_DI(info)
#endif

/*
 *  get old binded CPUs and bind new CPUs in specify sockets,
 *  run function f,
 *  bind to old CPUs again
 */
void NumaSensitiveTask::Run() {
  vector<int> old_cpu = GetCurrentCpuAffinity();

  DLOG_TP("before setting, Current cpu is: ");
  for (auto it : old_cpu) DLOG_TP(it << " ");
  DLOG_TP("\n");
#ifdef SPECIFY_CPU
  int cpu_index = GetNextCPUinSocket(node_index_);
  SetCpuAffinity(cpu_index);
#else
  struct bitmask* bm = numa_allocate_nodemask();
  numa_bitmask_clearall(bm);
  for (int i = 0; i < node_index_.size(); ++i)
    numa_bitmask_setbit(bm, node_index_[i]);

  // bind to this NUMA nodes, ignoring CPU affinities
  numa_run_on_node_mask_all(bm);
#endif
  DLOG_TP("after setting, Current cpu is: ");
  for (auto it : GetCurrentCpuAffinity()) DLOG_TP(it << " ");
  DLOG_TP("\n");
  Task::Run();

  // restore old binding
  SetCpuAffinity(old_cpu);
  DLOG_TP("after restoring, Current cpu is: ");
  for (auto it : GetCurrentCpuAffinity()) DLOG_TP(it << " ");
  DLOG_TP("\n");
}

void NumaSensitiveTask::BindSocket(int node_index) {}

void CpuSensitiveTask::Run() {
  vector<int> old_cpu = GetCurrentCpuAffinity();
  DLOG_TP("before setting, Current cpu is: ");
  for (auto it : GetCurrentCpuAffinity()) DLOG_TP(it << " ");
  DLOG_TP("\n");
  if (false == SetCpuAffinity(cpu_index_)) {
    LOG(ERROR) << "failed to set affinity with CPU: ";
    for (int i = 0; i < cpu_index_.size(); ++i)
      LOG(ERROR) << cpu_index_[i] << " ";
    LOG(ERROR) << "\n";
  }
  DLOG_TP("after setting, Current cpu is: ");
  for (auto it : GetCurrentCpuAffinity()) DLOG_TP(it << " ");
  DLOG_TP("\n");
  Task::Run();

  // restore old binding
  SetCpuAffinity(old_cpu);
  DLOG_TP("after restoring, Current cpu is: ");
  for (auto it : GetCurrentCpuAffinity()) DLOG_TP(it << " ");
  DLOG_TP("\n");
}
