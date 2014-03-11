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
#include "../Catalog/stat/StatManager.h"
#define THREAD_COUNT 4
Aggregation::Aggregation(std::vector<Attribute> group_by_attribute_list,std::vector<Attribute> aggregation_attribute_list,std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_list,LogicalOperator* child)
:group_by_attribute_list_(group_by_attribute_list),aggregation_attribute_list_(aggregation_attribute_list),aggregation_list_(aggregation_list),dataflow_(0),child_(child){
	assert(aggregation_attribute_list_.size()==aggregation_list_.size());
}

Aggregation::~Aggregation() {
	dataflow_->~Dataflow();
	if(child_>0){
		child_->~LogicalOperator();
	}
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
			ret.attribute_list_=getAttributesListAfterAggregation();
//			ret.attribute_list_.insert(ret.attribute_list_.end(),group_by_attribute_list_.begin(),group_by_attribute_list_.end());
//			ret.attribute_list_.insert(ret.attribute_list_.end(),aggregation_attribute_list_.begin(),aggregation_attribute_list_.end());
			ret.property_.commnication_cost=child_dataflow.property_.commnication_cost;
			ret.property_.partitioner=child_dataflow.property_.partitioner;
			for(unsigned i=0;i<ret.property_.partitioner.getNumberOfPartitions();i++){
				const unsigned cardinality=ret.property_.partitioner.getPartition(i)->getDataCardinality();
//				ret.property_.partitioner.getPartition(i)->setDataCardinality(cardinality*predictSelectivity());
				ret.property_.partitioner.getPartition(i)->setDataCardinality(estimateGroupByCardinality(child_dataflow)/ret.property_.partitioner.getNumberOfPartitions());

			}

			break;
		}
		default:{// for hybrid and repartition
			/**
			 * repartition aggregation is currently simplified.
			 * TODO: ideally, the partition properties (especially the the number of partitions and partition fashion) after repartition
			 * aggregation should be decided by the partition property enforcement.
			 */
			ret.attribute_list_=getAttributesListAfterAggregation();
//			ret.attribute_list_.insert(ret.attribute_list_.end(),group_by_attribute_list_.begin(),group_by_attribute_list_.end());
//			ret.attribute_list_.insert(ret.attribute_list_.end(),aggregation_attribute_list_.begin(),aggregation_attribute_list_.end());
//			unsigned tuple_size=getSchema(child_dataflow.attribute_list_)->getTupleMaxSize();
			ret.property_.commnication_cost=child_dataflow.property_.commnication_cost+child_dataflow.getAggregatedDatasize();//.property_.partitioner.getAggregatedDataCardinality()*tuple_size;

			ret.property_.partitioner.setPartitionFunction(child_dataflow.property_.partitioner.getPartitionFunction());
			if(group_by_attribute_list_.empty())
				ret.property_.partitioner.setPartitionKey(Attribute());
			else{
				const Attribute partition_key=group_by_attribute_list_[0];
				ret.property_.partitioner.setPartitionKey(partition_key);
			}


			NodeID location=0;


//			unsigned long data_cardinality=child_dataflow.getAggregatedDataCardinality()*predictSelectivity();
			unsigned long data_cardinality=estimateGroupByCardinality(child_dataflow);
			PartitionOffset offset=0;
			DataflowPartition par(offset,data_cardinality,location);

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
	if(dataflow_==0){
		getDataflow();
	}
	BlockStreamIteratorBase* ret;
	Dataflow child_dataflow=child_->getDataflow();
	BlockStreamAggregationIterator::State aggregation_state;
	aggregation_state.groupByIndex=getInvolvedIndexList(group_by_attribute_list_,child_dataflow);
	aggregation_state.aggregationIndex=getInvolvedIndexList(aggregation_attribute_list_,child_dataflow);
	aggregation_state.aggregations=aggregation_list_;
	aggregation_state.block_size=block_size;
//	aggregation_state.nbuckets=1024;
	aggregation_state.nbuckets=estimateGroupByCardinality(child_dataflow);
	printf("# of hash buckets:%d\n",aggregation_state.nbuckets);

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
			expander_state.thread_count_=THREAD_COUNT;
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

			if(group_by_attribute_list_.empty())
				exchange_state.partition_key_index=0;
			else{
//				exchange_state.partition_key_index=getInvolvedIndexList(group_by_attribute_list_,*dataflow_)[0];
				exchange_state.partition_key_index=getInvolvedIndexList(getGroupByAttributeAfterAggregation(),*dataflow_)[0];
			}
			exchange_state.schema=getSchema(dataflow_->attribute_list_);
			BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(exchange_state);


			BlockStreamAggregationIterator::State global_aggregation_state;
			global_aggregation_state.aggregationIndex=getInvolvedIndexList(getAggregationAttributeAfterAggregation(),*dataflow_);
			global_aggregation_state.aggregations=convertionForHybrid(aggregation_list_);
			global_aggregation_state.block_size=block_size;
			global_aggregation_state.bucketsize=64;
			global_aggregation_state.child=exchange;
			global_aggregation_state.groupByIndex=getInvolvedIndexList(getGroupByAttributeAfterAggregation(),*dataflow_);
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
			expander_state.thread_count_=THREAD_COUNT;
			expander_state.child_=child_->getIteratorTree(block_size);;
			expander_state.schema_=getSchema(child_dataflow.attribute_list_);
			BlockStreamIteratorBase* expander=new BlockStreamExpander(expander_state);




			ExpandableBlockStreamExchangeEpoll::State exchange_state;
			exchange_state.block_size=block_size;
			exchange_state.child=expander;//child_->getIteratorTree(block_size);
			exchange_state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();
			exchange_state.lower_ip_list=convertNodeIDListToNodeIPList(getInvolvedNodeID(child_->getDataflow().property_.partitioner));
			exchange_state.upper_ip_list=convertNodeIDListToNodeIPList(getInvolvedNodeID(dataflow_->property_.partitioner));
			if(group_by_attribute_list_.empty()){
				/**
				 * scalar aggregation allows parallel partitions to be partitioned in any fashion.
				 * In the current implementation, we use the first aggregation attribute as the
				 * partition attribute.
				 * TODO: select the proper partition attribute by considering the cardinality and load balance.
				 */
				exchange_state.partition_key_index=getInvolvedIndexList(aggregation_attribute_list_,child_dataflow)[0];
			}
			else{
				exchange_state.partition_key_index=getInvolvedIndexList(group_by_attribute_list_,child_dataflow)[0];
			}
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
			if(attribute_list[j].isANY()||(dataflow.attribute_list_[j]==attribute_list[i])){
				found=true;
				ret.push_back(j);
				break;
			}
		}
