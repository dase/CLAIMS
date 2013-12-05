/*
 * Aggregation.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */
#include "../IDsGenerator.h"
#include "Aggregation.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
Aggregation::Aggregation(std::vector<Attribute> group_by_attribute_list,std::vector<Attribute> aggregation_attribute_list,std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_list,LogicalOperator* child)
:group_by_attribute_list_(group_by_attribute_list),aggregation_attribute_list_(aggregation_attribute_list),aggregation_list_(aggregation_list),dataflow_(0),child_(child){

}

Aggregation::~Aggregation() {
	// TODO Auto-generated destructor stub
}
Dataflow Aggregation::getDataflow(){
	if(dataflow_!=0)
		return *dataflow_;
	Dataflow ret;
	const Dataflow child_dataflow=child_->getDataflow();
	if(canLeverageHashPartition(child_dataflow)){
		fashion_=no_repartition;
		printf("no_repartition\n");
	}
	else{
//		fashion_=repartition;
		fashion_=hybrid;
		printf("hybrid\n");
	}
	switch(fashion_){
		case no_repartition:{
			ret.attribute_list_.insert(ret.attribute_list_.end(),group_by_attribute_list_.begin(),group_by_attribute_list_.end());
			ret.attribute_list_.insert(ret.attribute_list_.end(),aggregation_attribute_list_.begin(),aggregation_attribute_list_.end());
			ret.property_.commnication_cost=child_dataflow.property_.commnication_cost;
			ret.property_.partitioner=child_dataflow.property_.partitioner;
			for(unsigned i=0;i<ret.property_.partitioner.getNumberOfPartitions();i++){
				const unsigned size=ret.property_.partitioner.getPartition(i)->getDataSize();
				ret.property_.partitioner.getPartition(i)->setDataSize(size*predictSelectivity());
			}

			break;
		}
		default:{// for hybrid and repartition
			/**
			 * repartition aggregation is currently simplified.
			 * TODO: ideally, the partition properties (especially the the number of partitions and partition fashion) after repartition
			 * aggregation should be decided by the partition property enforcement.
			 */
			ret.attribute_list_.insert(ret.attribute_list_.end(),group_by_attribute_list_.begin(),group_by_attribute_list_.end());
			ret.attribute_list_.insert(ret.attribute_list_.end(),aggregation_attribute_list_.begin(),aggregation_attribute_list_.end());
			ret.property_.commnication_cost=child_dataflow.property_.commnication_cost+child_dataflow.property_.partitioner.getAggregatedDatasize();
			ret.property_.partitioner.setPartitionFunction(child_dataflow.property_.partitioner.getPartitionFunction());
			const Attribute partition_key=group_by_attribute_list_[0];
			ret.property_.partitioner.setPartitionKey(partition_key);


			NodeID location=0;
			unsigned datasize=child_dataflow.getAggregatedDatasize()*predictSelectivity();
			PartitionOffset offset=0;
			DataflowPartition par(offset,datasize,location);

			std::vector<DataflowPartition> partition_list;
			partition_list.push_back(par);
			ret.property_.partitioner.setPartitionList(partition_list);


			break;

		}
	}
	dataflow_=new Dataflow();
	*dataflow_=ret;
	return ret;
}

