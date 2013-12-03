/*
 * DataflowPartitioner.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#include "DataflowPartitionDescriptor.h"

DataflowPartitionDescriptor::DataflowPartitionDescriptor(const Partitioner& partitioner){
	partition_function_=partitioner.getPartitionFunction();
	partition_key_=partitioner.getPartitionKey();
	for(unsigned i=0;i<partitioner.getNumberOfPartitions();i++){
		DataflowPartition dataflow_partition;
		dataflow_partition.datasize_=partitioner.getPartitionDataSize(i);
		dataflow_partition.location_=partitioner.getPartitionLocation(i);
		dataflow_partition.partition_id_=i;
		partition_list_.push_back(dataflow_partition);
	}
}

DataflowPartitionDescriptor::~DataflowPartitionDescriptor() {

}
Attribute DataflowPartitionDescriptor::getPartitionKey()const{
	return partition_key_;
}

bool DataflowPartitionDescriptor::hasSamePartitionLocation(const DataflowPartitionDescriptor& target)const{
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

unsigned DataflowPartitionDescriptor::getAggregatedDatasize()const{
	unsigned ret=0;
	for(unsigned i=0;i<partition_list_.size();i++){
		ret+=partition_list_[i].datasize_;
	}
	return ret;
}
PartitionFunction::partition_fashion DataflowPartitionDescriptor::getPartitionFashion()const{
	return partition_function_->getPartitionFashion();
}
unsigned DataflowPartitionDescriptor::getNumberOfPartitions()const{
	return partition_list_.size();
}
DataflowPartition* DataflowPartitionDescriptor::getPartition(unsigned index)const{
	assert(index<partition_list_.size());
	DataflowPartition* ret=(DataflowPartition*)&partition_list_[index];
	return ret;
}
PartitionFunction* DataflowPartitionDescriptor::getPartitionFunction()const{
	return partition_function_;
}
void DataflowPartitionDescriptor::setPartitionKey(const Attribute& partitionkey){
	partition_key_=partitionkey;
}
void DataflowPartitionDescriptor::addShadowPartitionKey(const Attribute& partitionkey){
	shadow_partition_key_list_.push_back(partitionkey);
}

void DataflowPartitionDescriptor::setPartitionFunction(PartitionFunction* partition_function){
	partition_function_=partition_function;
}
bool DataflowPartitionDescriptor::hasShadowPartitionKey()const{
	return !shadow_partition_key_list_.empty();
}
std::vector<Attribute> DataflowPartitionDescriptor::getShadowAttributeList()const{
	return shadow_partition_key_list_;
}

std::vector<DataflowPartition> DataflowPartitionDescriptor::getPartitionList()const{
	return partition_list_;
}
void DataflowPartitionDescriptor::setPartitionList(const std::vector<DataflowPartition>& list){
	partition_list_=list;
}