//		if(found==false){
//			printf("Cannot find any matching attribute.\n");
//			assert(false);
//		}
	}
	return ret;
}
float Aggregation::predictSelectivity()const{
	return 0.1;
}
std::vector<BlockStreamAggregationIterator::State::aggregation> Aggregation::convertionForHybrid(const std::vector<BlockStreamAggregationIterator::State::aggregation> list)const{
	std::vector<BlockStreamAggregationIterator::State::aggregation> ret;
	for(unsigned i=0;i<list.size();i++){
		if(list[i]==BlockStreamAggregationIterator::State::count){
			ret.push_back(BlockStreamAggregationIterator::State::sum);
		}
		else
			ret.push_back(list[i]);
	}
	return ret;

}
/**
 * In the current implementation, we assume that aggregation creates a new table, i.e., intermediate table.
 * The id for the intermediate table is -1.
 *
 */
std::vector<Attribute> Aggregation::getAttributesListAfterAggregation()const{
	std::vector<Attribute> ret;
	ret=getGroupByAttributeAfterAggregation();
	const std::vector<Attribute> aggregation_attributes=getAggregationAttributeAfterAggregation();
	ret.insert(ret.end(),aggregation_attributes.begin(),aggregation_attributes.end());
	return ret;
}
std::vector<Attribute> Aggregation::getGroupByAttributeAfterAggregation()const{
	std::vector<Attribute> ret;

	for(unsigned i=0;i<group_by_attribute_list_.size();i++){
		Attribute temp=group_by_attribute_list_[i];
		temp.index=i;
		temp.table_id_=INTERMEIDATE_TABLEID;
		ret.push_back(temp);
	}
	return ret;
}
std::vector<Attribute> Aggregation::getAggregationAttributeAfterAggregation()const{
	std::vector<Attribute> ret;

	unsigned aggregation_start_index=group_by_attribute_list_.size();
		for(unsigned i=0;i<aggregation_attribute_list_.size();i++){
			Attribute temp=aggregation_attribute_list_[i];

			switch(aggregation_list_[i]){
				case BlockStreamAggregationIterator::State::count:{
					if(!(temp.isNULL()||temp.isANY()))
						temp.attrType->~column_type();
					temp.attrType=new column_type(data_type(t_u_long));
					temp.attrName="count("+temp.getName()+")";
					temp.index=aggregation_start_index++;
					temp.table_id_=INTERMEIDATE_TABLEID;
					break;
				}
				case BlockStreamAggregationIterator::State::max:{
					temp.attrName="max("+temp.getName()+")";
					temp.index=aggregation_start_index++;
					temp.table_id_=INTERMEIDATE_TABLEID;
					break;
				}
				case BlockStreamAggregationIterator::State::min:{
					temp.attrName="min("+temp.getName()+")";
					temp.index=aggregation_start_index++;
					temp.table_id_=INTERMEIDATE_TABLEID;
					break;
				}
				case BlockStreamAggregationIterator::State::sum:{
					temp.attrName="sum("+temp.getName()+")";
					temp.index=aggregation_start_index++;
					temp.table_id_=INTERMEIDATE_TABLEID;
					break;
				}
				default:{
					assert(false);
				}
			}
			ret.push_back(temp);
		}
		return ret;
}
unsigned long Aggregation::estimateGroupByCardinality(const Dataflow& dataflow)const{
	unsigned long data_card=dataflow.getAggregatedDataCardinality();
	unsigned long ret;
	for(unsigned i=0;i<group_by_attribute_list_.size();i++){
		if(group_by_attribute_list_[i].isUnique()){
			return ret=data_card;
		}
	}
	unsigned long group_by_domain_size=1;
	for(unsigned i=0;i<group_by_attribute_list_.size();i++){
		AttributeStatistics* attr_stat=StatManager::getInstance()->getAttributeStatistic(group_by_attribute_list_[i]);
		if(attr_stat==0){
			group_by_domain_size*=100;
		}
		else{
			group_by_domain_size*=attr_stat->getDistinctCardinality();
		}
	}
	ret=group_by_domain_size;//TODO: This is only the upper bound of group_by domain size;

	const unsigned long limits=1024*1024;
	ret=ret<limits?ret:limits;


	return ret;

}
void Aggregation::print(int level)const{
	printf("%*.sAggregation:\n",level*8," ");
	printf("%*.sgroup-by attributes:\n",level*8," ");
	for(unsigned i=0;i<this->group_by_attribute_list_.size();i++){
		printf("%*.s",level*8," ");
		printf("%s\n",group_by_attribute_list_[i].attrName.c_str());
	}
	printf("%*.saggregation attributes:\n",level*8," ");
	for(unsigned i=0;i<aggregation_attribute_list_.size();i++){
		printf("%*.s",level*8," ");
		switch(aggregation_list_[i]){
			case BlockStreamAggregationIterator::State::count:{
				printf("Count: %s\n",aggregation_attribute_list_[i].attrName.c_str());
				break;
			}
			case BlockStreamAggregationIterator::State::max:{
				printf("Max: %s\n",aggregation_attribute_list_[i].attrName.c_str());
				break;
			}
			case BlockStreamAggregationIterator::State::min:{
				printf("Min: %s\n",aggregation_attribute_list_[i].attrName.c_str());
				break;
			}
			case BlockStreamAggregationIterator::State::sum:{
				printf("Sum: %s\n",aggregation_attribute_list_[i].attrName.c_str());
				break;
			}
			default:{
				break;
			}
		}
	}
	child_->print(level+1);
//	right_child_->print(level+1);
}
