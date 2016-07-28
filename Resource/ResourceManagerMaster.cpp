/*
 * ResourceManagerMaster.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "ResourceManagerMaster.h"

#include <glog/logging.h>

#include "../Environment.h"
ResourceManagerMaster::ResourceManagerMaster() {
  node_tracker_ = NodeTracker::GetInstance();
}

ResourceManagerMaster::~ResourceManagerMaster() {}

void ResourceManagerMaster::RegisterNewSlave(const NodeID new_node_id) {
  node_to_resourceinfo_[new_node_id] = new InstanceResourceInfo();
}
void ResourceManagerMaster::UnRegisterSlave(const NodeID old_node_id){
  auto it = node_to_resourceinfo_.find(old_node_id);
  if (it != node_to_resourceinfo_.end())
  {
  delete node_to_resourceinfo_[old_node_id];
  node_to_resourceinfo_.erase(old_node_id);
  }
}

std::vector<NodeID> ResourceManagerMaster::getSlaveIDList() {
  std::vector<NodeID> ret;
  boost::unordered_map<NodeID, InstanceResourceInfo*>::iterator it =
      node_to_resourceinfo_.begin();
  while (it != node_to_resourceinfo_.end()) {
    ret.push_back(it->first);
    it++;
  }
  return ret;
}
bool ResourceManagerMaster::ApplyDiskBuget(NodeID target, unsigned size_in_mb) {
  if (node_to_resourceinfo_.find(target) == node_to_resourceinfo_.cend()) {
    /* target slave does not exist.*/
    return false;
  }
  if (node_to_resourceinfo_[target]->disk.take(size_in_mb)) return true;
  return false;
}

bool ResourceManagerMaster::ReturnDiskBuget(NodeID target,
                                            unsigned size_in_mb) {
  if (node_to_resourceinfo_.find(target) == node_to_resourceinfo_.cend())
    return false;
  node_to_resourceinfo_[target]->disk.put(size_in_mb);
  return true;
}

bool ResourceManagerMaster::ApplyMemoryBuget(NodeID target,
                                             unsigned size_in_mb) {
  if (node_to_resourceinfo_.find(target) == node_to_resourceinfo_.cend()) {
    /* target slave does not exist.*/
    return false;
  }
  if (node_to_resourceinfo_[target]->memory.take(size_in_mb)) return true;
  LOG(ERROR) << "node :"<<target<<"no memory!!" << endl;
  return false;
}

bool ResourceManagerMaster::ReturnMemoryBuget(NodeID target,
                                              unsigned size_in_mb) {
  if (node_to_resourceinfo_.find(target) == node_to_resourceinfo_.cend())
    return false;
  node_to_resourceinfo_[target]->memory.put(size_in_mb);
  return true;
}

bool ResourceManagerMaster::RegisterDiskBuget(NodeID report_node_id,
                                              unsigned size_in_mb) {
  if (node_to_resourceinfo_.find(report_node_id) ==
      node_to_resourceinfo_.end()) {
    LOG(WARNING) << "target slave " << report_node_id << " does not exists!";
    return false;
  }
  node_to_resourceinfo_[report_node_id]->disk.initialize(size_in_mb);
  LOG(INFO) << "Node(id= " << report_node_id
            << ") reports its disk capacity=" << size_in_mb;
  return true;
}
bool ResourceManagerMaster::RegisterMemoryBuget(NodeID report_node_id,
                                                unsigned size_in_mb) {
  if (node_to_resourceinfo_.find(report_node_id) ==
      node_to_resourceinfo_.end()) {
    LOG(WARNING) << "target slave " << report_node_id << " does not exists!";
    return false;
  }
  node_to_resourceinfo_[report_node_id]->memory.initialize(size_in_mb);
  LOG(INFO) << "Node(id= " << report_node_id
            << ") reports its memory capacity=" << size_in_mb;
  return true;
}
