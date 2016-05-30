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
 * /Claims/exec_tracker/stmt_exec_status.h
 *
 *  Created on: Apr 3, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef EXEC_TRACKER_STMT_EXEC_STATUS_H_
#define EXEC_TRACKER_STMT_EXEC_STATUS_H_
#include <boost/unordered/unordered_map.hpp>
#include <atomic>
#include <iosfwd>
#include <string>
#include "./segment_exec_status.h"
#include "../common/Block/ResultSet.h"
#include "../exec_tracker/segment_exec_tracker.h"
#include "../utility/lock.h"
using std::string;
namespace claims {
/// for monitoring the execution status of every segment, slave node has
/// remote_segment_status, coordinator has local_segment_status, and synchronize
/// the underlying 2 status at every reporting time
class StmtExecStatus {
 public:
  enum ExecStatus { kError, kOk, kCancelled, kDone };
  StmtExecStatus(string sql_stmt);
  virtual ~StmtExecStatus();
  u_int64_t get_query_id() { return query_id_; }
  void set_query_id(u_int64_t query_id) { query_id_ = query_id; }
  RetCode CancelStmtExec(bool locked = false);
  RetCode RegisterToTracker();
  RetCode UnRegisterFromTracker();
  short GenSegmentId() { return segment_id_gen_++; }
  void AddSegExecStatus(SegmentExecStatus* seg_exec_status);

  // update remote_segment_status and local_segment_status
  bool UpdateSegExecStatus(NodeSegmentID node_segment_id,
                           SegmentExecStatus::ExecStatus exec_status,
                           string exec_info, u_int64_t logic_time);
  bool CouldBeDeleted();
  bool HaveErrorCase(u_int64_t logic_time);
  void set_exec_status(ExecStatus exec_status) { exec_status_ = exec_status; }
  ExecStatus get_exec_status() { return exec_status_; }
  string get_exec_info() { return exec_info_; }
  void set_exec_info(string exec_info) {
    lock_.acquire();
    exec_info_ = exec_info;
    lock_.release();
  }
  ResultSet* get_query_result() { return query_result_; }
  void set_query_result(ResultSet* query_result) {
    query_result_ = query_result;
  }
  bool IsCancelled() { return exec_status_ == ExecStatus::kCancelled; }

 private:
  string exec_info_;
  ExecStatus exec_status_;
  ResultSet* query_result_;
  boost::unordered_map<NodeSegmentID, SegmentExecStatus*> node_seg_id_to_seges_;
  u_int64_t query_id_;
  string sql_stmt_;
  std::atomic_short segment_id_gen_;
  Lock lock_;
};
}  // namespace claims

#endif  //  EXEC_TRACKER_STMT_EXEC_STATUS_H_
