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

int NodeTracker::RegisterNode(std::string node_name){
	if(ip_to_id_.find(node_name)!=ip_to_id_.end()){
		/*node_name already exists.*/
		return -1;
	}
	const int allocated_id=allocate_cur_++;
	ip_to_id_[node_name]=allocated_id;
	return allocated_id;
}

std::string NodeTracker::getNodeIP(const NodeID& target)const{
	hashmap<NodeIP,NodeID>::const_iterator it=ip_to_id_.cbegin();
	while(it!=ip_to_id_.cend()){
		if(it->second==target)
			return it->first;
		it++;
	}
	return NULL;//TODO avoid return NULL in case of no matching target by changing the return type to be boolean.*/
//	return NULL;
}
