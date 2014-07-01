/*
 * BlockStreamBuffer.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */

#include "BlockStreamBuffer.h"

BlockStreamBuffer::BlockStreamBuffer(unsigned block_size, unsigned block_count, Schema* schema):received_block_count_(0),total_block_count_(block_count),block_size_(block_size) {
	lock_.acquire();
	for(unsigned i=0;i<block_count;i++){
		block_stream_empty_list_.push_back(BlockStreamBase::createBlock(schema,block_size));
		sema_empty_block_.post();
	}
//	sema_empty_block_.set_value(block_count);
	assert(block_stream_empty_list_.size()==sema_empty_block_.get_value());
	lock_.release();
}
//njcdhcjdhjhnjcdncjdncj
BlockStreamBuffer::~BlockStreamBuffer() {
//	printf("BlockStreawmBuffer being deconstructed<><><><><><><><><>!\n");
	while(!block_stream_empty_list_.empty()){
		const BlockStreamBase* block=block_stream_empty_list_.front();
		block_stream_empty_list_.pop_front();
//		block->~BlockStreamBase();
		delete block;
	}

	while(!block_stream_used_list_.empty()){
		const BlockStreamBase* block=block_stream_used_list_.front();
		block_stream_used_list_.pop_front();
//		block->~BlockStreamBase();
		delete block;
	}
//	assert(block_stream_used_list_.empty());
	sema_empty_block_.destroy();
//	printf("BlockStreawmBuffer being deconstructed<><><><><><><><><>!\n");
}

void BlockStreamBuffer::insertBlock(BlockStreamBase* block){

	sema_empty_block_.wait();
	lock_.acquire();

	BlockStreamBase* empty_block=block_stream_empty_list_.front();
	block_stream_empty_list_.pop_front();

//	assert(block_stream_empty_list_.size()==sema_empty_block_.get_value());

	if(block->isIsReference()){
		empty_block->deepCopy(block);
	}
	else{
		/* swap the data address of empty block and the input block to avoid memory copy.*/
		block->switchBlock(*empty_block);
	}
	/* add the empty block which now hold the read data into the used block list*/
	block_stream_used_list_.push_back(empty_block);
//	printf("[after inserted]: sema=%d, empty_list=%d\n",sema_empty_block_.get_value(),block_stream_empty_list_.size());
	received_block_count_++;
	lock_.release();
}

bool BlockStreamBuffer::getBlock(BlockStreamBase &block){
//	assert(block_stream_empty_list_.size()==sema_empty_block_.get_value());
	lock_.acquire();
//	assert(block_stream_empty_list_.size()==sema_empty_block_.get_value());
	if(!block_stream_used_list_.empty()){
		BlockStreamBase* fetched_block=block_stream_used_list_.front();
		block_stream_used_list_.pop_front();


		fetched_block->switchBlock(block);
		block_stream_empty_list_.push_back(fetched_block);
		sema_empty_block_.post();
//		assert(block_stream_empty_list_.size()==sema_empty_block_.get_value());
//		printf("[after get(suc)]:sema=%d, empty_list=%d\n",sema_empty_block_.get_value(),block_stream_empty_list_.size());
		lock_.release();
		return true;
	}
//	printf("[after get(fail)]sema=%d, empty_list=%d\n",sema_empty_block_.get_value(),block_stream_empty_list_.size());
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
	const unsigned ret=block_stream_used_list_.size();
	lock_.release();
	return ret;
}
double BlockStreamBuffer::getBufferUsage(){
	return (double)getBlockInBuffer()/(double)total_block_count_;
}
long BlockStreamBuffer::getReceivedDataSizeInKbytes(){
	return received_block_count_*block_size_/1024;
}
