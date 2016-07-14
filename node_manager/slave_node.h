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
 * /Claims/node_manager/slave_node.h
 *
 *  Created on: Jan 4, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef NODE_MANAGER_SLAVE_NODE_H_
#define NODE_MANAGER_SLAVE_NODE_H_
#include <string>
#include "./base_node.h"
#include "../common/error_define.h"
#include "caf/all.hpp"
#include "caf/behavior.hpp"
#include <string>
using caf::event_based_actor;
using std::string;
using caf::behavior;

namespace claims {
class SlaveNode : public BaseNode {
 public:
  friend class SlaveNodeActor;
  //  class SlaveNodeActor;
  SlaveNode();
  SlaveNode(string node_ip, uint16_t node_port);
  void CreateActor();
  virtual ~SlaveNode();
  RetCode RegisterToMaster();
  RetCode reRegisterToMaster();
  static SlaveNode* GetInstance();
  RetCode AddOneNode(const unsigned int& node_id, const string& node_ip,
                     const uint16_t& node_port);

 private:
  static SlaveNode* instance_;
  unsigned int heartbeat_count_;
};

}  // namespace claims

#endif  //  NODE_MANAGER_SLAVE_NODE_H_
