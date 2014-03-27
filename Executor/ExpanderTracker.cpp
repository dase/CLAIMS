/*
 * ExpanderTracker.cpp
 *
 *  Created on: Mar 12, 2014
 *      Author: wangli
 */

#include "ExpanderTracker.h"
#include <stdio.h>
#include "../IDsGenerator.h"
#include <pthread.h>

#define DECISION_SHRINK 0
#define DECISION_EXPAND 1
#define DECISION_KEEP 2

ExpanderTracker* ExpanderTracker::instance_=0;
ExpanderTracker::ExpanderTracker(){
	// TODO Auto-generated constructor stub
	pthread_t pid;
	pthread_create(&pid,0,monitoringThread,this);
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
bool ExpanderTracker::registerNewExpandedThreadStatus(expanded_thread_id id,ExpanderID exp_id,ExpandabilityShrinkability* expand_shrink){
	lock_.acquire();
	if(id_to_status_.find(id)!=id_to_status_.end()){
		assert(false);
		lock_.release();
		return false;
	}else{
		ExpandedThreadStatus status;
		status.call_back_=false;
		id_to_status_[id]=status;
//		printf("[ExpanderTracker]: %lx is registered!\n",id);
	}

	if(thread_id_to_expander_id_.find(id)!=thread_id_to_expander_id_.end()){
		assert(false);
		lock_.release();
		return false;
	}
	else{
		thread_id_to_expander_id_[id]=exp_id;
		expander_id_to_expand_shrink_[exp_id]=expand_shrink;
	}



	lock_.release();
	return true;
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

bool ExpanderTracker::addNewStageEndpoint(expanded_thread_id tid ,LocalStageEndPoint endpoint){


	lock_.acquire();
	if(thread_id_to_expander_id_.find(tid)==thread_id_to_expander_id_.end()){
		lock_.release();
//		assert(false);
		return false;
	}
	ExpanderID expender_id=thread_id_to_expander_id_[tid];

	if(expander_id_to_status_.find(expender_id)==expander_id_to_status_.end()){
		lock_.release();
		assert(false);
		return false;
	}
	expander_id_to_status_[expender_id].addNewEndpoint(endpoint);
	lock_.release();
	return true;
}

ExpanderID ExpanderTracker::registerNewExpander(MonitorableBuffer* buffer){
	ExpanderID ret;
	lock_.acquire();
	ret=IDsGenerator::getInstance()->getUniqueExpanderID();
	expander_id_to_status_[ret]=ExpanderStatus();
	expander_id_to_status_[ret].addNewEndpoint(LocalStageEndPoint(stage_desc,"Expander",buffer));
	lock_.release();
	return ret;
}
void ExpanderTracker::unregisterExpander(ExpanderID expander_id){

	lock_.acquire();
	expander_id_to_status_.erase(expander_id);
	expander_id_to_expand_shrink_.erase(expander_id);
//	expander_id_to_status_[ret].addNewEndpoint(LocalStageEndPoint(stage_desc,"Expander",buffer));
	lock_.release();

}
void ExpanderTracker::ExpanderStatus::addNewEndpoint(LocalStageEndPoint new_end_point){
//	lock.acquire();
//	if(pending_endpoints.empty()){
//		pending_endpoints.push(new_end_point);
//		lock.release();
//		return;
//	}
//	//if the endpoint is exchange or state_stage_start, then the segment might step into a new local stage.
//	switch(new_end_point.type){
//	case endpoint_state_stage_start:{
//		assert(!pending_endpoints.empty());
//		LocalStageEndPoint top=pending_endpoints.top();
//		pending_endpoints.pop();
//		current_stage=local_stage(new_end_point,top);
//		break;
//	}
//	case endpoint_exchange_:{
//		assert(!pending_endpoints.empty());
//		LocalStageEndPoint top=pending_endpoints.top();
//		pending_endpoints.pop();
//		current_stage=local_stage(new_end_point,top);
//		break;
//	}
//	default:{
//		pending_endpoints.push(new_end_point);
//		lock.release();
//		return;
//	}
//	}
//
//	lock.release();

	lock.acquire();
	if(new_end_point.type==stage_desc){
		pending_endpoints.push(new_end_point);
		printf("=======stage desc:%s\n",new_end_point.end_point_name.c_str());
	}
	else{
		/*new_end_point.type==stage_end*/
		LocalStageEndPoint top=pending_endpoints.top();
		pending_endpoints.pop();
		current_stage=local_stage(new_end_point,top);
		printf("The execution is in a new stage: %s ---> %s\n",new_end_point.end_point_name.c_str(),top.end_point_name.c_str());
	}
	lock.release();
}
int ExpanderTracker::decideExpandingOrShrinking(local_stage& current_stage){
	if(current_stage.type_==local_stage::incomplete||current_stage.type_==local_stage::no_buffer)
		return DECISION_KEEP;
	switch(current_stage.type_){
	case local_stage::incomplete:{
		return DECISION_KEEP;
	}
	case local_stage::no_buffer:{
		return DECISION_KEEP;
	}
	case local_stage::from_buffer:{
		printf("Bottom buffer usage: %lf\n",current_stage.dataflow_src_.monitorable_buffer->getBufferUsage());
		return DECISION_KEEP;
	}
	case local_stage::to_buffer:{
		printf("Top buffer usage: %lf\n",current_stage.dataflow_desc_.monitorable_buffer->getBufferUsage());
		return DECISION_KEEP;
	}
	case local_stage::buffer_to_buffer:{
		printf("Top buffer usage: %lf\t Bottom buffer usage: %lf\n",current_stage.dataflow_src_.monitorable_buffer->getBufferUsage(),current_stage.dataflow_desc_.monitorable_buffer->getBufferUsage());
		return DECISION_KEEP;
	}
	}

	return DECISION_KEEP;


}
void* ExpanderTracker::monitoringThread(void* arg){
	ExpanderTracker* Pthis=(ExpanderTracker*)arg;
	while(true){
//		std::map<ExpanderID,ExpanderStatus>::iterator it=Pthis->expander_id_to_status_.begin();
		for(std::map<ExpanderID,ExpanderStatus>::iterator it=Pthis->expander_id_to_status_.begin();it!=Pthis->expander_id_to_status_.end();it++){
			Pthis->decideExpandingOrShrinking(it->second.current_stage);
			usleep(10000);
		}
	}
}
