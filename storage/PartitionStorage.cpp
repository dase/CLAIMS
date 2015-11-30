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
 * /CLAIMS/storage/PartitionStorage.cpp
 *
 *  Created on: NOV 14 ,2013
 *  Modified on: NOV 28, 2015
 *      Author: Hanzhang, wangli
 *       Email:
 *
 * Description:
 *
 */

#include "PartitionStorage.h"
#include "../Debug.h"
#include "./MemoryManager.h"
#include "../Config.h"

/**
 * According to number_of_chunks, construct chunk from partition and add into
 * the chunk_list_. Meantime, you can get specific information about chunk.
 */
PartitionStorage::PartitionStorage(const PartitionID& partition_id,
                                   const unsigned& number_of_chunks,

                                   const StorageLevel& storage_level)
    : partition_id_(partition_id),
      number_of_chunks_(number_of_chunks),
      desirable_storage_level_(storage_level) {
  for (unsigned i = 0; i < number_of_chunks_; i++) {
    chunk_list_.push_back(new ChunkStorage(
        ChunkID(partition_id_, i), BLOCK_SIZE, desirable_storage_level_));
  }
}

PartitionStorage::~PartitionStorage() {
  for (unsigned i = 0; i < chunk_list_.size(); i++) {
    chunk_list_[i]->~ChunkStorage();
  }
  chunk_list_.clear();
}

void PartitionStorage::AddNewChunk() { number_of_chunks_++; }

/**
 * when appending data, the last chunk may be dirty so set storage level as HDFS
 * to make sure the data will be reload from HDFS. actually, DISK is not used,
 * only HDFS and MEMORY is used.
 */
void PartitionStorage::UpdateChunksWithInsertOrAppend(
    const PartitionID& partition_id, const unsigned& number_of_chunks,
    const StorageLevel& storage_level) {
  if (!chunk_list_.empty()) {
    MemoryChunkStore::getInstance()->returnChunk(
        chunk_list_.back()->getChunkID());
    chunk_list_.back()->SetCurrentStorageLevel(HDFS);
  }
  for (unsigned i = number_of_chunks_; i < number_of_chunks; i++)
    chunk_list_.push_back(
        new ChunkStorage(ChunkID(partition_id, i), BLOCK_SIZE, storage_level));
  number_of_chunks_ = number_of_chunks;
}

/**
 * By searching in chunk_list_ to get chunk address(physical information), and
 * free the memory. This function is a logical process of delete the chunk, and
 * call back actual method.
 */
void PartitionStorage::RemoveAllChunks(const PartitionID& partition_id) {
  if (!chunk_list_.empty()) {
    vector<ChunkStorage*>::iterator iter = chunk_list_.begin();
    MemoryChunkStore* mcs = MemoryChunkStore::getInstance();
    for (; iter != chunk_list_.end(); iter++) {
      mcs->returnChunk((*iter)->getChunkID());
    }
    chunk_list_.clear();
    number_of_chunks_ = 0;
  }
}

PartitionReaderIterator* PartitionStorage::CreateReaderIterator() {
  return new PartitionReaderIterator(this);
}
PartitionReaderIterator* PartitionStorage::CreateAtomicReaderIterator() {
  return new AtomicPartitionReaderIterator(this);
}
