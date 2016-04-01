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
 * /Claims/Executor/segment_exec_tracker.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include "../exec_tracker/segment_exec_tracker.h"
#include <glog/logging.h>
#include "caf/all.hpp"
#include <iosfwd>
#include <string>
#include "../Environment.h"
#include "../node_manager/base_node.h"
using caf::actor_pool;
using caf::event_based_actor;
using caf::io::remote_actor;
using caf::time_unit;
using std::string;
#include "caf/io/all.hpp"
namespace claims {

SegmentExecTracker::SegmentExecTracker() {
  segment_exec_tracker_actor_ = caf::spawn(ReportAllSegStatus, this);
}

SegmentExecTracker::~SegmentExecTracker() {
  caf::scoped_actor self;
  assert(node_segment_id_to_status_.size() == 0);
  self->send(segment_exec_tracker_actor_, ExitAtom::value);
}

RetCode SegmentExecTracker::CancelSegExec(NodeSegmentID node_segment_id) {}

RetCode SegmentExecTracker::RegisterSegES(NodeSegmentID node_segment_id,
                                          SegmentExecStatus* seg_exec_status) {
  map_lock_.acquire();
  assert(node_segment_id_to_status_.find(node_segment_id) ==
         node_segment_id_to_status_.end());
  node_segment_id_to_status_.insert(
      make_pair(node_segment_id, seg_exec_status));
  map_lock_.release();
  LOG(INFO) << node_segment_id.first << " , " << node_segment_id.second
            << " has been registered to segment tracker!";
  return 0;
}

RetCode SegmentExecTracker::UnRegisterSegES(NodeSegmentID node_segment_id) {
  map_lock_.acquire();
  auto it = node_segment_id_to_status_.find(node_segment_id);
  assert(it != node_segment_id_to_status_.end());
  node_segment_id_to_status_.erase(it);
  map_lock_.release();
  LOG(INFO) << node_segment_id.first << " , " << node_segment_id.second
            << " has been erased from segment tracker! then left segment= "
            << node_segment_id_to_status_.size();
  return 0;
}
// report all status of all segment that locate at this node, but if just one
// thread occur error, how to catch it and report it?
void SegmentExecTracker::ReportAllSegStatus(
    caf::event_based_actor* self, SegmentExecTracker* seg_exec_tracker) {
  self->become(

      [=](ReportSegESAtom) {
        seg_exec_tracker->map_lock_.acquire();
        if (seg_exec_tracker->node_segment_id_to_status_.size() > 0) {
          auto it = seg_exec_tracker->node_segment_id_to_status_.begin();
          for (; it != seg_exec_tracker->node_segment_id_to_status_.end();
               ++it) {
            assert(it->second != NULL);
            it->second->ReportStatus();
          }
        }
        seg_exec_tracker->map_lock_.release();
        self->delayed_send(self, std::chrono::milliseconds(1000),
                           ReportSegESAtom::value);
      },
      [=](ExitAtom) { self->quit(); },
      caf::others >> [=]() {
                       LOG(WARNING) << "segment report receives unkown message"
                                    << endl;
                     });
  self->send(self, ReportSegESAtom::value);
}

SegmentExecStatus::SegmentExecStatus(NodeSegmentID node_segment_id,
                                     NodeAddr coor_addr)
    : node_segment_id_(node_segment_id),
      coor_addr_(coor_addr),
      exec_info_("ok"),
      exec_status_(ExecStatus::kOk),
      ret_code_(0),
      logic_time_(0) {
  coor_actor_ = remote_actor(coor_addr.first, coor_addr.second);
  //  RegisterToTracker();
}
SegmentExecStatus::SegmentExecStatus(NodeSegmentID node_segment_id)
    : node_segment_id_(node_segment_id),
      exec_info_("ok"),
      exec_status_(ExecStatus::kOk),
      ret_code_(0),
      logic_time_(Environment::getInstance()
                      ->get_stmt_exec_tracker()
                      ->get_logic_time()) {}
SegmentExecStatus::~SegmentExecStatus() {
  ostringstream exec_info;
  exec_info << "query (" << node_segment_id_.first << " , "
            << node_segment_id_.second / kMaxNodeNum << " ) at node "
            << node_segment_id_.second % kMaxNodeNum << " execution succeed";
  //  UpdateStatus(SegmentExecStatus::ExecStatus::kDone, exec_info.str(), 0,
  //  true);
  //  ReportStatus(SegmentExecStatus::ExecStatus::kDone, exec_info.str());
  //  UnRegisterFromTracker();
}

RetCode SegmentExecStatus::CancelSegExec() {
  //  lock_.acquire();
  exec_status_ = kCancelled;
  //  lock_.release();
  LOG(INFO) << node_segment_id_.first << " , " << node_segment_id_.second
            << " need be cancelled!" << endl;
  return 0;
}
RetCode SegmentExecStatus::ReportStatus(ExecStatus exec_status,
                                        string exec_info) {
  if (kCancelled == exec_status_) {
    return 0;
  }
  //  lock_.acquire();
  try {
    caf::scoped_actor self;
    self->sync_send(coor_actor_, ReportSegESAtom::value, node_segment_id_,
                    exec_status, exec_info)
        .await(

            [=](OkAtom) {
              LOG(INFO) << node_segment_id_.first << " , "
                        << node_segment_id_.second
                        << " report0: " << exec_status << " , " << exec_info;
            },
            [=](CancelPlanAtom) { CancelSegExec(); },
            caf::after(std::chrono::seconds(kTimeout)) >>
                [&]() {
                  LOG(WARNING)
                      << "segment report status timeout and cancel self!";
                  CancelSegExec();
                });

  } catch (caf::network_error& e) {
    LOG(WARNING) << "cann't connect to coordinator ( " << coor_addr_.first
                 << " , " << coor_addr_.second << " )";
  }
  lock_.release();

  // for making sure kError or kDone be the last message sended to remote
  if (ExecStatus::kError == exec_status_ || ExecStatus::kDone == exec_status_) {
    exec_status_ = kCancelled;
  }
  return rSuccess;
}
RetCode SegmentExecStatus::ReportStatus() {
  // if this segment is cancelled, needn't report status
  if (kCancelled == exec_status_) {
    return 0;
  }
  lock_.acquire();
  try {
    caf::scoped_actor self;
    self->sync_send(coor_actor_, ReportSegESAtom::value, node_segment_id_,
                    exec_status_, exec_info_)
        .await(

            [=](OkAtom) {
              LOG(INFO) << node_segment_id_.first << " , "
                        << node_segment_id_.second
                        << " report : " << exec_status_ << " , " << exec_info_;
            },
            [=](CancelPlanAtom) { CancelSegExec(); },
            caf::after(std::chrono::seconds(kTimeout)) >>
                [&]() {
                  LOG(WARNING)
                      << "segment report status timeout and cancel self!";
                  CancelSegExec();
                });

  } catch (caf::network_error& e) {
    LOG(WARNING) << "cann't connect to coordinator ( " << coor_addr_.first
                 << " , " << coor_addr_.second << " )";
  }
  lock_.release();

  // for making sure kError or kDone be the last message sended to remote
  if (ExecStatus::kError == exec_status_ || ExecStatus::kDone == exec_status_) {
    exec_status_ = kCancelled;
  }
  return rSuccess;
}
bool SegmentExecStatus::UpdateStatus(ExecStatus exec_status, string exec_info,
                                     u_int64_t logic_time, bool need_report) {
  if (exec_status_ == kCancelled) {
    LOG(INFO) << node_segment_id_.first << " , " << node_segment_id_.second
              << " update status failed!";
    return false;
  }
  lock_.acquire();
  logic_time_ = logic_time;
  exec_status_ = exec_status;
  exec_info_ = exec_info;
  LOG(INFO) << node_segment_id_.first << " , " << node_segment_id_.second
            << " update logic_time= " << logic_time_
            << " exec_status_= " << exec_status_;
  lock_.release();
  if (need_report) {
    lock_.acquire();

    ReportStatus(exec_status, exec_info);
  }
  return true;
}
RetCode SegmentExecStatus::RegisterToTracker() {
  return Environment::getInstance()->get_segment_exec_tracker()->RegisterSegES(
      node_segment_id_, this);
}

RetCode SegmentExecStatus::UnRegisterFromTracker() {
  while (true) {
    if (exec_status_ == kOk) {
      LOG(INFO) << node_segment_id_.first << " , " << node_segment_id_.second
                << " " << exec_status_ << " should be 3";
      UpdateStatus(SegmentExecStatus::ExecStatus::kDone, "finished", 0, true);
    } else {
      break;
    }
  }
  return Environment::getInstance()
      ->get_segment_exec_tracker()
      ->UnRegisterSegES(node_segment_id_);
}
bool SegmentExecStatus::HaveErrorCase(u_int64_t logic_time) {
  LOG(INFO) << node_segment_id_.first << " , " << node_segment_id_.second << " "
            << exec_status_ << " " << logic_time << " - " << logic_time_
            << " = " << logic_time - logic_time_;
  // if the exection status is normal, but logic time falls behind 4s
  return (exec_status_ == kOk) && (logic_time - logic_time_ > 4);
}
}  // namespace claims
