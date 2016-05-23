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
 * /Claims/node_manager/master_node.cpp
 *
 *  Created on: Jan 4, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include "./master_node.h"

#include <glog/logging.h>
#include <pthread.h>
#include <string>
#include <utility>
#include <iostream>
#include "caf/all.hpp"
#include "caf/io/all.hpp"
#include <vector>

#include "../common/ids.h"
#include "../common/Message.h"
#include "../Environment.h"
using caf::io::remote_actor;
using caf::make_message;
using std::make_pair;
namespace claims {
MasterNode* MasterNode::instance_ = 0;
class MasterNodeActor : public event_based_actor {
 public:
  MasterNodeActor(MasterNode* master_node) : master_node_(master_node) {}

  behavior make_behavior() override {
    become(MainWork());
    return {};
  }
  behavior MainWork() {
    return {

        [=](RegisterAtom, string ip, uint16_t port) -> caf::message {
          unsigned id = master_node_->AddOneNode(ip, port);
          Environment::getInstance()
              ->getResourceManagerMaster()
              ->RegisterNewSlave(id);
          return make_message(OkAtom::value, id, *((BaseNode*)master_node_));
        },
        [&](StorageBudgetAtom, const StorageBudgetMessage& message) {
          Environment::getInstance()
              ->getResourceManagerMaster()
              ->RegisterDiskBuget(message.nodeid, message.disk_budget);
          Environment::getInstance()
              ->getResourceManagerMaster()
              ->RegisterMemoryBuget(message.nodeid, message.memory_budget);
          LOG(INFO) << "receive storage budget message!! node: "
                    << message.nodeid << " : disk = " << message.disk_budget
                    << " , mem = " << message.memory_budget << endl;
          return make_message(OkAtom::value);
        },
        [=](ExitAtom) {
          LOG(INFO) << "master " << master_node_->get_node_id() << " finish!"
                    << endl;
          quit();
        },
        caf::others >> [=]() {
                         LOG(WARNING) << "master node receives unkown message"
                                      << endl;
                       }

    };
  }
  MasterNode* master_node_;
};
MasterNode* MasterNode::GetInstance() {
  if (NULL == instance_) {
    instance_ = new MasterNode();
  }
  return instance_;
}

MasterNode::MasterNode() : node_id_gen_(-1) {
  instance_ = this;
  set_node_id(0);
  ReadMasterAddr();
  node_addr_ = master_addr_;
  CreateActor();
}

MasterNode::MasterNode(string node_ip, uint16_t node_port)
    : BaseNode(node_ip, node_port), node_id_gen_(-1) {
  CreateActor();
}

MasterNode::~MasterNode() { instance_ = NULL; }
void MasterNode::CreateActor() {
  auto master_actor = caf::spawn<MasterNodeActor>(this);
  try {
    caf::io::publish(master_actor, get_node_port());
    LOG(INFO) << "master ip port publish succeed!";
  } catch (caf::bind_failure& e) {
    LOG(ERROR) << "the specified port " << get_node_port() << " is used!";
  } catch (caf::network_error& e) {
    LOG(ERROR) << "connection error in publishing master actor port";
  }
}
void MasterNode::PrintNodeList() {
  for (auto it = node_id_to_addr_.begin(); it != node_id_to_addr_.end(); ++it) {
    std::cout << "node id = " << it->first << " ( " << it->second.first << " , "
              << it->second.second << " )" << std::endl;
  }
}
RetCode MasterNode::BroastNodeInfo(const unsigned int& node_id,
                                   const string& node_ip,
                                   const uint16_t& node_port) {
  caf::scoped_actor self;
  for (auto it = node_id_to_addr_.begin(); it != node_id_to_addr_.end(); ++it) {
    auto node_actor = remote_actor(it->second.first, it->second.second);
    self->send(node_actor, BroadcastNodeAtom::value, node_id, node_ip,
               node_port);
  }
  return rSuccess;
}
// should be atomic
unsigned int MasterNode::AddOneNode(string node_ip, uint16_t node_port) {
  node_id_gen_++;
  BroastNodeInfo((unsigned int)node_id_gen_, node_ip, node_port);
  node_id_to_addr_.insert(
      make_pair((unsigned int)node_id_gen_, make_pair(node_ip, node_port)));
  LOG(INFO) << "slave : register one node( " << node_id_gen_ << " < " << node_ip
            << " " << node_port << " > )" << std::endl;
  return node_id_gen_;
}

void MasterNode::FinishAllNode() {
  caf::scoped_actor self;
  for (auto it = node_id_to_addr_.begin(); it != node_id_to_addr_.end(); ++it) {
    auto pong = remote_actor(it->second.first.c_str(), it->second.second);
    self->send(pong, ExitAtom::value);
  }
  auto pong = remote_actor(master_addr_.first.c_str(), master_addr_.second);
  self->send(pong, ExitAtom::value);
}
}  // namespace claims
