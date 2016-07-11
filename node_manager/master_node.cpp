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
#include <vector>
#include <iostream>
#include "caf/all.hpp"
#include "caf/io/all.hpp"
#include "../common/error_define.h"
#include "../common/ids.h"
#include "../common/Message.h"
#include "../Environment.h"
using caf::io::remote_actor;
using caf::make_message;
using std::make_pair;
using claims::common::rConRemoteActorError;
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
        [=](HeartBeatAtom, unsigned int node_id_) -> caf::message {
          auto it = master_node_->node_id_to_heartbeat_.find(node_id_);
          if (it != master_node_->node_id_to_heartbeat_.end()){
              it->second = 0;
              cout<<"master get heartbeat from node :"<<node_id_<<endl;
            }
          return make_message(OkAtom::value);
        },
        [=](Updatelist){
          LOG(INFO) <<"master scan list"<<endl;
          if(master_node_->node_id_to_heartbeat_.size() > 0){
            for (auto it = master_node_->node_id_to_heartbeat_.begin();
                it != master_node_->node_id_to_heartbeat_.end(); ++it){
                  it->second++;
                  if (it->second >= kMaxTryTimes){
                    LOG(WARNING) <<"master : lost hearbeat from ( node "
                        <<it->first<<")"<<endl;
                    //TODO add remove dealing and broadcasting it
                    master_node_->RemoveOneNode(it->first, master_node_);
                  }
            }
          }
          delayed_send(this, std::chrono::seconds(kTimeout/5), Updatelist::value);
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
  master_actor_ = caf::spawn<MasterNodeActor>(this);
  try {
    caf::io::publish(master_actor_, get_node_port(), nullptr, 1);
    LOG(INFO) << "master ip port" << get_node_port() << " publish succeed!";
    caf::scoped_actor self;
    self->send(master_actor_,Updatelist::value);
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
    self->send(node_id_to_actor_.at(it->first), BroadcastNodeAtom::value,
               node_id, node_ip, node_port);
  }
  return rSuccess;
}
// should be atomic
unsigned int MasterNode::AddOneNode(string node_ip, uint16_t node_port) {
  lock_.acquire();
  node_id_gen_++;
  BroastNodeInfo((unsigned int)node_id_gen_, node_ip, node_port);
  node_id_to_addr_.insert(
      make_pair((unsigned int)node_id_gen_, make_pair(node_ip, node_port)));
  node_id_to_heartbeat_.insert(make_pair((unsigned int)node_id_gen_, 0));
  try {
    auto actor = remote_actor(node_ip, node_port);
    node_id_to_actor_.insert(make_pair((unsigned int)node_id_gen_, actor));
  } catch (caf::network_error& e) {
    LOG(WARNING) << "cann't connect to node ( " << node_ip << " , " << node_port
                 << " ) and create remote actor failed!!";
    assert(false);
  }
  LOG(INFO) << "slave : register one node( " << node_id_gen_ << " < " << node_ip
            << " " << node_port << " > )" << std::endl;
  lock_.release();
  return node_id_gen_;
}
void MasterNode::RemoveOneNode(unsigned int node_id, MasterNode* master_node){
  node_id_to_heartbeat_.erase(node_id);
  node_id_to_addr_.erase(node_id);
  node_id_to_actor_.erase(node_id);
  caf::scoped_actor self;
  for (auto it = node_id_to_addr_.begin(); it != node_id_to_addr_.end(); ++it) {
      self->send(node_id_to_actor_.at(it->first), SyncNodeInfo::value,
                 *((BaseNode*)master_node));
    }
}
void MasterNode::FinishAllNode() {
  caf::scoped_actor self;
  for (auto it = node_id_to_actor_.begin(); it != node_id_to_actor_.end();
       ++it) {
    self->send(it->second, ExitAtom::value);
  }
  self->send(master_actor_, ExitAtom::value);
}
}  // namespace claims
