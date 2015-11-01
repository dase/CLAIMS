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

#ifndef PHYSICAL_OPERATOR_EXCHANGE_SENDER_PIPELINE_H_
#define PHYSICAL_OPERATOR_EXCHANGE_SENDER_PIPELINE_H_

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <string>
#include <map>
#include <iostream>
#include "../../common/Schema/Schema.h"
#include "../../Executor/IteratorExecutorMaster.h"
#include "../../common/Block/PartitionedBlockBuffer.h"
#include "../../common/Block/BlockStream.h"
#include "../../common/Block/BlockStreamBuffer.h"
#include "../../common/Block/PartitionedBlockContainer.h"
#include "../../common/hash.h"
#include "../../common/Logging.h"
#include "../../common/partition_functions.h"
#include "../physical_operator/exchange_sender.h"
#include "../physical_operator/physical_operator_base.h"

namespace claims {
namespace physical_operator {
/**
 * get one block from child and send to mergers immediately during one active
 * pipeline
 */
class ExchangeSenderPipeline : public ExchangeSender {
 public:
  friend class ExchangeMerger;
  struct State {
    Schema* schema_;
    PhysicalOperatorBase* child_;
    unsigned long long int exchange_id_;
    std::vector<NodeID> upper_id_list_;
    unsigned block_size_;
    partition_schema partition_schema_;
    PartitionOffset partition_offset_;
    State(Schema* schema, PhysicalOperatorBase* child,
          std::vector<NodeID> upper_id_list, unsigned block_size,
          unsigned long long int exchange_id = 0,
          partition_schema partition_schema =
              partition_schema::set_hash_partition())
        : schema_(schema),
          child_(child),
          upper_id_list_(upper_id_list),
          block_size_(block_size),
          exchange_id_(exchange_id),
          partition_schema_(partition_schema),
          partition_offset_(0) {}
    State(){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& child_& exchange_id_& upper_id_list_& block_size_&
          partition_schema_& partition_offset_;
    }
  };
  explicit ExchangeSenderPipeline(State state);
  ExchangeSenderPipeline();
  virtual ~ExchangeSenderPipeline();
  /**
   * 1.prepare some buffers that store blocks for different partition.
   * 2.build socket connection with uppder mergers
   * 3.create sender thread that sends blocks to different upper mergers.
   */
  bool Open(const PartitionOffset& partition_offset = 0);
  /**
   * divide block that come from child and insert them into corresponding
   * partition buffer
   */
  bool Next(BlockStreamBase* no_block);
  bool Close();

 private:
  /**
   * fetch one block from buffer which Next() insert blocks into, and send to
   * corresponding merger through socket connection.
   */
  static void* Sender(void* arg);
  void CancelSenderThread();

 private:
  State state_;
  unsigned upper_num_;
  int* socket_fd_upper_list_;
  PartitionedBlockBuffer* partitioned_data_buffer_;
  BlockStreamBase** partitioned_block_stream_;
  BlockContainer* block_for_sending_;
  BlockContainer* block_for_sending_buffer_;
  PartitionedBlockContainer* sending_buffer_;
  Block* block_for_serialization_;
  BlockStreamBase* block_for_asking_;
  pthread_t sender_thread_id_;
  pthread_t debug_thread_id_;
  PartitionFunction* partition_function_;
  unsigned sendedblocks_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};
}  // namespace physical_operator
}  // namespace claims
#endif  // PHYSICAL_OPERATOR_EXCHANGE_SENDER_PIPELINE_H_
