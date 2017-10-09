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
using caf::actor;
using caf::after;
using caf::event_based_actor;
using caf::io::remote_actor;
using caf::response_handle;

using claims::AskExchAtom;
using claims::OkAtom;
ExchangeTracker::ExchangeTracker() {}

ExchangeTracker::~ExchangeTracker() {}
bool ExchangeTracker::RegisterExchange(ExchangeID id, std::string port,
                                       BlockStreamBuffer* const buffer) {
  lock_.acquire();
  if (id_to_port.find(id) != id_to_port.end()) {
    LOG(ERROR) << "RegisterExchange fails because the exchange id has already "
                  "existed.";
    lock_.release();
    return false;
  }
  id_to_port[id] = port;
  id_to_buffer_[id] = buffer;
  LOG(INFO) << "New exchange with id= " << id.exchange_id << " (port = " << port
            << ") and buffer is successfully registered!";
  lock_.release();
  return true;
}
void ExchangeTracker::LogoutExchange(const ExchangeID& id) {
  lock_.acquire();
  boost::unordered_map<ExchangeID, std::string>::const_iterator it =
      id_to_port.find(id);
  assert(it != id_to_port.cend());
  id_to_port.erase(it);
  auto ita = id_to_buffer_.find(id);
  assert(ita != id_to_buffer_.cend());
  id_to_buffer_.erase(ita);
  lock_.release();
  LOG(INFO) << "Exchange with id=(" << id.exchange_id << " , "
            << id.partition_offset << " ) is logged out!";
}
bool ExchangeTracker::AskForSocketConnectionInfo(const ExchangeID& exchange_id,
                                                 const NodeID& target_id,
                                                 NodeAddress& node_addr,
                                                 actor& target_actor) {
  caf::scoped_actor self;
  node_addr.ip = "0";
  node_addr.port = "0";
  int try_times = 0;
  while (try_times < 3) {
    try {
      LOG(INFO)<<"ask exch Atom to "<<target_id<<endl;
      self->sync_send(target_actor, AskExchAtom::value, exchange_id).await(
          /// should add overtime!
          [&](OkAtom, const string& ip, const string& port) {
            node_addr.ip = ip;
            node_addr.port = port;
            try_times = 100;
            LOG(INFO)<<"ip ~~~:"<<node_addr.ip<<"port ~~~"<<node_addr.port<<endl;
          },
          after(std::chrono::seconds(5)) >>
              [&]() {
                ++try_times;
                LOG(WARNING) << "asking exchange connection info, but timeout "
                                "5s!!! times= " << try_times << endl;
              }

          );
    } catch (caf::network_error& e) {
      PLOG(ERROR) << "master socket related errors occur when asking for socke "
                     "conn info " << endl;
      assert(false);
      return false;
    }
  }
  return node_addr.ip != "0";
}
bool ExchangeTracker::AskForSocketConnectionInfo(const ExchangeID& exchange_id,
                                                 const NodeID& target_id,
                                                 NodeAddress& node_addr) {
  auto target_actor =
      Environment::getInstance()->get_slave_node()->GetNodeActorFromId(
          target_id);

  return AskForSocketConnectionInfo(exchange_id, target_id, node_addr,
                                    target_actor);
}
NodeAddress ExchangeTracker::GetExchAddr(ExchangeID exch_id) {
  lock_.acquire();
  NodeAddress ret;
  if (id_to_port.find(exch_id) != id_to_port.cend()) {
    ret.ip = Environment::getInstance()->getIp();
    ret.port = id_to_port[exch_id];
  } else {
    ret.ip = "0";
    ret.port = "0";
  }
  lock_.release();
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

BlockStreamBuffer* const ExchangeTracker::getBuffer(const ExchangeID id) {
  lock_.acquire();
  auto it = id_to_buffer_.find(id);
  assert(it != id_to_buffer_.cend());
  lock_.release();
  return it->second;
}
