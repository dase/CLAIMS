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
ChunkStorage::ChunkStorage(const ChunkID& chunk_id,const unsigned& block_size,const StorageLevel& desirable_level)
:chunk_id_(chunk_id),block_size_(block_size),desirable_storage_level_(desirable_level),current_storage_level_(HDFS),chunk_size_(CHUNK_SIZE){
//printf("CHUNKSTORAGE****:level=%d\n",desirable_storage_level_);
}

ChunkStorage::~ChunkStorage() {
	// TODO Auto-generated destructor stub
}

ChunkReaderIterator* ChunkStorage::createChunkReaderIterator(){
	printf("level value:%d\n",current_storage_level_);
	switch(current_storage_level_){
		case MEMORY:{
			printf("current storage level: MEMORY\n");
			HdfsInMemoryChunk chunk_info;
			BlockManager::getInstance()->getMemoryChunkStore()->getChunk(chunk_id_,chunk_info);
			return new InMemoryChunkReaderItetaor(chunk_info.hook,chunk_info.length,chunk_info.length/block_size_,block_size_);
			break;
		}
		case DISK:{
			break;
		}
		case HDFS:{
			printf("current storage level: HDFS\n");
			if(desirable_storage_level_==MEMORY){
				HdfsInMemoryChunk chunk_info;
				chunk_info.length=CHUNK_SIZE;
				if(BlockManager::getInstance()->getMemoryChunkStore()->applyChunk(chunk_id_,chunk_info)){
					/* there is enough memory storage space, so the storage level can be shifted.*/
					BlockManager::getInstance()->loadFromDisk(chunk_id_,chunk_info.hook,chunk_info.length);
//					BlockManager::getInstance()->getMemoryChunkStore()->putChunk(chunk_id_,chunk_info);
					current_storage_level_=MEMORY;
					return new InMemoryChunkReaderItetaor(chunk_info.hook,chunk_info.length,chunk_info.length/block_size_,block_size_);
				}
				else{
					/*
					 * The storage memory is full, some swap algorithm is needed here.
					 * TODO: swap algorithm.
					 */
					printf("Failed to get memory chunk budege!\n");
				}
			}
//			return new HDFSChunkReaderIterator(chunk_id_,chunk_size_,block_size_);
			return new DiskChunkReaderIteraror(chunk_id_,chunk_size_,block_size_);
		}
		default:{
			printf("current storage level: unknown!\n");
		}
	}
}
std::string ChunkStorage::printCurrentStorageLevel()const{
	return "";
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

DiskChunkReaderIteraror::DiskChunkReaderIteraror(const ChunkID& chunk_id,unsigned& chunk_size,const unsigned& block_size)
:chunk_id_(chunk_id),chunk_size_(chunk_size),block_size_(block_size),cur_block_(0){
	block_buffer_=new Block(block_size_);
	fd_=FileOpen(chunk_id_.partition_id.getPathAndName().c_str(),O_RDONLY);
	if(fd_==-1){
		printf("Failed to open file [%c], reason:%s\n",chunk_id_.partition_id.getPathAndName().c_str(),strerror(errno));
		number_of_blocks_=0;
	}
	else{
		const unsigned start_pos=CHUNK_SIZE*chunk_id_.chunk_off;
		const unsigned long length=lseek(fd_,0,SEEK_END);

		if(length<=start_pos){
			printf("fails to set the start offset %d for [%s]\n",start_pos,chunk_id.partition_id.getName().c_str());
			number_of_blocks_=0;
		}
		else{
			const unsigned offset=lseek(fd_,start_pos,SEEK_SET);
			printf("The file is set to be %d\n",offset);
			sleep(1);
			if(start_pos+CHUNK_SIZE<length){
				number_of_blocks_=CHUNK_SIZE/block_size_;
			}
			else{
				number_of_blocks_=(length-start_pos)/block_size_;
				printf("This chunk has only %d blocks!\n",number_of_blocks_);
			}

		}
	}
}
DiskChunkReaderIteraror::~DiskChunkReaderIteraror(){
	block_buffer_->~Block();
	FileClose(fd_);
}
bool DiskChunkReaderIteraror::nextBlock(BlockStreamBase*& block){
	if(cur_block_>=number_of_blocks_)
		return false;
	const unsigned posistion=lseek(fd_,0,SEEK_CUR);
//	printf("***** the data is read from position:[ %d MB %d KB ]*******\n",posistion/1024/1024,(posistion/1024)%1024);
//	sleep(1);
	/*
	 * the read function will automatically move the read position, so the lseek is not needed here.
	 */
	tSize bytes_num=read(fd_,block_buffer_->getBlock(),block_buffer_->getsize());
//	printf("Tuple count=%d\n",*(int*)((char*)block_buffer_->getBlock()+65532));
	if(bytes_num==block_size_){
		cur_block_++;
//		lseek(fd_,64*1024,SEEK_CUR);
		block->constructFromBlock(*block_buffer_);
		return true;
	}
	else{
		cur_block_++;
		printf("failed to read one block, only %d bytes are read!,error=%s\n",bytes_num,strerror(errno));
		return false;
	}

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
	if(bytes_num==block_size_){
		cur_block_++;
		block->constructFromBlock(*block_buffer_);
		return true;
	}
	else{
		cur_block_++;
		return false;
	}

}
