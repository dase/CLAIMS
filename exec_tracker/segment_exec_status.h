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
 * /Claims/exec_tracker/segment_exec_status.h
 *
 *  Created on: Apr 3, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef EXEC_TRACKER_SEGMENT_EXEC_STATUS_H_
#define EXEC_TRACKER_SEGMENT_EXEC_STATUS_H_
#include "../common/error_define.h"
#include "../exec_tracker/segment_exec_tracker.h"
#include "../node_manager/base_node.h"
#include <string>

#include "../utility/lock.h"
#include "caf/all.hpp"
#include <atomic>
using std::string;
namespace claims {

// due to the conflict between deleting SegmentExecStatus and reporting the
// last message (deleting is faster than reporting, so the last message doesn't
// been sent successfully), so all instance of SegmentExecStatus should be
// governed by SegmentExecTracker, new it and register it, then after the last
// message (e.t kDone of kCancelled), unregister it and delete it(may controlled
// by object poor).
class SegmentExecStatus {
 public:
  enum ExecStatus { kError, kOk, kCancelled, kDone };
  SegmentExecStatus(NodeSegmentID node_segment_id, unsigned int coor_node_id);
  SegmentExecStatus(NodeSegmentID node_segment_id);
  virtual ~SegmentExecStatus();
  // first cancel data source, e.t. exchange merger
  RetCode CancelSegExec();
  RetCode ReportStatus();
  RetCode ReportStatus(ExecStatus exec_status, string exec_info);
  bool UpdateStatus(ExecStatus exec_status, string exec_info,
                    u_int64_t logic_time = 0, bool need_report = false);
  RetCode RegisterToTracker();
  RetCode UnRegisterFromTracker();
  NodeSegmentID get_node_segment_id() { return node_segment_id_; }
  bool HaveErrorCase(u_int64_t logic_time);
  ExecStatus get_exec_status() { return exec_status_; }
  void set_exec_status(ExecStatus exec_status) { exec_status_ = exec_status; }
  string get_exec_info() { return exec_info_; }
  void set_exec_info(string exec_info) { exec_info_ = exec_info; }
  bool is_cancelled() { return kCancelled == exec_status_; }

 private:
  ExecStatus exec_status_;
  RetCode ret_code_;
  string exec_info_;
  u_int64_t logic_time_;
  actor coor_actor_;
  unsigned int coor_node_id_;
  NodeSegmentID node_segment_id_;
  Lock lock_;
  std::atomic_bool stop_report_;
};
#define UNLIKELY(expr) __builtin_expect(!!(expr), 0)

#define RETURN_IF_CANCELLED(exec_status)                                \
  do {                                                                  \
    if (UNLIKELY((exec_status)->is_cancelled())) {                      \
      LOG(WARNING) << exec_status->get_node_segment_id().first << " , " \
                   << exec_status->get_node_segment_id().second         \
                   << " is cancelled and exited execution!";            \
      return false;                                                     \
    }                                                                   \
  } while (false)

}  // namespace claims

#endif  //  EXEC_TRACKER_SEGMENT_EXEC_STATUS_H_
