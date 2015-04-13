/*
 * LogicalQueryPlanRoot.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#include "LogicalQueryPlanRoot.h"
#include "../Resource/NodeTracker.h"
#include "../IDsGenerator.h"
#include "../Config.h"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"

#include "../BlockStreamIterator/BlockStreamResultCollector.h"
#include "../BlockStreamIterator/BlockStreamPrint.h"
#include "../BlockStreamIterator/BlockStreamPerformanceMonitorTop.h"
#include "../BlockStreamIterator/BlockStreamLimit.h"

LogicalQueryPlanRoot::LogicalQueryPlanRoot(NodeID collecter,LogicalOperator* child,const outputFashion& fashion,LimitConstraint limit_constraint)
:collecter_(collecter),child_(child),fashion_(fashion),limit_constraint_(limit_constraint){
	// TODO Auto-generated constructor stub
	setOperatortype(l_root);
}


LogicalQueryPlanRoot::~LogicalQueryPlanRoot() {
	if(child_>0){
		delete child_;
	}
}

BlockStreamIteratorBase* LogicalQueryPlanRoot::getIteratorTree(const unsigned& block_size){
	getDataflow();
	BlockStreamIteratorBase* child_iterator=child_->getIteratorTree(block_size);
	Dataflow child_dataflow=child_->getDataflow();
//	Schema* schema=getSchema(child_dataflow.attribute_list_);
	NodeTracker* node_tracker=NodeTracker::getInstance();

	bool data_exchange_used=false;
	/**
	 * If the number of partitions in the child dataflow is 1 and the the location is right in the collector,
	 * then exchange is not necessary.
	 */
	if(!(child_dataflow.property_.partitioner.getNumberOfPartitions()==1&&child_dataflow.property_.partitioner.getPartitionList()[0].getLocation()==collecter_)){
		data_exchange_used=true;

		BlockStreamExpander::State expander_state_lower;
		expander_state_lower.block_count_in_buffer_=10;
		expander_state_lower.block_size_=block_size;
		expander_state_lower.init_thread_count_=Config::initial_degree_of_parallelism;
		expander_state_lower.child_=child_iterator;
		expander_state_lower.schema_=getSchema(child_dataflow.attribute_list_);
		BlockStreamIteratorBase* expander_lower=new BlockStreamExpander(expander_state_lower);

		ExpandableBlockStreamExchangeEpoll::State state;
		state.block_size_=block_size;
		state.child_=expander_lower;//child_iterator;
		state.exchange_id_=IDsGenerator::getInstance()->generateUniqueExchangeID();
		state.schema_=getSchema(child_dataflow.attribute_list_);
		state.upper_ip_list_.push_back(node_tracker->getNodeIP(collecter_));
		state.partition_schema_=partition_schema::set_hash_partition(0);
		std::vector<NodeID> lower_id_list=getInvolvedNodeID(child_dataflow.property_.partitioner);
		for(unsigned i=0;i<lower_id_list.size();i++){
			const std::string ip=node_tracker->getNodeIP(lower_id_list[i]);
			assert(ip!="");
			state.lower_ip_list_.push_back(ip);
		}
		child_iterator=new ExpandableBlockStreamExchangeEpoll(state);
	}

	BlockStreamExpander::State expander_state;
	expander_state.block_count_in_buffer_=10;
	expander_state.block_size_=block_size;
	if(data_exchange_used){
		/** if data exchange is used, only one expanded thread is enough.**/
		expander_state.init_thread_count_=1;
	}
	else{
		expander_state.init_thread_count_=Config::initial_degree_of_parallelism;
	}
	expander_state.child_=child_iterator;
	expander_state.schema_=getSchema(child_dataflow.attribute_list_);
	BlockStreamIteratorBase* expander=new BlockStreamExpander(expander_state);

	BlockStreamIteratorBase* middle_tier;
	if(!limit_constraint_.canBeOmitted()){
		/* we should add a limit operator*/
		BlockStreamLimit::State limit_state(expander_state.schema_->duplicateSchema(),expander,limit_constraint_.returned_tuples_,block_size,limit_constraint_.start_position_);
		BlockStreamIteratorBase* limit=new BlockStreamLimit(limit_state);
		middle_tier=limit;
	}
	else{
		middle_tier=expander;
	}

	BlockStreamIteratorBase* ret;
	switch(fashion_){
		case PRINT:{

			BlockStreamPrint::State print_state(getSchema(child_dataflow.attribute_list_),middle_tier,block_size,getAttributeName(child_dataflow));
			ret=new BlockStreamPrint(print_state);
			break;
		}
		case PERFORMANCE:{
			BlockStreamPerformanceMonitorTop::State performance_state(getSchema(child_dataflow.attribute_list_),middle_tier,block_size);
			ret=new BlockStreamPerformanceMonitorTop(performance_state);
			break;
		}
		case RESULTCOLLECTOR:{
			std::vector<std::string> column_header;
			for(unsigned i=0;i<child_dataflow.attribute_list_.size();i++){
				column_header.push_back(child_dataflow.attribute_list_[i].getName());
			}
			BlockStreamResultCollector::State result_state(getSchema(child_dataflow.attribute_list_),middle_tier,block_size,column_header);
			ret=new BlockStreamResultCollector(result_state);
			break;
		}
	}


	return ret;
}
Dataflow LogicalQueryPlanRoot::getDataflow(){
	Dataflow ret=child_->getDataflow();
	QueryOptimizationLogging::log("Communication cost:%ld, predicted ouput size=%ld\n",ret.property_.commnication_cost,ret.property_.partitioner.getAggregatedDataCardinality());
//	print();
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
			state.block_size_=block_size;
			state.child_=physical_plan.plan;//child_iterator;
			state.exchange_id_=IDsGenerator::getInstance()->generateUniqueExchangeID();
			state.schema_=getSchema(physical_plan.dataflow.attribute_list_);
			state.upper_ip_list_.push_back(NodeTracker::getInstance()->getNodeIP(collecter_));
			state.partition_schema_=partition_schema::set_hash_partition(0);
			std::vector<NodeID> lower_id_list=getInvolvedNodeID(physical_plan.dataflow.property_.partitioner);
			for(unsigned i=0;i<lower_id_list.size();i++){
				const std::string ip=NodeTracker::getInstance()->getNodeIP(lower_id_list[i]);
				assert(ip!="");
				state.lower_ip_list_.push_back(ip);
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
			state.block_size_=block_size;
			state.child_=best_plan.plan;//child_iterator;
			state.exchange_id_=IDsGenerator::getInstance()->generateUniqueExchangeID();
			state.schema_=getSchema(best_plan.dataflow.attribute_list_);
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

			state.upper_ip_list_=convertNodeIDListToNodeIPList(upper_id_list);

			assert(requirement.hasReuiredPartitionKey());

			state.partition_schema_=partition_schema::set_hash_partition(this->getIndexInAttributeList(best_plan.dataflow.attribute_list_,requirement.getPartitionKey()));
			assert(state.partition_schema_.partition_key_index>=0);

			std::vector<NodeID> lower_id_list=getInvolvedNodeID(best_plan.dataflow.property_.partitioner);

			state.lower_ip_list_=convertNodeIDListToNodeIPList(lower_id_list);



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
	if(!limit_constraint_.canBeOmitted()){
		printf("With limit constaint: %ld, %ld\n",limit_constraint_.start_position_,limit_constraint_.returned_tuples_);
	}
	child_->print(level+1);

}
