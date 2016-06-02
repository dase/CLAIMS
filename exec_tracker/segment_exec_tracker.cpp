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
#include <string>
#include "caf/all.hpp"
#include "../Environment.h"
#include "../node_manager/base_node.h"
#include "caf/io/all.hpp"
using caf::actor_pool;
using caf::event_based_actor;
using caf::io::remote_actor;
using caf::time_unit;
using std::string;
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
  if (node_segment_id_to_status_.find(node_segment_id) ==
      node_segment_id_to_status_.end()) {
    node_segment_id_to_status_.insert(
        make_pair(node_segment_id, seg_exec_status));
    LOG(INFO) << node_segment_id.first << " , " << node_segment_id.second
              << " register to segment tracker successfully!";
    map_lock_.release();
  } else {
    LOG(ERROR) << "node_segment_id < " << node_segment_id.first << " , "
               << node_segment_id.second << " >already in segment tracker";
    map_lock_.release();
    assert(false);
  }
  return rSuccess;
}

RetCode SegmentExecTracker::UnRegisterSegES(NodeSegmentID node_segment_id) {
  map_lock_.acquire();
  auto it = node_segment_id_to_status_.find(node_segment_id);
  if (it != node_segment_id_to_status_.end()) {
    node_segment_id_to_status_.erase(it);
    LOG(INFO) << node_segment_id.first << " , " << node_segment_id.second
              << " has been erased from segment tracker! then left segment= "
              << node_segment_id_to_status_.size();
    map_lock_.release();
  } else {
    LOG(ERROR) << node_segment_id.first << " , " << node_segment_id.second
               << " couldn't be found when unregister segment status";
    map_lock_.release();
    assert(false);
  }
  return rSuccess;
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
        self->delayed_send(self, std::chrono::milliseconds(kReportIntervalTime),
                           ReportSegESAtom::value);
      },
      [=](ExitAtom) { self->quit(); },
      caf::others >> [=]() {
                       LOG(WARNING) << "segment report receives unkown message"
                                    << endl;
                     });
  self->send(self, ReportSegESAtom::value);
}

}  // namespace claims
