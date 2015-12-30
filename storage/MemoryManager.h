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
 * /Claims/storage/MemoryManager.h
 *
 *  Created on: 2015年11月19日
 *      Author: Han
 *		   Email:
 *
 * Description:
 *
 */

#ifndef STORAGE_MEMORYMANAGER_H_
#define STORAGE_MEMORYMANAGER_H_

#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>

#include <string>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>
#include <boost/pool/pool.hpp>
#include <iostream>
#include "../common/error_define.h"
#include "../common/error_no.h"
#include "BlockStore.h"
#include "../common/rename.h"
#include "../Debug.h"
#include "../utility/lock.h"

using std::cout;
using std::vector;
using std::map;
using std::list;
using std::endl;
using boost::pool;

/**
 * @brief Method description: the struct of chunk, include the start_address,
 * the length of chunk, and the life time in the memory.
 */
typedef struct HdfsBlock {
  HdfsBlock() : hook(NULL), length(0), lifetime_(0) {}
  HdfsBlock(void* add, int length) : hook(add), length(length), lifetime_(0) {}
  void* hook;
  // record every block that is the length of file.
  int length;
  // whether is serialized
  // the time stay in memory，this value is used to LIRS.
  int lifetime_;
} HdfsInMemoryChunk;

/**
 * @brief Method description: memorystore only responsible for access data,
 * but this is noting to data management and how to store. when accessing the
 * data, it must check out how many space in memory. if no enough memory, we
 * should choose the policy to remove the space. There are a lot of strategy
 * selections, so design memorystore in the pattern of singleton.
 */
class MemoryChunkStore {
 public:
  /**
   * @brief: the implement of the  singleton pattern
   */
  static MemoryChunkStore* GetInstance();
  MemoryChunkStore();
  virtual ~MemoryChunkStore();

  /**
   * @brief Method description: judge whether the chunk in the chunk_list_
   * @param ChunkID: the only token.
   * @return True: the chunk in the chunk_list_. False: or not.
   */
  bool IsExist(ChunkID& chunk_id);

  /**
   * @brief Method description: apply the space of memory for chunk .if the
   * system is no enough memory, it will free other chunk before malloc the new
   * chunk in the pool of memory. the policy is decided before construct the
   * partition storage.
   * @param ChunkID: the only token.
   * @param start_address: if the start_address is null, it will assign the new
   * space for it. if not, just skip the step of malloc.
   * @return True: apply successful. False: Error occurs when mmealign. it
   * happened in the step of malloc, you should check out the reminder of
   * operation system's memory.
   */
  bool ApplyChunk(ChunkID chunk_id, void*& start_address);

  /**
   * @brief Method description: return the resource which be occupied by each of
   * chunk_list_
   * @param ChunkID: the only token.
   */
  void ReturnChunk(const ChunkID& chunk_id);

  /**
   * @brief Method description: update the information of chunk info, and avoid
   * the waste of resource.
   * @param ChunkID: the only token.
   * @param chunk_info: use for replace.
   */
  bool UpdateChunkInfo(const ChunkID& chunk_id,
                       const HdfsInMemoryChunk& chunk_info);

  /**
   * @brief: the method is that increasing the time component.
   */
  void WasteTime() {
    for (auto& i : chunk_list_) i.second.lifetime_++;
  };

  /**
   * @brief: the base class for the method of free chunk.Aimed to polymorphic.
   * the implement of the strategy pattern.
   */
  class FreeChunk {
   public:
    FreeChunk(){};
    virtual ~FreeChunk(){};
    virtual void WayOfFreeChunk(){};
  };
  class FreeChunkLRU : public FreeChunk {
   public:
    FreeChunkLRU(){};
    ~FreeChunkLRU(){};
    void WayOfFreeChunk() override;
  };

  class FreeChunkRandom : public FreeChunk {
   public:
    FreeChunkRandom(){};
    ~FreeChunkRandom(){};
    void WayOfFreeChunk() override;
  };
  /**
   * @brief Method description: Currently according to the partition apply the
   * space, we choose the best policy to remove chunk.
   * @param flag:0:Random;1:LRU;
   */
  // TODO(han): Add new algorithm in the future;
  void SetFreeAlgorithm(int flag) {
    if (NULL != fc_) {
      delete fc_;
      fc_ = NULL;
    }
    if (flag == 1)
      fc_ = new FreeChunkLRU();
    else
      fc_ = new FreeChunkRandom();
  }

  RetCode HasEnoughMemory();

  /* todo:这里还有可能是直接存储对象或者存储将对象序列化之后的结果两种
   * 在spark中要估计结果，所以有一个hdfsBlock中的length变量，在此留接口
   * 如果我们要估计结果，要写一个类来做估计
   * */
  bool putValue(string chunkId, void* value) {
    // todo:
    // 在此如果放不下了怎么办？采用什么样的交换策略，留接口，tryToPut中应该还有个接口是估计出来的值
    tryToPut(chunkId, value);
    return true;
  };
  void* getChunk(string blockId) {
    map<string, HdfsBlock>::iterator it_;
    it_ = bufferpool_.find(blockId);
    if (it_ != bufferpool_.end()) {
      return it_->second.hook;
    }
  };
  bool GetChunk(const ChunkID& chunk_id, HdfsInMemoryChunk& chunk_info);

  bool PutChunk(const ChunkID& chunk_id, HdfsInMemoryChunk& chunk_info);
  /*
   * 将block为单位放到buffer pool中
   */
  bool tryToPut(string chunkId, void* value) {
    if (ensureFreeSpace()) {
      lock_.acquire();
      HdfsBlock chunkin;
      chunkin.hook = (char*)value;
      // todo:接口，就先等于这个吧
      chunkin.length = CHUNK_SIZE;
      bufferpool_[chunkId.c_str()] = chunkin;
      lock_.release();
    } else {
    }
    return true;
  }  //这个是以block为单位维护消息表？ --han

  bool ensureFreeSpace() {
    // todo: 基于LRU的column-based交换
    return true;
  }
  /* 有这个函数提供一个文件到block的映射,这个地方可以用iterator模式将其从
   * master端获取，因为做iterator的节zcl点肯定不是主节点，下面为调试用
   * */
  vector<string> getFileLocation(string partition_file_name) {
    vector<string> block_set;
    block_set.push_back("/home/casa/storage/data/1");
    block_set.push_back("/home/casa/storage/data/2");
    return block_set;
  }

 private:
  static MemoryChunkStore* instance_;
  /**
   * @brief: The logical struct about the chunk in the memory
   */
  boost::unordered_map<ChunkID, HdfsInMemoryChunk> chunk_list_;
  /**
   * @brief: when you store in the pool, you should lock to avoid the deadlock.
   */
  Lock lock_;

  /**
   * @brief: Instantiate the fc_ in different way to choose the policy.
   */
  FreeChunk* fc_;

  /**
   * @brief: the memory pool is used to the strategy of remove the chunk.
   */
  pool<> chunk_pool_;
  map<string, HdfsBlock> bufferpool_;
  pool<> block_pool_;
};

#endif  //  STORAGE_MEMORYMANAGER_H_
