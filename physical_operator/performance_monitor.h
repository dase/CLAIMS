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
 * /CLAIMS/physical_operator/performance_mornitor.h
 *  Created on: Aug 31, 2013
 *      Author: wangli
 */

#ifndef PHYSICAL_OPERATOR_PERFORMANCE_MONITOR_H_
#define PHYSICAL_OPERATOR_PERFORMANCE_MONITOR_H_
#include <pthread.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "../common/Schema/Schema.h"
#include "../physical_operator/physical_operator_base.h"
namespace claims {
namespace physical_operator {
class PerformanceMonitor : public PhysicalOperatorBase {
 public:
  struct State {
    State(){};
    State(Schema* schema, PhysicalOperatorBase* child, unsigned block_size,
          unsigned report_cycles = 1000)
        : schema_(schema),
          child_(child),
          block_size_(block_size),
          report_cycles_(report_cycles){};
    Schema* schema_;
    PhysicalOperatorBase* child_;
    unsigned block_size_;
    unsigned report_cycles_;  // in milliseconds.
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& child_& block_size_& report_cycles_;
    }
  };
  PerformanceMonitor(State state_);
  virtual ~PerformanceMonitor();
  bool Open(const PartitionOffset& partition_offset = 0);
  bool Next(BlockStreamBase* block);
  bool Close();
  void Print();
  unsigned long int GetNumberOfTuples() const;

 private:
  static void* ReportPerformance(void* arg);

 private:
  State state_;
  BlockStreamBase* block_;
  unsigned long int tuplecount_;
  unsigned long long int start_;
  pthread_t report_tid_;

 private:
  PerformanceMonitor();
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};
}  // namespace physical_operator
}  // namespace claims
#endif  //  PHYSICAL_OPERATOR_PERFORMANCE_MONITOR_H_
