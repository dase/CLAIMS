/*
 * Message.cpp
 *
 *  Created on: May 12, 2013
 *      Author: wangli
 */

#include "Message.h"

void PhysicalQueryPlan::run() {
  block_stream_iterator_root_->Open();
  while (block_stream_iterator_root_->Next(0)) {
  }
  block_stream_iterator_root_->Close();
}
