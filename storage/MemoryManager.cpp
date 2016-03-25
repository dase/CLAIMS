/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * /Claims/storage/MemoryManager.cpp
 *
 *  Created on: 2015年11月19日
 *      Author: Han,casa
 *		   Email:
 *
 * Description:
 *
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <glog/logging.h>
#include "MemoryManager.h"
#include "../configure.h"
#include "../Resource/BufferManager.h"
#include "../common/error_define.h"
#include "../common/error_no.h"
using namespace std;
using claims::common::rSuccess;
using claims::common::rNoEnoughMemory;
using claims::common::rMemoryPoolMallocFail;
using claims::common::rReturnFailFindTargetChunkId;

// namespace claims {
// namespace stroage {

MemoryChunkStore* MemoryChunkStore::instance_ = NULL;
MemoryChunkStore::MemoryChunkStore()
    : chunk_pool_(CHUNK_SIZE), block_pool_(BLOCK_SIZE), fc_(NULL) {}

MemoryChunkStore::~MemoryChunkStore() {
  chunk_pool_.purge_memory();
  block_pool_.purge_memory();
  if (NULL != fc_) {
    delete fc_;
    fc_ = NULL;
  }
}

bool MemoryChunkStore::IsExist(ChunkID& chunk_id) {
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::iterator it =
      chunk_list_.find(chunk_id);
  WasteTime();
  if (it != chunk_list_.cend()) {
    DLOG(INFO) << "chunk id already exists (chunk id = " << chunk_id.chunk_off
               << ")" << endl;
    it->second.lifetime_ = 0;
    return true;
  } else {
    return false;
  }
}

bool MemoryChunkStore::ApplyChunk(ChunkID chunk_id, void*& start_address) {
  lock_.acquire();
  if (true == IsExist(chunk_id)) {
    lock_.release();
    return false;
  }

  if (rSuccess != HasEnoughMemory()) {
    fc_->WayOfFreeChunk();
    DLOG(INFO) << "not enough memory!!" << std::endl;
  }

  if (NULL != (start_address = chunk_pool_.malloc())) {
    chunk_list_[chunk_id] = HdfsInMemoryChunk(start_address, CHUNK_SIZE);
    lock_.release();
    return true;
  } else {
    ELOG(rMemoryPoolMallocFail, "Error occurs when memalign!");
    lock_.release();
    return false;
  }
}

void MemoryChunkStore::ReturnChunk(const ChunkID& chunk_id) {
  lock_.acquire();
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::iterator it =
      chunk_list_.find(chunk_id);

  if (it == chunk_list_.cend()) {
    WLOG(rReturnFailFindTargetChunkId,
         "return fail to find the target chunk id !");
    lock_.release();
    return;
  }
  HdfsInMemoryChunk chunk_info = it->second;
  chunk_pool_.free(chunk_info.hook);
  chunk_list_.erase(it);
  BufferManager::getInstance()->returnStorageBudget(chunk_info.length);
  lock_.release();
}

bool MemoryChunkStore::GetChunk(const ChunkID& chunk_id,
                                HdfsInMemoryChunk& chunk_info) {
  lock_.acquire();
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::const_iterator it =
      chunk_list_.find(chunk_id);
  if (it != chunk_list_.cend()) {
    chunk_info = it->second;
    lock_.release();
    return true;
  }
  lock_.release();
  return false;
}
bool MemoryChunkStore::UpdateChunkInfo(const ChunkID& chunk_id,
                                       const HdfsInMemoryChunk& chunk_info) {
  lock_.acquire();
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::iterator it =
      chunk_list_.find(chunk_id);
  if (it == chunk_list_.cend()) {
    lock_.release();
    return false;
  }
  it->second = chunk_info;
  lock_.release();
  return true;
}

bool MemoryChunkStore::PutChunk(const ChunkID& chunk_id,
                                HdfsInMemoryChunk& chunk_info) {
  lock_.acquire();
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::const_iterator it =
      chunk_list_.find(chunk_id);
  if (it != chunk_list_.cend()) {
    LOG(INFO) << "The memory chunk is already existed!" << endl;
    lock_.release();
    return false;
  }
  chunk_list_[chunk_id] = chunk_info;
  lock_.release();
  return true;
}

// TODO(han)：LIRS is the optimization of LRU.
void MemoryChunkStore::FreeChunkLRU::WayOfFreeChunk() {
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::iterator target_ =
      chunk_list_.begin();
  for (boost::unordered_map<ChunkID, HdfsInMemoryChunk>::iterator mei_ =
           target_;
       mei_ != MemoryChunkStore::GetInstance()->chunk_list_.end(); mei_++) {
    if (mei_->second.lifetime_ >= target_->second.lifetime_) {
      target_ = mei_;
    }
  }
  LOG(INFO) << "The way is LRU: the free chunk: " << target_->first.chunk_off
            << endl;
  MemoryChunkStore::GetInstance()->chunk_pool_.free(target_->second.hook);
  MemoryChunkStore::GetInstance()->chunk_list_.erase(target_);
}

void MemoryChunkStore::FreeChunkRandom::WayOfFreeChunk() {
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::iterator it_ =
      chunk_list_.begin();
  int count = (int)MemoryChunkStore::GetInstance()->chunk_list_.size();
  srand((unsigned)time(NULL));
  int size = rand() % count;
  for (int i = 0; i < size; i++) it_++;
  LOG(INFO) << "The way is Random: the free chunk: " << it_->first.chunk_off
            << endl;
  MemoryChunkStore::GetInstance()->chunk_pool_.free(it_->second.hook);
  MemoryChunkStore::GetInstance()->chunk_list_.erase(it_);
}

MemoryChunkStore* MemoryChunkStore::GetInstance() {
  if (NULL == instance_) {
    instance_ = new MemoryChunkStore();
  }
  return instance_;
}
/**
 * @brief: By adjusting the parameters to judge whether system has enough
 * memory.
 */
RetCode MemoryChunkStore::HasEnoughMemory() {
  if (!BufferManager::getInstance()->applyStorageDedget(CHUNK_SIZE)) {
    return rNoEnoughMemory;
  } else
    return rSuccess;
}

//}  // namespace stroage
//}  // namespace claims
