/*
 * IteratorExecutorMaster.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#include "IteratorExecutorMaster.h"
#include <assert.h>
#include <string>
#include "../Environment.h"
#include "../utility/rdtsc.h"
#include "caf/io/all.hpp"
#include "../node_manager/base_node.h"
#include "caf/all.hpp"
using caf::io::remote_actor;
using claims::SendPlanAtom;
IteratorExecutorMaster* IteratorExecutorMaster::_instance = 0;

IteratorExecutorMaster::IteratorExecutorMaster() { _instance = this; }

IteratorExecutorMaster::~IteratorExecutorMaster() { _instance = 0; }

IteratorExecutorMaster* IteratorExecutorMaster::getInstance() {
  if (_instance == 0) {
    return new IteratorExecutorMaster();
  } else {
    return _instance;
  }
}

bool IteratorExecutorMaster::ExecuteBlockStreamIteratorsOnSites(
    PhysicalOperatorBase* it, std::vector<std::string> ip_list) {
  assert(false);  // shouldn't be here;
  return true;
}
// send serialized plan string to target
bool IteratorExecutorMaster::ExecuteBlockStreamIteratorsOnSite(
    PhysicalOperatorBase* it, NodeID target_id) {
  PhysicalQueryPlan* im = new PhysicalQueryPlan(it);
  string str = PhysicalQueryPlan::TextSerializePlan(*im);
  caf::scoped_actor self;
  auto node_addr =
      Environment::getInstance()->get_master_node()->GetNodeAddrFromId(
          target_id);
  auto target_actor = remote_actor(node_addr.first.c_str(), node_addr.second);
  self->send(target_actor, SendPlanAtom::value, str);
  LOG(INFO) << "master send serialized plan to target slave : " << target_id
            << " succeed!" << endl;
  return true;
}
bool IteratorExecutorMaster::Propogation(const int count, std::string target) {
  assert(false);  // shouldn't be here;
  return true;
}
