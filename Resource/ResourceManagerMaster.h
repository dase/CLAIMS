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
#include "../hashmap.hpp"
#include "ResourceInfo.h"
#include "NodeTracker.h"
#include "../Logging.h"
class ResourceManagerMaster {
public:
	ResourceManagerMaster();
	virtual ~ResourceManagerMaster();
	bool RegisterNewSlave(NodeIP);
	/* notify the ResourceManager how large the budget is for a target node*/
	bool RegisterDiskBuget(NodeID report_node_id, unsigned size_in_mb);
	std::vector<NodeID> getSlaveIDList();
	bool ApplyDiskBuget(NodeID target, unsigned size_in_mb);
private:
	hashmap<NodeID,ResourceInfo*> node_to_resourceinfo_;
	NodeTracker *node_tracker_;
	Logging* logging_;
};

#endif /* RESOURCEMANAGERMASTER_H_ */
