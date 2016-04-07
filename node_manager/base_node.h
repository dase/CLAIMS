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
 * /Claims/node_manager/base_node.h
 *
 *  Created on: Jan 4, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef NODE_MANAGER_BASE_NODE_H_
#define NODE_MANAGER_BASE_NODE_H_
#include "./base_node.h"

#include <string>
#include <utility>
#include <vector>
#include <map>
#include "caf/all.hpp"
#include "caf/io/all.hpp"

#include "../common/ids.h"
using std::pair;
using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;
namespace claims {
using OkAtom = caf::atom_constant<caf::atom("ok")>;
using RegisterAtom = caf::atom_constant<caf::atom("register")>;
using ExitAtom = caf::atom_constant<caf::atom("exit")>;
using SendPlanAtom = caf::atom_constant<caf::atom("send_plan")>;
using AskExchAtom = caf::atom_constant<caf::atom("ask_exch")>;
using BindingAtom = caf::atom_constant<caf::atom("binding")>;
using UnBindingAtom = caf::atom_constant<caf::atom("unbinding")>;
using StorageBudgetAtom = caf::atom_constant<caf::atom("storage")>;
using BroadcastNodeAtom = caf::atom_constant<caf::atom("brdst_node")>;
using ReportSegESAtom = caf::atom_constant<caf::atom("rept_sts")>;
using CheckStmtESAtom = caf::atom_constant<caf::atom("ck_stmtes")>;
using CancelPlanAtom = caf::atom_constant<caf::atom("cancel")>;

const int kTimeout = 5;
class MemoryInfo {};
class DiskInfo {};
typedef pair<string, uint16_t> NodeAddr;
class BaseNode {
 public:
  BaseNode();
  BaseNode(string node_ip, uint16_t node_port);
  virtual ~BaseNode();
  void set_node_id(unsigned int node_id) { node_id_ = node_id; }
  unsigned int get_node_id() { return node_id_; }
  string get_node_ip() { return node_addr_.first; }
  uint16_t get_node_port() { return node_addr_.second; }
  NodeAddr GetNodeAddr();
  void ReadNodeAddr();
  NodeAddr GetMasterAddr();
  void ReadMasterAddr();
  NodeAddr GetNodeAddrFromId(const unsigned int& id);
  vector<NodeID> GetAllNodeID();

  bool operator==(const BaseNode& r) const {
    if (r.node_id_to_addr_.size() != node_id_to_addr_.size()) {
      return false;
    }
    auto ita = r.node_id_to_addr_.begin();
    auto itb = node_id_to_addr_.begin();
    for (; itb != node_id_to_addr_.end(); ++ita, ++itb) {
      if (ita->first != itb->first || ita->second.first != itb->second.first ||
          ita->second.second != itb->second.second) {
        return false;
      }
    }
    return true;
  }

 protected:
  unsigned int node_id_;
  NodeAddr node_addr_;
  NodeAddr master_addr_;

 public:
  map<int, pair<string, uint16_t>> node_id_to_addr_;
};

}  // namespace claims

#endif  //  NODE_MANAGER_BASE_NODE_H_
