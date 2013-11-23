/*
 * BufferManager.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: wangli
 */

#include "BufferManager.h"
BufferManager* BufferManager::instance_=0;
BufferManager::BufferManager() {
	totol_capacity_=1024*1024*1024;
	storage_budget_max_=896*1024*1024;
	storage_budget_min_=128*1024*1024;
	storage_used_=0;
	intermediate_buffer_budget_max_=896*1024*1024;
	intermediate_buffer_budget_min_=896*1024*1024;
	intermediate_buffer_used_=0;
	memory_storage_=MemoryChunkStore::getInstance();
	logging_=new BufferManagerLogging();
	logging_->log("Initialized!");

}

BufferManager::~BufferManager() {
	// TODO Auto-generated destructor stub
}

BufferManager* BufferManager::getInstance(){
	if(instance_==0){
		instance_= new BufferManager();
	}
	return instance_;
}
unsigned long int BufferManager::getTotalUsed()const{
	return intermediate_buffer_used_+storage_used_;
}
bool BufferManager::applyStorageDedget(unsigned long size){
	bool ret;
	lock_.acquire();
	if(storage_used_+size<=storage_budget_max_){
		storage_used_+=size;
		ret=true;
	}
	logging_->log("%d MB applied, %d MB left!",size/1024/1024,(storage_budget_max_-storage_used_)/1024/1024);
	lock_.release();
	return ret;
}
unsigned BufferManager::getStorageMemoryBudegeInMilibyte()const{
	return storage_budget_max_/1024/1024;
}
