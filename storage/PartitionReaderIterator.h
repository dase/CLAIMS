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
 * /Claims/storage/PartitionReaderIderator.h
 *
 *  Created on: Nov 19 ,2015
 *      Author: Han，wangli
 *		   Email:
 *
 * Description: It belongs to design patterns of iterator. This is a iterator of
 * PartitionStorage.
 *
 */

#ifndef STORAGE_PARTITIONREADERITERATOR_H_
#define STORAGE_PARTITIONREADERITERATOR_H_

#include "./ChunkStorage.h"
#include "./StorageLevel.h"
#include "./PartitionReaderIterator.h"
#include "../utility/lock.h"

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
   * @brief Method description: Construct the partition iterator. Different from
   * PartitionReaderiterator, it don't copy next block one by one, just using
   * the block_accessor that store the point of block to assign.
   * @param  PartitionStorage: include the point of container, the cursor of
   * chunk, and chunk_list_.
   */
  AtomicPartitionReaderIterator(PartitionStorage* partition_storage)
      : PartitionReaderIterator(partition_storage){};

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

#endif  //  STORAGE_PARTITIONREADERITERATOR_H_
