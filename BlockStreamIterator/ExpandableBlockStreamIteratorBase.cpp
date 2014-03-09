/*
 * ExpandableBlockStreamIteratorBase.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wangli
 */

#include "ExpandableBlockStreamIteratorBase.h"

ExpandableBlockStreamIteratorBase::ExpandableBlockStreamIteratorBase() {
	// TODO Auto-generated constructor stub

}

ExpandableBlockStreamIteratorBase::~ExpandableBlockStreamIteratorBase() {
	pthread_mutex_destroy(&sync_lock_);
	pthread_cond_destroy(&sync_cv_);
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
	sema_compete_open_.set_value(1);
}
void ExpandableBlockStreamIteratorBase::broadcaseOpenFinishedSignal(){
	pthread_mutex_lock(&sync_lock_);
	open_finished_=true;

	/*wake up all the waiting thread*/
	pthread_cond_broadcast(&sync_cv_);

	pthread_mutex_unlock(&sync_lock_);
}
bool ExpandableBlockStreamIteratorBase::completeForInitializationJob(){
	return sema_compete_open_.try_wait();
}
void ExpandableBlockStreamIteratorBase::setOpenReturnValue(bool value){
	open_ret_=value;
}
bool ExpandableBlockStreamIteratorBase::getOpenReturnValue()const{
	return open_ret_;
}
