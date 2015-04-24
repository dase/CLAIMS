/*
 * PartitoinedBlockBuffer.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wangli
 */

#include "PartitionedBlockBuffer.h"
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "../../configure.h"
PartitionedBlockBuffer::PartitionedBlockBuffer(unsigned nPartitions,unsigned block_size,unsigned nBlocks)
:nPartitions(nPartitions),nBlocks(nBlocks){
	blocks_in_partition_list=new std::list<Block*>[nPartitions];
	for(unsigned i=0;i<nBlocks;i++){
		empty_block_list.push_back(new Block(block_size));
	}
	used_blocks.set_value(0);
	empty_blocks.set_value(nBlocks);
	inserted=0;
	removed=0;
}
PartitionedBlockBuffer::PartitionedBlockBuffer(unsigned nPartitions,unsigned block_size)
:nPartitions(nPartitions),nBlocks(nPartitions*3){
	blocks_in_partition_list=new std::list<Block*>[nPartitions];
	for(unsigned i=0;i<nBlocks;i++){
		Block* block=new Block(block_size);
//		 block->~Block();
		empty_block_list.push_back(block);
//		void* abc=malloc(block_size);		//newmalloc
//		free(abc);
//		free(empty_block_list.back()->getBlock());
	}
	used_blocks.set_value(0);
	empty_blocks.set_value(nBlocks);
	inserted=0;
	removed=0;
//	destoryEmptyBlocks();
}
PartitionedBlockBuffer::~PartitionedBlockBuffer() {
	assert(used_blocks.get_value()==0);
	for(unsigned i=0;i<nPartitions;i++){
		assert(blocks_in_partition_list[i].empty());
	}
	delete [] blocks_in_partition_list;
	unsigned free_count=0;
//	printf("in ~PartitionedBlockBuffer()\n");

	destoryEmptyBlocks();
}

bool PartitionedBlockBuffer::hasEmptyBlock()const{
	return !empty_block_list.empty();
}
bool PartitionedBlockBuffer::isEmpty(){
	return this->used_blocks.get_value()==0;

}
void PartitionedBlockBuffer::destoryEmptyBlocks(){
	unsigned free_count=0;
	while(!empty_block_list.empty()){
		Block* block=empty_block_list.front();
//		free(block->getBlock());
		empty_block_list.pop_front();
		delete block;
		free_count++;
	}
//	printf("%d buffer is freed!<><><><><><><><><>\n",free_count);
}

/**
 * 'copy' a block into the corresponding partitioned block list.
 * In fact, the swap of pointer is used rather than actually coping the content for
 * performance concern.
 */
bool PartitionedBlockBuffer::insertBlockToPartitionedList(Block* src, unsigned partition_id){
	lock.acquire();
	inserted++;
	lock.release();

	assert(partition_id<nPartitions);

	empty_blocks.wait();
	lock.acquire();

	/* pick up one empty block from the empty block list */
	Block* emptyBlock=empty_block_list.front();
	empty_block_list.pop_front();
	/* swap the content of src and emptyBlock*/
//	void* content=src->getBlock();
//	src->setBlock(emptyBlock->getBlock());
//	emptyBlock->setBlock(content);

	/*
	 * the above three lines are replaced by the following lines for memory leak debugging.
	 */
	assert(emptyBlock->getsize()==src->getsize());
	memcpy(emptyBlock->getBlock(),src->getBlock(),src->getsize());

	/* put the empty block into the partitioned list.*/
	blocks_in_partition_list[partition_id].push_back(emptyBlock);

	lock.release();
	used_blocks.post();
	return true;
}
unsigned PartitionedBlockBuffer::getBlockInBuffer(){
	lock.acquire();
	unsigned ret=used_blocks.get_value();
	lock.release();
	return ret;
}
unsigned PartitionedBlockBuffer::getBlockNumberOfPartitionInBuffer(unsigned partition_id){
	lock.acquire();
	unsigned ret=blocks_in_partition_list[partition_id].size();
	lock.release();
	return ret;
}
int PartitionedBlockBuffer::getBlock(Block &desc){
	int ret;
	used_blocks.wait();
	lock.acquire();
	std::vector<unsigned> non_empty_partition_index_list;
	for(unsigned i=0;i<nPartitions;i++){
		if(!blocks_in_partition_list[i].empty())
			non_empty_partition_index_list.push_back(i);
	}
//	if(non_empty_partition_index_list.empty()){
//		return -1;
//	}

	const unsigned seed=rand()%non_empty_partition_index_list.size();

	unsigned partition_id=non_empty_partition_index_list[seed];
	if(getBlockInPartitionedBlockList(desc,partition_id)==true){
		ret=partition_id;
	}


	lock.release();
	empty_blocks.post();

	return ret;
}

bool PartitionedBlockBuffer::getBlock(Block &desc,unsigned partition_id){
	if(getBlockNumberOfPartitionInBuffer(partition_id)>0){
		lock.acquire();
		getBlockInPartitionedBlockList(desc,partition_id);
		used_blocks.wait();
		empty_blocks.post();
		lock.release();
		return true;
	}
	else{
		return false;
	}
}

bool PartitionedBlockBuffer::getBlockInPartitionedBlockList(Block &desc,unsigned partition_id){
	assert(!blocks_in_partition_list[partition_id].empty());

	/* get one block from the head of the list */
	Block* targetBlock=blocks_in_partition_list[partition_id].front();
	blocks_in_partition_list[partition_id].pop_front();

	/* swap the content of the desc block and the target block*/
//	void* content=desc.getBlock();
//	desc.setBlock(targetBlock->getBlock());
//	targetBlock->setBlock(content);

	/*
	 * The three lines above are replaced by the following lines for memory leak debugging
	 */

	memcpy(desc.getBlock(),targetBlock->getBlock(),targetBlock->getsize());

	/* put the targetBlock into the empty list*/
	empty_block_list.push_back(targetBlock);

	return true;

}
