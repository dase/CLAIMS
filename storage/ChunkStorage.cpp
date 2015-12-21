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
 * /CLAIMS/storage/ChunkStorage.cpp
 *
 *  Created on: NOV 14, 2013
 *  Modified on: NOV 29, 2015
 *      Author: Hanzhang,wangli
 *       Email:
 *
 * Description:
 *
 */
#include <hdfs.h>
#include "./ChunkStorage.h"
#include <glog/logging.h>
#include "./BlockManager.h"
#include "../Debug.h"
#include "../utility/warmup.h"
#include "../utility/rdtsc.h"
#include "../Config.h"
#include "../common/error_define.h"
#include "../common/error_no.h"

using claims::common::CStrError;
using claims::common::rUnkownStroageLevel;
using claims::common::rFailOpenFileInDiskChunkReaderIterator;
using claims::common::rFailReadOneBlockInDiskChunkReaderIterator;
using claims::common::rFailOpenHDFSFileInStorage;
using claims::common::rFailSetStartOffsetInStorage;

bool ChunkReaderIterator::NextBlock() {
  lock_.acquire();
  if (this->cur_block_ >= this->number_of_blocks_) {
    lock_.release();
    return false;
  }
  cur_block_++;
  lock_.release();
  return true;
}

ChunkStorage::ChunkStorage(const ChunkID& chunk_id, const unsigned& block_size,
                           const StorageLevel& desirable_storage_level)
    : chunk_id_(chunk_id),
      block_size_(block_size),
      desirable_storage_level_(desirable_storage_level),
      current_storage_level_(HDFS),
      chunk_size_(CHUNK_SIZE) {}

ChunkStorage::~ChunkStorage() {
  // TODO(wangli): Auto-generated destructor stub
}
/**
 * The function create the chunk iterator. Meantime, according to the storage
 * level, create the chunk reader iterator in which storage level. It is a
 * optimization that memory store data as a buffer. The granularity of reading
 * file is chunk.
 */
ChunkReaderIterator* ChunkStorage::CreateChunkReaderIterator() {
  ChunkReaderIterator* ret;

  lock_.acquire();
  HdfsInMemoryChunk chunk_info;
  if (current_storage_level_ == MEMORY &&
      !BlockManager::getInstance()->getMemoryChunkStore()->getChunk(
          chunk_id_, chunk_info)) {
    current_storage_level_ = HDFS;
    cout << "clean dirty data" << endl;
    cout << "the chunk has be freed" << chunk_id_.chunk_off << endl;
  }  //判断之前被free调的chunk，脏数据的storage——level修改。
  switch (current_storage_level_) {
    case MEMORY: {
      if (BlockManager::getInstance()->getMemoryChunkStore()->getChunk(
              chunk_id_, chunk_info))
        ret = new InMemoryChunkReaderItetaor(chunk_info.hook, chunk_info.length,
                                             chunk_info.length / block_size_,
                                             block_size_, chunk_id_);
      else
        ret = NULL;  //问题点
      break;
    }
    case DISK: {
      printf("Currently, current storage level should not be DISK~! -_-\n");
      assert(false);
      break;
    }
    case HDFS: {
      if (desirable_storage_level_ == MEMORY) {
        chunk_info.length = CHUNK_SIZE;
        if (BlockManager::getInstance()->getMemoryChunkStore()->applyChunk(
                chunk_id_, chunk_info.hook)) {
          /* there is enough memory storage space, so the storage level can be
           * shifted.*/
          if (Config::local_disk_mode) {
            chunk_info.length = BlockManager::getInstance()->LoadFromDisk(
                chunk_id_, chunk_info.hook, chunk_info.length);
          } else {
            chunk_info.length = BlockManager::getInstance()->LoadFromHdfs(
                chunk_id_, chunk_info.hook, chunk_info.length);
          }
          if (chunk_info.length <= 0) {
            /**
          * chunk_info.length<=0 means that either the file does not exist or
          * the current chunk_id exceeds the actual size of the file.
             */
            BlockManager::getInstance()->getMemoryChunkStore()->returnChunk(
                chunk_id_);
            ret = NULL;
            break;
          }
          current_storage_level_ = MEMORY;

          /* update the chunk info in the Chunk store in case that the
           * chunk_info is updated.*/
          BlockManager::getInstance()->getMemoryChunkStore()->updateChunkInfo(
              chunk_id_, chunk_info);
          ret = new InMemoryChunkReaderItetaor(
              chunk_info.hook, chunk_info.length,
              chunk_info.length / block_size_, block_size_, chunk_id_);
          break;
        } else {
          /*The storage memory is full, some swap algorithm is needed here.
           * TODO: swap algorithm. I finish in applychunk().*/
          printf("Failed to get memory chunk budege!\n");
          LOG(WARNING) << "Failed to get memory chunk budege!" << endl;
          //          BlockManager::getInstance()->getMemoryChunkStore()->FreeChunk();

          //          current_storage_level_ =
          //          HDFS;//chunk_list_.setStorageLevel
          //          assert(false);
        }
      }

      ret = new DiskChunkReaderIteraror(chunk_id_, chunk_size_, block_size_);
      break;
    }
    default: { WLOG(rUnkownStroageLevel, "current storage level: unknown!"); }
  }
  lock_.release();
  return ret;
}

