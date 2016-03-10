/*
 * exchange_tracker.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: wangli
 */

#include "./exchange_tracker.h"

#include <glog/logging.h>
#include <string>
#include <sstream>
#include "../Environment.h"
#include "../utility/rdtsc.h"
#include "../common/ids.h"
#include "../node_manager/base_node.h"
#include "caf/all.hpp"
#include "caf/io/all.hpp"
#include "caf/response_handle.hpp"
using caf::after;
using caf::event_based_actor;
using caf::io::remote_actor;
using caf::response_handle;

using claims::AskExchAtom;
ExchangeTracker::ExchangeTracker() {}

ExchangeTracker::~ExchangeTracker() {}
bool ExchangeTracker::RegisterExchange(ExchangeID id, std::string port) {
  lock_.acquire();
  if (id_to_port.find(id) != id_to_port.end()) {
    LOG(ERROR) << "RegisterExchange fails because the exchange id has already "
                  "existed.";
    lock_.release();
    return false;
  }
  id_to_port[id] = port;
  LOG(INFO) << "New exchange with id= " << id.exchange_id << " (port = " << port
            << ")is successfully registered!";
  lock_.release();
  return true;
}
void ExchangeTracker::LogoutExchange(const ExchangeID& id) {
  lock_.acquire();
  boost::unordered_map<ExchangeID, std::string>::const_iterator it =
      id_to_port.find(id);
  assert(it != id_to_port.cend());
  id_to_port.erase(it);
  lock_.release();
  LOG(INFO) << "Exchange with id=(" << id.exchange_id << " , "
            << id.partition_offset << " ) is logged out!";
}

bool ExchangeTracker::AskForSocketConnectionInfo(ExchangeID exchange_id,
                                                 NodeID target_id,
                                                 NodeAddress& node_addr) {
  caf::scoped_actor self;
  auto target_node_addr =
      Environment::getInstance()->get_master_node()->GetNodeAddrFromId(
          target_id);
  auto target_actor =
      remote_actor(target_node_addr.first.c_str(), target_node_addr.second);

  self->sync_send(target_actor, AskExchAtom::value, exchange_id).await(
      /// should add overtime!
      [&](const NodeAddress exch_node) {
        LOG(INFO) << "exchange tracker received node addr < " << exch_node.ip
                  << " , " << exch_node.port << " >" << endl;
        node_addr.ip = exch_node.ip;
        node_addr.port = exch_node.port;
      },
      after(std::chrono::seconds(30)) >>
          [=]() {
            LOG(WARNING) << "asking exchange connection info, but timeout 30s!!"
                         << endl;
            return 0;
          }

      );
  return node_addr.ip != "0";
}
NodeAddress ExchangeTracker::GetExchAddr(ExchangeID exch_id) {
  NodeAddress ret;
  if (id_to_port.find(exch_id) != id_to_port.cend()) {
    ret.ip = Environment::getInstance()->getIp();
    ret.port = id_to_port[exch_id];
  } else {
    ret.ip = "0";
    ret.port = "0";
  }
  return ret;
}
void ExchangeTracker::printAllExchangeId() const {
  for (boost::unordered_map<ExchangeID, std::string>::const_iterator it =
           id_to_port.cbegin();
       it != id_to_port.cend(); it++) {
    printf("(%ld,%ld) --->%s\n", it->first.exchange_id,
           it->first.partition_offset, it->second.c_str());
  }
}
