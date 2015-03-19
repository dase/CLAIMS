/*
 * NodeTracker.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: wangli
 */

#include "NodeTracker.h"
NodeTracker* NodeTracker::instance_=0;
NodeTracker::NodeTracker():allocate_cur_(0) {

}
NodeTracker* NodeTracker::getInstance(){
	if(instance_==0){
		instance_=new NodeTracker();
	}
		return instance_;
}
NodeTracker::~NodeTracker() {
	// TODO Auto-generated destructor stub
}

int NodeTracker::RegisterNode(NodeAddress new_node_address){
	if(address_to_id_.find(new_node_address)!=address_to_id_.end()){
		/*node_name already exists.*/
		return -1;
	}
	const int allocated_id=allocate_cur_++;
	address_to_id_[new_node_address]=allocated_id;
	return allocated_id;
}

std::string NodeTracker::getNodeIP(const NodeID& target)const{
	boost::unordered_map<NodeAddress,NodeID>::const_iterator it=address_to_id_.cbegin();
	while(it!=address_to_id_.cend()){
		if(it->second==target)
			return it->first.ip;
		it++;
	}
	return NULL;//TODO avoid return NULL in case of no matching target by changing the return type to be boolean.*/
//	return NULL;
}
std::vector<NodeID> NodeTracker::getNodeIDList()const{
	std::vector<NodeID> ret;
	boost::unordered_map<NodeAddress,NodeID>::const_iterator it=address_to_id_.cbegin();
	while(it!=address_to_id_.cend()){
		ret.push_back(it->second);
		it++;
	}
	return ret;
}
