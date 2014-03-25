/*
 * ExpandableBlockStreamIteratorBase.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wangli
 */

#include "ExpandableBlockStreamIteratorBase.h"

ExpandableBlockStreamIteratorBase::ExpandableBlockStreamIteratorBase(unsigned number_of_barrier,unsigned number_of_seriliazed_section)
:number_of_barrier_(number_of_barrier),number_of_seriliazed_section_(number_of_seriliazed_section){
	// TODO Auto-generated constructor stub
	barrier_=new Barrier[number_of_barrier_];
	seriliazed_section_entry_key_=new semaphore[number_of_seriliazed_section_];
}

ExpandableBlockStreamIteratorBase::~ExpandableBlockStreamIteratorBase() {
	pthread_mutex_destroy(&sync_lock_);
	pthread_cond_destroy(&sync_cv_);
	for(unsigned i=0;i<number_of_barrier_;i++){
		barrier_[i].~Barrier();
	}
	for(unsigned i=0;i<number_of_seriliazed_section_;i++){
		seriliazed_section_entry_key_[i].destroy();
	}
	delete[] barrier_;
	delete[] seriliazed_section_entry_key_;
}
void ExpandableBlockStreamIteratorBase::waitForOpenFinished(){
	pthread_mutex_lock(&sync_lock_);
	if(open_finished_==false){
		pthread_cond_wait(&sync_cv_, &sync_lock_);
	}
	pthread_mutex_unlock(&sync_lock_);
}
void ExpandableBlockStreamIteratorBase::initialize_expanded_status(){
	int ret;
	ret = pthread_mutex_init(&sync_lock_, NULL);
	if(ret!=0)
		printf("pthread_mutex_init failed at barrier creation.\n");
	ret = pthread_cond_init(&sync_cv_, NULL);
	if(ret!=0)
		printf("pthread_cond_init failed at barrier creation.\n");

	open_finished_=false;
	/* only one thread wins when complete for the job of initialization*/


	for(unsigned i=0;i<number_of_barrier_;i++){
		barrier_[i].setEmpty();
	}

	for(unsigned i=0;i<number_of_seriliazed_section_;i++){
		seriliazed_section_entry_key_[i].set_value(1);
	}
}
void ExpandableBlockStreamIteratorBase::broadcaseOpenFinishedSignal(){
	pthread_mutex_lock(&sync_lock_);
	open_finished_=true;

	/*wake up all the waiting thread*/
	pthread_cond_broadcast(&sync_cv_);

	pthread_mutex_unlock(&sync_lock_);
}
bool ExpandableBlockStreamIteratorBase::tryEntryIntoSerializedSection(unsigned phase_id){
	assert(phase_id<number_of_seriliazed_section_);
	return seriliazed_section_entry_key_[phase_id].try_wait();
}
void ExpandableBlockStreamIteratorBase::setOpenReturnValue(bool value){
	open_ret_=value;
}
bool ExpandableBlockStreamIteratorBase::getOpenReturnValue()const{
	return open_ret_;
}
void ExpandableBlockStreamIteratorBase::RegisterNewThreadToAllBarriers(){
//	assert(barrier_index<number_of_barrier_);
	for(unsigned i=0;i<number_of_barrier_;i++){
		barrier_[i].RegisterOneThread();
	}
}
void ExpandableBlockStreamIteratorBase::barrierArrive(unsigned barrier_index){
	assert(barrier_index<number_of_barrier_);
	barrier_[barrier_index].Arrive();
}
