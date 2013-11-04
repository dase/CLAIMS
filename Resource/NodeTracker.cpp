/*
 * NodeTracker.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: wangli
 */

#include "NodeTracker.h"

NodeTracker::NodeTracker():allocate_cur_(0) {

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

