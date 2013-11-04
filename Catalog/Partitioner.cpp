/*
 * Partitioner.cpp
 *
 *  Created on: 2013-7-9
 *      Author: root
 */
#include <assert.h>
#include <stdio.h>
#include "Partitioner.h"
#define CHUNKSIZE_IN_MB 64

Partitioner::Partitioner(unsigned number_of_partitions,PartitionFunction* partitioning_functin)
:number_of_partitions_(number_of_partitions),partition_key_(0),partition_functin_(partitioning_functin),mode_(OneToOne)
{

}
Partitioner::Partitioner(unsigned number_of_partitions,Attribute &partition_key,PartitionFunction* partitioning_function)
:number_of_partitions_(number_of_partitions),partition_functin_(partitioning_function),mode_(OneToOne){
	partition_key_=new Attribute(partition_key);
}

Partitioner::~Partitioner() {
	// TODO Auto-generated destructor stub
}
unsigned Partitioner::getNumberOfPartitions(){
	return partition_functin_->getNumberOfPartitions();
}

bool Partitioner::bindPartitionToNode(PartitionID partition_id,NodeID target_node){
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

void Partitioner::unbindPartitionToNode(PartitionID partition_id){
//	std::map<PartitionID,NodeID>::iterator target;
//	if(partitionid_to_nodeid_.find(partition_id)!=partitionid_to_nodeid_.end())
//		return false;
//	partitionid_to_nodeid_.erase(target);
	partition_info_list[partition_id]->unbind_all_blocks();

}

void Partitioner::RegisterPartition(unsigned partitoin_key,std::string file_name,unsigned number_of_chunks){
	assert(partitoin_key<partition_functin_->getNumberOfPartitions());
//	partitionid_to_filename_[partitoin_key]=file_name;

}

void Partitioner::print(){
//	hashmap<PartitionID,NodeID>::iterator it=partitionid_to_nodeid_.begin();
//	while(it!=partitionid_to_nodeid_.end()){
//		printf("<%d, %d>\n",it->first,it->second);
//		it++;
//	}
}
unsigned Partitioner::getPartitionDataSize(unsigned partitoin_index){
	return partition_info_list[partitoin_index]->number_of_blocks*CHUNKSIZE_IN_MB;
}
