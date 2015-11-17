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
 * physical_operator/exchange_merger.h
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#ifndef PHYSICAL_OPERATOR_EXCHANGE_SENDER_MATERIALIZED_H_
#define PHYSICAL_OPERATOR_EXCHANGE_SENDER_MATERIALIZED_H_

#include <pthread.h>
#include <stdio.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <string>
#include "../../common/Schema/Schema.h"
#include "../../Executor/IteratorExecutorMaster.h"
#include "../../utility/lock.h"
#include "../../common/Block/PartitionedBlockBuffer.h"
#include "../../common/Block/BlockStream.h"
#include "../../common/Block/BlockStreamBuffer.h"
#include "../../common/Logging.h"
#include "../../common/partition_functions.h"
#include "../physical_operator/exchange_sender.h"

namespace claims {
namespace physical_operator {
/**
 * write block come from child into disk file, and send thread fetch block from
 * the file. others are similar to ExchangeSenderPipeline.
 */
class ExchangeSenderMaterialized : public ExchangeSender {
  friend class ExchangeMerger;

 public:
  struct State {
    Schema* schema_;
    PhysicalOperatorBase* child_;
    unsigned long long int exchange_id_;
    std::vector<NodeID> upper_id_list_;
    unsigned block_size_;
    PartitionOffset partition_offset_;
    partition_schema partition_schema_;
    State(Schema* schema, PhysicalOperatorBase* child,
          std::vector<NodeID> upper_id_list, unsigned block_size,
          unsigned long long int exchange_id = 0,
          partition_schema partition_key_index =
              partition_schema::set_hash_partition())
        : schema_(schema),
          child_(child),
          upper_id_list_(upper_id_list),
          block_size_(block_size),
          exchange_id_(exchange_id),
          partition_offset_(0),
          partition_schema_(partition_key_index) {}
    State(){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& child_& exchange_id_& upper_id_list_& block_size_&
          partition_offset_& partition_schema_;
    }
  };
  explicit ExchangeSenderMaterialized(State state);
  ExchangeSenderMaterialized();
  virtual ~ExchangeSenderMaterialized();
  bool Open(const PartitionOffset& part_off = 0);
  bool Next(BlockStreamBase* no_block);
  bool Close();

 private:
  void Send();
  bool Materialize();
  static void* MaterializeAndSend(void* arg);
  /* the thread for outputing debug information*/
  static void* debug(void* arg);

  /** random select one block from the partitioned files and return the
   * partition
   *  index. Return -1 if all the partitioned files reach EOF.
   */
  int GetBlockFromFile(Block& block);
  bool CreateWorkerThread();
  void CancelWorkerThread();
  void CloseDiskFiles();
  void DeleteDiskFiles();
  std::string GetPartititionedFileName(int partition_index) const;

 private:
  State state_;
  unsigned nuppers_;
  int* socket_fd_upper_list_;
  PartitionedBlockBuffer* partitioned_data_buffer_;

  /* one BlockStream for each uppers, the tuples from the child
   * iterator are fed to the cur_block_stream_list_ according to their
   * partition key.
   */
  BlockStreamBase** partitioned_block_stream_;
  Block* block_for_sending_;
  Block* block_for_serialization_;
  BlockStreamBase* block_stream_for_asking_;
  pthread_t sender_thread_id_;
  pthread_t debug_thread_id_;
  int* disk_fd_list_;
  volatile bool child_exhausted_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<ExchangeSender>(*this) & state_;
  }
};
}  // namespace physical_operator
}  // namespace claims
#endif  // PHYSICAL_OPERATOR_EXCHANGE_SENDER_MATERIALIZED_H_
