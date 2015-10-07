/*
 * ExpandableBlockStreamProjectionScan.cpp
 *
 *  Created on: Nov.14, 2013
 *      Author: wangli
 */
#include "../physical_query_plan/ExpandableBlockStreamProjectionScan.h"

#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include "../../common/rename.h"
#include "../../storage/BlockManager.h"
#include "../../Executor/ExpanderTracker.h"
#include "../../Config.h"
#include "../../utility/warmup.h"
#include "../../storage/ChunkStorage.h"

//#define AVOID_CONTENTION_IN_SCAN

ExpandableBlockStreamProjectionScan::ExpandableBlockStreamProjectionScan(State state)
:state_(state), partition_reader_iterator_(0) {
	InitExpandedStatus();
}

ExpandableBlockStreamProjectionScan::ExpandableBlockStreamProjectionScan()
:partition_reader_iterator_(0){
	InitExpandedStatus();
}

ExpandableBlockStreamProjectionScan::~ExpandableBlockStreamProjectionScan() {
	delete state_.schema_;
}


ExpandableBlockStreamProjectionScan::State::State(ProjectionID projection_id,Schema* schema, unsigned block_size, float sample_rate)
:schema_(schema), projection_id_(projection_id), block_size_(block_size), sample_rate_(sample_rate) {
}


bool ExpandableBlockStreamProjectionScan::Open(const PartitionOffset& partition_offset) {
	if(TryEntryIntoSerializedSection()){
		/* this is the first expanded thread*/
		PartitionStorage* partition_handle_;
		return_blocks_=0;
		if((partition_handle_=BlockManager::getInstance()->getPartitionHandle(PartitionID(state_.projection_id_,partition_offset)))==0){
			printf("The partition[%s] does not exists!\n",PartitionID(state_.projection_id_,partition_offset).getName().c_str());
			SetReturnStatus(false);
		}
		else{
			partition_reader_iterator_=partition_handle_->createAtomicReaderIterator();
			SetReturnStatus(true);
		}

#ifdef AVOID_CONTENTION_IN_SCAN
		unsigned long long start=curtick();

		ChunkReaderIterator* chunk_reader_it;
		ChunkReaderIterator::block_accessor* ba;
		while(chunk_reader_it=partition_reader_iterator_->nextChunk()){
			while(chunk_reader_it->getNextBlockAccessor(ba)){
				ba->getBlockSize();
				input_dataset_.input_data_blocks.push_back(ba);
			}
		}
//		printf("%lf seconds for initializing!\n",getSecond(start));
#endif
		ExpanderTracker::getInstance()->addNewStageEndpoint(pthread_self(),LocalStageEndPoint(stage_src,"Scan",0));
		perf_info=ExpanderTracker::getInstance()->getPerformanceInfo(pthread_self());
		perf_info->initialize();
	}
	BarrierArrive();
	return GetReturnStatus();
}

