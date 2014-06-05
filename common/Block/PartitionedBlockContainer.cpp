/*
 * PartitionedBlockContainer.cpp
 *
 *  Created on: Sep 22, 2013
 *      Author: wangli
 */
#include <stdlib.h>
#include "PartitionedBlockContainer.h"

PartitionedBlockContainer::PartitionedBlockContainer(unsigned partition_count,unsigned block_size)
:partition_count_(partition_count),block_size_(block_size)
{
	for(unsigned i=0;i<partition_count_;i++){
		block_list_.push_back(new BlockContainer(block_size_));
		status_list_.push_back(status());
	}
}

PartitionedBlockContainer::~PartitionedBlockContainer() {
	for(unsigned i=0;i<partition_count_;i++){
		delete block_list_[i];
	}
	block_list_.clear();
	status_list_.clear();
}

int PartitionedBlockContainer::getBlockForSending(BlockContainer* &block){
//	std::vector<unsigned> avaiable_index;
//	for(unsigned i=0;i<partition_count_;i++){
//		if(status_list_[i].avaiable==true)
//			avaiable_index.push_back(i);
//	}
//	if(avaiable_index.empty())
//		return -1;
//	unsigned rand_seed=rand()%avaiable_index.size();
	unsigned rand_seed=rand()%partition_count_;
	block=block_list_[rand_seed];
	return rand_seed;
}

void PartitionedBlockContainer::insert(unsigned partition_id,BlockContainer* & block){
	BlockContainer* temp=block_list_[partition_id];
	block_list_[partition_id]=block;
	block=temp;
//	status_list_[partition_id].avaiable=true;
}

void PartitionedBlockContainer::Initialized(){
	for(unsigned i=0;i<partition_count_;i++){
		block_list_[i]->reset();
		block_list_[i]->IncreaseActualSize(block_list_[i]->GetMaxSize());
//		status_list_[i].avaiable=true;
	}
}