std::string ChunkStorage::PrintCurrentStorageLevel() const { return ""; }

InMemoryChunkReaderItetaor::InMemoryChunkReaderItetaor(
    void* const& start, const unsigned& chunk_size,
    const unsigned& number_of_blocks, const unsigned& block_size,
    const ChunkID& chunk_id)
    : start_(start),
      ChunkReaderIterator(chunk_id, block_size, chunk_size, number_of_blocks) {}

bool InMemoryChunkReaderItetaor::NextBlock(BlockStreamBase*& block) {
  lock_.acquire();
  if (cur_block_ >= number_of_blocks_) {
    lock_.release();
    return false;
  }
  cur_block_++;
  lock_.release();

  /* calculate the block start address.*/
  const char* block_start_address = (char*)start_ + cur_block_ * block_size_;

  /* Create a block, which will not free block_start_address when destructed.*/
  Block temp_block(block_size_, block_start_address);

  /*construct the block stream from temp_block. In the current version, the
   * memory copy is used for simplicity.
   * TODO(wangli): avoid memory copy.*/
  block->constructFromBlock(temp_block);
  return true;
}
InMemoryChunkReaderItetaor::~InMemoryChunkReaderItetaor() {}

DiskChunkReaderIteraror::DiskChunkReaderIteraror(const ChunkID& chunk_id,
                                                 unsigned& chunk_size,
                                                 const unsigned& block_size)
    : ChunkReaderIterator(chunk_id, block_size, chunk_size) {
  block_buffer_ = new Block(block_size_);
  fd_ = FileOpen(chunk_id_.partition_id.getPathAndName().c_str(), O_RDONLY);
  if (fd_ == -1) {
    ELOG(rFailOpenFileInDiskChunkReaderIterator,
         chunk_id_.partition_id.getPathAndName().c_str());
    number_of_blocks_ = 0;
  } else {
    const unsigned start_pos = CHUNK_SIZE * chunk_id_.chunk_off;
    const unsigned long length = lseek(fd_, 0, SEEK_END);

    if (length <= start_pos) {
      printf("fails to set the start offset %d for [%s]\n", start_pos,
             chunk_id.partition_id.getName().c_str());
      number_of_blocks_ = 0;
    } else {
      const unsigned offset = lseek(fd_, start_pos, SEEK_SET);
      printf("The file is set to be %d\n", offset);
      //			sleep(1);
      if (start_pos + CHUNK_SIZE < length) {
        number_of_blocks_ = CHUNK_SIZE / block_size_;
      } else {
        number_of_blocks_ = (length - start_pos) / block_size_;
        LOG(INFO) << "This chunk has only" << number_of_blocks_ << "blocks!"
                  << endl;
      }
    }
  }
}
DiskChunkReaderIteraror::~DiskChunkReaderIteraror() {
  block_buffer_->~Block();
  FileClose(fd_);
}

bool DiskChunkReaderIteraror::NextBlock(BlockStreamBase*& block) {
  lock_.acquire();
  if (cur_block_ >= number_of_blocks_) {
    lock_.release();
    return false;
  }
  const unsigned posistion = lseek(fd_, 0, SEEK_CUR);
  /*
   * the read function will automatically move the read position, so the lseek
   * is not needed here.
   */
  tSize bytes_num =
      read(fd_, block_buffer_->getBlock(), block_buffer_->getsize());
  if (bytes_num == block_size_) {
    cur_block_++;
    block->constructFromBlock(*block_buffer_);
    lock_.release();
    return true;
  } else {
    cur_block_++;
    ELOG(rFailReadOneBlockInDiskChunkReaderIterator,
         "failed to read one block");
    lock_.release();
    return false;
  }
}