bool ExpandableBlockStreamProjectionScan::Next(BlockStreamBase* block) {
	unsigned long long total_start=curtick();
#ifdef AVOID_CONTENTION_IN_SCAN
	scan_thread_context* stc=(scan_thread_context*)GetContext();
	if(stc==0){
		stc=new scan_thread_context();
		InitContext(stc);
	}
	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
		//		printf("<<<<<<<<<<<<<<<<<Scan detected call back signal!>>>>>>%lx>>>>>>>>>>>\n",pthread_self());
		input_dataset_.atomicPut(stc->assigned_data_);
		delete stc;
		destorySelfContext();
		perf_info->report_instance_performance_in_millibytes();
		return false;
	}

	if(!stc->assigned_data_.empty()){
		ChunkReaderIterator::block_accessor* ba=stc->assigned_data_.front();
		stc->assigned_data_.pop_front();
//		const unsigned long long int start=curtick();

		ba->getBlock(block);

		// whether delete InMemeryBlockAccessor::target_block_start_address
		// is depend on whether use copy in ba->getBlock(block);
		delete ba;
//
//		void* start_addr=((ChunkReaderIterator::InMemeryBlockAccessor*)ba)->getTargetBlockStartAddress();
//		memcpy(block->getBlock(),
//				start_addr,block->getSerializedBlockSize()-4);
//		const unsigned tuple_count=*(unsigned*)(start_addr+block->getSerializedBlockSize()-4);
////		printf("tuple count=%d",tuple_count);
//		((BlockStreamFix*)block)->free_=(char*)((BlockStreamFix*)block)->getBlock()+tuple_count*state_.schema_->getTupleMaxSize();


//		warmup(block->getBlock(),block->getSerializedBlockSize());
//		printf("%ld cycles\n",curtick()-start);

//		printf("scan_call %ld cycles\n",curtick()-total_start);
		perf_info->processed_one_block();
//		printf("[]");
//		perf_info->report_instance_performance_in_millibytes();
		return true;
	}
	else{
		if(input_dataset_.atomicGet(stc->assigned_data_,Config::scan_batch)){
			return Next(block);
		}
		else{
			delete stc;
			destorySelfContext();
//			printf("scan_call %ld cycles\n",curtick()-total_start);
//			perf_info->report_instance_performance_in_millibytes();
			return false;
		}
	}

#else

	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
		//		printf("<<<<<<<<<<<<<<<<<Scan detected call back signal!>>>>>>%lx>>>>>>>>>>>\n",pthread_self());
		return false;
	}
	perf_info->processed_one_block();
	return partition_reader_iterator_->nextBlock(block);

#endif



//	//	return false;
//	allocated_block allo_block_temp;
//	ChunkReaderIterator* chunk_reader_iterator;
//	if(atomicPopChunkReaderIterator(chunk_reader_iterator)){
//		/* there is unused ChunkReaderIterator*/
//		//		if(chunk_reader_iterator->nextBlock(block)){
//
//		bool next;
//		if(passSample()){
//			next=chunk_reader_iterator->nextBlock(block);
//		}
//		else{
//			next=chunk_reader_iterator->nextBlock();
//		}
//		if(next){
//			/* there is still unread block*/
//			atomicPushChunkReaderIterator(chunk_reader_iterator);
//			lock_.acquire();
//			return_blocks_++;
//			lock_.release();
//			printf("r!!!\n");
//			perf_info->processed_one_block();
//			return true;
//		}
//		else{
//			/* the ChunkReaderIterator is exhausted, so we destructe it.*/
//
//			//			printf("Chunk(%d,%d,%d,%d) is exhausted!\n",chunk_reader_iterator->chunk_id_.partition_id.projection_id.table_id,chunk_reader_iterator->chunk_id_.partition_id.projection_id.projection_off,chunk_reader_iterator->chunk_id_.partition_id.partition_off,chunk_reader_iterator->chunk_id_.chunk_off);
//			chunk_reader_iterator->~ChunkReaderIterator();
//
//		}
//	}
//	/* there isn't any unused ChunkReaderIterator or the ChunkReaderIterator is exhausted,
//	 * so we create new one*/
//	if((chunk_reader_iterator=partition_reader_iterator_->nextChunk())!=0){
//		atomicPushChunkReaderIterator(chunk_reader_iterator);
//		return next(block);
//	}
//	else{
//		printf("**********Scan is exhausted!\n");
//		return false;
//	}
}

bool ExpandableBlockStreamProjectionScan::Close() {
	delete partition_reader_iterator_;
	partition_reader_iterator_ = NULL;

	DestoryAllContext();

	/* reset the expanded status in that open and next will be re-invoked.*/
	InitExpandedStatus();

	return true;
}



void ExpandableBlockStreamProjectionScan::Print() {
	printf("Scan (ID=%d)\n",state_.projection_id_.table_id);
}

bool ExpandableBlockStreamProjectionScan::passSample() const {
	//	const float ram=(float)rand()/(float)RAND_MAX;
	if((rand()/(float)RAND_MAX)<state_.sample_rate_)
		return true;
	return false;
}
