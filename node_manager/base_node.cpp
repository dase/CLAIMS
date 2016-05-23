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
 * /Claims/node_manager/base_node.cpp
 *
 *  Created on: Jan 4, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include "./base_node.h"

#include <libconfig.h++>
#include <string>
#include <utility>
#include <vector>

#include "../Config.h"
#include "../Executor/PortManager.h"
using std::make_pair;
using std::string;
using std::vector;
namespace claims {

BaseNode::BaseNode() : node_id_(-1) {
  ReadNodeAddr();
  ReadMasterAddr();
}
BaseNode::BaseNode(string node_ip, uint16_t node_port)
    : node_addr_(make_pair(node_ip, node_port)), node_id_(-1) {}
BaseNode::~BaseNode() {}

NodeAddr BaseNode::GetNodeAddr() { return node_addr_; }

void BaseNode::ReadNodeAddr() {
  libconfig::Config cfg;
  cfg.readFile(Config::config_file.c_str());
  string ip = (const char *)cfg.lookup("ip");
  node_addr_ = make_pair(ip, PortManager::getInstance()->applyPort());
}

NodeAddr BaseNode::GetMasterAddr() { return master_addr_; }

void BaseNode::ReadMasterAddr() {
  libconfig::Config cfg;
  cfg.readFile(Config::config_file.c_str());
  std::string master_ip = (const char *)cfg.lookup("coordinator.ip");
  std::string master_port = (const char *)cfg.lookup("coordinator.port");
  master_addr_ = make_pair(master_ip, std::atoi(master_port.c_str()));
}
NodeAddr BaseNode::GetNodeAddrFromId(const unsigned int &id) {
  if (node_id_to_addr_.find(id) != node_id_to_addr_.end()) {
    return node_id_to_addr_[id];
  } else {
    return NodeAddr("0", 0);
  }
}
vector<NodeID> BaseNode::GetAllNodeID() {
  vector<NodeID> all_node_id;
  all_node_id.clear();
  for (auto it = node_id_to_addr_.begin(); it != node_id_to_addr_.end(); ++it) {
    all_node_id.push_back(it->first);
  }
  return all_node_id;
}
}  // namespace claims
