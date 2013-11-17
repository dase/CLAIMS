/*
 * ChunkStorage.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: wangli
 */
#include <hdfs.h>
#include "ChunkStorage.h"
#include "../Debug.h"
#include "BlockManager.h"
ChunkStorage::ChunkStorage(const ChunkID& chunk_id,const unsigned& block_size, StorageLevel desirable_level)
:chunk_id_(chunk_id),block_size_(block_size),desirable_storage_level_(desirable_level),current_storage_level_(HDFS),chunk_size_(CHUNK_SIZE){

}

ChunkStorage::~ChunkStorage() {
	// TODO Auto-generated destructor stub
}

ChunkReaderIterator* ChunkStorage::createChunkReaderIterator(){
	switch(current_storage_level_){
		case MEMORY:{
			HdfsInMemoryChunk chunk_info;
			BlockManager::getInstance()->getMemoryChunkStore()->getChunk(chunk_id_,chunk_info);
			return new InMemoryChunkReaderItetaor(chunk_info.hook,chunk_info.length,chunk_info.length/block_size_,block_size_);
			break;
		}
		case DISK:{
			break;
		}
		case HDFS:{
			if(desirable_storage_level_==MEMORY){
				HdfsInMemoryChunk chunk_info;
				if(BlockManager::getInstance()->getMemoryChunkStore()->applyChunk(chunk_id_,chunk_info)){
					/* there is enough memory storage space, so the storage level can be shifted.*/
					BlockManager::getInstance()->loadFromHdfs(chunk_id_,chunk_info.hook,chunk_info.length);
					current_storage_level_=MEMORY;
					return new InMemoryChunkReaderItetaor(chunk_info.hook,chunk_info.length,chunk_info.length/block_size_,block_size_);
				}
			}
			return new HDFSChunkReaderIterator(chunk_id_,chunk_size_,block_size_);
		}
	}
}

InMemoryChunkReaderItetaor::InMemoryChunkReaderItetaor(void* const &start,const unsigned& chunk_size,const unsigned & number_of_blocks,const unsigned& block_size)
:start_(start),chunk_size_(chunk_size),number_of_blocks_(number_of_blocks),block_cur_(0),block_size_(block_size){
}
bool InMemoryChunkReaderItetaor::nextBlock(BlockStreamBase* &block){
	if(block_cur_>=number_of_blocks_)
		return false;

	/* calculate the block start address.*/
	const char* block_start_address=(char*)start_+block_cur_*block_size_;

	/* Create a block, which will not free block_start_address when destructed.*/
	Block temp_block(block_size_,block_start_address);

	/*construct the block stream from temp_block. In the current version, the memory copy
	 * is used for simplicity.
	 * TODO: avoid memory copy.
	 */
	block->constructFromBlock(temp_block);

	block_cur_++;
	return true;
}
InMemoryChunkReaderItetaor::~InMemoryChunkReaderItetaor(){
}
HDFSChunkReaderIterator::HDFSChunkReaderIterator(const ChunkID& chunk_id, unsigned& chunk_size,const unsigned& block_size)
:chunk_id_(chunk_id),chunk_size_(chunk_size),block_size_(block_size),cur_block_(0){
	block_buffer_=new Block(block_size_);
	fs_=hdfsConnect(HDFS_N,9000);
	hdfs_fd_=hdfsOpenFile(fs_,chunk_id.partition_id.getName().c_str(),O_RDONLY,0,0,0);
	if(!hdfs_fd_){
		printf("fails to open HDFS file [%s]\n",chunk_id.partition_id.getName().c_str());
		number_of_blocks_=0;
	}


	const unsigned start_pos=start_pos+CHUNK_SIZE*chunk_id_.chunk_off;
	if(hdfsSeek(fs_,hdfs_fd_,start_pos)==-1){
		printf("fails to set the start offset %d for [%s]\n",start_pos,chunk_id.partition_id.getName().c_str());
		number_of_blocks_=0;
	}
	hdfsFileInfo *file_info=hdfsGetPathInfo(fs_,"/imdb/");// to be refined after communicating with Zhang Lei
	if(start_pos+CHUNK_SIZE<file_info->mSize){
		number_of_blocks_=CHUNK_SIZE/block_size_;
	}
	else{
		number_of_blocks_=(file_info->mSize-start_pos)/block_size_;
	}
	hdfsFreeFileInfo(file_info,1);

}
HDFSChunkReaderIterator::~HDFSChunkReaderIterator(){
	block_buffer_->~Block();
	hdfsCloseFile(fs_,hdfs_fd_);
	hdfsDisconnect(fs_);
}
bool HDFSChunkReaderIterator::nextBlock(BlockStreamBase*& block){
	if(cur_block_>=number_of_blocks_)
		return false;

	tSize bytes_num=hdfsPread(fs_,hdfs_fd_,cur_block_*block_size_,block_buffer_->getBlock(),CHUNK_SIZE);
	if(bytes_num==CHUNK_SIZE){
		cur_block_++;
		block->constructFromBlock(*block_buffer_);
		return true;
	}
	else{
		cur_block_++;
		return false;
	}

}
