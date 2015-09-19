/*
 * BlockStreamRandomMemAccess.cpp
 *
 *  Created on: 2013年9月9日
 *      Author: SCDONG
 */

#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include "../common/rename.h"
#include "BlockStreamRandomMemAccess.h"

BlockStreamRandomMemAccess::BlockStreamRandomMemAccess(State state)
:state_(state), block_for_asking_(0), block_for_asking_iterator_(0), fd_(0), base_(NULL), data_(NULL), file_length_(0), oid_(0)
{

}

BlockStreamRandomMemAccess::~BlockStreamRandomMemAccess() {

}

BlockStreamRandomMemAccess::State::State(std::string file_name, BlockStreamIteratorBase* child, Schema* schema_in, Schema* schema_out, unsigned block_size)
:filename_(file_name), child_(child), schema_in_(schema_in), schema_out_(schema_out), block_size_(block_size) {

}

bool BlockStreamRandomMemAccess::open(const PartitionOffset& part_off)
{
	fd_=FileOpen(state_.filename_.c_str(),O_RDONLY);
	if(fd_==-1){
		printf("Cannot open file %s! Reason: %s\n",state_.filename_.c_str(),strerror(errno));
		return false;
	}
	file_length_=lseek(fd_,0,SEEK_END);

	base_=(char*)mmap(0,file_length_,PROT_READ,MAP_PRIVATE,fd_,0);

	if(base_==0){
		printf("mmap errors!\n");
		return false;
	}

	data_=(char*)malloc(file_length_);		//newmalloc

	if(data_==0){
		printf("malloc errors!\n");
		return false;
	}
	memcpy(data_,base_,file_length_);
	if(data_==0)
		return false;

	block_for_asking_ = BlockStreamBase::createBlock(state_.schema_in_, state_.block_size_);
	block_for_asking_iterator_ = block_for_asking_->createIterator();

	return state_.child_->open();
}

bool BlockStreamRandomMemAccess::next(BlockStreamBase *block)
{
	void* tuple_in_child;
	void* tuple_in_block;

	while (tuple_in_child = block_for_asking_iterator_->currentTuple()) {//_->nextTuple()) {
		if ((tuple_in_block = block->allocateTuple(state_.schema_out_->getTupleActualSize(tuple_in_child))) > 0) {
			//test print oid
//			printf("Oids: %d\n", *(int*)tuple_in_child);
//			sleep(5);
			state_.schema_out_->copyTuple((void*)((char*)base_+(*(int*)tuple_in_child)*8), tuple_in_block);
//			printf("Records: %d\t%d\n", *(int*)tuple_in_block, *((int*)tuple_in_block+1));
//			sleep(5);
			block_for_asking_iterator_->increase_cur_();
		}
		else {
			return true;
		}
	}

	block_for_asking_->setEmpty();
	if (state_.child_->next(block_for_asking_)) {
		block_for_asking_iterator_->reset();
		return next(block);
	}
	else {
		return false;
	}
}

bool BlockStreamRandomMemAccess::close()
{
	block_for_asking_->~BlockStreamBase();
	free(data_);
	data_ = 0;
//	munmap(base, length);

	if(0 == FileClose(fd_)) {
		std::cout<<"in "<<__FILE__<<":"<<__LINE__;printf("-----for debug: close fd %d.\n", fd_);
		return true;
	}
	return false;
}


