/*
 * ExpanderTracker.cpp
 *
 *  Created on: Mar 12, 2014
 *      Author: wangli
 */

#include "ExpanderTracker.h"
#include <stdio.h>
ExpanderTracker* ExpanderTracker::instance_=0;
ExpanderTracker::ExpanderTracker() {
	// TODO Auto-generated constructor stub

}

ExpanderTracker::~ExpanderTracker() {
	// TODO Auto-generated destructor stub
}

ExpanderTracker* ExpanderTracker::getInstance(){
	if(instance_==0){
		instance_=new ExpanderTracker();
	}
	return instance_;
}
bool ExpanderTracker::registerNewExpandedThreadStatus(expanded_thread_id id){
	lock_.acquire();
	if(id_to_status_.find(id)==id_to_status_.end()){
		ExpandedThreadStatus status;
		status.call_back_=false;
		id_to_status_[id]=status;
//		printf("[ExpanderTracker]: %lx is registered!\n",id);
		lock_.release();
		return true;
	}
	assert(false);
	lock_.release();
	return false;
}
bool ExpanderTracker::deleteExpandedThreadStatus(expanded_thread_id id){
	lock_.acquire();
	if(id_to_status_.find(id)!=id_to_status_.end()){
		id_to_status_.erase(id);
//		printf("[ExpanderTracker]: %lx is delete!\n",id);
		lock_.release();
		return true;
	}
	assert(false);
	lock_.release();
	return false;
}
bool ExpanderTracker::isExpandedThreadCallBack(expanded_thread_id id){
	lock_.acquire();
	if(id_to_status_.find(id)!=id_to_status_.end()){
		bool ret= id_to_status_[id].call_back_;
		lock_.release();
		return ret;
	}
	lock_.release();
	return false;
}
bool ExpanderTracker::callbackExpandedThread(expanded_thread_id id){
	lock_.acquire();
	if(id_to_status_.find(id)!=id_to_status_.end()){
		if(id_to_status_[id].call_back_==true){
			lock_.release();
			return false;
		}
		else{
			id_to_status_[id].call_back_=true;
			lock_.release();
			return true;
		}
	}
	lock_.release();
	return false;
}
