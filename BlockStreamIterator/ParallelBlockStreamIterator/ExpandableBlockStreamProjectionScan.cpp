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
#include <limits.h>
#include "../../rename.h"
#include "ExpandableBlockStreamProjectionScan.h"
#include "../../storage/BlockManager.h"
#include "../../Executor/ExpanderTracker.h"

ExpandableBlockStreamProjectionScan::ExpandableBlockStreamProjectionScan(State state)
:state_(state), partition_reader_iterator_(0) {
//	sema_open_.set_value(1);
//	sema_open_finished_.set_value(0);
	initialize_expanded_status();
}

ExpandableBlockStreamProjectionScan::ExpandableBlockStreamProjectionScan()
:partition_reader_iterator_(0){
//	sema_open_.set_value(1);
//	sema_open_finished_.set_value(0);
	initialize_expanded_status();
	}

ExpandableBlockStreamProjectionScan::~ExpandableBlockStreamProjectionScan() {
	// TODO Auto-generated destructor stub
}


ExpandableBlockStreamProjectionScan::State::State(ProjectionID projection_id,Schema* schema, unsigned block_size, float sample_rate)
:schema_(schema), projection_id_(projection_id), block_size_(block_size), sample_rate_(sample_rate) {
}


bool ExpandableBlockStreamProjectionScan::open(const PartitionOffset& partition_offset) {
////////////////////THE VERSION BEFORE USING ExpandableBlockStrreamIteratorBase ///////////////////////////
//	if (sema_open_.try_wait()) {
//		PartitionStorage* partition_handle_;
//		if((partition_handle_=BlockManager::getInstance()->getPartitionHandle(PartitionID(state_.projection_id_,partition_offset)))==0){
//			printf("The partition[%s] does not exists!\n",PartitionID(state_.projection_id_,partition_offset).getName().c_str());
//			open_ret_=false;
//		}
//		else{
//			partition_reader_iterator_=partition_handle_->createAtomicReaderIterator();
//		}
//		open_ret_=true;
//		open_finished_ = true;
//
//	} else {
//		while (!open_finished_) {
//			usleep(1);
//		}
//	}
//	return open_ret_;
///////////////////////////////////////////// END ////////////////////////////////////////////////
	if(tryEntryIntoSerializedSection()){
		/* this is the first expanded thread*/
		PartitionStorage* partition_handle_;
		return_blocks_=0;
		if((partition_handle_=BlockManager::getInstance()->getPartitionHandle(PartitionID(state_.projection_id_,partition_offset)))==0){
			printf("The partition[%s] does not exists!\n",PartitionID(state_.projection_id_,partition_offset).getName().c_str());
			open_ret_=false;
		}
		else{
			partition_reader_iterator_=partition_handle_->createAtomicReaderIterator();
		}
		open_ret_=true;
		ExpanderTracker::getInstance()->addNewStageEndpoint(pthread_self(),LocalStageEndPoint(stage_src,"Scan",0));
		broadcaseOpenFinishedSignal();
	}
	else{
		/* this is not the first thread, so it will wait for the first thread finishing initialization*/
		waitForOpenFinished();
	}
}

bool ExpandableBlockStreamProjectionScan::next(BlockStreamBase* block) {
	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
//		printf("<<<<<<<<<<<<<<<<<Scan detected call back signal!>>>>>>%lx>>>>>>>>>>>\n",pthread_self());
		return false;
	}
//	return false;
	allocated_block allo_block_temp;
	ChunkReaderIterator* chunk_reader_iterator;
	if(atomicPopChunkReaderIterator(chunk_reader_iterator)){
		/* there is unused ChunkReaderIterator*/
//		if(chunk_reader_iterator->nextBlock(block)){

		bool next;
		if(passSample()){
			next=chunk_reader_iterator->nextBlock(block);
		}
		else{
			next=chunk_reader_iterator->nextBlock();
		}
		if(next){
			/* there is still unread block*/
			atomicPushChunkReaderIterator(chunk_reader_iterator);
			lock_.acquire();
			return_blocks_++;
			lock_.release();
			return true;
		}
		else{
			/* the ChunkReaderIterator is exhausted, so we destructe it.*/

//			printf("Chunk(%d,%d,%d,%d) is exhausted!\n",chunk_reader_iterator->chunk_id_.partition_id.projection_id.table_id,chunk_reader_iterator->chunk_id_.partition_id.projection_id.projection_off,chunk_reader_iterator->chunk_id_.partition_id.partition_off,chunk_reader_iterator->chunk_id_.chunk_off);
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
	printf("ProjectoinScan[%d]: returned %ld blocks\n",state_.projection_id_.projection_off,return_blocks_);
//	sema_open_.post();
	partition_reader_iterator_->~PartitionReaderItetaor();
	open_finished_ = false;

	/* reset the expanded status in that open and next will be re-invoked.*/
	initialize_expanded_status();

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
bool ExpandableBlockStreamProjectionScan::passSample()const{
//	const float ram=(float)rand()/(float)RAND_MAX;
	if((rand()/(float)RAND_MAX)<state_.sample_rate_)
		return true;
	return false;
}
