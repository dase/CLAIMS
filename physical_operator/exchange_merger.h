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

#ifndef PHYSICAL_OPERATOR_EXCHANGE_MERGER_H_
#define PHYSICAL_OPERATOR_EXCHANGE_MERGER_H_

#include <pthread.h>
#include <assert.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <string>
#include <map>
#include "../physical_operator/physical_operator_base.h"
#include "../utility/lock.h"
#include "../Executor/IteratorExecutorMaster.h"
#include "../common/Logging.h"
#include "../common/ExpandedThreadTracker.h"
#include "../common/partition_functions.h"
#include "../common/Schema/Schema.h"
#include "../common/Block/BlockContainer.h"
#include "../common/Block/PartitionedBlockBuffer.h"
#include "../common/Block/BlockStream.h"
#include "../common/Block/BlockStreamBuffer.h"
#include "../physical_operator/physical_operator.h"
namespace claims {
namespace physical_operator {
/**
 * ExchangeMerger is mainly for
 * 1. encapsulating PhysicalExchangeSender on the top of its
 * child_physical_plan, serializing the new child_physical_plan and sending it
 * to all low_ip_node;
 * 2. merging all data that comes from all senders through
 * socket connection.
 */
class ExchangeMerger : public PhysicalOperator {
 public:
  struct State {
    Schema* schema_;
    PhysicalOperatorBase* child_;
    unsigned block_size_;
    unsigned long long int exchange_id_;
    std::vector<NodeID> lower_id_list_;
    std::vector<NodeID> upper_id_list_;
    partition_schema partition_schema_;
    State(Schema* schema, PhysicalOperatorBase* child, unsigned block_size,
          std::vector<NodeID> lower_id_list, std::vector<NodeID> upper_id_list,
          unsigned long long int exchange_id,
          partition_schema partition_index =
              partition_schema::set_hash_partition())
        : schema_(schema),
          child_(child),
          block_size_(block_size),
          exchange_id_(exchange_id),
          lower_id_list_(lower_id_list),
          upper_id_list_(upper_id_list),
          partition_schema_(partition_index) {}
    State(){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& child_& block_size_& exchange_id_& lower_id_list_&
          upper_id_list_& partition_schema_;
    }
  };
  explicit ExchangeMerger(State state);
  ExchangeMerger();
  virtual ~ExchangeMerger();
  /**
   * 1.prepare block_for_socket for every socket and all_merged_block_buffer;
   * 2. prepare socket port, which senders connect;
   * 3. register exchange_id and socket_port into ExchangeTracker
   * for master role among the same level segments
   *  a. waiting every segment arrive this point;
   *  b. serialize next segment of plan and send to every lower senders;
   *
   * 4. create receive thread, if one block is enough, then will be put into
   * all_merged_block_buffer
   */
  bool Open(const PartitionOffset& partition_offset = 0);
  /// fetch block from all_merged_block_buffer and return.
  bool Next(BlockStreamBase* block);
  bool Close();
  void Print();

 private:
  /// prepare socket at this node, waiting senders connect it
  bool PrepareSocket();
  /** register current exchange to exchange tracker of this node, including the
   * sock_port, so senders can ask exchange tracker of this node about the
   * sock_port, then connect to this merger.
   */
  bool RegisterExchange();
  /** all upper mergers should synchronize at one point before the master role
   * of upper mergers sends next plan segment to lower senders.
   */
  bool IsOtherMergersRegistered();
  /// check whether this node is master, then do the work of master role.
  bool IsMaster();
  /// serialize the next child plan and sent it to all lower senders.
  bool SerializeAndSendPlan();
  bool CreateReceiverThread();
  void CancelReceiverThread();
  void SendBlockBufferedNotification(int target_socket_fd);
  /// reply to the Sender that this merger has consumed its blocks.
  void ReplyAllBlocksConsumed(int target_socket_fd);
  void CloseSocket();
  bool SetSocketNonBlocking(int socket_fd);
  void CreatePerformanceInfo();
  /**
   * this function is called to reset status of the iterator instance, so that
   * the following calling of open() and next() can act correctly.
   */
  void ResetStatus();

  static void* Receiver(void* arg);
  static void* debug(void* arg);

 private:
  State state_;
  BlockStreamBuffer* all_merged_block_buffer_;
  BlockContainer** block_for_socket_;
  BlockStreamBase* block_for_deserialization;
  int sock_fd_;
  unsigned socket_port_;
  unsigned lower_num_;
  int epoll_fd_;
  int* socket_fd_lower_list_;
  std::vector<std::string> lower_ip_list_;
  pthread_t receiver_thread_id_;
  pthread_t debug_thread_id_;
  unsigned exhausted_lowers;
  unsigned partition_offset_;
  semaphore sem_new_block_or_eof_;
  std::map<int, int> lower_sock_fd_to_id_;
  PerformanceInfo* perf_info_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }

 private:
  // debug
  //  unsigned debug_consumed_block[100];
  //  unsigned debug_received_block[100];
  //  unsigned debug_winner_thread;
};
}  // namespace physical_operator
}  // namespace claims
#endif  // PHYSICAL_OPERATOR_EXCHANGE_MERGER_H_
