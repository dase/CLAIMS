/*
 * PartitoinedBlockBuffer.h
 *
 *  Created on: Aug 15, 2013
 *      Author: wangli
 */

#ifndef PARTITOINEDBLOCKBUFFER_H_
#define PARTITOINEDBLOCKBUFFER_H_
#include "Block.h"
#include <list>
#include <stdio.h>
#include "../utility/lock.h"
class PartitionedBlockBuffer {
public:

	PartitionedBlockBuffer(unsigned nPartitions,unsigned block_size,unsigned nBlocks);
	PartitionedBlockBuffer(unsigned nPartitions,unsigned block_size);

	virtual ~PartitionedBlockBuffer();
	bool hasEmptyBlock()const ;
	bool isEmpty();
	bool insertBlockToPartitionedList(Block* src, unsigned partition_id);

	/* In current implementation, each non-empty partition list has the same
	 * probability to be consumed a block.
	 * TODO: a better solution is that the partiton lish containing more
	 * block should have bigger probability to be consumed a block*/
	int getBlock(Block &desc);

	bool getBlock(Block &desc,unsigned partition_id);
	unsigned getBlockInBuffer();

private:
	/* this is not thread-safe */
	bool getBlockInPartitionedBlockList(Block &desc,unsigned partition_id);
	unsigned getBlockNumberOfPartitionInBuffer(unsigned partition_id);
	void destoryEmptyBlocks();
private:
	unsigned nPartitions;
	unsigned nBlocks;
	std::list<Block*>* blocks_in_partition_list;
	std::list<Block*> empty_block_list;

	/* keeping the consistency while multiple threads are accessing
	 * the same instance of this class*/
	Lock lock;
	semaphore empty_blocks;
	semaphore used_blocks;
public:
	//debug
	unsigned inserted;
	unsigned removed;
};

#endif /* PARTITOINEDBLOCKBUFFER_H_ */
