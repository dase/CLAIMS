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
 * /Claims/node_manager/slave_node.cpp
 *
 *  Created on: Jan 4, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */
#include <glog/logging.h>
#include <string>
#include <utility>
#include <iostream>
#include "./slave_node.h"

#include <stdlib.h>
#include <unistd.h>

#include "../common/Message.h"
#include "caf/io/all.hpp"

#include "./base_node.h"
#include "../common/ids.h"
#include "../Environment.h"
#include "../storage/StorageLevel.h"
#include "caf/all.hpp"
#include <map>
#include <unordered_map>
#include "../common/error_define.h"
using caf::io::remote_actor;
using caf::make_message;
using std::make_pair;
using std::unordered_map;
using claims::common::rConRemoteActorError;
using claims::common::rRegisterToMasterTimeOut;
using claims::common::rRegisterToMasterError;
namespace claims {
SlaveNode* SlaveNode::instance_ = 0;
class SlaveNodeActor : public event_based_actor {
 public:
  SlaveNodeActor(SlaveNode* slave_node) : slave_node_(slave_node) {}

  behavior make_behavior() override {
    LOG(INFO) << "slave node actor is OK!" << std::endl;
    return {
        [=](ExitAtom) {
          LOG(INFO) << "slave " << slave_node_->get_node_id() << " finish!"
                    << endl;
          quit();
        },
        [=](SendPlanAtom, string str) {
          PhysicalQueryPlan* new_plan = new PhysicalQueryPlan(
              PhysicalQueryPlan::TextDeserializePlan(str));
          Environment::getInstance()
              ->getIteratorExecutorSlave()
              ->createNewThreadAndRun(new_plan);
          string log_message =
              "Slave: received plan segment and create new thread and run it!";
          LOG(INFO) << log_message;
        },
        [=](AskExchAtom, ExchangeID exch_id) {
          auto addr =
              Environment::getInstance()->getExchangeTracker()->GetExchAddr(
                  exch_id);
          return make_message(OkAtom::value, addr.ip, addr.port);
        },
        [=](BindingAtom, const PartitionID partition_id,
            const unsigned number_of_chunks,
            const StorageLevel desirable_storage_level) {
          LOG(INFO) << "receive binding message!" << endl;
          Environment::getInstance()->get_block_manager()->AddPartition(
              partition_id, number_of_chunks, desirable_storage_level);
          return make_message(OkAtom::value);
        },
        [=](UnBindingAtom, const PartitionID partition_id) {
          LOG(INFO) << "receive unbinding message~!" << endl;
          Environment::getInstance()->get_block_manager()->RemovePartition(
              partition_id);
          return make_message(OkAtom::value);
        },
        [&](BroadcastNodeAtom, const unsigned int& node_id,
            const string& node_ip, const uint16_t& node_port) {
          //check if this node is reregister node
          unsigned int tmp_node_id = -1;
          bool is_reregister = false;
          for(auto it = slave_node_->node_id_to_addr_.begin();
              it != slave_node_->node_id_to_addr_.end(); ++it){
            if(it->second.first == node_ip){
              is_reregister = true;
              tmp_node_id = it->first;
            }
          }
          if(is_reregister){
            slave_node_->node_id_to_addr_.erase(tmp_node_id);
            slave_node_->node_id_to_actor_.erase(tmp_node_id);
            LOG(INFO)<<"slave "<<slave_node_->get_node_id()<<"remove old node :"<<tmp_node_id<<"info"<<endl;
          }
          slave_node_->AddOneNode(node_id, node_ip, node_port);
        },
        [=](ReportSegESAtom, NodeSegmentID node_segment_id, int exec_status,
            string exec_info) {
          bool ret =
              Environment::getInstance()
                  ->get_stmt_exec_tracker()
                  ->UpdateSegExecStatus(
                      node_segment_id,
                      (SegmentExecStatus::ExecStatus)exec_status, exec_info);
          if (false == ret) {
            return make_message(CancelPlanAtom::value);
          }
          return make_message(OkAtom::value);
        },
        [=](HeartBeatAtom){
          try{
            slave_node_->master_actor_= caf::io::remote_actor(slave_node_->master_addr_.first,slave_node_->master_addr_.second);
            sync_send(slave_node_->master_actor_, HeartBeatAtom::value, slave_node_->get_node_id(),
                      slave_node_->node_addr_.first,slave_node_->node_addr_.second).then(
                [=](OkAtom){
                  slave_node_->heartbeat_count_ = 0;
                },
                [=](OkAtom ,unsigned int node_id){
                  slave_node_->heartbeat_count_ = 0;
                  auto old_id = slave_node_->get_node_id();
                  slave_node_->set_node_id(node_id);
                  LOG(INFO)<<"slave node change id from"<<old_id<<"to "<<node_id<<endl;
                }
            );
          }catch(caf::network_error& e){
            LOG(WARNING) << "node"<<slave_node_->get_node_id()
                <<"can't send heartbeart to master"<<endl;
          }catch(caf::bind_failure& e){
            LOG(WARNING) << "node"<<slave_node_->get_node_id()
                <<"occur bind failure"<<endl;
          }

          slave_node_->heartbeat_count_++;
          delayed_send(this, std::chrono::seconds(kTimeout/10), HeartBeatAtom::value);
          if(slave_node_->heartbeat_count_ > kTimeout){
            LOG(INFO)<<"slave"<<slave_node_->node_id_<<"lost heartbeat from master, start register again"<<endl;
            bool is_success = false;
            become(
                caf::keep_behavior,
                [=, &is_success](RegisterAtom){
                  std::cerr<<"slave reregister"<<endl;
                  auto ret = slave_node_->reRegisterToMaster();
                  if (ret == rSuccess){
                    LOG(INFO)<<"reregister successfully turn to healthy!!"<<endl;
                    slave_node_->heartbeat_count_= 0;
                    is_success = true;
                    unbecome();
                  }else{
                    caf::scoped_actor self;
                    delayed_send(this,std::chrono::seconds(kTimeout/2),RegisterAtom::value);
                    LOG(WARNING)<<"register fail, slave will register in 2 seconds"<<endl;
                  }
                });
            if(!is_success)
            {
              caf::scoped_actor self;
              self->send(this,RegisterAtom::value);
            }
          }
        },
        [=](SyncNodeInfo, const BaseNode& node){
          slave_node_->node_id_to_addr_.clear();
          slave_node_->node_id_to_actor_.clear();
          slave_node_->node_id_to_addr_.insert(node.node_id_to_addr_.begin(),
                                                   node.node_id_to_addr_.end());
          for (auto it = slave_node_->node_id_to_addr_.begin();
              it != slave_node_->node_id_to_addr_.end(); ++it) {
            try {
              auto actor =
                remote_actor(it->second.first, it->second.second);
              slave_node_->node_id_to_actor_.insert(make_pair(it->first, actor));
            }catch (caf::network_error& e) {
              LOG(WARNING) << "cann't connect to node ( " <<it->first<< " , "<<it->second.first <<
                            it->second.second<< " ) and create remote actor failed!!";
            }
          }
          LOG(INFO) <<"node"<<slave_node_->get_node_id()
              <<"update nodelist info successfully"<<endl;
        },
        [=](OkAtom){
        },
        caf::others >>
            [=]() { LOG(WARNING) << "unknown message at slave node!!!" << endl; }

    };
  }

