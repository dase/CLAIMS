/*
 * BlockStreamBuffer.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */

#include "BlockStreamBuffer.h"

BlockStreamBuffer::BlockStreamBuffer(unsigned block_size, unsigned block_count, Schema* schema) {

	for(unsigned i=0;i<block_count;i++){
		block_stream_empty_list_.push_back(BlockStreamBase::createBlock(schema,block_size));
	}
	sema_empty_block_.set_value(block_count);
}
//njcdhcjdhjhnjcdncjdncj
BlockStreamBuffer::~BlockStreamBuffer() {
	while(!block_stream_empty_list_.empty()){
		const BlockStreamBase* block=block_stream_empty_list_.front();
		block_stream_empty_list_.pop_front();
		block->~BlockStreamBase();
	}
//	assert(block_stream_used_list_.empty());
	sema_empty_block_.destroy();
}

void BlockStreamBuffer::insertBlock(BlockStreamBase* block){
	sema_empty_block_.wait();
	lock_.acquire();

	BlockStreamBase* empty_block=block_stream_empty_list_.front();
	block_stream_empty_list_.pop_front();

	/* swap the data address of empty block and the input block to avoid memory copy.*/
	block->switchBlock(*empty_block);

	/* add the empty block which now hold the read data into the used block list*/
	block_stream_used_list_.push_back(empty_block);

	lock_.release();
}

bool BlockStreamBuffer::getBlock(BlockStreamBase &block){
	lock_.acquire();

	if(!block_stream_used_list_.empty()){
		BlockStreamBase* fetched_block=block_stream_used_list_.front();
		block_stream_used_list_.pop_front();
		fetched_block->switchBlock(block);
		block_stream_empty_list_.push_back(fetched_block);
		sema_empty_block_.post();
		lock_.release();
		return true;
	}
	lock_.release();
	return false;
}

bool BlockStreamBuffer::Empty()  {
	lock_.acquire();
	const bool ret=block_stream_used_list_.empty();
	lock_.release();
	return ret;
}
unsigned BlockStreamBuffer::getBlockInBuffer(){
	lock_.acquire();
//	if(block_stream_used_list_.empty())
//		return 0;
//	return block_stream_used_list_.size();
	const bool ret=block_stream_used_list_.size();
	lock_.release();
	return ret;
}
