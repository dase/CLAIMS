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
 * /Claims/Executor/segment_exec_tracker.h
 *
 *  Created on: Mar 24, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef EXEC_TRACKER_SEGMENT_EXEC_TRACKER_H_
#define EXEC_TRACKER_SEGMENT_EXEC_TRACKER_H_
#include "../exec_tracker/segment_exec_tracker.h"
#include <boost/unordered/unordered_map.hpp>
#include <string>
#include <utility>

#include "caf/all.hpp"

#include "../common/error_define.h"
#include "../node_manager/base_node.h"
#include "../utility/lock.h"
using boost::unordered::unordered_map;
using caf::actor;
using std::pair;
using std::string;

namespace claims {
// first=query_id, second=segment_id*kMaxNodeNum + node_id
typedef std::pair<u_int64_t, u_int64_t> NodeSegmentID;
class SegmentExecStatus;
class SegmentExecTracker {
 public:
  SegmentExecTracker();
  virtual ~SegmentExecTracker();
  RetCode CancelSegExec(NodeSegmentID node_segment_id);
  RetCode RegisterSegES(NodeSegmentID node_segment_id,
                        SegmentExecStatus* seg_exec_status);
  RetCode UnRegisterSegES(NodeSegmentID node_segment_id);
  static void ReportAllSegStatus(caf::event_based_actor* self,
                                 SegmentExecTracker* seg_exec_tracker);

 private:
  actor segment_exec_tracker_actor_;
  boost::unordered_map<NodeSegmentID, SegmentExecStatus*>
      node_segment_id_to_status_;
  Lock map_lock_;
};
class SegmentExecStatus {
 public:
  enum ExecStatus { kError, kOk, kCancelled, kDone };
  SegmentExecStatus(NodeSegmentID node_segment_id, NodeAddr coor_addr);
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

 private:
  ExecStatus exec_status_;
  RetCode ret_code_;
  string exec_info_;
  u_int64_t logic_time_;
  actor coor_actor_;
  NodeAddr coor_addr_;
  NodeSegmentID node_segment_id_;
  Lock lock_;
};
}  // namespace claims

#endif  //  EXEC_TRACKER_SEGMENT_EXEC_TRACKER_H_
