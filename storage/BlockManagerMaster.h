/*
 * BlockManagerMaster.h
 *
 *  Created on: 2013-10-15
 *      Author: casa
 */
/* consider to be merged with Catalog.binding*/
#ifndef BLOCKMANAGERMASTER_H_
#define BLOCKMANAGERMASTER_H_
#include <string>
#include <iostream>
#include "BlockManagerId.h"
#include "AllBlockInfo.h"
#include "BlanceMatcher.h"
#include "../common/ids.h"
#include "../common/Message.h"
using namespace std;
/*
 * just for sending binding and unbinding message
 */
class BlockManagerMaster {
 public:
  enum storageLevel { memory, disk };
  struct BlockInfo {
    string blockId_;
    storageLevel level_;
  };

  static BlockManagerMaster *getInstance() {
    if (master_ == 0) {
      master_ = new BlockManagerMaster();
    }
    return master_;
  }
  virtual ~BlockManagerMaster();
  void initialize();

  // 这个函数返回的是blockmanagerId
  //	BlockManagerId getLocations(string blockId){};
  bool SendBindingMessage(const PartitionID &, const unsigned &number_of_chunks,
                          const StorageLevel &, const NodeID &) const;
  bool SendUnbindingMessage(const PartitionID &partition_id,
                            NodeID &target) const;

 private:
  BlockManagerMaster();

 private:
  static BlockManagerMaster *master_;
  // 将blockMessage收到之后，首先看他是什么消息，然后传给BlockManagerMasterActor处理
  AllBlockInfo *abi_;
  BlanceMatcher *bm_;
};

#endif /* BLOCKMANAGERMASTER_H_ */
