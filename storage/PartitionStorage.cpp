/*
 * ProjectionStorage.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: wangli
 */

#include "PartitionStorage.h"
#include "../Debug.h"
#include "MemoryStore.h"
#include "../Config.h"
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
}//确定chunk的具体信息。

PartitionStorage::~PartitionStorage() {
  for (unsigned i = 0; i < chunk_list_.size(); i++) {
    chunk_list_[i]->~ChunkStorage();
  }
  chunk_list_.clear();
}

void PartitionStorage::AddNewChunk() { number_of_chunks_++; }

void PartitionStorage::UpdateChunksWithInsertOrAppend(
    const PartitionID& partition_id, const unsigned& number_of_chunks,
    const StorageLevel& storage_level) {
  if (!chunk_list_.empty()) {
    MemoryChunkStore::getInstance()->returnChunk(
        chunk_list_.back()->getChunkID());
    //		if (Config::local_disk_mode == 0)
    chunk_list_.back()->setCurrentStorageLevel(HDFS);
    //		else
    //			chunk_list_.back()->setCurrentStorageLevel(DISK);
  }
  for (unsigned i = number_of_chunks_; i < number_of_chunks; i++)
    chunk_list_.push_back(
        new ChunkStorage(ChunkID(partition_id, i), BLOCK_SIZE, storage_level));
  number_of_chunks_ = number_of_chunks;
}

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

#ifdef storage
PartitionStorage::PartitionReaderIterator::PartitionReaderIterator(
    PartitionStorage* partition_storage)
    : ps(partition_storage), chunk_cur_(0), chunk_it_(0) {}

// PartitionStorage::PartitionReaderItetaor::PartitionReaderItetaor():chunk_cur_(0){
//
//}
PartitionStorage::PartitionReaderIterator::~PartitionReaderIterator() {}
ChunkReaderIterator* PartitionStorage::PartitionReaderIterator::nextChunk() {
  if (chunk_cur_ < ps->number_of_chunks_)
    return ps->chunk_list_[chunk_cur_++]->createChunkReaderIterator();
  else
    return 0;
}
// PartitionStorage::AtomicPartitionReaderIterator::AtomicPartitionReaderIterator():PartitionReaderItetaor(){
//
//}
PartitionStorage::AtomicPartitionReaderIterator::
    ~AtomicPartitionReaderIterator() {}
ChunkReaderIterator*
PartitionStorage::AtomicPartitionReaderIterator::nextChunk() {
  //	lock_.acquire();
  ChunkReaderIterator* ret;
  if (chunk_cur_ < ps->number_of_chunks_)
    ret = ps->chunk_list_[chunk_cur_++]->createChunkReaderIterator();
  else
    ret = 0;
  //	lock_.release();
  return ret;
}

bool PartitionStorage::PartitionReaderIterator::NextBlock(
    BlockStreamBase*& block) {
  assert(false);
  if (chunk_it_ > 0 && chunk_it_->NextBlock(block)) {
    return true;
  } else {
    if ((chunk_it_ = nextChunk()) > 0) {
      return NextBlock(block);
    } else {
      return false;
    }
  }
}

bool PartitionStorage::AtomicPartitionReaderIterator::NextBlock(
    BlockStreamBase*& block) {
  ////	lock_.acquire();
  //	if(chunk_it_>0&&chunk_it_->nextBlock(block)){
  ////		lock_.release();
  //		return true;
  //	}
  //	else{
  //		lock_.acquire();
  //		if((chunk_it_=nextChunk())>0){
  //			lock_.release();
  //			return nextBlock(block);
  //		}
  //		else{
  //			lock_.release();
  //			return false;
  //		}
  //	}
  //	lock_.acquire();

  lock_.acquire();
  ChunkReaderIterator::block_accessor* ba;
  if (chunk_it_ != 0 && chunk_it_->getNextBlockAccessor(ba)) {
    lock_.release();
    ba->GetBlock(block);
    return true;
  } else {
    if ((chunk_it_ = PartitionReaderIterator::nextChunk()) > 0) {
      lock_.release();
      return NextBlock(block);
    } else {
      lock_.release();
      return false;
    }
  }
}
#endif
