/*
 * ChunkStorage.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: wangli
 */
#include <hdfs.h>
#include "ChunkStorage.h"
#include "BlockManager.h"

#include "../Debug.h"
#include "../utility/warmup.h"
#include "../utility/rdtsc.h"
#include "../Config.h"


bool ChunkReaderIterator::nextBlock(){
	lock_.acquire();
	if(this->cur_block_>=this->number_of_blocks_){
		lock_.release();
		return false;
	}
	cur_block_++;
	lock_.release();
	return true;
}
ChunkStorage::ChunkStorage(const ChunkID& chunk_id,const unsigned& block_size,const StorageLevel& desirable_level)
:chunk_id_(chunk_id),block_size_(block_size),desirable_storage_level_(desirable_level),current_storage_level_(HDFS),chunk_size_(CHUNK_SIZE){
//printf("CHUNKSTORAGE****:level=%d\n",desirable_storage_level_);
}

ChunkStorage::~ChunkStorage() {
	// TODO Auto-generated destructor stub
}

ChunkReaderIterator* ChunkStorage::createChunkReaderIterator(){
//	printf("level value:%d\n",current_storage_level_);
	ChunkReaderIterator* ret;
	lock_.acquire();
	switch(current_storage_level_){
		case MEMORY:{
//			printf("current storage level: MEMORY\n");
			HdfsInMemoryChunk chunk_info;
			if(BlockManager::getInstance()->getMemoryChunkStore()->getChunk(chunk_id_,chunk_info))
				ret =new InMemoryChunkReaderItetaor(chunk_info.hook,chunk_info.length,chunk_info.length/block_size_,block_size_,chunk_id_);
			else
				ret =0;
			break;
		}
		case DISK: {
			printf("Currently, current storage level should not be DISK~! -_-\n");
			assert(false);
			break;
		}
		case HDFS: {
//			printf("%lx current storage level for %d %d: HDFS\n",this,this->chunk_id_.partition_id.partition_off,this->chunk_id_.chunk_off);
			if(desirable_storage_level_==MEMORY){
				HdfsInMemoryChunk chunk_info;
				chunk_info.length=CHUNK_SIZE;
				if(BlockManager::getInstance()->getMemoryChunkStore()->applyChunk(chunk_id_,chunk_info.hook)){
					/* there is enough memory storage space, so the storage level can be shifted.*/
					if(Config::local_disk_mode) {
						chunk_info.length = BlockManager::getInstance()->loadFromDisk(chunk_id_, chunk_info.hook, chunk_info.length);
					}
					else {
						chunk_info.length = BlockManager::getInstance()->loadFromHdfs(chunk_id_,chunk_info.hook,chunk_info.length);
					}
					if(chunk_info.length<=0){
						/*chunk_info.length<=0 means that either the file does not exist or
						 * the current chunk_id exceeds the actual size of the file.						 *
						 */
						BlockManager::getInstance()->getMemoryChunkStore()->returnChunk(chunk_id_);
						ret=0;
						break;
//						return 0;
					}
//					BlockManager::getInstance()->getMemoryChunkStore()->putChunk(chunk_id_,chunk_info);
					current_storage_level_=MEMORY;

					/* update the chunk info in the Chunk store in case that the chunk_info is updated.*/
					BlockManager::getInstance()->getMemoryChunkStore()->updateChunkInfo(chunk_id_,chunk_info);
//					printf("%lx current is set to memory!\n");
					ret = new InMemoryChunkReaderItetaor(chunk_info.hook,chunk_info.length,chunk_info.length/block_size_,block_size_,chunk_id_);
					break;
				}
				else{
					/*
					 * The storage memory is full, some swap algorithm is needed here.
					 * TODO: swap algorithm.
					 */
					printf("Failed to get memory chunk budege!\n");
					assert(false);
				}
			}
//			return new HDFSChunkReaderIterator(chunk_id_,chunk_size_,block_size_);
			ret=new DiskChunkReaderIteraror(chunk_id_,chunk_size_,block_size_);
			break;
		}
		default:{
			printf("current storage level: unknown!\n");
		}
	}
	lock_.release();
	return ret;
}
std::string ChunkStorage::printCurrentStorageLevel()const{
	return "";
}
InMemoryChunkReaderItetaor::InMemoryChunkReaderItetaor(void* const &start,const unsigned& chunk_size,const unsigned & number_of_blocks,const unsigned& block_size,const ChunkID& chunk_id)
:start_(start),ChunkReaderIterator(chunk_id,block_size,chunk_size,number_of_blocks){
}
bool InMemoryChunkReaderItetaor::nextBlock(BlockStreamBase* &block){
	lock_.acquire();
	if(cur_block_>=number_of_blocks_){
		lock_.release();
		return false;
	}
	cur_block_++;
	lock_.release();
//	printf("Read Block:%d:%d\n",chunk_id_.chunk_off,cur_block_);
	/* calculate the block start address.*/
	const char* block_start_address=(char*)start_+cur_block_*block_size_;

	/* Create a block, which will not free block_start_address when destructed.*/
	Block temp_block(block_size_,block_start_address);

	/*construct the block stream from temp_block. In the current version, the memory copy
	 * is used for simplicity.
	 * TODO: avoid memory copy.
	 */
	block->constructFromBlock(temp_block);

	return true;
}
InMemoryChunkReaderItetaor::~InMemoryChunkReaderItetaor(){
}

