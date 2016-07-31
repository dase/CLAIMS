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
 * /Claims/Executor/query_exec_tracker.h
 *
 *  Created on: Mar 24, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef EXEC_TRACKER_STMT_EXEC_TRACKER_H_
#define EXEC_TRACKER_STMT_EXEC_TRACKER_H_
#include "../exec_tracker/stmt_exec_tracker.h"
#include <sys/types.h>
#include <string>
#include <unordered_map>
#include "./segment_exec_status.h"
#include "./stmt_exec_status.h"
#include "../common/Block/ResultSet.h"
#include "../common/error_define.h"
#include "../exec_tracker/segment_exec_tracker.h"
#include "../utility/lock.h"
using std::string;
namespace claims {
#define kMaxNodeNum 10000
#define kCheckIntervalTime 5000
/// tracker the execution status of every statement, each StmtExecStatus should
/// register to it
class StmtExecTracker {
 public:
  StmtExecTracker();
  virtual ~StmtExecTracker();
  u_int64_t GenQueryId() { return query_id_gen_++; }
  RetCode RegisterStmtES(StmtExecStatus* stmtes);
  RetCode UnRegisterStmtES(u_int64_t query_id);
  RetCode CancelStmtExec(u_int64_t query_id);

  // check every StmtExecStatus in caf's thread, should be static due to used
  // for spawning caf thread
  static void CheckStmtExecStatus(caf::event_based_actor* self,
                                  StmtExecTracker* stmtes);
  // according to StmtExecTracker to find corresponding stmt_status, then to
  // update segment_status
  bool UpdateSegExecStatus(NodeSegmentID node_segment_id,
                           SegmentExecStatus::ExecStatus exec_status,
                           string exec_info);
  u_int64_t get_logic_time() { return logic_time_; }

 private:
  std::atomic_ullong logic_time_;
  actor stmt_exec_tracker_actor_;
  std::atomic_ullong query_id_gen_;
  std::unordered_map<u_int64_t, StmtExecStatus*> query_id_to_stmtes_;
  Lock lock_;
};

}  // namespace claims

#endif  // EXEC_TRACKER_STMT_EXEC_TRACKER_H_
