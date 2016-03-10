/*
* BlockManager.h
*
*  Created on: 2013-10-11
*      Author: casa
*/

#ifndef BLOCKMANAGER_H_
#define BLOCKMANAGER_H_

#include <boost/unordered_map.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <stdio.h>
#include "hdfs.h"
#include "MemoryStore.h"
#include "DiskStore.h"
#include "BlockManagerId.h"
#include "PartitionStorage.h"
#include "../Debug.h"
#include "../common/ids.h"
#include "../common/Message.h"
#include "../common/Logging.h"
#include "../utility/lock.h"
using namespace std;

struct ChunkInfo {
  ChunkID chunkId;
  void *hook;
};

// 在spark中，blockmanager中是含有成员变量blockmanagerMasterActor的，但是这里由于akka和theron的区别不能这么实现
// 所以blockmanager代表的就是worker节点其中有个类叫BlockManagerWorkerActor
// 为什么要写为blockmanager呢？而不是column的manager呢？因为如果block为粒度更小一点
// 但是我们在交换的时候要以column的存储文件为粒度来交换，找到最近最久未使用的column而不是block

/* A better name may be StorageManager, as this module manages projections,
 * chunks, and blocks.*/
class BlockManager {
 public:
  enum storageLevel { memory, disk, offline };

  struct BlockInfo {
    // 现在blockinfo还只有一个属性
    // unsigned size_;
    // 在磁盘上还是在哪里
    storageLevel level_;
    BlockInfo(storageLevel level) { level_ = level; }
  };

 public:
  static BlockManager *getInstance();
  MemoryChunkStore *getMemoryChunkStore() const;
  virtual ~BlockManager();

  // 读各种配置文件，初始化，向master注册，然后启动worker
  void initialize();
  // 刚启动时的注册
  void registerToMaster(BlockManagerId *blockManagerId);
  // 心跳监听
  void heartBeat();
  // 同步安全的注册，一般是在put失败时
  void asyncReregister();
  // 在心跳失联时重新注册
  void reregister();
  // 报告所有的block，在重新注册之后
  void reportAllBlocks();
  // put单个block之后
  bool reportBlockStatus(string blockId);
  // 在reportBlockStatus中调用
  bool tryToReportBlockStatus(string blockId);

  // todo:返回的是一个block一个数据结构，肯定不是一个void
  void get(string blockId);
  // 从本地获得数据，现在统一都是从本地获得数据，会调用loadFromHdfs,有没有getRemote还需要讨论
  // 应该是scan的state中输入文件名，然后将这个文件名组成blockId,然后在open当中调用，见hdfsscan
  void *getLocal(string blockId);
  ChunkInfo loadFromHdfs(string file_name);

  int loadFromHdfs(const ChunkID &, void *const &desc, const unsigned &);

  int loadFromDisk(const ChunkID &, void *const &desc, const unsigned &) const;
  // 将这个blockId所代表的数据存进内存或者磁盘，所以其中有个参数肯定是storagelevel
  bool put(string blockId, storageLevel level, void *value);

  /* poc测试 */
  BlockManagerId *getId();
  string askForMatch(string filename, BlockManagerId bmi);
  bool containsPartition(const PartitionID &part) const;
  bool addPartition(const PartitionID &, const unsigned &number_of_chunks,
                    const StorageLevel &desirable_storage_level);
  bool removePartition(const PartitionID &);
  PartitionStorage *getPartitionHandle(const PartitionID &partition_id) const;

 private:
  BlockManager();

 private:
  static BlockManager *blockmanager_;
  // 这里blockmanager只是管理的是block的id，这个block到底是由memorystore管理
  // 还是diskstore，在blockmanager中再去划分
  map<string, BlockInfo *> blockInfoPool_;
  BlockManagerId *blockManagerId_;
  MemoryChunkStore *memstore_;
  DiskStore *diskstore_;
  /* poc测试 filename和projectid的映射*/
  map<string, string> file_proj_;

  boost::unordered_map<PartitionID, PartitionStorage *>
      partition_id_to_storage_;
  Logging *logging_;
  Lock lock;
};

#endif /* BLOCKMANAGER_H_ */
