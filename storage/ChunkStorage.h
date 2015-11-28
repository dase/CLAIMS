/*
 * ChunkStorage.h
 *
 *  Created on: Nov 14, 2013
 *      Author: wangli
 */

#ifndef CHUNKSTORAGE_H_
#define CHUNKSTORAGE_H_
#include <string>
#include <hdfs.h>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "StorageLevel.h"
#include "../utility/lock.h"
#include "../common/ids.h"
#include "../common/Block/BlockStream.h"
class ChunkReaderIterator {
 public:
  /**
   * This structure maintains all the information needed to access
   *  a block in in-memory chunk, in-disk chunk, or in-hdfs chunk.
   *
   *  The underlying reason for using this structure is to.
   */
  class block_accessor {
   public:
    block_accessor() {}
    ~block_accessor() {}
    virtual void GetBlock(BlockStreamBase*& block) const { assert(false); }
    unsigned GetBlockSize() const { return block_size_; }

    void SetBlockSize(unsigned blockSize) { block_size_ = blockSize; }

   protected:
    unsigned block_size_;
  };
  class InMemeryBlockAccessor : public block_accessor {
   public:
    InMemeryBlockAccessor() : target_block_start_address_(NULL){};
    ~InMemeryBlockAccessor() {}
    void GetBlock(BlockStreamBase*& block) const;
    void* getTargetBlockStartAddress() const {
      return target_block_start_address_;
    }

    void setTargetBlockStartAddress(void* targetBlockStartAddress) {
      target_block_start_address_ = targetBlockStartAddress;
    }

   private:
    void* target_block_start_address_;
  };

  class InDiskBlockAccessor : public block_accessor {
   public:
    void GetBlock(BlockStreamBase*& block) const;

    unsigned getBlockCur() const { return block_cur_; }

    void setBlockCur(unsigned blockCur) { block_cur_ = blockCur; }

    const ChunkID& getChunkId() const { return chunk_id_; }

    void setChunkId(const ChunkID& chunkId) { chunk_id_ = chunkId; }

    unsigned getChunkSize() const { return chunk_size_; }

    void setChunkSize(unsigned chunkSize) { chunk_size_ = chunkSize; }

   private:
    unsigned chunk_size_;
    ChunkID chunk_id_;
    unsigned block_cur_;
  };

  class InHDFSBlockAccessor : public block_accessor {
   public:
    void GetBlock(BlockStreamBase*& block) const;

    unsigned getBlockCur() const { return block_cur_; }

    void setBlockCur(unsigned blockCur) { block_cur_ = blockCur; }

    const ChunkID& getChunkId() const { return chunk_id_; }

    void setChunkId(const ChunkID& chunkId) { chunk_id_ = chunkId; }

    unsigned getChunkSize() const { return chunk_size_; }

    void setChunkSize(unsigned chunkSize) { chunk_size_ = chunkSize; }

   private:
    unsigned chunk_size_;
    ChunkID chunk_id_;
    unsigned block_cur_;
  };

  ChunkReaderIterator(const ChunkID& chunk_id, unsigned block_size,
                      unsigned chunk_size, const unsigned& number_of_blocks = 0)
      : chunk_id_(chunk_id),
        number_of_blocks_(number_of_blocks),
        cur_block_(0),
        block_size_(block_size),
        chunk_size_(chunk_size){};
  virtual bool NextBlock(BlockStreamBase*& block) = 0;
  virtual bool getNextBlockAccessor(block_accessor*& ba) = 0;
  bool nextBlock();
  virtual ~ChunkReaderIterator(){};

 public:
  ChunkID chunk_id_;
  unsigned number_of_blocks_;
  unsigned cur_block_;
  Lock lock_;
  unsigned block_size_;
  unsigned chunk_size_;
};
class InMemoryChunkReaderItetaor : public ChunkReaderIterator {
 public:
  InMemoryChunkReaderItetaor(void* const& start, const unsigned& chunk_size,
                             const unsigned& number_of_blocks,
                             const unsigned& block_size,
                             const ChunkID& chunk_id);
  virtual ~InMemoryChunkReaderItetaor();
  bool NextBlock(BlockStreamBase*& block);  // 1126 工作点
  bool getNextBlockAccessor(block_accessor*& ba);

 private:
  void* start_;
};

class DiskChunkReaderIteraror : public ChunkReaderIterator {
 public:
  DiskChunkReaderIteraror(const ChunkID& chunk_id, unsigned& chunk_size,
                          const unsigned& block_size);
  virtual ~DiskChunkReaderIteraror();
  bool nextBlock(BlockStreamBase*& block);
  bool getNextBlockAccessor(block_accessor*& ba);

 private:
  //	unsigned number_of_blocks_;
  //	unsigned cur_block_;
  /*the iterator creates a buffer and allocates its memory such that the query
   * processing can just use the Block without the concern the memory allocation
   * and deallocation.
   */
  Block* block_buffer_;
  int fd_;
};

class HDFSChunkReaderIterator : public ChunkReaderIterator {
 public:
  HDFSChunkReaderIterator(const ChunkID& chunk_id, unsigned& chunk_size,
                          const unsigned& block_size);
  virtual ~HDFSChunkReaderIterator();
  bool nextBlock(BlockStreamBase*& block);
  bool getNextBlockAccessor(block_accessor*& ba);

 private:
  //	unsigned number_of_blocks_;
  //	unsigned cur_block_;
  /*the iterator creates a buffer and allocates its memory such that the query
   * processing can just use the Block without the concern the memory allocation
   * and deallocation.
   */
  Block* block_buffer_;
  hdfsFS fs_;
  hdfsFile hdfs_fd_;
};

class ChunkStorage {
 public:
  /* considering that how block size effects the performance is to be tested,
   * here we leave
   * a parameter block_size for the performance test concern.
   */
  ChunkStorage(const ChunkID& chunk_id, const unsigned& block_size,
               const StorageLevel& desirable_level);
  virtual ~ChunkStorage();
  ChunkReaderIterator* createChunkReaderIterator();
  std::string printCurrentStorageLevel() const;

  ChunkID getChunkID() { return chunk_id_; }
  void setCurrentStorageLevel(const StorageLevel& current_level) {
    current_storage_level_ = current_level;
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
