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
#include <Theron/Theron.h>
#include "../hashmap.hpp"
#include "ResourceInfo.h"
#include "NodeTracker.h"
#include "../Logging.h"
#include "../Message.h"
class ResourceManagerMaster {
public:
	ResourceManagerMaster();
	virtual ~ResourceManagerMaster();
	NodeID RegisterNewSlave(NodeIP);
	/* notify the ResourceManager how large the budget is for a target node*/
	bool RegisterDiskBuget(NodeID report_node_id, unsigned size_in_mb);
	std::vector<NodeID> getSlaveIDList();
	bool ApplyDiskBuget(NodeID target, unsigned size_in_mb);
private:
	hashmap<NodeID,ResourceInfo*> node_to_resourceinfo_;
	NodeTracker *node_tracker_;
	Logging* logging_;
	Theron::EndPoint *endpoint_;
	Theron::Framework *framework;

	/*Actor*/

	class ResourceManagerMasterActor:public Theron::Actor{
	public:
		ResourceManagerMasterActor(Theron::Framework* framework,ResourceManagerMaster* rmm);
	private:
		void ReceiveStorageBudgetReport(const RegisterStorageMessage &message,const Theron::Address from);
		void ReceiveNewNodeRegister(const NodeRegisterMessage &message,const Theron::Address from);
		ResourceManagerMaster* rmm_;
	};

};

#endif /* RESOURCEMANAGERMASTER_H_ */
