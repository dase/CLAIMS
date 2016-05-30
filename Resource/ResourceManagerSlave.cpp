/*
 * ResourceManagerSlave.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "ResourceManagerSlave.h"

#include <glog/logging.h>

#include "../Environment.h"
#include "../node_manager/base_node.h"
#include "caf/io/all.hpp"
#include "caf/all.hpp"
using caf::after;
using caf::io::remote_actor;
using claims::NodeAddr;
using claims::OkAtom;
using claims::StorageBudgetAtom;
InstanceResourceManager::InstanceResourceManager() {}

InstanceResourceManager::~InstanceResourceManager() {}

void InstanceResourceManager::ReportStorageBudget(
    StorageBudgetMessage& message) {
  caf::scoped_actor self;
  auto master_actor =
      Environment::getInstance()->get_slave_node()->GetMasterActor();
  self->sync_send(master_actor, StorageBudgetAtom::value, message).await(

      [=](OkAtom) { LOG(INFO) << "reporting storage budget is ok!" << endl; },
      after(std::chrono::seconds(30)) >>
          [=]() {
            LOG(WARNING) << "reporting storage budget, but timeout 30s !!"
                         << endl;
          });
}

void InstanceResourceManager::setStorageBudget(unsigned long memory,
                                               unsigned long disk) {}
