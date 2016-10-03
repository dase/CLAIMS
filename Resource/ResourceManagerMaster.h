/*
 * ResourceManagerMaster.h
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#ifndef RESOURCEMANAGERMASTER_H_
#define RESOURCEMANAGERMASTER_H_
#include <string>
#include <vector>
#include "ResourceInfo.h"
#include "NodeTracker.h"
#include "../common/Message.h"
class ResourceManagerMaster {
 public:
  ResourceManagerMaster();
  virtual ~ResourceManagerMaster();
  NodeID RegisterNewSlave(NodeAddress);
  void UnRegisterSlave(const NodeID old_node_id);
  void RegisterNewSlave(const NodeID new_node_id);
  /* notify the ResourceManager how large the budget is for a target node*/
  bool RegisterDiskBuget(NodeID report_node_id, unsigned size_in_mb);
  bool RegisterMemoryBuget(NodeID report_node_id, unsigned size_in_mb);
  std::vector<NodeID> getSlaveIDList();
  bool ApplyDiskBuget(NodeID target, unsigned size_in_mb);
  bool ReturnDiskBuget(NodeID target, unsigned size_in_mb);
  bool ApplyMemoryBuget(NodeID target, unsigned size_in_mb);
  bool ReturnMemoryBuget(NodeID target, unsigned size_in_mb);

 private:
  boost::unordered_map<NodeID, InstanceResourceInfo*> node_to_resourceinfo_;
  NodeTracker* node_tracker_;
};

#endif /* RESOURCEMANAGERMASTER_H_ */
