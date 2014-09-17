/*
 * BlockStreamExpender.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */
#include <iostream>
#include "BlockStreamExpander.h"
#include "../../Executor/ExpanderTracker.h"
#include "../../common/Logging.h"

struct ExpanderContext{
	BlockStreamExpander* pthis;
	semaphore sem;
};

BlockStreamExpander::BlockStreamExpander(State state)
:state_(state),block_stream_buffer_(0),finished_thread_count_(0),thread_count_(0),coordinate_pid_(0){
	// TODO Auto-generated constructor stub
	logging_=new BlockStreamExpanderLogging();
}

BlockStreamExpander::BlockStreamExpander()
:block_stream_buffer_(0),finished_thread_count_(0),thread_count_(0),coordinate_pid_(0){
	logging_=new BlockStreamExpanderLogging();
}

BlockStreamExpander::~BlockStreamExpander() {
	printf("Expander free!\n");
	delete logging_;
}

BlockStreamExpander::State::State(Schema* schema,BlockStreamIteratorBase* child,unsigned thread_count,unsigned block_size, unsigned block_count_in_buffer)
:schema_(schema),child_(child),init_thread_count_(thread_count),block_size_(block_size),block_count_in_buffer_(block_count_in_buffer){

}

bool BlockStreamExpander::open(const PartitionOffset& partitoin_offset){

//	printf("\n*************%lx****************\n",state_.child_);
//	state_.child_->print();
//	printf("*******************************\n\n\n\n");
	received_tuples_=0;
	logging_->log("[%ld] Expander open, thread count=%d\n",expander_id_,state_.init_thread_count_);
	state_.partition_offset=partitoin_offset;
	input_data_complete_=false;
	one_thread_finished_=false;
	finished_thread_count_=0;
	block_stream_buffer_=new BlockStreamBuffer(state_.block_size_,state_.block_count_in_buffer_,state_.schema_);

	in_work_expanded_thread_list_.clear();
	expander_id_=ExpanderTracker::getInstance()->registerNewExpander(block_stream_buffer_,this);
	for(unsigned i=0;i<state_.init_thread_count_;i++){
		if(createNewExpandedThread()==false){
			logging_->log("[%ld] Failed to create initial expanded thread********\n",expander_id_);
			return false;
		}
	}



	/**
	 * The following three lines test set callback status to expanded threads.
	 */
//	assert(pthread_create(&coordinate_pid_,NULL,coordinate_work,this)==0);
//	logging_->log("coordinate>>>>>>>>>>>>>>...\n");
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
	logging_->log("Expander:[%ld]: received %ld kByte %ld tuples!\n",expander_id_,block_stream_buffer_->getReceivedDataSizeInKbytes(),received_tuples_);
//	if(expander_id_%3!=0){
//		if(received_tuples_!=3966020&&received_tuples_!=3780597){
//			assert(false);
//		}
//	}
//	pthread_cancel(coordinate_pid_);
//	void* res;
//	pthread_join(coordinate_pid_,&res);
	ExpanderTracker::getInstance()->unregisterExpander(expander_id_);
	for(std::set<pthread_t>::iterator it=in_work_expanded_thread_list_.begin();it!=in_work_expanded_thread_list_.end();it++){
//		pthread_cancel(*it);
		void* res;

		pthread_join(*it,&res);
		assert(res==0);
		logging_->elog("[%ld] A expander thread is killed before close!\n",expander_id_);
	}
//	while(!in_work_expanded_thread_list_.empty()){
//		in_work_expanded_thread_list_.begin();
//	}
	assert(input_data_complete_);
	input_data_complete_=false;
	one_thread_finished_=false;
	assert(in_work_expanded_thread_list_.empty());
	assert(being_called_bacl_expanded_thread_list_.empty());
	finished_thread_count_=0;

//	assert(ExpanderTracker::getInstance()->expander_id_to_status_.size()==0);
	delete block_stream_buffer_;
	logging_->log("[%ld] Buffer is freed in Expander!\n",expander_id_);
	state_.child_->close();
	thread_count_=0;
	logging_->log("[%ld] <<<<<<<Expander closed!>>>>>>>>>>\n",expander_id_);
	return true;
}
void BlockStreamExpander::print(){
	printf("Expander: thread num:%d\n",state_.init_thread_count_);
//	printf("---------------------\n");
	state_.child_->print();

}
void* BlockStreamExpander::expanded_work(void* arg){
	const pthread_t pid=pthread_self();

	bool expanding=true;
	ticks start=curtick();

	BlockStreamExpander* Pthis=((ExpanderContext*)arg)->pthis;
	Pthis->addIntoInWorkingExpandedThreadList(pid);
	ExpanderTracker::getInstance()->registerNewExpandedThreadStatus(pid,Pthis->expander_id_);
//	const unsigned thread_id=rand()%100;
	unsigned block_count=0;
	((ExpanderContext*)arg)->sem.post();



	if(Pthis->ChildExhausted()){
		return 0;
	}

	Pthis->logging_->log("[%ld] %lx begins to open child\n",Pthis->expander_id_,pid);
	Pthis->state_.child_->open(Pthis->state_.partition_offset);
	Pthis->logging_->log("[%ld] %lx finished opening child\n",Pthis->expander_id_,pid);
	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pid)){
//		unregisterNewThreadToAllBarriers();
		Pthis->logging_->log("[%ld]<<<<<<<<<<<<<<<<<Expander detected call back signal after open!>>>>>>>>%lx>>>>>>>>>\n",Pthis->expander_id_,pthread_self());
		Pthis->removeFromBeingCalledBackExpandedThreadList(pid);
		Pthis->tid_to_shrink_semaphore[pid]->post();
	}
	else{
			if(expanding==true){
				expanding=false;
//				printf("Expanding time:%f  %ld cycles\n",getSecond(start),curtick()-start);
			}
		BlockStreamBase* block_for_asking=BlockStreamBase::createBlock(Pthis->state_.schema_,Pthis->state_.block_size_);
		block_for_asking->setEmpty();
		while(Pthis->state_.child_->next(block_for_asking)){


//			assert(!block_for_asking->Empty());
			if(!block_for_asking->Empty()){
				Pthis->lock_.acquire();
				Pthis->received_tuples_+=block_for_asking->getTuplesInBlock();
				Pthis->lock_.release();
				Pthis->block_stream_buffer_->insertBlock(block_for_asking);
				block_for_asking->setEmpty();
				block_count++;
			}
		}
		delete block_for_asking;
		if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
	//		unregisterNewThreadToAllBarriers();
			Pthis->logging_->log("[%ld]<<<<<<<<<<<<<<<<<Expander detected call back signal during next!>>>>>>>>%lx>>>>>>>>>\n",Pthis->expander_id_,pthread_self());
			Pthis->lock_.acquire();

			Pthis->input_data_complete_=false;

			Pthis->lock_.release();
			Pthis->removeFromBeingCalledBackExpandedThreadList(pthread_self());
			Pthis->logging_->log("%lx Produced %d block before called-back\n",pthread_self(),block_count);
			Pthis->tid_to_shrink_semaphore[pid]->post();
		}
		else{
			Pthis->logging_->log("%lx Produced %d block before finished\n",pthread_self(),block_count);
//			assert(block_count!=0);
//			block_for_asking->~BlockStreamBase();
			Pthis->lock_.acquire();
			Pthis->finished_thread_count_++;

//			if(Pthis->in_work_expanded_thread_list_.empty()){
				Pthis->input_data_complete_=true;

			/**
			 * The finish of one expanded thread does not always mean the complete of input data.
			 *
			 */
				Pthis->block_stream_buffer_->setInputComplete();
//			}
			Pthis->logging_->log("Thread %x generated %d blocks.\n",pthread_self(),block_count);
			Pthis->lock_.release();

			if(!Pthis->removeFromInWorkingExpandedThreadList(pthread_self())){
				/* current thread has been called back*/
				Pthis->removeFromBeingCalledBackExpandedThreadList(pthread_self());
				Pthis->tid_to_shrink_semaphore[pid]->post();
			}
		}
	}


	/* delete its stauts from expander tracker before exit*/
	ExpanderTracker::getInstance()->deleteExpandedThreadStatus(pthread_self());
	Pthis->logging_->log("[%ld] One expande thread finished!\n",Pthis->expander_id_);
	return 0;

}

