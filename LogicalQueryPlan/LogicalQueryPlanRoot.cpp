/*
 * LogicalQueryPlanRoot.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#include "LogicalQueryPlanRoot.h"
#include "../Resource/NodeTracker.h"
#include "../IDsGenerator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../BlockStreamIterator/BlockStreamResultCollector.h"
#include "../BlockStreamIterator/BlockStreamPrint.h"
#include "../PerformanceMonitor/BlockStreamPerformanceMonitorTop.h"
#define Thread_Count 1
LogicalQueryPlanRoot::LogicalQueryPlanRoot(NodeID collecter,LogicalOperator* child,const outputFashion& fashion)
:collecter_(collecter),child_(child),fashion_(fashion){
	// TODO Auto-generated constructor stub

}

LogicalQueryPlanRoot::~LogicalQueryPlanRoot() {
	if(child_>0){
		child_->~LogicalOperator();
	}
}

BlockStreamIteratorBase* LogicalQueryPlanRoot::getIteratorTree(const unsigned& block_size){
	getDataflow();
	BlockStreamIteratorBase* child_iterator=child_->getIteratorTree(block_size);
	Dataflow dataflow=child_->getDataflow();
	Schema* schema=getSchema(dataflow.attribute_list_);
	NodeTracker* node_tracker=NodeTracker::getInstance();

	BlockStreamExpander::State expander_state;
	expander_state.block_count_in_buffer_=10;
	expander_state.block_size_=block_size;
	expander_state.init_thread_count_=Thread_Count;
	expander_state.child_=child_iterator;
	expander_state.schema_=getSchema(dataflow.attribute_list_);
	BlockStreamIteratorBase* expander=new BlockStreamExpander(expander_state);


	ExpandableBlockStreamExchangeEpoll::State state;
	state.block_size=block_size;
	state.child=expander;//child_iterator;
	state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();
	state.schema=schema;
	state.upper_ip_list.push_back(node_tracker->getNodeIP(collecter_));
	state.partition_key_index=0;
	std::vector<NodeID> lower_id_list=getInvolvedNodeID(dataflow.property_.partitioner);
	for(unsigned i=0;i<lower_id_list.size();i++){
		const std::string ip=node_tracker->getNodeIP(lower_id_list[i]);
		assert(ip!="");
		state.lower_ip_list.push_back(ip);
	}

	BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(state);

	BlockStreamIteratorBase* ret;
	switch(fashion_){
		case PRINT:{

			BlockStreamPrint::State print_state(schema,exchange,block_size,getAttributeName(dataflow));
			ret=new BlockStreamPrint(print_state);
			break;
		}
		case PERFORMANCE:{
			BlockStreamPerformanceMonitorTop::State performance_state(schema,exchange,block_size);
			ret=new BlockStreamPerformanceMonitorTop(performance_state);
			break;
		}
		case RESULTCOLLECTOR:{
			std::vector<std::string> column_header;
			for(unsigned i=0;i<dataflow.attribute_list_.size();i++){
				column_header.push_back(dataflow.attribute_list_[i].getName());
			}
			BlockStreamResultCollector::State result_state(schema,exchange,block_size,column_header);
			ret=new BlockStreamResultCollector(result_state);
			break;
		}
	}

	return ret;
}
Dataflow LogicalQueryPlanRoot::getDataflow(){
	Dataflow ret=child_->getDataflow();
	printf("Communication cost:%ld, predicted ouput size=%ld\n",ret.property_.commnication_cost,ret.property_.partitioner.getAggregatedDataCardinality());
	return child_->getDataflow();
}

bool LogicalQueryPlanRoot::GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& final_physical_plan_desc, const unsigned & block_size){
	std::vector<PhysicalPlanDescriptor> candidate_physical_plan;
	Requirement current_req;
	current_req.setRequiredLocations(std::vector<NodeID>(1,collecter_));

	Requirement merged_req;
	bool requirement_merged=current_req.tryMerge(requirement,merged_req);
	if(requirement_merged){
		current_req=merged_req;
	}


	PhysicalPlanDescriptor physical_plan;

	/** no requirement**/
	if(child_->GetOptimalPhysicalPlan(Requirement(),physical_plan,block_size)){

		NetworkTransfer transfer=current_req.requireNetworkTransfer(physical_plan.dataflow);

		if(transfer==NONE){
			candidate_physical_plan.push_back(physical_plan);
		}
		else if((transfer==OneToOne)||(transfer==Shuffle)){
			/* the input data flow should be transfered in the network to meet the requirement
			 * TODO: implement OneToOne Exchange
			 * */
			physical_plan.cost+=physical_plan.dataflow.getAggregatedDatasize();

			ExpandableBlockStreamExchangeEpoll::State state;
			state.block_size=block_size;
			state.child=physical_plan.plan;//child_iterator;
			state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();
			state.schema=getSchema(physical_plan.dataflow.attribute_list_);
			state.upper_ip_list.push_back(NodeTracker::getInstance()->getNodeIP(collecter_));
			state.partition_key_index=0;
			std::vector<NodeID> lower_id_list=getInvolvedNodeID(physical_plan.dataflow.property_.partitioner);
			for(unsigned i=0;i<lower_id_list.size();i++){
				const std::string ip=NodeTracker::getInstance()->getNodeIP(lower_id_list[i]);
				assert(ip!="");
				state.lower_ip_list.push_back(ip);
			}

			BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(state);
			physical_plan.plan=exchange;

		}
	}

	/** with requirement**/
	if(child_->GetOptimalPhysicalPlan(current_req,physical_plan,block_size)){
		candidate_physical_plan.push_back(physical_plan);
	}

	PhysicalPlanDescriptor best_plan=getBestPhysicalPlanDescriptor(candidate_physical_plan);

	PhysicalPlan final_plan;
	switch(fashion_){
		case PRINT:{
			BlockStreamPrint::State print_state(getSchema(best_plan.dataflow.attribute_list_),best_plan.plan,block_size,getAttributeName(physical_plan.dataflow));
			final_plan=new BlockStreamPrint(print_state);
			break;
		}
		case PERFORMANCE:{
			BlockStreamPerformanceMonitorTop::State performance_state(getSchema(best_plan.dataflow.attribute_list_),best_plan.plan,block_size);
			final_plan=new BlockStreamPerformanceMonitorTop(performance_state);
		}
	}


	if(requirement_merged){
		final_physical_plan_desc.cost=best_plan.cost;
		final_physical_plan_desc.dataflow=best_plan.dataflow;
		final_physical_plan_desc.plan=final_plan;
	}
	else{
		NetworkTransfer transfer=current_req.requireNetworkTransfer(best_plan.dataflow);

		if(transfer==NONE){
			final_physical_plan_desc.cost=best_plan.cost;
			final_physical_plan_desc.dataflow=best_plan.dataflow;
			final_physical_plan_desc.plan=final_plan;
		}
		else if((transfer==OneToOne)||(transfer==Shuffle)){
			/* the input data flow should be transfered in the network to meet the requirement
			 * TODO: implement OneToOne Exchange
			 * */

			ExpandableBlockStreamExchangeEpoll::State state;
			state.block_size=block_size;
			state.child=best_plan.plan;//child_iterator;
			state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();
			state.schema=getSchema(best_plan.dataflow.attribute_list_);
			std::vector<NodeID> upper_id_list;
			if(requirement.hasRequiredLocations()){
				upper_id_list=requirement.getRequiredLocations();
			}
			else{
				if(requirement.hasRequiredPartitionFunction()){
					/* partition function contains the number of partitions*/
					PartitionFunction* partitoin_function=requirement.getPartitionFunction();
					upper_id_list=std::vector<NodeID>(NodeTracker::getInstance()->getNodeIDList().begin(),NodeTracker::getInstance()->getNodeIDList().begin()+partitoin_function->getNumberOfPartitions()-1);
				}
				else{
					//TODO: decide the degree of parallelism
					upper_id_list=NodeTracker::getInstance()->getNodeIDList();
				}
			}

			state.upper_ip_list=convertNodeIDListToNodeIPList(upper_id_list);

			assert(requirement.hasReuiredPartitionKey());

			state.partition_key_index=this->getIndexInAttributeList(best_plan.dataflow.attribute_list_,requirement.getPartitionKey());
			assert(state.partition_key_index>=0);

			std::vector<NodeID> lower_id_list=getInvolvedNodeID(best_plan.dataflow.property_.partitioner);

			state.lower_ip_list=convertNodeIDListToNodeIPList(lower_id_list);


			BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(state);
			best_plan.plan=exchange;
			best_plan.cost+=best_plan.dataflow.getAggregatedDatasize();

			final_physical_plan_desc.cost=best_plan.cost;
			final_physical_plan_desc.dataflow=best_plan.dataflow;
			final_physical_plan_desc.plan=exchange;

		}
	}


	if(requirement.passLimits(final_physical_plan_desc.cost))
		return true;
	else
		return false;
}

std::vector<std::string> LogicalQueryPlanRoot::getAttributeName(const Dataflow& dataflow)const{
	std::vector<std::string> attribute_name_list;
	for(unsigned i=0;i<dataflow.attribute_list_.size();i++){
		attribute_name_list.push_back(dataflow.attribute_list_[i].attrName);
	}
	return attribute_name_list;
}
void LogicalQueryPlanRoot::print(int level)const{
	printf("Root\n");
	child_->print(level+1);

}
