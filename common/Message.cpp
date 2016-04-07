/*
 * Message.cpp
 *
 *  Created on: May 12, 2013
 *      Author: wangli
 */

#include "Message.h"
using claims::SegmentExecStatus;
void PhysicalQueryPlan::run() {
  SegmentExecStatus* segment_exec_status = new SegmentExecStatus(
      make_pair(query_id_, segment_id_ * kMaxNodeNum + target_node_id_),
      coor_addr_);
  segment_exec_status->RegisterToTracker();
  segment_exec_status->UpdateStatus(
      SegmentExecStatus::ExecStatus::kOk,
      "physical plan reveived succeed and begin to open()", 0, true);
  bool ret = block_stream_iterator_root_->Open(segment_exec_status);
  if (ret) {
    segment_exec_status->UpdateStatus(SegmentExecStatus::ExecStatus::kOk,
                                      "physical plan open() succeed", 0, true);
  } else {
    segment_exec_status->UpdateStatus(SegmentExecStatus::ExecStatus::kError,
                                      "physical plan open() failed", 0, true);
  }
  if (segment_id_ == 3 || segment_id_ == 2) {
    segment_exec_status->UpdateStatus(SegmentExecStatus::ExecStatus::kError,
                                      "cancelled", 0, true);
  }
  while (ret && block_stream_iterator_root_->Next(segment_exec_status, 0)) {
  }
  segment_exec_status->UpdateStatus(SegmentExecStatus::ExecStatus::kOk,
                                    "physical plan next() succeed", 0, true);
  block_stream_iterator_root_->Close(segment_exec_status);
  segment_exec_status->UpdateStatus(SegmentExecStatus::ExecStatus::kDone,
                                    "physical plan close() succeed", 0, true);
  segment_exec_status->UnRegisterFromTracker();
  delete segment_exec_status;
}
