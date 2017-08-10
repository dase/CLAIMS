/*
 * exchange_tracker.h
 * ExchangeTracker is the coordinator between the exchanges and
 * their lower-exchangers. The exchanges register their socket
 * port in the open function, then the exchange-lowers ask their
 * upper ExchangeTrackers for the ports so that the socket
 * connection can be done automatically rather than setting the
 * ports manually.
 *  Created on: Aug 11, 2013
 *      Author: wangli
 */

#ifndef EXCHANGETRACKER_H_
#define EXCHANGETRACKER_H_
#include <boost/unordered_map.hpp>
#include <map>
#include "../utility/lock.h"
#include "../common/ids.h"
#include "caf/all.hpp"

#include "../common/Block/BlockStreamBuffer.h"
using caf::actor;

/*
 * maintain pair<id,port> information of exchange, and provide interface for
 * other exchange asking connection port
 */
class ExchangeTracker {
 public:
  ExchangeTracker();
  virtual ~ExchangeTracker();
  bool RegisterExchange(ExchangeID exchange_id, std::string port,
                        BlockStreamBuffer* const buffer);
  void LogoutExchange(const ExchangeID& exchange_id);
  bool AskForSocketConnectionInfo(const ExchangeID& exchange_id,
                                  const NodeID& target_id,
                                  NodeAddress& node_addr);
  bool AskForSocketConnectionInfo(const ExchangeID& exchange_id,
                                  const NodeID& target_id,
                                  NodeAddress& node_addr, actor& target_actor);
  void printAllExchangeId() const;
  NodeAddress GetExchAddr(ExchangeID exch_id);
  BlockStreamBuffer* const getBuffer(const ExchangeID exch_id);

 private:
  boost::unordered_map<ExchangeID, std::string> id_to_port;
  boost::unordered_map<ExchangeID, BlockStreamBuffer*> id_to_buffer_;
  Lock lock_;
};

#endif /* EXCHANGETRACKER_H_ */