bool BlockStreamExpander::ChildExhausted(){

	 /* first acquire the exclusive lock to prevent creating expanded thread,
	  * Otherwise, newly created thread may not be detected by ChildExhausted().*/
	exclusive_expanding_.acquire();
	lock_.acquire();
	bool ret=input_data_complete_==true&&
			in_work_expanded_thread_list_.empty()&&
			being_called_bacl_expanded_thread_list_.empty()&&
			this->block_stream_buffer_->Empty();
	lock_.release();
	exclusive_expanding_.release();
	if(ret==true&&coordinate_pid_!=0){
		void* res;
		pthread_join(coordinate_pid_,&res);
		coordinate_pid_=0;
		return ChildExhausted();
	}
	if(ret){
		logging_->log("[%ld] child iterator is exhausted!\n",expander_id_);
	}
	return ret;
}
bool BlockStreamExpander::createNewExpandedThread(){
	pthread_t tid;


	ExpanderContext para;
	para.pthis=this;
	ticks start=curtick();
	if(exclusive_expanding_.try_acquire()){
		const int error=pthread_create(&tid,NULL,expanded_work,&para);
		if(error!=0){
			std::cout<<"cannot create thread!!!!!!!!!!!!!!!"<<std::endl;
			return false;
		}
		para.sem.wait();
		exclusive_expanding_.release();
	//	printf("[Expander %d ]Expanded!\n",expander_id_);
		logging_->log("[%ld] New expanded thread [%lx] created!\n",expander_id_,tid);

		lock_.acquire();
		thread_count_++;
		lock_.release();
	//	in_work_expanded_thread_list_.insert(tid);
//		printf("Expand time :%lf \n",getSecond(start));
		return true;
	}
	else{
		printf("Fails to obtain the exclusive lock to expanding!\n");
		return false;
	}
}
void BlockStreamExpander::terminateExpandedThread(pthread_t pid){
//	if(ExpanderTracker::getInstance()->callbackExpandedThread(pid)){
//		printf("---> shrink+ %lx\n",pid);
//		semaphore sem;
//		tid_to_shrink_semaphore[pid]=&sem;
//		removeFromInWorkingExpandedThreadList(pid);
//
//		addIntoBeingCalledBackExpandedThreadList(pid);
//		printf("---> shrink added %lx\n",pid);
//		tid_to_shrink_semaphore[pid]->wait();
//		lock_.acquire();
//		tid_to_shrink_semaphore.erase(pid);
//		lock_.release();
//
//		lock_.acquire();
//		thread_count_--;
//		lock_.release();
//		logging_->log("[%ld] A thread is called back !******** working_thread_count=%d, being_called_back_thread_count:%d\n",expander_id_,this->in_work_expanded_thread_list_.size(),this->being_called_bacl_expanded_thread_list_.size());
//	}
//	else{
//		logging_->log("[%ld] This thread has already been called back!.\n",expander_id_);
//	}
	if(!ExpanderTracker::getInstance()->isExpandedThreadCallBack(pid)){
		semaphore sem;
		tid_to_shrink_semaphore[pid]=&sem;
		removeFromInWorkingExpandedThreadList(pid);

		addIntoBeingCalledBackExpandedThreadList(pid);
		ExpanderTracker::getInstance()->callbackExpandedThread(pid);
		tid_to_shrink_semaphore[pid]->wait();
		lock_.acquire();
		tid_to_shrink_semaphore.erase(pid);
		lock_.release();

		lock_.acquire();
		thread_count_--;
		lock_.release();
		logging_->log("[%ld] A thread is called back !******** working_thread_count=%d, being_called_back_thread_count:%d\n",expander_id_,this->in_work_expanded_thread_list_.size(),this->being_called_bacl_expanded_thread_list_.size());
	}
	else{
		logging_->log("[%ld] This thread has already been called back!.\n",expander_id_);
	}
}
void BlockStreamExpander::addIntoInWorkingExpandedThreadList(pthread_t pid){
	lock_.acquire();
//	assert(in_work_expanded_thread_list_.find(pid)==in_work_expanded_thread_list_.end());
	in_work_expanded_thread_list_.insert(pid);
	logging_->log("[%ld] %lx is added into in working list!\n",expander_id_,pid);
	lock_.release();
}
bool BlockStreamExpander::removeFromInWorkingExpandedThreadList(pthread_t pid){
	lock_.acquire();
	if(in_work_expanded_thread_list_.find(pid)!=in_work_expanded_thread_list_.end()){
		in_work_expanded_thread_list_.erase(pid);
		logging_->log("[%ld] %lx is removed from in working list!\n",expander_id_,pid);
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
//	logging_->log("[%ld] %lx is added into being called back list!\n",expander_id_,pid);
	lock_.release();
}
void BlockStreamExpander::removeFromBeingCalledBackExpandedThreadList(pthread_t pid){
	lock_.acquire();
	assert(being_called_bacl_expanded_thread_list_.find(pid)!=being_called_bacl_expanded_thread_list_.end());
	being_called_bacl_expanded_thread_list_.erase(pid);
//	logging_->log("[%ld] %lx is removed from being called back list!\n",expander_id_,pid);
	lock_.release();
}
unsigned BlockStreamExpander::getDegreeOfParallelism(){
	unsigned ret;
	lock_.acquire();
	ret=in_work_expanded_thread_list_.size();
	lock_.release();
	return ret;
}
void* BlockStreamExpander::coordinate_work(void* arg){
//	logging_->log("coordinate thread is created!!!!!!!!!!!!!\n");
//	BlockStreamExpander* pthis=static_cast<BlockStreamExpander*>(arg);
//	while(!pthis->ChildExhausted()){
//		if(rand()%100>50){
//			pthis->createNewThread();
//		}
//		else{
//			if(pthis->in_work_expanded_thread_list_.size()<=1){
////				logging_->log("Coordiante: this is the last working thread, cannot be called back!\n");
//				continue;
//			}
//			int drop_thread_index=rand()%pthis->in_work_expanded_thread_list_.size();
//			std::set<pthread_t>::iterator it=pthis->in_work_expanded_thread_list_.begin();
//			for(unsigned i=0;i!=drop_thread_index;i++){
//				it++;
//			}
//			pthis->callBackThread(*it);
//		}
//		usleep(100000);
//		logging_->log("%d thread in expander\n",pthis->in_work_expanded_thread_list_.size());
////		break;
//	}
//	logging_->log("coordinate thread is terminated!!!!!!!!!!!!!\n");
}
bool BlockStreamExpander::Expand(){
	if(input_data_complete_){
//		/*
//		 * Expander does not expand when at least one expanded thread has completely processed
//		 * the input data flow. Otherwise the newly created expanded thread might not be able to
//		 * work properly if the expander's close is called before its create.
//		 */
//		printf("[Expander %d ]Expanding failed because the input data is complete!\n",expander_id_);
		return false;
	}
	return createNewExpandedThread();
}

bool BlockStreamExpander::Shrink(){
//	return true;
//	bool ret;
	ticks start=curtick();
	lock_.acquire();
	if(in_work_expanded_thread_list_.empty()){
		lock_.release();
		return false;
	}
	else{
		pthread_t cencel_thread_id=*in_work_expanded_thread_list_.begin();
//		in_work_expanded_thread_list_.erase(cencel_thread_id);
		lock_.release();
		this->terminateExpandedThread(cencel_thread_id);
//		printf("\n\nShrink time :%f\t %ld cycles \n\n",getSecond(start),curtick()-start);
		return true;
	}
//	lock_.release();
//	return ret;
}
