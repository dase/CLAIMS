/*
 * BufferManager.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: wangli
 */

#include "BufferManager.h"

#include <fcntl.h>
#include <fstream>

using std::__basic_file;
using std::basic_fstream;
BufferManager* BufferManager::instance_ = NULL;
BufferManager::BufferManager() {
  page_size = sysconf(_SC_PAGESIZE);
  total_memory = sysconf(_SC_PHYS_PAGES);
  totol_capacity_ = (unsigned long)total_memory * page_size;
  storage_budget_max_ = totol_capacity_;
  storage_budget_min_ = totol_capacity_;
  //  totol_capacity_ = (unsigned long)1024 * 1024 * 1024 * 80;
  //  storage_budget_max_ = (unsigned long)1024 * 1024 * 1024 * 60;
  //  storage_budget_min_ = (unsigned long)1024 * 1024 * 1024 * 60;
  storage_used_ = 0;
  intermediate_buffer_budget_max_ = 896 * 1024 * 1024;
  intermediate_buffer_budget_min_ = 896 * 1024 * 1024;
  intermediate_buffer_used_ = 0;
  memory_storage_ = MemoryChunkStore::GetInstance();
  logging_ = new BufferManagerLogging();
  logging_->log("Initialized!");
  logging_->log("%d MB total memory", totol_capacity_ / 1024 / 1024);
}

BufferManager::~BufferManager() {
  // TODO Auto-generated destructor stub
  instance_ = NULL;
  delete logging_;
}

BufferManager* BufferManager::getInstance() {
  if (NULL == instance_) {
    instance_ = new BufferManager();
  }
  return instance_;
}
unsigned long int BufferManager::getTotalUsed() const {
  return intermediate_buffer_used_ + storage_used_;
}
bool BufferManager::applyStorageDedget(unsigned long size) {
  bool ret = false;
  actucl_free_memory = sysconf(_SC_AVPHYS_PAGES) * page_size;
  lock_.acquire();

  if (size <= (actucl_free_memory)) {
    if (storage_used_ + size <=
        (storage_budget_max_ * Config::memory_utilization / 100)) {
      storage_used_ += size;
      ret = true;
    }
  }
  logging_->log("%d MB applied, %d MB left!", size / 1024 / 1024,
                (storage_budget_max_ - storage_used_) / 1024 / 1024);
  logging_->log("%d MB actucl left free memory",
                actucl_free_memory / 1024 / 1024);
  lock_.release();
  return ret;
}
void BufferManager::returnStorageBudget(unsigned long size) {
  lock_.acquire();
  storage_used_ -= size;
  lock_.release();
}

unsigned BufferManager::getStorageMemoryBudegeInMilibyte() const {
  logging_->log("%d MB MAX STORAGE BUDGET memory",
                storage_budget_max_ / 1024 / 1024);
  return storage_budget_max_ / 1024 / 1024;
}
