/*
 * BlockStreamExpender.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */
#include <iostream>
#include "BlockStreamExpander.h"
#include "../../Executor/ExpanderTracker.h"

BlockStreamExpander::BlockStreamExpander(State state)
:state_(state),block_stream_buffer_(0),finished_thread_count_(0),thread_count_(0){
	// TODO Auto-generated constructor stub

}

BlockStreamExpander::BlockStreamExpander()
:block_stream_buffer_(0),finished_thread_count_(0),thread_count_(0){

}

BlockStreamExpander::~BlockStreamExpander() {

}

BlockStreamExpander::State::State(Schema* schema,BlockStreamIteratorBase* child,unsigned thread_count,unsigned block_size, unsigned block_count_in_buffer)
:schema_(schema),child_(child),init_thread_count_(thread_count),block_size_(block_size),block_count_in_buffer_(block_count_in_buffer){

}

bool BlockStreamExpander::open(const PartitionOffset& partitoin_offset){
	printf("Expander open, thread count=%d\n",state_.init_thread_count_);
	state_.partition_offset=partitoin_offset;
	working_thread_successful_=false;
	finished_thread_count_=0;
	block_stream_buffer_=new BlockStreamBuffer(state_.block_size_,state_.block_count_in_buffer_,state_.schema_);

	in_work_expanded_thread_list_.clear();
	for(unsigned i=0;i<state_.init_thread_count_;i++){
		if(createNewThread()==false){
			printf("Failed!**************\n");
			return false;
		}
	}

	/**
	 * The following three lines test set callback status to expanded threads.
	 */
	assert(pthread_create(&coordinate_pid_,NULL,coordinate_work,this)==0);
	printf("coordinate>>>>>>>>>>>>>>...\n");
//	for(std::set<pthread_t>::iterator it=expanded_thread_list_.begin();it!=expanded_thread_list_.end();it++){
////	 	assert(ExpanderTracker::getInstance()->callbackExpandedThread(*it));
//	assert(ExpanderTracker::getInstance()->callbackExpandedThread(*it));
//	sleep(1);
//	createNewThread();
////	assert(ExpanderTracker::getInstance()->callbackExpandedThread(*expanded_thread_list_.begin()));
//	}
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

//	pthread_cancel(coordinate_pid_);
//	void* res;
//	pthread_join(coordinate_pid_,&res);
	for(std::set<pthread_t>::iterator it=in_work_expanded_thread_list_.begin();it!=in_work_expanded_thread_list_.end();it++){
//		pthread_cancel(*it);
		void* res;

		pthread_join(*it,&res);
		assert(res==0);
		printf("A expander thread is killed before close!\n");
	}
//	while(!in_work_expanded_thread_list_.empty()){
//		in_work_expanded_thread_list_.begin();
//	}
	working_thread_successful_=false;
	assert(in_work_expanded_thread_list_.empty());
	assert(being_called_bacl_expanded_thread_list_.empty());
	finished_thread_count_=0;

//	assert(ExpanderTracker::getInstance()->id_to_status_.size()==0);
	block_stream_buffer_->~BlockStreamBuffer();

	state_.child_->close();
	thread_count_=0;
	printf("<<<<<<<Expander closed!>>>>>>>>>>\n");
	return true;
}
void BlockStreamExpander::print(){
	printf("Expander: thread num:%d\n",state_.init_thread_count_);
//	printf("---------------------\n");
	state_.child_->print();

}
void* BlockStreamExpander::expanded_work(void* arg){
	BlockStreamExpander* Pthis=(BlockStreamExpander*)arg;
	ExpanderTracker::getInstance()->registerNewExpandedThreadStatus(pthread_self());
	Pthis->addIntoInWorkingExpandedThreadList(pthread_self());
	const unsigned thread_id=rand()%100;
	unsigned block_count=0;




	if(Pthis->ChildExhausted()){
		return 0;
	}

	printf("%lx begins to open child\n",pthread_self());
	Pthis->state_.child_->open(Pthis->state_.partition_offset);
	printf("%lx finished opening child\n",pthread_self());
	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
//		unregisterNewThreadToAllBarriers();
		printf("<<<<<<<<<<<<<<<<<Expander detected call back signal after open!>>>>>>>>%lx>>>>>>>>>\n",pthread_self());
		Pthis->removeFromBeingCalledBackExpandedThreadList(pthread_self());
	}
	else{
		BlockStreamBase* block_for_asking=BlockStreamBase::createBlock(Pthis->state_.schema_,Pthis->state_.block_size_);
		block_for_asking->setEmpty();
		while(Pthis->state_.child_->next(block_for_asking)){
			Pthis->block_stream_buffer_->insertBlock(block_for_asking);
			block_for_asking->setEmpty();
			block_count++;
		}
		block_for_asking->~BlockStreamBase();
		if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
	//		unregisterNewThreadToAllBarriers();
			printf("<<<<<<<<<<<<<<<<<Expander detected call back signal during next!>>>>>>>>%lx>>>>>>>>>\n",pthread_self());
			Pthis->removeFromBeingCalledBackExpandedThreadList(pthread_self());
		}
		else{
			block_for_asking->~BlockStreamBase();
			Pthis->lock_.acquire();
			Pthis->finished_thread_count_++;
			Pthis->working_thread_successful_=true;
		//	printf("Thread %x generated %d blocks.\n",pthread_self(),block_count);
	//		Pthis->in_work_expanded_thread_list_.erase(pthread_self());
			Pthis->lock_.release();

			if(!Pthis->removeFromInWorkingExpandedThreadList(pthread_self())){
				/* current thread has been called back*/
				Pthis->removeFromBeingCalledBackExpandedThreadList(pthread_self());
			}
		}
	}


	/* delete its stauts from expander tracker before exit*/
	ExpanderTracker::getInstance()->deleteExpandedThreadStatus(pthread_self());
	printf("One expande thread finished!\n");
	return 0;

}