  SlaveNode* slave_node_;
};

SlaveNode* SlaveNode::GetInstance() {
  if (NULL == instance_) {
    instance_ = new SlaveNode();
  }
  return instance_;
}
RetCode SlaveNode::AddOneNode(const unsigned int& node_id,
                              const string& node_ip,
                              const uint16_t& node_port) {
  lock_.acquire();
  RetCode ret = rSuccess;
  node_id_to_addr_.insert(make_pair(node_id, make_pair(node_ip, node_port)));

  try {
    auto actor = remote_actor(node_ip, node_port);
    node_id_to_actor_.insert(make_pair(node_id, actor));
  } catch (caf::network_error& e) {
    LOG(WARNING) << "cann't connect to node ( " << node_ip << " , " << node_port
                 << " ) and create remote actor failed!!";
    ret = rConRemoteActorError;
  }
  LOG(INFO) << "slave : get broadested node( " << node_id << " < " << node_ip
            << " " << node_port << " > )" << std::endl;
  lock_.release();
  return rSuccess;
}
SlaveNode::SlaveNode() : BaseNode() {
  instance_ = this;
  CreateActor();
}
SlaveNode::SlaveNode(string node_ip, uint16_t node_port)
    : BaseNode(node_ip, node_port) {
  instance_ = this;
  CreateActor();
}
SlaveNode::~SlaveNode() { instance_ = NULL; }
void SlaveNode::CreateActor() {
  auto slave_actor = caf::spawn<SlaveNodeActor>(this);
  bool is_done = false;

  for (int try_time = 0; try_time < 20 && !is_done; ++try_time) {
    try {
      master_actor_ =
          caf::io::remote_actor(master_addr_.first, master_addr_.second);
      is_done = true;
    } catch (caf::network_error& e) {
      cout << "slave node connect remote_actor error due to network "
              "error! will try " << 19 - try_time << " times" << endl;
    }
    sleep(1);
  }
  if (!is_done) {
    cout << "Node(" << get_node_ip() << " , " << get_node_port()
         << ") register to master(" << master_addr_.first << " , "
         << master_addr_.second
         << ") failed after have tried 20 times! please check ip and "
            "port, then try again!!!" << endl;
    LOG(ERROR) << "Node(" << get_node_ip() << " , " << get_node_port()
               << ") register to master(" << master_addr_.first << " , "
               << master_addr_.second
               << ") failed after have tried 20 times! please check ip and "
                  "port, then try again!!!" << endl;
    exit(0);
  } else {
    LOG(INFO) << "the node connect to master succeed!!!";
    cout << "the node connect to master succeed!!!" << endl;
  }
  try {
    caf::io::publish(slave_actor, get_node_port(), nullptr, 1);
    LOG(INFO) << "slave node publish port " << get_node_port()
              << " successfully!";
  } catch (caf::bind_failure& e) {
    LOG(ERROR) << "slave node binds port error when publishing";
  } catch (caf::network_error& e) {
    LOG(ERROR) << "slave node publish error due to network error!";
  }
}

RetCode SlaveNode::RegisterToMaster() {
  RetCode ret = rSuccess;
  caf::scoped_actor self;
  try {
    LOG(INFO)<<"slave just RegisterToMaster!!"<<endl;
    self->sync_send(master_actor_, RegisterAtom::value, get_node_ip(),
                    get_node_port())
        .await([=](OkAtom, const unsigned int& id, const BaseNode& node) {
                 set_node_id(id);
                 heartbeat_count_ = 0;
                 node_id_to_addr_.insert(node.node_id_to_addr_.begin(),
                                         node.node_id_to_addr_.end());
                 for (auto it = node_id_to_addr_.begin();
                      it != node_id_to_addr_.end(); ++it) {
                   auto actor =
                       remote_actor(it->second.first, it->second.second);
                   node_id_to_actor_.insert(make_pair(it->first, actor));
                 }
                 LOG(INFO) << "register node succeed! insert "
                           << node.node_id_to_addr_.size() << " nodes";
                 caf::scoped_actor self1;
                 auto slave_self = caf::io::remote_actor(get_node_ip(), get_node_port());
                 self1->send(slave_self,HeartBeatAtom::value);
    },
               [&](const caf::sync_exited_msg& msg) {
                 LOG(WARNING) << "register link fail";
               },
               caf::after(std::chrono::seconds(kTimeout)) >>
                   [&]() {
                     ret = rRegisterToMasterTimeOut;
                     LOG(WARNING) << "slave register timeout!";
                   });
  } catch (caf::network_error& e) {
    ret = rRegisterToMasterError;
    LOG(WARNING) << "cann't connect to " << master_addr_.first << " , "
                 << master_addr_.second << " in register";
  }
  return ret;
}

RetCode SlaveNode::reRegisterToMaster() {
  RetCode ret = rSuccess;
  caf::scoped_actor self;
  try {
    LOG(INFO)<<"slave reRegisterToMaster!!"<<endl;
    master_actor_= caf::io::remote_actor(master_addr_.first,master_addr_.second);
    self->sync_send(master_actor_, RegisterAtom::value, get_node_ip(),
                    get_node_port())
        .await([=](OkAtom, const unsigned int& id, const BaseNode& node) {
                 set_node_id(id);
                 node_id_to_addr_.clear();
                 node_id_to_actor_.clear();
                 node_id_to_addr_.insert(node.node_id_to_addr_.begin(),
                                         node.node_id_to_addr_.end());
                 for (auto it = node_id_to_addr_.begin();
                      it != node_id_to_addr_.end(); ++it) {
                   auto actor =
                       remote_actor(it->second.first, it->second.second);
                   node_id_to_actor_.insert(make_pair(it->first, actor));
                 }
                 LOG(INFO) << "register node succeed! insert "
                           << node.node_id_to_addr_.size() << " nodes";
                 caf::scoped_actor self1;
                 auto slave_self = caf::io::remote_actor(get_node_ip(), get_node_port());
                 self1->send(slave_self,HeartBeatAtom::value);
               },
               [&](const caf::sync_exited_msg& msg) {
                 LOG(WARNING) << "register link fail";
               },
               caf::after(std::chrono::seconds(kTimeout)) >>
                   [&]() {
                     ret = rRegisterToMasterTimeOut;
                     LOG(WARNING) << "slave register timeout!";
                   });
  } catch (caf::network_error& e) {
    ret = rRegisterToMasterError;
    LOG(WARNING) << "cann't connect to " << master_addr_.first << " , "
                 << master_addr_.second << " in register";
  }
  return ret;
}
} /* namespace claims */
