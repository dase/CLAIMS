/*
 * BlockManagerMaster.h
 *
 *  Created on: 2013-10-15
 *      Author: casa
 */
/* consider to be merged with Catalog.binding*/
#ifndef BLOCKMANAGERMASTER_H_
#define BLOCKMANAGERMASTER_H_
/*
 * 和blockmanager一样尚未完成的带有actor的类
 * */
#include <Theron/Theron.h>
#include <Theron/Defines.h>
#include <string>
#include <iostream>
#ifdef DMALLOC
#include "dmalloc.h"
#endif

#include "BlockManagerId.h"
#include "AllBlockInfo.h"
#include "BlanceMatcher.h"
#include "../common/Logging.h"
#include "../common/ids.h"
#include "../common/Message.h"
using namespace std;

class BlockManagerMaster {
 public:
  enum storageLevel { memory, disk };
  struct BlockInfo {
    string blockId_;
    storageLevel level_;
  };
  class BlockManagerMasterActor : public Theron::Actor {
    friend class BlockManagerMaster;

   public:
    BlockManagerMasterActor(Theron::Framework *framework, const char *name);
    virtual ~BlockManagerMasterActor();

    void workerRegister(const StorageBudgetMessage &message,
                        const Theron::Address from);
    void heartbeatReceiver(const HeartBeatMessage &message,
                           const Theron::Address from);
    void blockStatusReceiver(const BlockStatusMessage &message,
                             const Theron::Address from);
    void matcherReceiver(const MatcherMessage &message,
                         const Theron::Address from);
    void SendInvalidSingle();  //脏数据失效信号，告知其他。（可以使用gossip
    // protocl，复杂度在log（n）;） --han

   private:
  };

  static BlockManagerMaster *getInstance() {
    if (master_ == 0) {
      master_ = new BlockManagerMaster();
    }
    return master_;
  }
  virtual ~BlockManagerMaster();

  void initialize();

  /* 共测试使用 */
  void testForPoc();

  // 这个函数返回的是blockmanagerId
  //	BlockManagerId getLocations(string blockId){};
  bool SendBindingMessage(const PartitionID &, const unsigned &number_of_chunks,
                          const StorageLevel &, const NodeID &) const;
  bool SendUnbindingMessage(const PartitionID &partition_id,
                            NodeID &target) const;

 private:
  BlockManagerMaster();
  std::string generateSlaveActorName(const NodeID &) const;

 private:
  static BlockManagerMaster *master_;
  // 将blockMessage收到之后，首先看他是什么消息，然后传给BlockManagerMasterActor处理
  AllBlockInfo *abi_;
  BlanceMatcher *bm_;
  Theron::Framework *framework_;
  BlockManagerMasterActor *actor_;
  Logging *logging_;
};

#endif /* BLOCKMANAGERMASTER_H_ */
