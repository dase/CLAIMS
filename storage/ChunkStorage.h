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
 * /CLAIMS/storage/ChunkStorage.h
 *
 *  Created on: NOV 14, 2013
 *  Modified on: NOV 29, 2015
 *      Author: Hanzhang,wangli
 *       Email:
 *
 * Description:
 *
 */

#ifndef CHUNKSTORAGE_H_
#define CHUNKSTORAGE_H_
#include <string>
#include <hdfs.h>
#include "./StorageLevel.h"
#include "../utility/lock.h"
#include "../common/ids.h"
#include "../common/Block/BlockStream.h"

/**
 * Design by the iterator pattern. ChunkReaderIterator gets blocks and handle
 * the container of chunk.
 */
class ChunkReaderIterator {
 public:
  /**
   * This structure maintains all the information needed to access a block in
   * in-memory chunk, in-disk chunk, or in-hdfs chunk. The underlying reason for
   * using this structure is to traverse each blocks.
   */
  class block_accessor {
   public:
    block_accessor() {}

    ~block_accessor() {}

    virtual void GetBlock(BlockStreamBase*& block) const { assert(false); }

    unsigned GetBlockSize() const { return block_size_; }

    void SetBlockSize(unsigned block_size) { block_size_ = block_size; }

   protected:
    unsigned block_size_;
  };

  /**
   * The structure of block_accessor in memory just need the address of blocks.
   */
  class InMemeryBlockAccessor : public block_accessor {
   public:
    InMemeryBlockAccessor() : target_block_start_address_(NULL){};

    ~InMemeryBlockAccessor() {}

    void GetBlock(BlockStreamBase*& block) const;

    void* GetTargetBlockStartAddress() const {
      return target_block_start_address_;
    }

    void SetTargetBlockStartAddress(void* target_block_start_address) {
      target_block_start_address_ = target_block_start_address;
    }

   private:
    void* target_block_start_address_;
  };

  class InDiskBlockAccessor : public block_accessor {
   public:
    void GetBlock(BlockStreamBase*& block) const;

    unsigned GetBlockCur() const { return block_cur_; }

    void SetBlockCur(unsigned block_cur) { block_cur_ = block_cur; }

    const ChunkID& GetChunkId() const { return chunk_id_; }

    void SetChunkId(const ChunkID& chunk_id) { chunk_id_ = chunk_id; }

    unsigned GetChunkSize() const { return chunk_size_; }

    void SetChunkSize(unsigned chunk_size) { chunk_size_ = chunk_size; }

   private:
    unsigned chunk_size_;
    ChunkID chunk_id_;
    unsigned block_cur_;
  };

  class InHDFSBlockAccessor : public block_accessor {
   public:
    void GetBlock(BlockStreamBase*& block) const;

    unsigned GetBlockCur() const { return block_cur_; }

    void SetBlockCur(unsigned block_cur) { block_cur_ = block_cur; }

    const ChunkID& GetChunkId() const { return chunk_id_; }

    void SetChunkId(const ChunkID& chunk_id) { chunk_id_ = chunk_id; }

    unsigned GetChunkSize() const { return chunk_size_; }

    void SetChunkSize(unsigned chunk_size) { chunk_size_ = chunk_size; }

   private:
    unsigned chunk_size_;
    ChunkID chunk_id_;
    unsigned block_cur_;
  };

  /**
   * @brief Method description: construct the iterator of chunk by using the
   * initialization list.
   * @param :ChunkID: the id of chunk.
   * @param: block_size: the size of block.
   * @param: chunk_size: the size of chunk.
   * @param: number_of_blocks: how many blocks in the chunk.
   */
  ChunkReaderIterator(const ChunkID& chunk_id, unsigned block_size,
                      unsigned chunk_size, const unsigned& number_of_blocks = 0)
      : chunk_id_(chunk_id),
        number_of_blocks_(number_of_blocks),
        cur_block_(0),
        block_size_(block_size),
        chunk_size_(chunk_size){};

  virtual bool NextBlock(BlockStreamBase*& block) = 0;

  virtual bool GetNextBlockAccessor(block_accessor*& ba) = 0;

  /**
   * @brief Method description: Just judge whether the cursor of block arrived
   * at the end of block. It is a logical information of the cursor of block.
   * @return true: Has block in the chunk.
   * @return false: nothing left in the chunk.
   */
  bool NextBlock();

  virtual ~ChunkReaderIterator(){};

