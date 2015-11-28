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
 * @brief Method description:
 * @param
 * @return
 * @details   (additional)
 */

typedef struct HdfsBlock {
  HdfsBlock() : hook(0), length(0), lifetime_(0) {}
  HdfsBlock(void* add, int length) : hook(add), length(length), lifetime_(0) {}
  void* hook;
  /*记录每个block大小也就是文件长度*/
  int length;
  // 是否被序列化过
  int lifetime_;
  //生存周期,用于LRU
} HdfsInMemoryChunk;

// typedef HdfsBlock HdfsInMemoryChunk;

/*
 * memorystore只是负责数据的存取，而和数据的管理和为什么存储是没有关系的，
 * 在负责数据存取的同时，put的时候还要看看内存够不够，如果不够就要开始内存
 * 空间的移除，在此有很多的策略选择，要将memstore写成单例模式
 * */
class MemoryChunkStore {
 public:
  static MemoryChunkStore* getInstance();
  MemoryChunkStore();
  virtual ~MemoryChunkStore();

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

  bool IsExist(ChunkID& chunk_id);

  bool applyChunk(ChunkID chunk_id, void*& start_address);

  void returnChunk(const ChunkID& chunk_id);

  bool updateChunkInfo(const ChunkID& chunk_id,
                       const HdfsInMemoryChunk& chunk_info);

  void* getChunk(string blockId) {
    map<string, HdfsBlock>::iterator it_;
    it_ = bufferpool_.find(blockId);
    if (it_ != bufferpool_.end()) {
      return it_->second.hook;
    }
  };
  bool getChunk(const ChunkID& chunk_id, HdfsInMemoryChunk& chunk_info);

  bool putChunk(const ChunkID& chunk_id, HdfsInMemoryChunk& chunk_info);

  bool remove(string blockId) { return true; };

  bool contains(string blockId) { return false; };

  unsigned getSize(string blockId) { return 0; };

  void ClearTime() {
    for (auto& i : chunk_list_) i.second.lifetime_ = 0;
  };
  void WasteTime() {
    for (auto& i : chunk_list_) i.second.lifetime_++;
  };  //用于增加时间成分的。 --han

  /*
       * 将block为单位放到buffer pool中
       * */
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

  void FreeChunk() {}  //选择内存池哪些应该被释放。基于LRU。 --han

  /* 有这个函数提供一个文件到block的映射,这个地方可以用iterator模式将其从
   * master端获取，因为做iterator的节zcl点肯定不是主节点，下面为调试用
   * */
  vector<string> getFileLocation(string partition_file_name) {
    vector<string> block_set;
    block_set.push_back("/home/casa/storage/data/1");
    block_set.push_back("/home/casa/storage/data/2");
    return block_set;
  }
  //获得当前memory情况～～～  --han(做性能优化～～ )
  void GetAvailableMemory() {}
  void GetFreeMemory() {}

 private:
  map<string, HdfsBlock> bufferpool_;
  boost::unordered_map<ChunkID, HdfsInMemoryChunk> chunk_list_;
  // 本节点能使用的最大的内存，以兆为单位
  long maxMemory_;
  // 现在使用了多少内存？以兆为单位
  long currentMemory_;
  // 在存储进去buffer pool的时候要枷锁
  Lock lock_;

  pool<> chunk_pool_;
  pool<> block_pool_;

  static MemoryChunkStore* instance_;
};

#endif  //  STORAGE_MEMORYMANAGER_H_
