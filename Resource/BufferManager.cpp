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
  totol_capacity_ = (unsigned long)1024 * 1024 * 1024 * 80;
  storage_budget_max_ = (unsigned long)1024 * 1024 * 1024 * 4;
  storage_budget_min_ = (unsigned long)1024 * 1024 * 1024 * 4;
  storage_used_ = 0;
  intermediate_buffer_budget_max_ = 896 * 1024 * 1024;
  intermediate_buffer_budget_min_ = 896 * 1024 * 1024;
  intermediate_buffer_used_ = 0;
  page_size = sysconf(_SC_PAGESIZE);
  total_memory = sysconf(_SC_PHYS_PAGES);
  memory_storage_ = MemoryChunkStore::GetInstance();
  logging_ = new BufferManagerLogging();
  logging_->log("Initialized!");
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
    if (storage_used_ + size <= (storage_budget_max_ / 2)) {
      storage_used_ += size;
      ret = true;
    }
  }
  cout << "storage_used_ : " << storage_used_ << endl;
  logging_->log("%d MB applied, %d MB left!", size / 1024 / 1024,
                (storage_budget_max_ - storage_used_) / 1024 / 1024);
  logging_->log("%d actucl left free memory", actucl_free_memory / 1024 / 1024);
  lock_.release();
  return ret;
}
void BufferManager::returnStorageBudget(unsigned long size) {
  lock_.acquire();
  storage_used_ -= size;
  lock_.release();
}

unsigned BufferManager::getStorageMemoryBudegeInMilibyte() const {
  return storage_budget_max_ / 1024 / 1024;
}
