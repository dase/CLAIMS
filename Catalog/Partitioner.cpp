/*
 * Partitioner.cpp
 *
 *  Created on: 2013-7-9
 *      Author: root
 */
#include <assert.h>
#include <stdio.h>
#include "Partitioner.h"
#include "../Catalog/Catalog.h"
#define CHUNKSIZE_IN_MB 64

Partitioner::Partitioner(ProjectionID projection_id,unsigned number_of_partitions,PartitionFunction* partitioning_functin)
:projection_id_(projection_id),number_of_partitions_(number_of_partitions),partition_key_(0),partition_functin_(partitioning_functin),mode_(OneToOne)
{

}
Partitioner::Partitioner(ProjectionID projection_id,unsigned number_of_partitions,const Attribute &partition_key,PartitionFunction* partitioning_function)
:projection_id_(projection_id),number_of_partitions_(number_of_partitions),partition_functin_(partitioning_function),mode_(OneToOne){
	partition_key_=new Attribute(partition_key);
	for(unsigned i=0;i<number_of_partitions;i++){
		PartitionID pid(projection_id_,i);
		partition_info_list.push_back(new OneToOnePartitionInfo(pid));
	}
}

Partitioner::~Partitioner() {
	// TODO Auto-generated destructor stub
}
unsigned Partitioner::getNumberOfPartitions()const{
	return partition_functin_->getNumberOfPartitions();
}

bool Partitioner::bindPartitionToNode(PartitionOffset partition_id,NodeID target_node){
//	if(partitionid_to_nodeid_.find(partition_id)!=partitionid_to_nodeid_.end()){
//		/* the partition has already been bind to a node.*/
//		return false;
//	}
//	partitionid_to_nodeid_[partition_id]=target_node;
//	return true;

	printf("PartitionID:%d is bound to NodeID:%d\n",partition_id,target_node);
//
//	if(partition_node_blockinfo_.contains(partition_id)){
//		return false;
//	}
//
//	pair<NodeID,vector<BlockInfo> > tmp;
//	tmp.first=target_node;
//	tmp.second=vector<BlockInfo>();
//
//	partition_node_blockinfo_[partition_id]=tmp;
//	return true;
	return partition_info_list[partition_id]->bind_all_blocks(target_node);
}

void Partitioner::unbindPartitionToNode(PartitionOffset partition_id){
//	std::map<PartitionID,NodeID>::iterator target;
//	if(partitionid_to_nodeid_.find(partition_id)!=partitionid_to_nodeid_.end())
//		return false;
//	partitionid_to_nodeid_.erase(target);
	partition_info_list[partition_id]->unbind_all_blocks();

}

void Partitioner::RegisterPartition(unsigned partition_key,unsigned number_of_chunks){
	assert(partition_key<partition_functin_->getNumberOfPartitions());

	partition_info_list[partition_key]->hdfs_file_name=partition_info_list[partition_key]->partition_id_.getName();
	partition_info_list[partition_key]->number_of_blocks=number_of_chunks;

}

void Partitioner::print(){
//	hashmap<PartitionID,NodeID>::iterator it=partitionid_to_nodeid_.begin();
//	while(it!=partitionid_to_nodeid_.end()){
//		printf("<%d, %d>\n",it->first,it->second);
//		it++;
//	}
}
bool Partitioner::hasSamePartitionLocation(const Partitioner & target_partition )const{
	if(mode_==OneToMany||target_partition.get_bing_mode_()==OneToMany){
		/** in the current version, any the location detection in OneToMany mode is ommited.*/
		return false;
	}
	if(getNumberOfPartitions()!=target_partition.getNumberOfPartitions())
		return false;
	for(unsigned i=0;i<getNumberOfPartitions();i++){
		if(!partition_info_list[i]->is_colocated(*target_partition.partition_info_list[i])){
			return false;
		}
	}
	return true;
}
unsigned Partitioner::getPartitionDataSize(unsigned partitoin_index)const{
	return partition_info_list[partitoin_index]->number_of_blocks*CHUNKSIZE_IN_MB;
}
unsigned long Partitioner::getPartitionCardinality(unsigned partition_index)const{
	unsigned tuple_bytes=Catalog::getInstance()->getProjection(projection_id_)->getSchema()->getTupleMaxSize();
	const unsigned data_size_in_MB=getPartitionDataSize(partition_index);
	return data_size_in_MB*(unsigned long)1024*1024/tuple_bytes;
}
unsigned Partitioner::getPartitionChunks(unsigned partitoin_index)const{
	return partition_info_list[partitoin_index]->number_of_blocks;
}
NodeID Partitioner::getPartitionLocation(unsigned partition_index)const{
	if(partition_info_list[partition_index]->get_mode()==OneToOne){
//		return (*(OneToOnePartitionInfo*)&partition_info_list[partition_index]).get_location();
		return partition_info_list[partition_index]->get_location();
	}
	else{
		return -1;
	}
}

PartitionFunction::partition_fashion Partitioner::getPartitionFashion()const{
	return partition_functin_->getPartitionFashion();
}
Attribute Partitioner::getPartitionKey()const{
	return *partition_key_;
}
PartitionFunction* Partitioner::getPartitionFunction()const{
	return partition_functin_;
}
ProjectionID Partitioner::getProejctionID()const{
	return projection_id_;
}
bool Partitioner::allPartitionBound()const{
	for(unsigned i=0;i<number_of_partitions_;i++){
		if(!partition_info_list[i]->is_all_blocks_bound()){
			return false;
		}
	}
	return true;
}
vector<PartitionID> Partitioner::getPartitionIDList()
{
	vector<PartitionID> ret;
	ret.clear();
	for (vector<PartitionInfo*>::iterator iter = partition_info_list.begin(); iter != partition_info_list.end(); iter++)
		ret.push_back((*iter)->partition_id_);
	return ret;
}
bool OneToOnePartitionInfo::is_colocated(const PartitionInfo & target)const{
	if(target.get_mode()==OneToMany)
		return false;
	if(binding_node_id_==-1||((OneToOnePartitionInfo*)&target)->binding_node_id_==-1)
		return false;
	return binding_node_id_==((OneToOnePartitionInfo*)&target)->binding_node_id_;

}
bool OneToManyPartitionInfo::is_colocated(const PartitionInfo & target)const{
	return false;
}
