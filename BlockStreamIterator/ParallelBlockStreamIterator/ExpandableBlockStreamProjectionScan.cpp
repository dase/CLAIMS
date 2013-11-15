/*
 * ExpandableBlockStreamProjectionScan.cpp
 *
 *  Created on: Nov.14, 2013
 *      Author: wangli
 */
#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include "../../rename.h"
#include "ExpandableBlockStreamProjectionScan.h"
#include "../../storage/BlockManager.h"

ExpandableBlockStreamProjectionScan::ExpandableBlockStreamProjectionScan(State state)
:state_(state), open_finished_(false),partition_reader_iterator_(0) {
	sema_open_.set_value(1);
	sema_open_finished_.set_value(0);
}

ExpandableBlockStreamProjectionScan::ExpandableBlockStreamProjectionScan()
:open_finished_(false),partition_reader_iterator_(0){
	sema_open_.set_value(1);
	sema_open_finished_.set_value(0);
	}

ExpandableBlockStreamProjectionScan::~ExpandableBlockStreamProjectionScan() {
	// TODO Auto-generated destructor stub
}


ExpandableBlockStreamProjectionScan::State::State(ProjectionID projection_id,Schema* schema, unsigned block_size)
:schema_(schema), projection_id_(projection_id), block_size_(block_size) {
}


bool ExpandableBlockStreamProjectionScan::open() {
	PartitionStorage* partition_handle_;
	if (sema_open_.try_wait()) {
		if((partition_handle_=BlockManager::getInstance()->getPartitionHandle(PartitionID(state_.projection_id_,partition_offset_)))==0){
			printf("The partition[%s] does not exists!",PartitionID(state_.projection_id_,partition_offset_).getName().c_str());
		}
		partition_reader_iterator_=partition_handle_->createAtomicReaderIterator();

		open_finished_ = true;


	} else {
		while (!open_finished_) {
			usleep(1);
		}
	}
	return partition_handle_!=0;
}

bool ExpandableBlockStreamProjectionScan::next(BlockStreamBase* block) {
	allocated_block allo_block_temp;
	ChunkReaderIterator* chunk_reader_iterator;
	if(atomicPopChunkReaderIterator(chunk_reader_iterator)){
		/* there is unused ChunkReaderIterator*/
		if(chunk_reader_iterator->nextBlock(block)){
			/* there is still unread block*/
			atomicPushChunkReaderIterator(chunk_reader_iterator);
			return true;
		}
		else{
			/* the ChunkReaderIterator is exhausted, so we destructe it.*/
			chunk_reader_iterator->~ChunkReaderIterator();
		}
	}
	/* there isn't any unused ChunkReaderIterator or the ChunkReaderIterator is exhausted,
	 * so we create new one*/
	if((chunk_reader_iterator=partition_reader_iterator_->nextChunk())!=0){
		atomicPushChunkReaderIterator(chunk_reader_iterator);
		return next(block);
	}
	else{
		return false;
	}
}

bool ExpandableBlockStreamProjectionScan::close() {
	sema_open_.post();

	open_finished_ = false;
	return true;
}


void ExpandableBlockStreamProjectionScan::atomicPushChunkReaderIterator(ChunkReaderIterator* item){
	chunk_reader_container_lock_.acquire();
	remaining_chunk_reader_iterator_list_.push_back(item);
	chunk_reader_container_lock_.release();
}
bool ExpandableBlockStreamProjectionScan::atomicPopChunkReaderIterator(ChunkReaderIterator*& target){
	bool ret;
	chunk_reader_container_lock_.acquire();
	if(remaining_chunk_reader_iterator_list_.size()==0){
		ret=false;
	}
	else{
		target=remaining_chunk_reader_iterator_list_.front();
		remaining_chunk_reader_iterator_list_.pop_front();
		ret=true;
	}
	chunk_reader_container_lock_.release();
	return ret;
}
