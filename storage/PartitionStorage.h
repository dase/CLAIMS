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
 * /Claims/storage/PartitionReaderIterator.cpp
 *
 *  Created on: Nov 14, 2013
 *  Modified on: Nov 28, 2015
 *      Author: wangli, Hanzhang
 *       Email:
 *
 * Description:
 *
 */

#ifndef PARTITIONSTORAGE_H_
#define PARTITIONSTORAGE_H_
#include "ChunkStorage.h"
#include "StorageLevel.h"
#include "./PartitionReaderIterator.h"
#include "../utility/lock.h"

// namespace claims {
// namespace storage {
/**
 * Design by The Iterator pattern. It is a container of partition. We should
 * create iterator in the container and use the iterator to operator this
 * container. By operating the partition iterator, it can generate chunk.
 */
class PartitionStorage {
  friend class PartitionReaderIterator;

 public:
  class PartitionReaderIterator {
   public:
    /**
     * @brief Method description: construct the partition iterator.
     * @param PartitionStorage: include the point of container, the cursor of
     * chunk, and chunk_list_.
     */
    PartitionReaderIterator(PartitionStorage* partition_storage);

    virtual ~PartitionReaderIterator();

    /**
     * @brief Method description: According the iterator to call chunk list and
     * create chunk iterator.
     * @return ret: NULL: create the chunk iterator failed. NOt NULL: succeed.
     */
    virtual ChunkReaderIterator* NextChunk();

    virtual bool NextBlock(BlockStreamBase*& block);

   protected:
    PartitionStorage* ps_;
    unsigned chunk_cur_;
    ChunkReaderIterator* chunk_it_;
  };

  class AtomicPartitionReaderIterator : public PartitionReaderIterator {
   public:
    /**
     * @brief Method description: Construct the partition iterator. Different
     * from
     * PartitionReaderiterator, it don't copy next block one by one, just using
     * the block_accessor that store the point of block to assign.
     * @param  PartitionStorage: include the point of container, the cursor of
     * chunk, and chunk_list_.
     */
    AtomicPartitionReaderIterator(PartitionStorage* partition_storage)
        : PartitionReaderIterator(partition_storage) {}

    virtual ~AtomicPartitionReaderIterator() override;

    /**
     * @brief Method description: According the iterator to call chunk list and
     * create chunk iterator. Different from PartitionReaderiterator, it don't
     * copy next block one by one, just using the block_accessor that store the
     * point of block to assign.
     * @return ret: NULL: create the chunk iterator failed. NOt NULL: succeed.
     */
    ChunkReaderIterator* NextChunk() override;

    virtual bool NextBlock(BlockStreamBase*& block);

   private:
    Lock lock_;
  };

  /**
   * @brief Method description: construct the partition container.
   * @param :PartitionID: identify which partition is our require.
   * @param :number_of_chunks: get how many chunks in the partition.
   * @param :storage_level: the information of chunk in which storage level.
   */
  PartitionStorage(const PartitionID& partition_id,
                   const unsigned& number_of_chunks, const StorageLevel&);

  /**
   * @brief Method description: Destruction. Release all chunk_list_.
   */
  virtual ~PartitionStorage();

  void AddNewChunk();

  /**
   * @brief Method description: Expand the container of partition
   * @param :PartitionID: identify which partition is our require.
   * @param :number_of_chunks: get how many chunks in the partition.
   * @param :storage_level: the information of chunk in which storage level.
   */
  void UpdateChunksWithInsertOrAppend(const PartitionID& partition_id,
                                      const unsigned& number_of_chunks,
                                      const StorageLevel& storage_level);

  /**
   * @brief Method description: Clear the container of partition
   * @param :PartitionID: Choose the partition what we need to delete.
   */
  void RemoveAllChunks(const PartitionID& partition_id);

  /**
  * @brief Method description: Generate the iterator in iterator pattern.
  */
  PartitionStorage::PartitionReaderIterator* CreateReaderIterator();

  /**
   * @brief Method description: Generate the iterator in iterator
   * pattern. Different from PartitionReaderiterator, it don't copy next block
   * one by one, just using the block_accessor that store the point of block.
   */
  PartitionStorage::PartitionReaderIterator* CreateAtomicReaderIterator();

 protected:
  PartitionID partition_id_;
  unsigned number_of_chunks_;
  std::vector<ChunkStorage*> chunk_list_;
  StorageLevel desirable_storage_level_;
};
//}  // namespace storage
//}  // namespace claims

#endif /* PARTITIONSTORAGE_H_ */