HDFSChunkReaderIterator::HDFSChunkReaderIterator(const ChunkID& chunk_id,
                                                 unsigned& chunk_size,
                                                 const unsigned& block_size)
    : ChunkReaderIterator(chunk_id, block_size, chunk_size) {
  block_buffer_ = new Block(block_size_);
  fs_ = hdfsConnect(Config::hdfs_master_ip.c_str(), Config::hdfs_master_port);
  hdfs_fd_ = hdfsOpenFile(fs_, chunk_id.partition_id.getName().c_str(),
                          O_RDONLY, 0, 0, 0);
  if (!hdfs_fd_) {
    ELOG(rFailOpenHDFSFileInStorage, chunk_id.partition_id.getName().c_str());
    number_of_blocks_ = 0;
  }

  const unsigned start_pos = start_pos + CHUNK_SIZE * chunk_id_.chunk_off;
  if (hdfsSeek(fs_, hdfs_fd_, start_pos) == -1) {
    LOG(WARNING) << "[" << rFailSetStartOffsetInStorage << " , "
                 << CStrError(rFailSetStartOffsetInStorage) << "]"
                 << "fails to set the start offset" << start_pos << "for "
                 << chunk_id.partition_id.getName().c_str() << endl;
    number_of_blocks_ = 0;
  }
  hdfsFileInfo* file_info = hdfsGetPathInfo(
      fs_, "/imdb/");  // to be refined after communicating with Zhang Lei
  if (start_pos + CHUNK_SIZE < file_info->mSize) {
    number_of_blocks_ = CHUNK_SIZE / block_size_;
  } else {
    number_of_blocks_ = (file_info->mSize - start_pos) / block_size_;
  }
  hdfsFreeFileInfo(file_info, 1);
}
HDFSChunkReaderIterator::~HDFSChunkReaderIterator() {
  block_buffer_->~Block();
  hdfsCloseFile(fs_, hdfs_fd_);
  hdfsDisconnect(fs_);
}
bool HDFSChunkReaderIterator::NextBlock(BlockStreamBase*& block) {
  if (cur_block_ >= number_of_blocks_) {
    lock_.acquire();
    return false;
  }
  tSize bytes_num = hdfsPread(fs_, hdfs_fd_, cur_block_ * block_size_,
                              block_buffer_->getBlock(), CHUNK_SIZE);
  if (bytes_num == block_size_) {
    printf("cur block=%d\n", cur_block_);
    cur_block_++;
    block->constructFromBlock(*block_buffer_);
    lock_.release();
    return true;
  } else {
    cur_block_++;
    lock_.release();
    return false;
  }
}
/**
 * Generate the block_accessor and get information from block_accessor to
 * acquire blocks.
 */
bool InMemoryChunkReaderItetaor::GetNextBlockAccessor(block_accessor*& ba) {
  lock_.acquire();
  if (cur_block_ >= number_of_blocks_) {
    lock_.release();
    return false;
  }
  const unsigned cur_block = cur_block_;
  cur_block_++;
  lock_.release();
  ba = new InMemeryBlockAccessor();
  InMemeryBlockAccessor* imba = (InMemeryBlockAccessor*)ba;

  imba->SetBlockSize(block_size_);

  imba->SetTargetBlockStartAddress((char*)start_ + cur_block * block_size_);
  return true;
}

/**
 * Generate the block_accessor and get information from block_accessor to
 * acquire blocks.
 */
bool DiskChunkReaderIteraror::GetNextBlockAccessor(block_accessor*& ba) {
  lock_.acquire();
  if (cur_block_ >= number_of_blocks_) {
    lock_.release();
    return false;
  }
  const unsigned cur_block = cur_block_;
  cur_block_++;
  lock_.release();
  ba = new InDiskBlockAccessor();
  InDiskBlockAccessor* idba = (InDiskBlockAccessor*)ba;
  idba->SetBlockCur(cur_block);

  idba->SetBlockSize(block_size_);
  idba->SetChunkId(chunk_id_);
  idba->SetBlockSize(chunk_size_);

  return true;
}

/**
 * Generate the block_accessor and get information from block_accessor to
 * acquire blocks.
 */
bool HDFSChunkReaderIterator::GetNextBlockAccessor(block_accessor*& ba) {
  lock_.acquire();
  if (cur_block_ >= number_of_blocks_) {
    lock_.release();
    return false;
  }
  const unsigned cur_block = cur_block_;
  cur_block_++;
  lock_.release();
  ba = new InHDFSBlockAccessor();
  InHDFSBlockAccessor* ihba = (InHDFSBlockAccessor*)ba;
  ihba->SetBlockCur(cur_block);
  ihba->SetBlockSize(block_size_);
  ihba->SetChunkId(chunk_id_);
  ihba->SetBlockSize(chunk_size_);
  return true;
}

void ChunkReaderIterator::InMemeryBlockAccessor::GetBlock(

    BlockStreamBase*& block) const {
//#define MEMORY_COPY
#ifdef MEMORY_COPY

  /* Create a block, which will not free block_start_address when destructed.*/
  Block temp_block(block_size, target_block_start_address);

  block->constructFromBlock(temp_block);
#else

  /* According to my experiment, the performance can by improved by 2x by
   * avoiding the memory copy here */

  block->setIsReference(true);
  block->setBlock(target_block_start_address_);
  int tuple_count =
      *(unsigned*)((char*)target_block_start_address_ +
                   block->getSerializedBlockSize() - sizeof(unsigned));
  dynamic_cast<BlockStreamFix*>(block)->setTuplesInBlock(tuple_count);
//  ((BlockStreamFix*)block)->free_ =
//      (char*)block->getBlock() +
//      tuple_count * ((BlockStreamFix*)block)->tuple_size_;

#endif
}

void ChunkReaderIterator::InDiskBlockAccessor::GetBlock(
    BlockStreamBase*& block) const {
  printf("InDiskBlockAccessor::getBlock() is not implemented!\n");
  assert(false);
}

void ChunkReaderIterator::InHDFSBlockAccessor::GetBlock(

    BlockStreamBase*& block) const {
  printf("InHDFSBlockAccessor::getBlock() is not implemented!\n");
  assert(false);
}
