/*
 * ResourceManagerMaster.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "ResourceManagerMaster.h"

ResourceManagerMaster::ResourceManagerMaster() {
	node_tracker_=new NodeTracker();
	logging_=new ResourceManagerMasterLogging();
}

ResourceManagerMaster::~ResourceManagerMaster() {
	// TODO Auto-generated destructor stub
}
bool ResourceManagerMaster::RegisterNewSlave(NodeIP new_slave_ip_){
	NodeID new_node_id=node_tracker_->RegisterNode(new_slave_ip_);
	if(new_node_id==-1){
		/* Node with the given ip has already existed.*/
		logging_->elog("[%s] has already exists",new_slave_ip_.c_str());
		return false;
	}

//
//	if(node_to_resourceinfo_.find(new_node_id)!=node_to_resourceinfo_.end()){
//		/*The slaveId has already existed.*/
//		return false;
//	}

	node_to_resourceinfo_[new_node_id]=new ResourceInfo();

	logging_->log("[ip+%s, id=%d] is successfully registered.",new_slave_ip_,new_node_id);

	return true;
}
std::vector<NodeID> ResourceManagerMaster::getSlaveIDList(){
	std::vector<NodeID> ret;
	hashmap<NodeID,ResourceInfo*>::iterator it=node_to_resourceinfo_.begin();
	while(it!=node_to_resourceinfo_.end()){
		ret.push_back(it->first);
	}
	return ret;
}
bool ResourceManagerMaster::ApplyDiskBuget(NodeID target, unsigned size_in_mb){
	if(node_to_resourceinfo_.find(target)==node_to_resourceinfo_.end()){
		/* target slave does not exist.*/
		return false;
	}
	if(node_to_resourceinfo_[target]->disk.take(size_in_mb))
		return true;
	return false;
}
bool ResourceManagerMaster::RegisterDiskBuget(NodeID report_node_id, unsigned size_in_mb){
	if(node_to_resourceinfo_.find(report_node_id)==node_to_resourceinfo_.end()){
		/* target slave does not exists*/
		return false;
	}
	node_to_resourceinfo_[report_node_id]->disk.initialize(size_in_mb);
	logging_->log("Node(id=%d) reports its disk capacity=%d",report_node_id,size_in_mb);
	return true;
}
