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
 * /CLAIMS/physical_operator/performance_mornitor.cpp
 *  Created on: Aug 31, 2013
 *      Author: wangli
 */

#include "../physical_operator/performance_monitor.h"

#include <glog/logging.h>
#include <iostream>
#include "../utility/rdtsc.h"

using std::endl;
namespace claims {
namespace physical_operator {
PerformanceMonitor::PerformanceMonitor(State state) : state_(state) {}
PerformanceMonitor::PerformanceMonitor() {}
PerformanceMonitor::~PerformanceMonitor() {}

bool PerformanceMonitor::Open(const PartitionOffset& partition_offset) {
  start_ = curtick();
  state_.child_->Open(partition_offset);
  block_ = BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
  tuplecount_ = 0;
  int error;
  error = pthread_create(&report_tid_, NULL, ReportPerformance, this);
  if (error != 0) {
    std::cout << "create threads error!" << std::endl;
  }

  return true;
}

bool PerformanceMonitor::Next(BlockStreamBase*) {
  //	PartitionFunction*
  // hash=PartitionFunctionFactory::createBoostHashFunction(4);
  //	const int partition_index=3;
  block_->setEmpty();
  if (state_.child_->Next(block_)) {
    BlockStreamBase::BlockStreamTraverseIterator* it = block_->createIterator();
    while (it->nextTuple()) {
      //			tuplecount_++;
      //			if(rand()%10000<3){
      //				logging_->log("partition
      // value:%d",state_.schema_->getcolumn(partition_index).operate->ge)
      //			}
    }
    tuplecount_ += block_->getTuplesInBlock();
    return true;
  }
  return false;
}

bool PerformanceMonitor::Close() {
  pthread_cancel(report_tid_);
  double eclipsed_seconds = getSecond(start_);
  double processed_data_in_bytes =
      tuplecount_ * state_.schema_->getTupleMaxSize();

  LOG(INFO) << "Total time: " << getSecond(start_) << "  seconds" << endl;
  LOG(INFO) << "Total tuples: " << tuplecount_ << endl;
  LOG(INFO) << "Avg throughput: "
            << processed_data_in_bytes / eclipsed_seconds / 1024 / 1024
            << " M data/s, "
            << (float)tuplecount_ / 2014 / 1024 / eclipsed_seconds
            << " M tuples/s" << endl;
  block_->~BlockStreamBase();
  state_.child_->Close();
  return true;
}
void PerformanceMonitor::Print() {
  printf("Performance");
  printf("-------------\n");
  state_.child_->Print();
}
unsigned long int PerformanceMonitor::GetNumberOfTuples() const {
  return tuplecount_;
}

void* PerformanceMonitor::ReportPerformance(void* arg) {
  PerformanceMonitor* Pthis = (PerformanceMonitor*)arg;

  while (true) {
    const unsigned long last_tuple_count = Pthis->tuplecount_;
    usleep(Pthis->state_.report_cycles_ * 1000);

    double eclipsed_seconds = getSecond(Pthis->start_);
    double processed_data_in_bytes =
        Pthis->tuplecount_ * Pthis->state_.schema_->getTupleMaxSize();
    double processed_data_in_bytes_during_last_cycle =
        (Pthis->tuplecount_ - last_tuple_count) *
        Pthis->state_.schema_->getTupleMaxSize();
    LOG(INFO) << eclipsed_seconds << " s  Real Time: "
              << processed_data_in_bytes_during_last_cycle /
                     (Pthis->state_.report_cycles_ / 1000) / 1024 / 1024
              << "M/s    AVG: "
              << (processed_data_in_bytes / eclipsed_seconds / 1024 / 1024)
              << " M/s.   " << ((float)Pthis->tuplecount_ / 1024 / 1024)
              << " M tuples received." << endl;
  }
}
}  // namespace physical_operator
}  // namespace claims
