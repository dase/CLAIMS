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
 * /Claims/Executor/query_exec_tracker.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include "../exec_tracker/stmt_exec_tracker.h"

#include <string>
#include <iostream>
#include <utility>

#include "../Environment.h"
#include "../node_manager/base_node.h"
#include "caf/all.hpp"
using std::make_pair;
using std::string;
namespace claims {

StmtExecTracker::StmtExecTracker() : query_id_gen_(0), logic_time_(0) {
  stmt_exec_tracker_actor_ = caf::spawn(CheckStmtExecStatus, this);
}

StmtExecTracker::~StmtExecTracker() {
  caf::scoped_actor self;
  self->send(stmt_exec_tracker_actor_, ExitAtom::value);
  assert(query_id_to_stmtes_.size() == 0);
}

RetCode StmtExecTracker::RegisterStmtES(StmtExecStatus* stmtes) {
  stmtes->set_query_id(GenQueryId());
  lock_.acquire();
  query_id_to_stmtes_.insert(make_pair(stmtes->get_query_id(), stmtes));
  lock_.release();
  return 0;
}

RetCode StmtExecTracker::UnRegisterStmtES(u_int64_t query_id) {
  lock_.acquire();
  auto it = query_id_to_stmtes_.find(query_id);
  if (it == query_id_to_stmtes_.end()) {
    LOG(WARNING) << "invalide query id at UnRegisterStmtES" << endl;
  }
  query_id_to_stmtes_.erase(it);
  lock_.release();
  LOG(INFO) << "query id= " << query_id
            << " has erased from StmtEs! then left stmt = "
            << query_id_to_stmtes_.size() << endl;
  return 0;
}

RetCode StmtExecTracker::CancelStmtExec(u_int64_t query_id) {
  lock_.acquire();
  auto it = query_id_to_stmtes_.find(query_id);
  if (it == query_id_to_stmtes_.end()) {
    LOG(WARNING) << "inval query id at cancel query of stmt exec tracker"
                 << endl;
    lock_.release();
    assert(false);
    return -1;
  }
  it->second->CancelStmtExec();
  lock_.release();
  return 0;
}

void StmtExecTracker::CheckStmtExecStatus(caf::event_based_actor* self,
                                          StmtExecTracker* stmtes) {
  self->become(

      [=](CheckStmtESAtom) {
        //        stmtes->lock_.acquire();
        for (auto it = stmtes->query_id_to_stmtes_.begin();
             it != stmtes->query_id_to_stmtes_.end(); ++it) {
          if (it->second->CouldBeDeleted()) {
            delete it->second;
            it->second = NULL;
            stmtes->UnRegisterStmtES(it->first);
          } else {
            if (it->second->HaveErrorCase(stmtes->logic_time_)) {
              assert(false);
              stmtes->CancelStmtExec(it->first);
            }
          }
        }
        //        stmtes->lock_.release();
        stmtes->logic_time_++;
        self->delayed_send(self, std::chrono::milliseconds(2000),
                           CheckStmtESAtom::value);
      },
      [=](ExitAtom) { self->quit(); },
      caf::others >> [=]() {
                       LOG(WARNING) << "stmt checking receives unkown message"
                                    << endl;
                     });
  self->send(self, CheckStmtESAtom::value);
}

bool StmtExecTracker::UpdateSegExecStatus(
    NodeSegmentID node_segment_id, SegmentExecStatus::ExecStatus exec_status,
    string exec_info) {
  lock_.acquire();
  auto it = query_id_to_stmtes_.find(node_segment_id.first);
  assert(it != query_id_to_stmtes_.end());
  bool ret = it->second->UpdateSegExecStatus(node_segment_id, exec_status,
                                             exec_info, logic_time_);
  lock_.release();
  LOG(INFO) << node_segment_id.first << " , " << node_segment_id.second
            << " receive : " << exec_status << " , " << exec_info;
  return ret;
}

StmtExecStatus::StmtExecStatus(string sql_stmt)
    : sql_stmt_(sql_stmt),
      query_result_(NULL),
      exec_info_(""),
      exec_status_(ExecStatus::kOk),
      segment_id_gen_(0) {}

StmtExecStatus::~StmtExecStatus() {
  // due to the query result should return, so shouldn't be deleted here
  // if (NULL != query_result_) delete query_result_;

  for (auto it = node_seg_id_to_seges_.begin();
       it != node_seg_id_to_seges_.end(); ++it) {
    delete it->second;
    it->second = NULL;
  }
  node_seg_id_to_seges_.clear();
}

RetCode StmtExecStatus::CancelStmtExec() {
  if (ExecStatus::kCancelled == exec_status_) {
    return 0;
  }
  exec_status_ = kCancelled;
  //  LOG(INFO) << query_id_ << " query should be cancelled!";
  //  for (auto it = node_seg_id_to_seges_.begin();
  //       it != node_seg_id_to_seges_.end(); ++it) {
  //    it->second->set_exec_status(SegmentExecStatus::ExecStatus::kCancelled);
  //  }
  return 0;
}
// check every segment status
bool StmtExecStatus::CouldBeDeleted() {
  if (exec_status_ == kOk) {
    return false;
  }
  for (auto it = node_seg_id_to_seges_.begin();
       it != node_seg_id_to_seges_.end(); ++it) {
    if (it->second->get_exec_status() == SegmentExecStatus::ExecStatus::kOk) {
      return false;
    }
  }
  LOG(INFO) << query_id_ << " query can be deleted";
  return true;
}
bool StmtExecStatus::HaveErrorCase(u_int64_t logic_time) {
  for (auto it = node_seg_id_to_seges_.begin();
       it != node_seg_id_to_seges_.end(); ++it) {
    if (it->second->HaveErrorCase(logic_time)) {
      return true;
    }
  }
  return false;
}
RetCode StmtExecStatus::RegisterToTracker() {
  return Environment::getInstance()->get_stmt_exec_tracker()->RegisterStmtES(
      this);
}

RetCode StmtExecStatus::UnRegisterFromTracker() {
  return Environment::getInstance()->get_stmt_exec_tracker()->UnRegisterStmtES(
      query_id_);
}
void StmtExecStatus::AddSegExecStatus(SegmentExecStatus* seg_exec_status) {
  node_seg_id_to_seges_.insert(
      make_pair(seg_exec_status->get_node_segment_id(), seg_exec_status));
}

bool StmtExecStatus::UpdateSegExecStatus(
    NodeSegmentID node_segment_id, SegmentExecStatus::ExecStatus exec_status,
    string exec_info, u_int64_t logic_time) {
  if (SegmentExecStatus::ExecStatus::kError == exec_status) {
    CancelStmtExec();
  }
  //  lock_.acquire();
  auto it = node_seg_id_to_seges_.find(node_segment_id);
  assert(it != node_seg_id_to_seges_.end());

  if (kCancelled == exec_status_) {
    it->second->UpdateStatus(SegmentExecStatus::ExecStatus::kCancelled,
                             exec_info, logic_time);
    //    lock_.release();
    return false;
  } else {
    it->second->UpdateStatus(exec_status, exec_info, logic_time);
  }
  //  lock_.release();
  return true;
}

}  // namespace claims
