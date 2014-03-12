/*
 * ExpanderTracker.cpp
 *
 *  Created on: Mar 12, 2014
 *      Author: wangli
 */

#include "ExpanderTracker.h"
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

	if(id_to_status_.find(id)==id_to_status_.cend()){
		ExpandedThreadStatus status;
		status.call_back_=false;
		id_to_status_[id]=status;
		return true;
	}
	return false;
}
bool ExpanderTracker::deleteExpandedThreadStatus(expanded_thread_id id){
	if(id_to_status_.find(id)!=id_to_status_.cend()){
		id_to_status_.erase(id);
		return true;
	}
	return false;
}
bool ExpanderTracker::isExpandedThreadCallBack(expanded_thread_id id){
	if(id_to_status_.find(id)!=id_to_status_.cend()){
		return id_to_status_[id].call_back_;
	}
	return false;
}
bool ExpanderTracker::callbackExpandedThread(expanded_thread_id id){
	if(id_to_status_.find(id)!=id_to_status_.cend()){
		id_to_status_[id].call_back_=true;
		return true;
	}
	return false;
}
