/*
 * DataflowPartitioner.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#include "../logical_query_plan/DataflowPartitionDescriptor.h"

#include "../utility/print_tool.h"

DataflowPartitioningDescriptor::DataflowPartitioningDescriptor(const Partitioner& partitioner){
	partition_function_=partitioner.getPartitionFunction();
	partition_key_=partitioner.getPartitionKey();
	for(unsigned i=0;i<partitioner.getNumberOfPartitions();i++){
		DataflowPartition dataflow_partition;
		dataflow_partition.cardinality_=partitioner.getPartitionCardinality(i);
		dataflow_partition.location_=partitioner.getPartitionLocation(i);
		dataflow_partition.partition_id_=i;
		partition_list_.push_back(dataflow_partition);
	}
}
DataflowPartitioningDescriptor::DataflowPartitioningDescriptor(const DataflowPartitioningDescriptor& r){
	this->partition_function_=r.partition_function_;
	this->partition_key_=r.partition_key_;
	this->partition_list_=r.partition_list_;
	this->shadow_partition_key_list_=r.shadow_partition_key_list_;
}
DataflowPartitioningDescriptor::~DataflowPartitioningDescriptor() {

}
Attribute DataflowPartitioningDescriptor::getPartitionKey()const{
	return partition_key_;
}

bool DataflowPartitioningDescriptor::hasSamePartitionLocation(const DataflowPartitioningDescriptor& target)const{
	if(partition_list_.size()!=target.partition_list_.size())
		return false;
	for(unsigned i=0;i<partition_list_.size();i++){
		if(partition_list_[i].isAvaliable()&&target.partition_list_[i].isAvaliable()){
		/**
		 * if no party is filtered.
		 */
			if(partition_list_[i].location_!=target.partition_list_[i].location_)
				return false;
		}
	}
	return true;
}

unsigned DataflowPartitioningDescriptor::getAggregatedDatasize()const{
	unsigned ret=0;
	for(unsigned i=0;i<partition_list_.size();i++){
		ret+=partition_list_[i].cardinality_;
	}
	return ret;
}
unsigned long DataflowPartitioningDescriptor::getAggregatedDataCardinality()const{
	unsigned long ret=0;
	for(unsigned i=0;i<partition_list_.size();i++){
		ret+=partition_list_[i].getDataCardinality();
	}
	return ret;
}
PartitionFunction::partition_fashion DataflowPartitioningDescriptor::getPartitionFashion()const{
	return partition_function_->getPartitionFashion();
}
unsigned DataflowPartitioningDescriptor::getNumberOfPartitions()const{
	return partition_list_.size();
}
DataflowPartition* DataflowPartitioningDescriptor::getPartition(unsigned index)const{
	assert(index<partition_list_.size());
	DataflowPartition* ret=(DataflowPartition*)&partition_list_[index];
	return ret;
}
PartitionFunction* DataflowPartitioningDescriptor::getPartitionFunction()const{
	return partition_function_;
}
void DataflowPartitioningDescriptor::setPartitionKey(const Attribute& partitionkey){
	partition_key_=partitionkey;
}
void DataflowPartitioningDescriptor::addShadowPartitionKey(const Attribute& partitionkey){
	shadow_partition_key_list_.push_back(partitionkey);
}

void DataflowPartitioningDescriptor::setPartitionFunction(PartitionFunction* partition_function){
	partition_function_=partition_function;
}
bool DataflowPartitioningDescriptor::hasShadowPartitionKey()const{
	return !shadow_partition_key_list_.empty();
}
std::vector<Attribute> DataflowPartitioningDescriptor::getShadowAttributeList()const{
	return shadow_partition_key_list_;
}

std::vector<DataflowPartition> DataflowPartitioningDescriptor::getPartitionList()const{
	return partition_list_;
}
void DataflowPartitioningDescriptor::setPartitionList(const std::vector<DataflowPartition>& list){
	partition_list_=list;
}
