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
 *  Created on: Sep 4, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */

#ifndef PHYSICAL_OPERATOR_EXCHANGE_SENDER_H_
#define PHYSICAL_OPERATOR_EXCHANGE_SENDER_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <string>
#include "../physical_operator/physical_operator_base.h"
#include "../common/ids.h"
#include "../common/Logging.h"

namespace claims {
namespace physical_operator {
/**
 * ExchangeSender is the base class of ExchangeSenderPipeline and
 * ExchangeSenderMaterialized.
 */
class ExchangeSender : public PhysicalOperatorBase {
 public:
  ExchangeSender();
  virtual ~ExchangeSender();
  virtual bool Open(const PartitionOffset& part_off = 0) = 0;
  virtual bool Next(BlockStreamBase* no_block) = 0;
  virtual bool Close() = 0;

 protected:
  // build socket connection with upper mergers
  bool ConnectToUpper(const ExchangeID& exchange_id, const NodeID& id,
                      int& sock_fd) const;
  // wait info from upper mergers.
  void WaitingForNotification(const int& target_socket_fd) const;
  // wait close info from upper mergers
  void WaitingForCloseNotification(const int& target_socket_fd) const;
  unsigned GetHashPartitionId(void* input_tuple, Schema* schema,
                              unsigned partition_key_index, unsigned nuppers);

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this);
  }
};
}  // namespace physical_operator
}  // namespace claims
#endif  // PHYSICAL_OPERATOR_EXCHANGE_SENDER_H_