DiskChunkReaderIteraror::DiskChunkReaderIteraror(const ChunkID& chunk_id,unsigned& chunk_size,const unsigned& block_size)
:ChunkReaderIterator(chunk_id,block_size,chunk_size){
	block_buffer_=new Block(block_size_);
	fd_=FileOpen(chunk_id_.partition_id.getPathAndName().c_str(),O_RDONLY);
	if(fd_==-1){
		printf("Failed to open file [%s], reason:%s\n",chunk_id_.partition_id.getPathAndName().c_str(),strerror(errno));
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
//			sleep(1);
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
	lock_.acquire();
	if(cur_block_>=number_of_blocks_){
		lock_.release();
		return false;
	}
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
		lock_.release();
		return true;
	}
	else{
		cur_block_++;
		printf("failed to read one block, only %d bytes are read!,error=%s\n",bytes_num,strerror(errno));
		lock_.release();
		return false;
	}

}
HDFSChunkReaderIterator::HDFSChunkReaderIterator(const ChunkID& chunk_id, unsigned& chunk_size,const unsigned& block_size)
:ChunkReaderIterator(chunk_id,block_size,chunk_size){
	block_buffer_=new Block(block_size_);
	fs_=hdfsConnect(Config::hdfs_master_ip.c_str(),Config::hdfs_master_port);
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
	if(cur_block_>=number_of_blocks_){
		lock_.acquire();
		return false;
	}
	tSize bytes_num=hdfsPread(fs_,hdfs_fd_,cur_block_*block_size_,block_buffer_->getBlock(),CHUNK_SIZE);
	if(bytes_num==block_size_){
		printf("cur block=%d\n",cur_block_);
		cur_block_++;
		block->constructFromBlock(*block_buffer_);
		lock_.release();
		return true;
	}
	else{
		cur_block_++;
		lock_.release();
		return false;
	}

}

//bool InMemoryChunkReaderItetaor::getNextBlockAccessor(block_accessor & ba) {
//	if(cur_block_>=number_of_blocks_){
//		lock_.release();
//		return false;
//	}
//	ba.target_block_start_address=(char*)start_+cur_block_*block_size_;
//	ba.block_size=block_size_;
//	return true;
//
//}
//
//void ChunkReaderIterator::getBlock(const block_accessor& ba) const {
//}

bool InMemoryChunkReaderItetaor::getNextBlockAccessor(block_accessor*& ba) {
	lock_.acquire();
	if(cur_block_>=number_of_blocks_){
		lock_.release();
		return false;
	}
	const unsigned cur_block=cur_block_;
	cur_block_++;
	lock_.release();
	ba=new InMemeryBlockAccessor();
	InMemeryBlockAccessor* imba=(InMemeryBlockAccessor*)ba;
	imba->setBlockSize(block_size_);
	imba->setTargetBlockStartAddress((char*)start_+cur_block*block_size_);
	return true;
}

bool DiskChunkReaderIteraror::getNextBlockAccessor(block_accessor*& ba) {
	lock_.acquire();
	if(cur_block_>=number_of_blocks_){
		lock_.release();
		return false;
	}
	const unsigned cur_block=cur_block_;
	cur_block_++;
	lock_.release();
	ba=new InDiskBlockAccessor();
	InDiskBlockAccessor* idba=(InDiskBlockAccessor*)ba;
	idba->setBlockCur(cur_block);
	idba->setBlockSize(block_size_);
	idba->setChunkId(chunk_id_);
	idba->setBlockSize(chunk_size_);
	return true;
}

bool HDFSChunkReaderIterator::getNextBlockAccessor(block_accessor*& ba) {
	lock_.acquire();
	if(cur_block_>=number_of_blocks_){
		lock_.release();
		return false;
	}
	const unsigned cur_block=cur_block_;
	cur_block_++;
	lock_.release();
	ba=new InHDFSBlockAccessor();
	InHDFSBlockAccessor* ihba=(InHDFSBlockAccessor*)ba;
	ihba->setBlockCur(cur_block);
	ihba->setBlockSize(block_size_);
	ihba->setChunkId(chunk_id_);
	ihba->setBlockSize(chunk_size_);
	return true;
}




void ChunkReaderIterator::InMemeryBlockAccessor::getBlock(BlockStreamBase*& block) const {

//#define MEMORY_COPY
#ifdef MEMORY_COPY

	/* Create a block, which will not free block_start_address when destructed.*/
	Block temp_block(block_size,target_block_start_address);

	block->constructFromBlock(temp_block);
#else

	/* According to my experiment, the performance can by improved by 2x by aoviding the memocy copy here */

	block->setIsReference(true);
	block->setBlock(target_block_start_address);
	int tuple_count=*(unsigned*)((char*)target_block_start_address+block->getSerializedBlockSize()-sizeof(unsigned));
	((BlockStreamFix*)block)->free_=(char*)block->getBlock()+tuple_count*((BlockStreamFix*)block)->tuple_size_;

#endif

}

void ChunkReaderIterator::InDiskBlockAccessor::getBlock(BlockStreamBase*& block) const {
	printf("InDiskBlockAccessor::getBlock() is not implemented!\n");
	assert(false);
}

void ChunkReaderIterator::InHDFSBlockAccessor::getBlock(BlockStreamBase*& block) const {
	printf("InHDFSBlockAccessor::getBlock() is not implemented!\n");
	assert(false);
}

