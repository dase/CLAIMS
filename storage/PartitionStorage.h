/*
 * ProjectionStorage.h
 *
 *  Created on: Nov 14, 2013
 *      Author: wangli
 */

#ifndef PARTITIONSTORAGE_H_
#define PARTITIONSTORAGE_H_
#include "ChunkStorage.h"
#include "StorageLevel.h"
#include "PartitionReaderIterator.h"
#include "../utility/lock.h"

class PartitionStorage {
  friend class PartitionReaderIterator;
  PartitionStorage(const PartitionID& partition_id,
                   const unsigned& number_of_chunks, const StorageLevel&);
  virtual ~PartitionStorage();
  void AddNewChunk();
  void UpdateChunksWithInsertOrAppend(const PartitionID& partition_id,
                                      const unsigned& number_of_chunks,
                                      const StorageLevel& storage_level);
  void RemoveAllChunks(const PartitionID& partition_id);
  PartitionReaderIterator* CreateReaderIterator();  //迭代器模式的迭代器生成。
  PartitionReaderIterator* CreateAtomicReaderIterator();

 protected:
  PartitionID partition_id_;
  unsigned number_of_chunks_;
  std::vector<ChunkStorage*> chunk_list_;
  StorageLevel desirable_storage_level_;
};

#endif /* PARTITIONSTORAGE_H_ */