bool Aggregation::canLeverageHashPartition(const Dataflow& child_dataflow)const{
	const Attribute partition_key=child_dataflow.property_.partitioner.getPartitionKey();
	if(!child_dataflow.isHashPartitioned())
		return false;
	/** the hash property of the input data flow can be leveraged in the aggragation as long
	 * as the hash attribute is one of the group-by attributes.
	 */
	for(unsigned i=0;i<group_by_attribute_list_.size();i++){
		if(group_by_attribute_list_[i]==partition_key)
			return true;
	}
	return false;
}
BlockStreamIteratorBase* Aggregation::getIteratorTree(const unsigned &block_size){
	BlockStreamIteratorBase* ret;
	Dataflow child_dataflow=child_->getDataflow();
	BlockStreamAggregationIterator::State aggregation_state;
	aggregation_state.groupByIndex=getInvolvedIndexList(group_by_attribute_list_,child_dataflow);
	aggregation_state.aggregationIndex=getInvolvedIndexList(aggregation_attribute_list_,child_dataflow);
	aggregation_state.aggregations=aggregation_list_;
	aggregation_state.block_size=block_size;
	aggregation_state.nbuckets=1024*1024;
	aggregation_state.bucketsize=64;
	aggregation_state.input=getSchema(child_dataflow.attribute_list_);
	aggregation_state.output=getSchema(dataflow_->attribute_list_);
	aggregation_state.child=child_->getIteratorTree(block_size);
	switch(fashion_){
		case no_repartition:{


			ret=new BlockStreamAggregationIterator(aggregation_state);
			break;
		}
		case hybrid:{
			BlockStreamAggregationIterator* private_aggregation=new BlockStreamAggregationIterator(aggregation_state);

			BlockStreamExpander::State expander_state;
			expander_state.block_count_in_buffer_=10;
			expander_state.block_size_=block_size;
			expander_state.thread_count_=3;
			expander_state.child_=private_aggregation;
			expander_state.schema_=getSchema(dataflow_->attribute_list_);
			BlockStreamIteratorBase* expander_lower=new BlockStreamExpander(expander_state);


			ExpandableBlockStreamExchangeEpoll::State exchange_state;
			exchange_state.block_size=block_size;
//			exchange_state.child=private_aggregation;
			exchange_state.child=expander_lower;
			exchange_state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();
			exchange_state.lower_ip_list=convertNodeIDListToNodeIPList(getInvolvedNodeID(child_->getDataflow().property_.partitioner));
			exchange_state.upper_ip_list=convertNodeIDListToNodeIPList(getInvolvedNodeID(dataflow_->property_.partitioner));
//			exchange_state.partition_key_index=getInvolvedIndexList(group_by_attribute_list_,child_dataflow)[0];
			exchange_state.partition_key_index=getInvolvedIndexList(group_by_attribute_list_,*dataflow_)[0];
			exchange_state.schema=getSchema(dataflow_->attribute_list_);
			BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(exchange_state);


			BlockStreamAggregationIterator::State global_aggregation_state;
			global_aggregation_state.aggregationIndex=getInvolvedIndexList(aggregation_attribute_list_,*dataflow_);
			global_aggregation_state.aggregations=aggregation_list_;
			global_aggregation_state.block_size=block_size;
			global_aggregation_state.bucketsize=64;
			global_aggregation_state.child=exchange;
			global_aggregation_state.groupByIndex=getInvolvedIndexList(group_by_attribute_list_,*dataflow_);
			global_aggregation_state.input=getSchema(dataflow_->attribute_list_);
			global_aggregation_state.nbuckets=aggregation_state.nbuckets;
			global_aggregation_state.output=getSchema(dataflow_->attribute_list_);
			BlockStreamIteratorBase* global_aggregation=new BlockStreamAggregationIterator(global_aggregation_state);
			ret=global_aggregation;
			break;
		}
		case repartition:{

			BlockStreamExpander::State expander_state;
			expander_state.block_count_in_buffer_=10;
			expander_state.block_size_=block_size;
			expander_state.thread_count_=3;
			expander_state.child_=child_->getIteratorTree(block_size);;
			expander_state.schema_=getSchema(child_dataflow.attribute_list_);
			BlockStreamIteratorBase* expander=new BlockStreamExpander(expander_state);




			ExpandableBlockStreamExchangeEpoll::State exchange_state;
			exchange_state.block_size=block_size;
			exchange_state.child=expander;//child_->getIteratorTree(block_size);
			exchange_state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();
			exchange_state.lower_ip_list=convertNodeIDListToNodeIPList(getInvolvedNodeID(child_->getDataflow().property_.partitioner));
			exchange_state.upper_ip_list=convertNodeIDListToNodeIPList(getInvolvedNodeID(dataflow_->property_.partitioner));
			exchange_state.partition_key_index=getInvolvedIndexList(group_by_attribute_list_,child_dataflow)[0];
			exchange_state.schema=getSchema(child_dataflow.attribute_list_);
			BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(exchange_state);

			aggregation_state.child=exchange;
			ret=new BlockStreamAggregationIterator(aggregation_state);
			break;
		}
	}




	return ret;


}
std::vector<unsigned> Aggregation::getInvolvedIndexList(const std::vector<Attribute>& attribute_list,const Dataflow& dataflow )const{
	std::vector<unsigned> ret;
	for(unsigned i=0;i<attribute_list.size();i++){
		bool found=false;
		for(unsigned j=0;j<dataflow.attribute_list_.size();j++){
			if(dataflow.attribute_list_[j]==attribute_list[i]){
				found=true;
				ret.push_back(j);
				break;
			}
		}
		if(found==false){
			printf("Cannot find any matching attribute.\n");
			assert(false);
		}
	}
	return ret;
}
float Aggregation::predictSelectivity()const{
	return 0.1;
}
