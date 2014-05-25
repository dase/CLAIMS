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
#include "../Config.h"
#include "../common/ids.h"

#define DECISION_SHRINK 0
#define DECISION_EXPAND 1
#define DECISION_KEEP 2

#define THRESHOLD 0.1
#define THRESHOLD_EMPTY (THRESHOLD)
#define THRESHOLD_FULL (1-THRESHOLD)


///**
// * Ideally, this should be guaranteed by resource manager.
// */
//#define MAX_DEGREE_OF_PARALLELISM 6


ExpanderTracker* ExpanderTracker::instance_=0;
ExpanderTracker::ExpanderTracker(){
	// TODO Auto-generated constructor stub
	log_=new ExpanderTrackerLogging();
	pthread_t pid;
	pthread_create(&pid,0,monitoringThread,this);
	log_->log("************Monitoring thread id=%lx\n",pid);
}

ExpanderTracker::~ExpanderTracker() {
	log_->~Logging();
}

ExpanderTracker* ExpanderTracker::getInstance(){
	if(instance_==0){
		instance_=new ExpanderTracker();
	}
	return instance_;
}
bool ExpanderTracker::registerNewExpandedThreadStatus(expanded_thread_id id,ExpanderID exp_id){
	lock_.acquire();
	if(id_to_status_.find(id)!=id_to_status_.end()){
		assert(false);
		lock_.release();
		return false;
	}else{
		ExpandedThreadStatus status;
		status.call_back_=false;
		id_to_status_[id]=status;
	}

	if(thread_id_to_expander_id_.find(id)!=thread_id_to_expander_id_.end()){
		assert(false);
		lock_.release();
		return false;
	}
	else{
		thread_id_to_expander_id_[id]=exp_id;

		assert(expander_id_to_expand_shrink_[exp_id]!=0);
	}



	lock_.release();
	return true;
}
bool ExpanderTracker::deleteExpandedThreadStatus(expanded_thread_id id){
	lock_.acquire();
	if(id_to_status_.find(id)==id_to_status_.end()){
		assert(false);
		lock_.release();
		return false;
	}
	else{
		id_to_status_.erase(id);
	}
////		log_->log("[ExpanderTracker]: %lx is delete!\n",id);
//		lock_.release();
//		return true;

	if(thread_id_to_expander_id_.find(id)==thread_id_to_expander_id_.end()){
		assert(false);
		lock_.release();
		return false;
	}

	thread_id_to_expander_id_.erase(id);
	lock_.release();
	return true;

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
PerformanceInfo* ExpanderTracker::getPerformanceInfo(expanded_thread_id tid){
	lock_.acquire();
	if(thread_id_to_expander_id_.find(tid)==thread_id_to_expander_id_.end()){
		lock_.release();
		assert(false);
//		return false;
	}
	ExpanderID expender_id=thread_id_to_expander_id_[tid];

	if(expander_id_to_status_.find(expender_id)==expander_id_to_status_.end()){
		lock_.release();
		assert(false);
//		return false;
	}
	PerformanceInfo* ret=&expander_id_to_status_[expender_id].perf_info;
	lock_.release();
	return ret;
}
ExpanderID ExpanderTracker::registerNewExpander(MonitorableBuffer* buffer,ExpandabilityShrinkability* expand_shrink){
	ExpanderID expander_id;
	lock_.acquire();
	expander_id=IDsGenerator::getInstance()->getUniqueExpanderID();
	expander_id_to_status_[expander_id]=ExpanderStatus();
	expander_id_to_status_[expander_id].addNewEndpoint(LocalStageEndPoint(stage_desc,"Expander",buffer));
	expander_id_to_expand_shrink_[expander_id]=expand_shrink;
	lock_.release();
	log_->log("New Expander is registered, ID=%ld\n",expander_id);
	return expander_id;
}
void ExpanderTracker::unregisterExpander(ExpanderID expander_id){

	lock_.acquire();

	for(std::map<expanded_thread_id,ExpanderID>::iterator it=thread_id_to_expander_id_.begin();it!=thread_id_to_expander_id_.end();it++){
		assert(it->second!=expander_id);
	}
	expander_id_to_status_.erase(expander_id);
	expander_id_to_expand_shrink_.erase(expander_id);
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
//		printf("=======stage desc:%s\n",new_end_point.end_point_name.c_str());
	}
	else{
		/*new_end_point.type==stage_end*/
		LocalStageEndPoint top=pending_endpoints.top();
		pending_endpoints.pop();
		current_stage=local_stage(new_end_point,top);
		perf_info=PerformanceInfo();
//		printf("The execution is in a new stage: %s ---> %s\n",new_end_point.end_point_name.c_str(),top.end_point_name.c_str());
	}
	lock.release();
}
int ExpanderTracker::decideExpandingOrShrinking(local_stage& current_stage,unsigned int current_degree_of_parallelism){
	/**
	 * In the initial implementation, if all expanded threads are shrunk and the condition
	 *  for expanding will never be triggered due to the exhaust of input data-flow, then
	 *  the remaining work will never be done.
	 *
	 *  To avoid this problem, we should first check whether the dataflow is exhausted. If so, we should guarantee that
	 *  there is at least one expanded threads to process the remaining data in buffer.
	 */

	int ret;
	switch(current_stage.type_){
	case local_stage::incomplete:{
		ret=DECISION_KEEP;
		break;
		return DECISION_KEEP;
	}
	case local_stage::no_buffer:{
		ret=DECISION_KEEP;
		/**
		 * Currently, for the stage without synchronization buffer, the workload is not known and hence
		 * maximum degree of parallelism is used.
		 */
		if(current_degree_of_parallelism==Config::max_degree_of_parallelism){
			ret=DECISION_KEEP;
			break;
			return DECISION_KEEP;
		}
		else if (current_degree_of_parallelism<Config::max_degree_of_parallelism){
			ret=DECISION_EXPAND;
			break;
			return DECISION_EXPAND;
		}
		ret=DECISION_SHRINK;
		break;
		return DECISION_SHRINK;
	}
	case local_stage::from_buffer:{
		ret=DECISION_KEEP;
//		break;
		log_->log("%lf=====>N/A\n",current_stage.dataflow_src_.monitorable_buffer->getBufferUsage());
		const double current_usage=current_stage.dataflow_src_.monitorable_buffer->getBufferUsage();
		if(current_stage.dataflow_src_.monitorable_buffer->inputComplete()){
			if(current_degree_of_parallelism==0){
				ret=DECISION_EXPAND;
				break;
				return DECISION_EXPAND;
			}
			else{
				ret=DECISION_KEEP;
				break;
				return DECISION_KEEP;
			}
		}
		if(current_usage>THRESHOLD_FULL){
			ret=DECISION_EXPAND;
			break;
			return DECISION_EXPAND;
		}
		else if(current_usage<THRESHOLD_EMPTY){
			ret=DECISION_SHRINK;
			break;
			return DECISION_SHRINK;
		}
		else{
			ret=DECISION_KEEP;
			break;
			return DECISION_KEEP;
		}
	}
	case local_stage::to_buffer:{
		log_->log("N/A=====>%lf\n",current_stage.dataflow_desc_.monitorable_buffer->getBufferUsage());
		const double current_usage=current_stage.dataflow_desc_.monitorable_buffer->getBufferUsage();
		if(current_usage>THRESHOLD_FULL){
			ret=DECISION_SHRINK;
			break;
			return DECISION_SHRINK;
		}
		else if(current_usage<THRESHOLD_EMPTY){
//			if(current_degree_of_parallelism>=MAX_DEGREE_OF_PARALLELISM)
//				return DECISION_KEEP;
//			else
			ret=DECISION_EXPAND;
			break;
			return DECISION_EXPAND;
		}
		else{
			ret=DECISION_KEEP;
			break;
			return DECISION_KEEP;
		}
//		return DECISION_KEEP;
	}
	case local_stage::buffer_to_buffer:{
		log_->log("%lf=====> %lf\n",current_stage.dataflow_src_.monitorable_buffer->getBufferUsage(),current_stage.dataflow_desc_.monitorable_buffer->getBufferUsage());
		const double bottom_usage=current_stage.dataflow_src_.monitorable_buffer->getBufferUsage();
		const double top_usage=current_stage.dataflow_desc_.monitorable_buffer->getBufferUsage();
		/* guarantee that there is at least one expanded thread when the input is complete so that the stage can be finished soon.*/
		if(current_stage.dataflow_src_.monitorable_buffer->inputComplete()){
			if(current_degree_of_parallelism==0){
				ret=DECISION_EXPAND;
				break;
				return DECISION_EXPAND;
			}
			else{
				ret=DECISION_KEEP;
				break;
				return DECISION_KEEP;
			}
		}

		if(bottom_usage>THRESHOLD_FULL&&top_usage<THRESHOLD_EMPTY){
//			if(current_degree_of_parallelism>=MAX_DEGREE_OF_PARALLELISM)
//				return DECISION_KEEP;
//			else
			ret=DECISION_EXPAND;
			break;
			return DECISION_EXPAND;
		}
		if(top_usage>THRESHOLD_FULL || bottom_usage<THRESHOLD_EMPTY){
			ret=DECISION_SHRINK;
			break;
			return DECISION_SHRINK;
		}
//		log_->elog("The theory is not complete!\n");
		return DECISION_KEEP;
	}
	}

	if(ret==DECISION_EXPAND){
		return expandeIfNotExceedTheMaxDegreeOfParallelism(current_degree_of_parallelism);
	}
	if(ret==DECISION_SHRINK){
		return shrinkIfNotExceedTheMinDegreeOfParallelims(current_degree_of_parallelism);
	}

	return ret;


}
void* ExpanderTracker::monitoringThread(void* arg){
	if(!Config::enable_expander_adaptivity){
		return 0;
	}
	ExpanderTracker* Pthis=(ExpanderTracker*)arg;
	int cur=0;
	while(true){
//		std::map<ExpanderID,ExpanderStatus>::iterator it=Pthis->expander_id_to_status_.begin();
		usleep(Config::expander_adaptivity_check_frequency);
//		sleep(1000);
		Pthis->lock_.acquire();
		if(Pthis->expander_id_to_status_.size()<=cur){
			cur=0;
			Pthis->lock_.release();
			continue;
		}

		std::map<ExpanderID,ExpanderStatus>::iterator it=Pthis->expander_id_to_status_.begin();
		for(int tmp=0;tmp<cur;tmp++)
			it++;

		assert(!Pthis->expander_id_to_expand_shrink_.empty());
		Pthis->log_->log("-------------------");

		printf("Instance throughput: %lf Mbytes\n",it->second.perf_info.report_instance_performance_in_millibytes());

		const unsigned int current_degree_of_parallelism=Pthis->expander_id_to_expand_shrink_[it->first]->getDegreeOfParallelism();
		int decision=Pthis->decideExpandingOrShrinking(it->second.current_stage,current_degree_of_parallelism);
		Pthis->log_->log("%s---->%s\t  d=%d\t",it->second.current_stage.dataflow_src_.end_point_name.c_str(),it->second.current_stage.dataflow_desc_.end_point_name.c_str(),current_degree_of_parallelism);
		ExpanderID exp_id=it->first;
		Pthis->lock_.release();
		switch(decision){
		case DECISION_EXPAND:{
			if(Pthis->expander_id_to_expand_shrink_[it->first]->Expand()){
				Pthis->log_->log("=========Expanding======== %d-->%d ",current_degree_of_parallelism,current_degree_of_parallelism+1);
			}
			else{
				Pthis->log_->log("=========Expanding======== Failed!");
			}
			break;
		}
		case DECISION_SHRINK:{
//			if(Pthis->expander_id_to_expand_shrink_[it->first]->getDegreeOfParallelism()<=1)
//				break;
			if(Pthis->expander_id_to_expand_shrink_[it->first]->Shrink()){
				Pthis->log_->log("=========Shrinking========  %d-->%d",current_degree_of_parallelism,current_degree_of_parallelism-1);
			}
			else{
				Pthis->log_->log("=========Shrinking======== Failed!");
			}
			break;
		}
		default:{
			break;
		}
		}
//		Pthis->lock_.release();
		cur++;

//
//
//		for(std::map<ExpanderID,ExpanderStatus>::iterator it=Pthis->expander_id_to_status_.begin();it!=Pthis->expander_id_to_status_.end();it++){
//			Pthis->lock_.acquire();
//			if(Pthis->expander_id_to_status_.find(it->first)==Pthis->expander_id_to_status_.end()){
//				Pthis->lock_.release();
//				continue;
//			}
//
//			log_->log("%s---->%s",it->second.current_stage.dataflow_src_.end_point_name.c_str(),it->second.current_stage.dataflow_desc_.end_point_name.c_str());
//			Pthis->decideExpandingOrShrinking(it->second.current_stage);
//			Pthis->lock_.release();
//b			usleep(10000);
//		}
	}
}

int ExpanderTracker::expandeIfNotExceedTheMaxDegreeOfParallelism(
		int current_degree_of_parallelism) const {
	if(current_degree_of_parallelism<Config::max_degree_of_parallelism){
		return DECISION_EXPAND;
	}
	else
		return DECISION_KEEP;
}

int ExpanderTracker::shrinkIfNotExceedTheMinDegreeOfParallelims(
		int current_degree_of_parallelism) const {
	if(current_degree_of_parallelism>0){
		return DECISION_SHRINK;
	}
	return DECISION_KEEP;
}
