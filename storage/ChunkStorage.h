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
#include "../Block/BlockStream.h"
#include "../Block/synch.h"
#include "StorageLevel.h"
#include "../ids.h"
class ChunkReaderIterator{
public:
	ChunkReaderIterator(const ChunkID& chunk_id):chunk_id_(chunk_id){};
	virtual bool nextBlock(BlockStreamBase* & block)=0;
	virtual ~ChunkReaderIterator(){};
public:
	ChunkID chunk_id_;
};
class InMemoryChunkReaderItetaor:public ChunkReaderIterator{
public:
	InMemoryChunkReaderItetaor(void* const &start,const unsigned& chunk_size,const unsigned & number_of_blocks,const unsigned& block_size,const ChunkID& chunk_id);
	virtual ~InMemoryChunkReaderItetaor();
	bool nextBlock(BlockStreamBase*& block);
private:
	void* start_;
	unsigned chunk_size_;
	unsigned number_of_blocks_;
	unsigned block_size_;
	unsigned block_cur_;

	Lock lock_;

};

class DiskChunkReaderIteraror:public ChunkReaderIterator{
public:
	DiskChunkReaderIteraror(const ChunkID& chunk_id,unsigned& chunk_size,const unsigned& block_size);
	virtual ~DiskChunkReaderIteraror();
	bool nextBlock(BlockStreamBase*& block);
private:

	unsigned chunk_size_;
	unsigned number_of_blocks_;
	unsigned block_size_;
	unsigned cur_block_;
	/*the iterator creates a buffer and allocates its memory such that the query processing
	 * can just use the Block without the concern the memory allocation and deallocation.
	 */
	Block* block_buffer_;
	int fd_;
	Lock lock_;
};

class HDFSChunkReaderIterator:public ChunkReaderIterator{
public:
	HDFSChunkReaderIterator(const ChunkID& chunk_id, unsigned& chunk_size,const unsigned& block_size);
	virtual ~HDFSChunkReaderIterator();
	bool nextBlock(BlockStreamBase*& block);
private:

	unsigned chunk_size_;
	unsigned number_of_blocks_;
	unsigned block_size_;
	unsigned cur_block_;
	/*the iterator creates a buffer and allocates its memory such that the query processing
	 * can just use the Block without the concern the memory allocation and deallocation.
	 */
	Block* block_buffer_;
	hdfsFS fs_;
	hdfsFile hdfs_fd_;
	Lock lock_;
};

class ChunkStorage {
public:


	/* considering that how block size effects the performance is to be tested, here we leave
	 * a parameter block_size for the performance test concern.
	 */
	ChunkStorage(const ChunkID& chunk_id,const unsigned& block_size, const StorageLevel& desirable_level);
	virtual ~ChunkStorage();
	ChunkReaderIterator* createChunkReaderIterator();
	std::string printCurrentStorageLevel()const;
private:
	unsigned block_size_;
	unsigned chunk_size_;
	StorageLevel desirable_storage_level_;
	StorageLevel current_storage_level_;
	ChunkID chunk_id_;


};



#endif /* CHUNKSTORAGE_H_ */
