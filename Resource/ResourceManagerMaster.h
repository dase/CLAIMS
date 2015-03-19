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
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "ResourceInfo.h"
#include "NodeTracker.h"
#include "../common/Message.h"
#include "../common/Logging.h"
class ResourceManagerMaster {
public:
	class ResourceManagerMasterActor:public Theron::Actor{
	public:
		ResourceManagerMasterActor(Theron::Framework* framework,ResourceManagerMaster* rmm);
		~ResourceManagerMasterActor(){};
	private:
		void ReceiveStorageBudgetReport(const StorageBudgetMessage &message,const Theron::Address from);
		void ReceiveNewNodeRegister(const NodeRegisterMessage &message,const Theron::Address from);
		ResourceManagerMaster* rmm_;
	};
	ResourceManagerMaster();
	virtual ~ResourceManagerMaster();
	NodeID RegisterNewSlave(NodeAddress);
	/* notify the ResourceManager how large the budget is for a target node*/
	bool RegisterDiskBuget(NodeID report_node_id, unsigned size_in_mb);
	bool RegisterMemoryBuget(NodeID report_node_id, unsigned size_in_mb);
	std::vector<NodeID> getSlaveIDList();
	bool ApplyDiskBuget(NodeID target, unsigned size_in_mb);
	bool ReturnDiskBuget(NodeID target, unsigned size_in_mb);
	bool ApplyMemoryBuget(NodeID target, unsigned size_in_mb);
	bool ReturnMemoryBuget(NodeID target, unsigned size_in_mb);
private:
	boost::unordered_map<NodeID,InstanceResourceInfo*> node_to_resourceinfo_;
	NodeTracker *node_tracker_;
	Logging* logging_;
	Theron::EndPoint *endpoint_;
	Theron::Framework *framework;
	ResourceManagerMasterActor* acter_;
	/*Actor*/



};

#endif /* RESOURCEMANAGERMASTER_H_ */
