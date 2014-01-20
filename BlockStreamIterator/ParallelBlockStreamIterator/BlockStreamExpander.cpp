/*
 * BlockStreamExpender.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */
#include <iostream>
#include "BlockStreamExpander.h"


BlockStreamExpander::BlockStreamExpander(State state)
:state_(state),block_stream_buffer_(0),finished_thread_count_(0){
	// TODO Auto-generated constructor stub

}

BlockStreamExpander::BlockStreamExpander()
:block_stream_buffer_(0),finished_thread_count_(0){

}

BlockStreamExpander::~BlockStreamExpander() {

}

BlockStreamExpander::State::State(Schema* schema,BlockStreamIteratorBase* child,unsigned thread_count,unsigned block_size, unsigned block_count_in_buffer)
:schema_(schema),child_(child),thread_count_(thread_count),block_size_(block_size),block_count_in_buffer_(block_count_in_buffer){

}

bool BlockStreamExpander::open(const PartitionOffset& partitoin_offset){
	state_.partition_offset=partitoin_offset;
//	printf("Expander open!\n");
	finished_thread_count_=0;
	block_stream_buffer_=new BlockStreamBuffer(state_.block_size_,state_.block_count_in_buffer_,state_.schema_);

	expanded_thread_list_.clear();
	for(unsigned i=0;i<state_.thread_count_;i++){
		pthread_t tid;
		const int error=pthread_create(&tid,NULL,expanded_work,this);
		if(error!=0){
			std::cout<<"cannot create thread!"<<std::endl;
			return false;
		}
		expanded_thread_list_.insert(tid);
	}
	return true;
}

bool BlockStreamExpander::next(BlockStreamBase* block){
	while(!block_stream_buffer_->getBlock(*block)){
		if(ChildExhausted()){
			return false;
		}
		else{
			usleep(1);
		}
	}
	return true;
}

bool BlockStreamExpander::close(){

	for(std::set<pthread_t>::iterator it=expanded_thread_list_.begin();it!=expanded_thread_list_.end();it++){
		pthread_cancel(*it);
		void* res;

		pthread_join(*it,&res);
		if(res!=PTHREAD_CANCELED)
			assert(false);
		printf("A expander thread is killed before close!\n");
	}
	while(!expanded_thread_list_.empty()){
		expanded_thread_list_.begin();
	}

	block_stream_buffer_->~BlockStreamBuffer();

	state_.child_->close();
	printf("<<<<<<<Expander closed!>>>>>>>>>>\n");
	return true;
}
void BlockStreamExpander::print(){
	printf("Expander: thread num:%d\n",state_.thread_count_);
	printf("---------------------\n");
	state_.child_->print();

}
void* BlockStreamExpander::expanded_work(void* arg){
	const unsigned thread_id=rand()%100;
	unsigned block_count=0;


	BlockStreamExpander* Pthis=(BlockStreamExpander*)arg;
	Pthis->state_.child_->open(Pthis->state_.partition_offset);
	BlockStreamBase* block_for_asking=BlockStreamBase::createBlock(Pthis->state_.schema_,Pthis->state_.block_size_);

	while(Pthis->state_.child_->next(block_for_asking)){
		Pthis->block_stream_buffer_->insertBlock(block_for_asking);
		block_count++;
	}
	/*TODO: the following increment may need to use atomic add for the thread-safety.*/
	Pthis->lock_.lock();
	Pthis->finished_thread_count_++;

	printf("Thread %x generated %d blocks.\n",pthread_self(),block_count);
	Pthis->expanded_thread_list_.erase(pthread_self());
	Pthis->lock_.unlock();
	return 0;

}

bool BlockStreamExpander::ChildExhausted(){

	return finished_thread_count_==state_.thread_count_;
}
