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
 * /Claims/node_manager/master_node.h
 *
 *  Created on: Jan 4, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef NODE_MANAGER_MASTER_NODE_H_
#define NODE_MANAGER_MASTER_NODE_H_
#include <glog/logging.h>
#include <atomic>
#include <iosfwd>
#include <map>
#include <string>
#include <utility>
#include "caf/all.hpp"
#include "caf/io/all.hpp"
#include <vector>

#include "./base_node.h"
#include "../common/error_define.h"
#include "../common/ids.h"
using caf::behavior;
using caf::event_based_actor;
using std::atomic_uint;
using std::map;
using std::pair;
using std::string;
using std::vector;
namespace claims {

class MasterNode : public BaseNode {
 public:
  friend class MasterNodeActor;
  //  class MasterNodeActor;
  static MasterNode* GetInstance();
  virtual ~MasterNode();
  void PrintNodeList();
  void FinishAllNode();
  unsigned int AddOneNode(string node_ip, uint16_t node_port);
  void SyncNodeList(MasterNode* master_node);
  void RemoveOneNode(unsigned int node_id, MasterNode* master_node);
  RetCode BroastNodeInfo(const unsigned int& node_id, const string& node_ip,
                         const uint16_t& node_port);
  MasterNode();
  MasterNode(string node_ip, uint16_t node_port);

 private:
  void CreateActor();

 private:
  static MasterNode* instance_;
  atomic_uint node_id_gen_;
  std::unordered_map<unsigned int, int> node_id_to_heartbeat_;
};

}  // namespace claims


#endif  //  NODE_MANAGER_MASTER_NODE_H_