 public:
  ChunkID chunk_id_;
  unsigned number_of_blocks_;
  // the cursor of block in the chunk to make sure the operating position.
  unsigned cur_block_;
  Lock lock_;
  unsigned block_size_;
  unsigned chunk_size_;
};

/**
 * Inherit ChunkReaderIterator to manager the chunk in the memory. Different
 * from other Chunk iterator, it just need the point of blocks.
 */
class InMemoryChunkReaderItetaor : public ChunkReaderIterator {
 public:
  InMemoryChunkReaderItetaor(void* const& start, const unsigned& chunk_size,
                             const unsigned& number_of_blocks,
                             const unsigned& block_size,
                             const ChunkID& chunk_id);
  virtual ~InMemoryChunkReaderItetaor();

  /**
   * @brief Method description: Just judge whether the cursor of block arrived
   * at the end of block. It is a logical information of the cursor of block.
   * @return true: Has block in the chunk.
   * @return false: nothing left in the chunk.
   */
  bool NextBlock(BlockStreamBase*& block);

  bool GetNextBlockAccessor(block_accessor*& ba);

 private:
  void* start_;
};

/**
 * the iterator creates a buffer and allocates its memory such that the query
 * processing can just use the Block without the concern the memory allocation
 * and deallocation.
 */

class DiskChunkReaderIteraror : public ChunkReaderIterator {
 public:
  /**
   * @brief Method description: By call back read() and lseek() to construct,
   * chunk reader iterator can read chunk which in disk.
   * @param: ChunkID: the ID of chunk.
   * @param: chunk_size: the size of chunk.
   * @param: block_size: the size of block.
   */
  DiskChunkReaderIteraror(const ChunkID& chunk_id, unsigned& chunk_size,
                          const unsigned& block_size);

  virtual ~DiskChunkReaderIteraror();

  /**
   * @brief Method description: Just judge whether the cursor of block arrived
   * at the end of block. It is a logical information of the cursor of block.
   * @param: block
   * @return true: Has block in the chunk.
   * @return false: nothing left in the chunk.
   */
  bool NextBlock(BlockStreamBase*& block);

  bool GetNextBlockAccessor(block_accessor*& ba);

 private:
  Block* block_buffer_;
  int fd_;
};

/**
 * the iterator creates a buffer and allocates its memory such that the query
 * processing can just use the Block without the concern the memory allocation
 * and deallocation.
 */
class HDFSChunkReaderIterator : public ChunkReaderIterator {
 public:
  /**
   * @brief Method description: By call back the c++ api of hdfs to construct,
   * chunk reader iterator can read chunk which in hdfs.
   * @param: ChunkID: the ID of chunk.
   * @param: chunk_size: the size of chunk.
   * @param: block_size: the size of block.
   */
  HDFSChunkReaderIterator(const ChunkID& chunk_id, unsigned& chunk_size,
                          const unsigned& block_size);

  virtual ~HDFSChunkReaderIterator();

  /**
   * @brief Method description: Just judge whether the cursor of block arrived
   * at the end of block. It is a logical information of the cursor of block.
   * @param: block
   * @return true: Has block in the chunk.
   * @return false: nothing left in the chunk.
   */
  bool NextBlock(BlockStreamBase*& block);

  bool GetNextBlockAccessor(block_accessor*& ba);

 private:
  Block* block_buffer_;
  hdfsFS fs_;
  hdfsFile hdfs_fd_;
};

class ChunkStorage {
 public:
  /**
   * Considering that how block size effects the performance is to be tested,
   * here we leave a parameter block_size for the performance test concern.
   */
  ChunkStorage(const ChunkID& chunk_id, const unsigned& block_size,
               const StorageLevel& desirable_storage_level);

  virtual ~ChunkStorage();

  /**
   * @brief Method description: Generate the iterator for chunk storage to
   * handle chunks.
   */
  ChunkReaderIterator* CreateChunkReaderIterator();

  std::string PrintCurrentStorageLevel() const;

  ChunkID GetChunkID() { return chunk_id_; }

  void SetCurrentStorageLevel(const StorageLevel& current_storage_level) {
    current_storage_level_ = current_storage_level;
  }

 private:
  unsigned block_size_;
  unsigned chunk_size_;
  StorageLevel desirable_storage_level_;
  StorageLevel current_storage_level_;
  ChunkID chunk_id_;
  Lock lock_;
};

#endif /* CHUNKSTORAGE_H_ */
