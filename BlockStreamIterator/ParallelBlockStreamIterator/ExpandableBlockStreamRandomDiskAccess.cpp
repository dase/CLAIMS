/*
 * ExpandableBlockStreamRandomDiskAccess.cpp
 *
 *  Created on: 2013年10月17日
 *      Author: SCDONG
 */

#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include "../../common/rename.h"
#include "ExpandableBlockStreamRandomDiskAccess.h"

ExpandableBlockStreamRandomDiskAccess::ExpandableBlockStreamRandomDiskAccess(State state)
:state_(state), fd_(NULL), file_length_(0), oid_(0), open_finished_(false) {
	sema_open_.set_value(1);
}

ExpandableBlockStreamRandomDiskAccess::ExpandableBlockStreamRandomDiskAccess()
:fd_(NULL), file_length_(0), oid_(0), open_finished_(false) {
	sema_open_.set_value(1);
}

ExpandableBlockStreamRandomDiskAccess::~ExpandableBlockStreamRandomDiskAccess() {
	// TODO Auto-generated destructor stub
}

ExpandableBlockStreamRandomDiskAccess::State::State(std::string file_name, BlockStreamIteratorBase* child,
		Schema* f_schema, Schema* c_schema, unsigned block_size) :
		filename_(file_name), child_(child), f_schema_(f_schema), c_schema_(c_schema), block_size_(block_size) {

}

bool ExpandableBlockStreamRandomDiskAccess::open(const PartitionOffset& part_off) {
	AtomicPushFreeBlockStream(BlockStreamBase::createBlock(state_.c_schema_,state_.block_size_));
	printf("Free block stream list added!\n");

	if (sema_open_.try_wait()) {
		printf("RDA: Scan open!\n");

		/* the winning thread does the read job in the open function*/
		fd_ = fopen(state_.filename_.c_str(), "r+");
		if (NULL == fd_)
		{
			printf("Cannot open file %s!\n", state_.filename_.c_str());
			return false;
		}
		fseek(fd_, 0, SEEK_END);
		file_length_ = ftell(fd_);
		printf("RDA: Open is successful!\n");
		open_finished_ = true;
		return state_.child_->open();
	}
	else {
		while (!open_finished_) {
			usleep(1);
		}
		return state_.child_->open();
	}
}

bool ExpandableBlockStreamRandomDiskAccess::next(BlockStreamBase* block) {
	remaining_block rb;
	void* tuple_from_child;
	void* tuple_in_block;

	if (atomicPopRemainingBlock(rb))
	{
		while ((tuple_from_child = rb.iterator->currentTuple()) > 0)
		{
			const unsigned bytes = state_.f_schema_->getTupleActualSize(tuple_in_block);
			if ((tuple_in_block = block->allocateTuple(bytes)) > 0)
			{
				/* the block has enough space to hold this tuple */
				fseek(fd_, (*(int*)tuple_from_child)*bytes, SEEK_SET);
				fread(tuple_in_block, 1, state_.f_schema_->getTupleActualSize(tuple_in_block), fd_);
//				state_.f_schema_->copyTuple((void*)(base_+(*(int*)tuple_from_child)*bytes), tuple_in_block);
				rb.iterator->increase_cur_();
			}
			else
			{
				/* the block is full, before we return, we pop the remaining block. */
				atomicPushRemainingBlock(rb);
//				cout << "return 1 true\n";
				return true;
			}
		}
		AtomicPushFreeBlockStream(rb.block);
	}

	/* When the program arrivals here, it mains that there is no remaining blocks or the remaining block
	 * is exhausted, so we read a new block from the child
	 */
	BlockStreamBase* block_for_asking = AtomicPopFreeBlockStream();
	block_for_asking->setEmpty();
	while (state_.child_->next(block_for_asking))
	{
		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator = block_for_asking->createIterator();
		while((tuple_from_child = traverse_iterator->currentTuple()) > 0)
		{
			const unsigned bytes = state_.f_schema_->getTupleActualSize(tuple_from_child);
			if ((tuple_in_block = block->allocateTuple(bytes)) > 0)
			{
				/* the block has enough space to hold this tuple */
				fseek(fd_, (*(int*)tuple_from_child)*bytes, SEEK_SET);
				fread(tuple_in_block, 1, state_.f_schema_->getTupleActualSize(tuple_in_block), fd_);
//				state_.f_schema_->copyTuple((void*)(base_+(*(int*)tuple_from_child)*bytes), tuple_in_block);
				traverse_iterator->increase_cur_();
			}
			else
			{
				/* the block is full, before we return, we pop the remaining block. */
				atomicPushRemainingBlock(remaining_block(block_for_asking, traverse_iterator));
//				cout << "return 2 true\n";
				return true;
			}
		}
		/* the block_for_asking is exhausted, but the block is not full */
		traverse_iterator->~BlockStreamTraverseIterator();
		block_for_asking->setEmpty();
	}
	/* the child iterator is exhausted, but the block is not full */
	AtomicPushFreeBlockStream(block_for_asking);
	if (!block->Empty())
	{
//		cout << "return 3 true\n";
		return true;
	}
	else
	{
		cout << "Random disk access return false!\n";
		return false;
	}

}

bool ExpandableBlockStreamRandomDiskAccess::close() {
	sema_open_.post();
	open_finished_ = false;
	remaining_block_list.clear();
	free_block_stream_list_.clear();
	state_.child_->close();
	if (fclose(fd_) == 0) {
		std::cout<<"in "<<__FILE__<<":"<<__LINE__;printf("-----for debug: close fd %d.\n", fd_);
		return true;
	}
	else {
		return false;
	}
}

bool ExpandableBlockStreamRandomDiskAccess::atomicPopRemainingBlock(remaining_block & rb){
	lock_.acquire();

	if(remaining_block_list.size()>0){
		rb=remaining_block_list.front();
		remaining_block_list.pop_front();

		lock_.release();

		return true;
	}
	else{

		lock_.release();

		return false;
	}
}

void ExpandableBlockStreamRandomDiskAccess::atomicPushRemainingBlock(remaining_block rb){
	lock_.acquire();
	remaining_block_list.push_back(rb);
	lock_.release();
}

BlockStreamBase* ExpandableBlockStreamRandomDiskAccess::AtomicPopFreeBlockStream(){
	assert(!free_block_stream_list_.empty());
	lock_.acquire();
	BlockStreamBase *block=free_block_stream_list_.front();
	free_block_stream_list_.pop_front();
	lock_.release();
	return block;
}

void ExpandableBlockStreamRandomDiskAccess::AtomicPushFreeBlockStream(BlockStreamBase* block){
	lock_.acquire();
	free_block_stream_list_.push_back(block);
	lock_.release();
}
