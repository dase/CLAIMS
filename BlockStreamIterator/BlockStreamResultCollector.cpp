/*
 * ExpandableBlockStreamBuffer.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */

#include "BlockStreamResultCollector.h"
#include "../utility/rdtsc.h"
BlockStreamResultCollector::BlockStreamResultCollector()
:finished_thread_count_(0),registered_thread_count_(0){
	// TODO Auto-generated constructor stub
	sema_open_.set_value(1);
	sema_open_finished_.set_value(0);
	sema_input_complete_.set_value(0);
}
BlockStreamResultCollector::BlockStreamResultCollector(State state)
:finished_thread_count_(0),registered_thread_count_(0),state_(state){
	// TODO Auto-generated constructor stub
	sema_open_.set_value(1);
	sema_open_finished_.set_value(0);
	sema_input_complete_.set_value(0);
}
BlockStreamResultCollector::~BlockStreamResultCollector() {
	delete state_.input_;
	delete state_.child_;
}
BlockStreamResultCollector::State::State():input_(0),child_(0),block_size_(0),partition_offset_(0){}
BlockStreamResultCollector::State::State(Schema* input,BlockStreamIteratorBase* child,const unsigned block_size,std::vector<std::string> column_header,const PartitionOffset partitoin_offset)
:input_(input),child_(child),block_size_(block_size),partition_offset_(partitoin_offset),column_header_(column_header){

}
bool BlockStreamResultCollector::open(const PartitionOffset& part_off){
	//TODO: expandable
	state_.partition_offset_=part_off;

	if (sema_open_.try_wait()) {
		block_buffer_=new ResultSet();
		block_buffer_iterator_=block_buffer_->createIterator();
		open_finished_ = true;
	} else {
		while (!open_finished_) {
			usleep(1);
		}
	}


	registered_thread_count_++;
	pthread_t tid;
	pthread_create(&tid,NULL,worker,this);
	unsigned long long int start=curtick();
//	while(!ChildExhausted()){
//		usleep(1);
//	}x
	sema_input_complete_.wait();
	block_buffer_->query_time_=getSecond(start);
	return true;
}
bool BlockStreamResultCollector::next(BlockStreamBase* block){
//	BlockStreamBase* buffered_block;
//	while((buffered_block=(BlockStreamBase*)block_buffer_iterator_.atomicNextBlock())==0){
//		if(ChildExhausted()){
//			return false;
//		}
//		else{
//			usleep(1);
//		}
//	}
//	block->deepCopy(buffered_block);

	return false;
}
bool BlockStreamResultCollector::close(){
//	DynamicBlockBuffer::Iterator it=block_buffer_.createIterator();
//	BlockStreamBase* block_to_deallocate;
//	while(block_to_deallocate=(BlockStreamBase*)it.nextBlock()){
//		deallocateBlockStream(block_to_deallocate);
//	}
	state_.child_->close();
	sema_input_complete_.set_value(0);
	return true;
}
void BlockStreamResultCollector::print(){
	printf("------------\n");
	printf("ResultCollector\n");
	printf("------------\n");
	state_.child_->print();
}
ResultSet* BlockStreamResultCollector::getResultSet(){

	while(!ChildExhausted()){
		usleep(1);
	}
	return block_buffer_;
}

bool BlockStreamResultCollector::createBlockStream(BlockStreamBase*& target)const{
	//TODO: the block allocation should apply for the memory budget from the buffer manager first.
	target=BlockStreamBase::createBlock(state_.input_,state_.block_size_);
	return target!=0;
}

void BlockStreamResultCollector::deallocateBlockStream(BlockStreamBase*& target)const{
	//TODO: return the allocated memory to the buffer manager, but this is for now.
	target->~BlockStreamBase();
}
void* BlockStreamResultCollector::worker(void* arg){

	BlockStreamResultCollector* Pthis=(BlockStreamResultCollector*)arg;
	Pthis->state_.child_->open(Pthis->state_.partition_offset_);
	BlockStreamBase* block_for_asking;
	if(Pthis->createBlockStream(block_for_asking)==false){
		assert(false);
		return 0;
	}
	Pthis->block_buffer_->column_header_list_=Pthis->state_.column_header_;

	unsigned long long start = 0;
	start = curtick();

	while(Pthis->state_.child_->next(block_for_asking)){
		Pthis->block_buffer_->atomicAppendNewBlock(block_for_asking);
		if(Pthis->createBlockStream(block_for_asking)==false){
			assert(false);
			return 0;
		}
	}
	Pthis->sema_input_complete_.post();
	double eclipsed_seconds = getSecond(start);
	Pthis->block_buffer_->query_time_=eclipsed_seconds;
	Pthis->block_buffer_->schema_=Pthis->state_.input_->duplicateSchema();
	Pthis->finished_thread_count_++;

	return 0;

}
bool BlockStreamResultCollector::ChildExhausted(){

	return finished_thread_count_==registered_thread_count_;
}


unsigned long BlockStreamResultCollector::getNumberOftuples()const{
	unsigned long ret=0;
	ResultSet::Iterator it=block_buffer_->createIterator();
	BlockStreamBase* block;
	while(block=(BlockStreamBase*)it.nextBlock()){
		ret+=block->getTuplesInBlock();
	}
	return ret;
}

BlockStreamResultCollector::State::~State() {
//	delete input_;
//	delete child_;
}
