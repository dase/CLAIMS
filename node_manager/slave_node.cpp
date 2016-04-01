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
#include "../common/Message.h"
#include "caf/io/all.hpp"

#include "./base_node.h"
#include "../common/ids.h"
#include "../Environment.h"
#include "../storage/StorageLevel.h"
#include "caf/all.hpp"
#include <map>
using caf::make_message;
using std::make_pair;

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
          // should delete new_plan, but where to do it ???
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
          Environment::getInstance()->get_block_manager()->addPartition(
              partition_id, number_of_chunks, desirable_storage_level);
          return make_message(OkAtom::value);
        },
        [=](UnBindingAtom, const PartitionID partition_id) {
          LOG(INFO) << "receive unbinding message~!" << endl;
          Environment::getInstance()->get_block_manager()->removePartition(
              partition_id);
          return make_message(OkAtom::value);
        },
        [&](BroadcastNodeAtom, const unsigned int& node_id,
            const string& node_ip, const uint16_t& node_port) {
          slave_node_->node_id_to_addr_.insert(
              make_pair(node_id, make_pair(node_ip, node_port)));
        },
        [=](ReportSegESAtom, NodeSegmentID node_segment_id,
            SegmentExecStatus::ExecStatus exec_status, string exec_info) {
          bool ret = Environment::getInstance()
                         ->get_stmt_exec_tracker()
                         ->UpdateSegExecStatus(node_segment_id,
                                                      exec_status, exec_info);
          if (false == ret) {
            return make_message(CancelPlanAtom::value);
          }
          return make_message(OkAtom::value);
        },
        caf::others >>
            [=]() { LOG(WARNING) << "unkown message at slave node!!!" << endl; }

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
  node_id_to_addr_.insert(make_pair(node_id, make_pair(node_ip, node_port)));
  LOG(INFO) << "slave : get broadested node( " << node_id << " < " << node_ip
            << " " << node_port << " > )" << std::endl;
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
  try {
    caf::io::publish(slave_actor, get_node_port());
  } catch (caf::bind_failure& e) {
  } catch (caf::network_error& e) {
  }
}

RetCode SlaveNode::RegisterToMaster() {
  RetCode ret = 0;
  caf::scoped_actor self;
  try {
    auto master_actor =
        caf::io::remote_actor(master_addr_.first, master_addr_.second);
    self->sync_send(master_actor, RegisterAtom::value, get_node_ip(),
                    get_node_port())
        .await([=](OkAtom, const unsigned int& id, const BaseNode& node) {
                 set_node_id(id);
                 node_id_to_addr_.insert(node.node_id_to_addr_.begin(),
                                         node.node_id_to_addr_.end());
                 LOG(INFO) << "register node succeed! intsert "
                           << node.node_id_to_addr_.size() << " nodes";
               },
               [&](const caf::sync_exited_msg& msg) {
                 LOG(WARNING) << "register link fail";
               },
               caf::after(std::chrono::seconds(kTimeout)) >>
                   [&]() {
                     ret = -1;
                     LOG(WARNING) << "slave register timeout!";
                   });
  } catch (caf::network_error& e) {
    ret = -1;
    LOG(WARNING) << "cann't connect to " << master_addr_.first << " , "
                 << master_addr_.second << " in register";
  }
  return ret;
}

} /* namespace claims */
