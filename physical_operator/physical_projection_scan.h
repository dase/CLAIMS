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
 * /CLAIMS/physical_operator/physical_projection_scan.h
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli,Hanzhang
 *		   Email: wangli1426@gmail.com
 *
 * Description: Implementation of Scan operator in physical layer. In the
 * current implementation, for simplicity, we assume that the underlying storage
 * is arranged in blocks, each of which is the same as the size of the block in
 * the parameter of the next function.
 * TODO(wangli): adjust or rewrite this iterator if the consumption above does
 * not hold.
 *
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_PROJECTION_SCAN_H_
#define PHYSICAL_OPERATOR_PHYSICAL_PROJECTION_SCAN_H_

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <glog/logging.h>
#include <string>
#include <list>
#include "../common/error_no.h"
#include "../physical_operator/physical_operator_base.h"
#include "../common/Schema/Schema.h"
#include "../storage/ChunkStorage.h"
#include "../storage/PartitionStorage.h"
#include "../physical_operator/physical_operator.h"
#include "../common/ExpandedThreadTracker.h"

namespace claims {
namespace physical_operator {

typedef std::list<ChunkReaderIterator::block_accessor*> assigned_data;
/**
 * @brief Method description: As a buffer for input.
 */

struct input_dataset {
  assigned_data input_data_blocks_;
  SpineLock lock;
  bool AtomicGet(assigned_data& target, unsigned number_of_block) {
    lock.acquire();
    bool not_empty = !target.empty();
    while (number_of_block-- && (!input_data_blocks_.empty())) {
      target.push_back(input_data_blocks_.front());
      input_data_blocks_.pop_front();
    }
    lock.release();
    return not_empty;
  }
  void AtomicPut(assigned_data blocks) {
    lock.acquire();
    for (assigned_data::iterator it = blocks.begin(); it != blocks.end(); it++)
      input_data_blocks_.push_front(*it);
    lock.release();
  }
};

/**
 * Implementation of Scan operator in physical layer. Get blocks for Storage
 * medium.  In the current implementation, for simplicity, the underlying
 * storage is arranged in blocks, each of which is the same as the size of the
 * block in the parameter of the next function. Actually, read chunks from
 * partition, read blocks from chunk.
 */
class PhysicalProjectionScan : public PhysicalOperator {
 public:
  class ScanThreadContext : public ThreadContext {
   public:
    ~ScanThreadContext(){};
    assigned_data assigned_data_;
  };

  //  struct allocated_block {
  //    char* start;
  //    unsigned length;
  //  };
  class State {
    friend class PhysicalProjectionScan;

   public:
    State(ProjectionID projection_id, Schema* schema, unsigned block_size,
          float sample_rate = 1);
    State(){};

   public:
    Schema* schema_;
    ProjectionID projection_id_;
    unsigned block_size_;
    float sample_rate_;
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& projection_id_& block_size_& sample_rate_;
    }
  };
  PhysicalProjectionScan(State state);
  PhysicalProjectionScan();
  virtual ~PhysicalProjectionScan();
  /**
   * @brief Method description: Initialize the operator and get the initial
   * position of chunk read iterator.
   */
  bool Open(const PartitionOffset& partition_offset = 0);

  /**
   * @brief: fetch block from child operator.
   */

  bool Next(BlockStreamBase* block);
  /**
   * @brief: revoke resource.
   */
  bool Close();
  void Print();

 private:
  bool PassSample() const;

 private:
  State state_;
  PartitionStorage::PartitionReaderItetaor* partition_reader_iterator_;
  std::list<ChunkReaderIterator*> remaining_chunk_iterator_list_;
  Lock chunk_reader_container_lock_;
  // like a buffer
  input_dataset input_dataset_;

  const PerformanceInfo* kPerfInfo;

  // The following code is for boost serialization.
 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

}  // namespace physical_operator
}  // namespace claims

#endif  //  PHYSICAL_OPERATOR_PHYSICAL_PROJECTION_SCAN_H_
