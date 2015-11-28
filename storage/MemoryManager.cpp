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
#include <memory.h>
#include "MemoryManager.h"
#include <glog/logging.h>
#include "../configure.h"
#include "../Resource/BufferManager.h"
using namespace std;

typedef int RetCode;
// namespace claims {
// namespace stroage {

MemoryChunkStore* MemoryChunkStore::instance_ = 0;
MemoryChunkStore::MemoryChunkStore()
    : chunk_pool_(CHUNK_SIZE), block_pool_(BLOCK_SIZE) {
  //  cout<<"in the memorystroage initialize"<<endl;
}

MemoryChunkStore::~MemoryChunkStore() {
  chunk_pool_.purge_memory();
  block_pool_.purge_memory();
}

bool MemoryChunkStore::IsExist(ChunkID& chunk_id) {
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::const_iterator it =
      chunk_list_.find(chunk_id);
  WasteTime();
  if (it != chunk_list_.cend()) {
    LOG(INFO) << "chunk id already exists (chunk id = " << chunk_id.chunk_off
              << ")" << endl;
    it->second.lifetime_ = 0;
    return true;
  } else {
    return false;
  }
}

bool MemoryChunkStore::applyChunk(ChunkID chunk_id, void*& start_address) {
  lock_.acquire();
  //  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::const_iterator it =
  //      chunk_list_.find(chunk_id);
  //  WasteTime();  //--Han
  //  if (it != chunk_list_.cend()) {
  //    printf("chunk id already exists (chunk id =%d)!\n", chunk_id.chunk_off);
  //    lock_.release();
  //    return false;
  //  }
  if (IsExist(chunk_id) == false) {
    if (!BufferManager::getInstance()->applyStorageDedget(CHUNK_SIZE)) {
      printf("not enough memory!!\n");
      FreeChunk();
      lock_.release();
      return false;
    }
  } else {
    if ((start_address = chunk_pool_.malloc()) != 0) {
      chunk_list_[chunk_id] = HdfsInMemoryChunk(start_address, CHUNK_SIZE);
      lock_.release();
      return true;
    }  //通过pool获取系统内存，malloc（） --han
    else {
      LOG(ERROR) << "Error occurs when memalign!"
                 << endl;  //应该有错误码，现在暂时不管了。
      // printf("Error occurs when memalign!\n");
      lock_.release();
      return false;
    }
  }
}  //目前完成，需要测试～～ --han

void MemoryChunkStore::returnChunk(const ChunkID& chunk_id) {
  lock_.acquire();
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::const_iterator it =
      chunk_list_.find(chunk_id);
  if (it == chunk_list_.cend()) {
    LOG(WARNING) << "return fail to the target chunk id !" << endl;
    // printf("return fail to find the target chunk id !\n");
    lock_.release();
    return;
  }
  HdfsInMemoryChunk chunk_info = it->second;

  chunk_pool_.free(chunk_info.hook);
  chunk_list_.erase(it);
  BufferManager::getInstance()->returnStorageBudget(chunk_info.length);
  lock_.release();
}

bool MemoryChunkStore::getChunk(const ChunkID& chunk_id,
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
bool MemoryChunkStore::updateChunkInfo(const ChunkID& chunk_id,
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
}  //更新在原有的基础上，进行操作。

bool MemoryChunkStore::putChunk(const ChunkID& chunk_id,
                                HdfsInMemoryChunk& chunk_info) {
  lock_.acquire();
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::const_iterator it =
      chunk_list_.find(chunk_id);
  if (it != chunk_list_.cend()) {
    LOG(INFO) << "The memory chunk is already existed!" << endl;
    // printf("The memory chunk is already existed!\n");
    lock_.release();
    return false;
  }
  chunk_list_[chunk_id] = chunk_info;
  lock_.release();
  return true;
}

// todo：清空最近最少使用的块，但是我没有加入LIRS
void MemoryChunkStore::FreeChunk() {
  lock_.acquire();
  boost::unordered_map<ChunkID, HdfsInMemoryChunk>::const_iterator target_ =
      chunk_list_.begin();
  for (boost::unordered_map<ChunkID, HdfsInMemoryChunk>::const_iterator mei_ =
           chunk_list_.begin();
       mei_ != chunk_list_.end(); mei_++) {
    if ((*mei_).second.lifetime_ > (*target_).second.lifetime_) {
      target_ = mei_;
    }
  }
  //释放最近最少使用的chunk，在内存池释放并且将它在chunk_list_位置清空。
  chunk_pool_.free(target_->second.hook);
  chunk_list_.erase(target_);
  lock_.release();  //需不需要加锁  --han
}
MemoryChunkStore* MemoryChunkStore::getInstance() {
  if (instance_ == 0) {
    instance_ = new MemoryChunkStore();
  }
  return instance_;
}

//}  // namespace stroage
//}  // namespace claims
