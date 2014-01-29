/*
 * Scan.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */
#include <stdio.h>
#include "Scan.h"
#include "../Catalog/Catalog.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamProjectionScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../IDsGenerator.h"
LogicalScan::LogicalScan(std::vector<Attribute> attribute_list)
:scan_attribute_list_(attribute_list),target_projection_(0) {
	// TODO Auto-generated constructor stub

}
LogicalScan::LogicalScan(const TableID& table_id):target_projection_(0) {
	TableDescriptor* table=Catalog::getInstance()->getTable(table_id);
	if(table==0){
		printf("Table[id=%d] does not exists!\n",table_id);
	}
	scan_attribute_list_=table->getAttributes();
}
LogicalScan::LogicalScan(ProjectionDescriptor* projection,const float sample_rate):sample_rate_(sample_rate){
	scan_attribute_list_=projection->getAttributeList();
	target_projection_=projection;
}
LogicalScan::LogicalScan(const TableID& table_id,const std::vector<unsigned>& selected_attribute_index_list)
:target_projection_(0) {
	TableDescriptor* table=Catalog::getInstance()->getTable(table_id);
	if(table==0){
		printf("Table[id=%d] does not exists!\n",table_id);
	}
	for(unsigned i=0;i<selected_attribute_index_list.size();i++){
		scan_attribute_list_.push_back(table->getAttribute(selected_attribute_index_list[i]));
	}
}

LogicalScan::~LogicalScan() {
	// TODO Auto-generated destructor stub
}

LogicalProjection LogicalScan::getLogcialProjection()const{
	return logical_projection_;
}

Dataflow LogicalScan::getDataflow(){
	TableID table_id=scan_attribute_list_[0].table_id_;
	TableDescriptor* table=Catalog::getInstance()->getTable(table_id);
	if(target_projection_==0){
		ProjectionOffset target_projection_off=-1;
		for(ProjectionOffset projection_off=0;projection_off<table->getNumberOfProjection();projection_off++){
			ProjectionDescriptor* projection=table->getProjectoin(projection_off);
			bool fail=false;
			for(std::vector<Attribute>::iterator it=scan_attribute_list_.begin();it!=scan_attribute_list_.end();it++){
				if(!projection->hasAttribute(*it)){
					/*the attribute *it does not in the projection*/
					fail=true;
					break;
				}
			}
			if(fail==true){
				continue;
			}
			target_projection_off=projection_off;
			break;
		}

		if(target_projection_off==-1){
			/*fail to find a projection that contains all the scan attribute*/
			printf("The current implementation does not support the scanning that involves more than one projection.\n");
			assert(false);
		}
		target_projection_=table->getProjectoin(target_projection_off);

	}
	if(!target_projection_->AllPartitionBound()){
		Catalog::getInstance()->getBindingModele()->BindingEntireProjection(target_projection_->getPartitioner(),MEMORY);
	}

	/*build the data flow*/


	dataflow_.attribute_list_=scan_attribute_list_; /*attribute_list*/

	Partitioner* par=target_projection_->getPartitioner();
	dataflow_.property_.partitioner=DataflowPartitioningDescriptor(*par);
	dataflow_.property_.commnication_cost=0;
	return dataflow_;

}
BlockStreamIteratorBase* LogicalScan::getIteratorTree(const unsigned &block_size){
	/* In the current implementation, all the attributes within the involved projection
	 * are read.
	 * TODO: Ideally, the columns in one projection are stored separately and only the
	 * needed columns are touch for a given query.
	 */
	ExpandableBlockStreamProjectionScan::State state;
	state.block_size_=block_size;
	state.projection_id_=target_projection_->getProjectionID();
	state.schema_=getSchema(dataflow_.attribute_list_);
	state.sample_rate_=sample_rate_;
	return new ExpandableBlockStreamProjectionScan(state);

//
//	ExpandableBlockStreamSingleColumnScan::State state;
//	state.block_size_=block_size;
//	state.filename_="/home/imdb/data/wangli/T0G0P0";
//	state.schema_=getSchema(dataflow_.attribute_list_);
//	return new ExpandableBlockStreamSingleColumnScan(state);
}
bool LogicalScan::GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size){
	Dataflow dataflow=getDataflow();
	NetworkTransfer transfer=requirement.requireNetworkTransfer(dataflow);

	ExpandableBlockStreamProjectionScan::State state;
	state.block_size_=block_size;
	state.projection_id_=target_projection_->getProjectionID();
	state.schema_=getSchema(dataflow_.attribute_list_);
	state.sample_rate_=sample_rate_;

	PhysicalPlan scan=new ExpandableBlockStreamProjectionScan(state);

	if(transfer==NONE){

		physical_plan_descriptor.plan=scan;
		physical_plan_descriptor.dataflow=dataflow;
		physical_plan_descriptor.cost+=0;
	}
	else{
		physical_plan_descriptor.cost+=dataflow.getAggregatedDatasize();

		ExpandableBlockStreamExchangeEpoll::State state;
		state.block_size=block_size;
		state.child=scan;//child_iterator;
		state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();
		state.schema=getSchema(dataflow.attribute_list_);

		std::vector<NodeID> lower_id_list=getInvolvedNodeID(dataflow.property_.partitioner);
		state.lower_ip_list=convertNodeIDListToNodeIPList(lower_id_list);

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

		state.partition_key_index=getIndexInAttributeList(dataflow.attribute_list_,requirement.getPartitionKey());
		assert(state.partition_key_index>=0);

		BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(state);

		Dataflow new_dataflow;
		new_dataflow.attribute_list_=dataflow.attribute_list_;
		new_dataflow.property_.partitioner.setPartitionKey(requirement.getPartitionKey());
		new_dataflow.property_.partitioner.setPartitionFunction(PartitionFunctionFactory::createBoostHashFunction(state.upper_ip_list.size()));

		const unsigned total_size=dataflow.getAggregatedDatasize();
		const unsigned degree_of_parallelism=state.upper_ip_list.size();
		std::vector<DataflowPartition> dataflow_partition_list;
			for(unsigned i=0;i<degree_of_parallelism;i++){
				const NodeID location=upper_id_list[i];

				/* Currently, the join output size cannot be predicted due to the absence of data statistics.
				 * We just use the magic number as following */
				const unsigned datasize=total_size/degree_of_parallelism;
				DataflowPartition dfp(i,datasize,location);
				dataflow_partition_list.push_back(dfp);
			}
		new_dataflow.property_.partitioner.setPartitionList(dataflow_partition_list);


		physical_plan_descriptor.plan=exchange;
		physical_plan_descriptor.dataflow=new_dataflow;
		physical_plan_descriptor.cost+=new_dataflow.getAggregatedDatasize();
	}

	if(requirement.passLimits(physical_plan_descriptor.cost))
		return true;
	else
		return false;

}
void LogicalScan::print(int level)const{
//	align(level);
	printf("%*.sScan: %s\n",level*8," ",Catalog::getInstance()->getTable(target_projection_->getProjectionID().table_id)->getTableName().c_str());

}
