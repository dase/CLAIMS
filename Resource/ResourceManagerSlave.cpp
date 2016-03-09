/*
 * ResourceManagerSlave.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "ResourceManagerSlave.h"
#include "../Environment.h"
#include "../common/TimeOutReceiver.h"
#include "../node_manager/base_node.h"
#include "caf/io/all.hpp"

#include "caf/all.hpp"
using caf::after;
using caf::io::remote_actor;
using claims::NodeAddr;
using claims::OkAtom;
using claims::StorageBudgetAtom;
#define ResourceManagerMasterName "ResourceManagerMaster"
InstanceResourceManager::InstanceResourceManager() {
  framework_ =
      new Theron::Framework(*Environment::getInstance()->getEndPoint());
  logging_ = new ResourceManagerMasterLogging();
}

InstanceResourceManager::~InstanceResourceManager() {
  delete framework_;
  delete logging_;
}
NodeID InstanceResourceManager::Register() {
#ifdef THERON
  NodeID ret = 10;
  TimeOutReceiver receiver(Environment::getInstance()->getEndPoint());
  Theron::Catcher<NodeID> resultCatcher;
  receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<NodeID>::Push);

  std::string ip = Environment::getInstance()->getIp();
  unsigned port = Environment::getInstance()->getPort();
  NodeRegisterMessage message(ip, port);

  framework_->Send(message, receiver.GetAddress(),
                   Theron::Address("ResourceManagerMaster"));
  Theron::Address from;
  if (receiver.TimeOutWait(1, 1000) == 1) {
    resultCatcher.Pop(ret, from);
    logging_->log(
        "Successfully registered to the master, the allocated id =%d.", ret);
    return ret;
  } else {
    logging_->elog("Failed to get NodeId from the master.");
    return -1;
  }
//#else
//  std::string ip = Environment::getInstance()->getIp();
//  unsigned port = Environment::getInstance()->getPort();
//  NodeAddr node_addr(ip, (u_int16_t)port);
//  caf::scoped_actor self;
//  auto master_addr =
//      Environment::getInstance()->get_master_node()->GetMasterAddr();
//  auto master_actor = remote_actor(master_addr.first, master_addr.second);
//  self->sync_send(master_actor, RRegisterAtom::value, node_addr).await(
//      [=](OkAtom) { cout << "resource register ok!" << endl; },
//      after(std::chrono::seconds(30)) >>
//          [=]() {
//            LOG(WARNING) << "asking exchange connection info, but timeout
//            30s!!"
//                         << endl;
//            return -1;
//          }
//
//      );

#endif
}
void InstanceResourceManager::ReportStorageBudget(
    StorageBudgetMessage& message) {
#ifdef THERON
  framework_->Send(message, Theron::Address(),
                   Theron::Address(ResourceManagerMasterName));
#else
  caf::scoped_actor self;
  auto master_addr =
      Environment::getInstance()->get_master_node()->GetMasterAddr();
  auto master_actor = remote_actor(master_addr.first, master_addr.second);
  self->sync_send(master_actor, StorageBudgetAtom::value, message).await(

      [=](OkAtom) { cout << "reporting storage budget is ok!" << endl; },
      after(std::chrono::seconds(30)) >>
          [=]() {
            LOG(WARNING) << "reporting storage budget, but timeout 30s !!"
                         << endl;
          });

#endif
}

void InstanceResourceManager::setStorageBudget(unsigned long memory,
                                               unsigned long disk) {}