bool BlockStreamExpander::ChildExhausted(){
	lock_.acquire();
	bool ret=working_thread_successful_==true&&
			in_work_expanded_thread_list_.empty()&&
			being_called_bacl_expanded_thread_list_.empty()&&
			this->block_stream_buffer_->Empty();
	lock_.release();
	if(ret==true&&coordinate_pid_!=0){
		void* res;
		pthread_join(coordinate_pid_,&res);
		coordinate_pid_=0;
		return ChildExhausted();
	}
	if(ret){
		printf("child iterator is exhausted!\n");
	}
//	return finished_thread_count_==thread_count_;
//	return finished_thread_count_==in_work_expanded_thread_list_.size();
	return ret;
}
bool BlockStreamExpander::createNewThread(){
	printf("New expanded thread created!\n");
	pthread_t tid;
	const int error=pthread_create(&tid,NULL,expanded_work,this);
	if(error!=0){
		std::cout<<"cannot create thread!!!!!!!!!!!!!!!"<<std::endl;
		return false;
	}


	lock_.acquire();
	thread_count_++;
	lock_.release();
//	in_work_expanded_thread_list_.insert(tid);
	return true;
}
void BlockStreamExpander::callBackThread(pthread_t pid){
	if(ExpanderTracker::getInstance()->callbackExpandedThread(pid)){

//		in_work_expanded_thread_list_.erase(pid);
		removeFromInWorkingExpandedThreadList(pid);
		addIntoBeingCalledBackExpandedThreadList(pid);
//		being_called_bacl_expanded_thread_list_.insert(pid);

		lock_.acquire();
		thread_count_--;
		lock_.release();
//		printf("A thread is called back !******** working_thread_count=%d, being_called_back_thread_count:%d\n",this->in_work_expanded_thread_list_.size(),this->being_called_bacl_expanded_thread_list_.size());
	}
	else{
//		printf("This thread has already been called back!.\n");
	}
}
void BlockStreamExpander::addIntoInWorkingExpandedThreadList(pthread_t pid){
	lock_.acquire();
	assert(in_work_expanded_thread_list_.find(pid)==in_work_expanded_thread_list_.end());
	in_work_expanded_thread_list_.insert(pid);
//	printf("%lx is added into in working list!\n",pid);
	lock_.release();
}
bool BlockStreamExpander::removeFromInWorkingExpandedThreadList(pthread_t pid){
	lock_.acquire();
	if(in_work_expanded_thread_list_.find(pid)!=in_work_expanded_thread_list_.end()){
		in_work_expanded_thread_list_.erase(pid);
//		printf("%lx is removed from in working list!\n",pid);
		lock_.release();
		return true;
	}
	else{
		lock_.release();
		return false;
	}
}
void BlockStreamExpander::addIntoBeingCalledBackExpandedThreadList(pthread_t pid){
	lock_.acquire();
	assert(being_called_bacl_expanded_thread_list_.find(pid)==being_called_bacl_expanded_thread_list_.end());
	being_called_bacl_expanded_thread_list_.insert(pid);
//	printf("%lx is added into being called back list!\n",pid);
	lock_.release();
}
void BlockStreamExpander::removeFromBeingCalledBackExpandedThreadList(pthread_t pid){
	lock_.acquire();
	assert(being_called_bacl_expanded_thread_list_.find(pid)!=being_called_bacl_expanded_thread_list_.end());
	being_called_bacl_expanded_thread_list_.erase(pid);
//	printf("%lx is removed from being called back list!\n",pid);
	lock_.release();
}
void* BlockStreamExpander::coordinate_work(void* arg){
//	printf("coordinate thread is created!!!!!!!!!!!!!\n");
//	BlockStreamExpander* pthis=static_cast<BlockStreamExpander*>(arg);
//	while(!pthis->ChildExhausted()){
//		if(rand()%100>50){
//			pthis->createNewThread();
//		}
//		else{
//			if(pthis->in_work_expanded_thread_list_.size()<=1){
////				printf("Coordiante: this is the last working thread, cannot be called back!\n");
//				continue;
//			}
//			int drop_thread_index=rand()%pthis->in_work_expanded_thread_list_.size();
//			std::set<pthread_t>::iterator it=pthis->in_work_expanded_thread_list_.begin();
//			for(unsigned i=0;i!=drop_thread_index;i++){
//				it++;
//			}
//			pthis->callBackThread(*it);
//		}
//		usleep(10000);
//		printf("%d thread in expander\n",pthis->in_work_expanded_thread_list_.size());
////		break;
//	}
//	printf("coordinate thread is terminated!!!!!!!!!!!!!\n");
}
