/*
 * MemoryStore.cpp
 *
 *  Created on: 2013-10-11
 *      Author: casa
 */

#include <iostream>
#include <memory.h>
#include "MemoryStore.h"
#include "../configure.h"
#include "../Resource/BufferManager.h"
using namespace std;
MemoryChunkStore* MemoryChunkStore::instance_=0;
MemoryChunkStore::MemoryChunkStore():chunk_pool_(CHUNK_SIZE),block_pool_(BLOCK_SIZE){
//	cout<<"in the memorystroage initialize"<<endl;

}

MemoryChunkStore::~MemoryChunkStore() {
	chunk_pool_.purge_memory();
	block_pool_.purge_memory();
}
bool MemoryChunkStore::applyChunk(ChunkID chunk_id, void*& start_address){
	boost::unordered_map<ChunkID,HdfsInMemoryChunk>::const_iterator it=chunk_list_.find(chunk_id);
	if(it!=chunk_list_.cend()){
		printf("chunk id already exists (chunk id =%d)!\n",chunk_id.chunk_off);
		return false;
	}
	if(!BufferManager::getInstance()->applyStorageDedget(CHUNK_SIZE)){
		printf("not enough memory!!\n");
		return false;
	}
	if((start_address=chunk_pool_.malloc())!=0){
		chunk_list_[chunk_id]=HdfsInMemoryChunk(start_address,CHUNK_SIZE);
		return true;
	}
	else{
		printf("Error occurs when memalign!\n");
		return false;
	}
}

void MemoryChunkStore::returnChunk(const ChunkID& chunk_id){
	boost::unordered_map<ChunkID,HdfsInMemoryChunk>::const_iterator it=chunk_list_.find(chunk_id);
	if(it==chunk_list_.cend()){
		printf("return fail to find the target chunk id !\n");
		return;
	}
	HdfsInMemoryChunk chunk_info=it->second;

	chunk_pool_.free(chunk_info.hook);
	chunk_list_.erase(it);
	BufferManager::getInstance()->returnStorageBudget(chunk_info.length);
}

bool MemoryChunkStore::getChunk(const ChunkID& chunk_id,HdfsInMemoryChunk& chunk_info)const{
	boost::unordered_map<ChunkID,HdfsInMemoryChunk>::const_iterator it=chunk_list_.find(chunk_id);
	if(it!=chunk_list_.cend()){
		chunk_info=it->second;
		return true;
	}
	return false;
}
bool MemoryChunkStore::updateChunkInfo(const ChunkID & chunk_id, const HdfsInMemoryChunk & chunk_info){
	boost::unordered_map<ChunkID,HdfsInMemoryChunk>::iterator it=chunk_list_.find(chunk_id);
	if(it==chunk_list_.cend()){
		return false;
	}
	it->second=chunk_info;
	return true;


}

bool MemoryChunkStore::putChunk(const ChunkID& chunk_id,HdfsInMemoryChunk& chunk_info){
	boost::unordered_map<ChunkID,HdfsInMemoryChunk>::const_iterator it=chunk_list_.find(chunk_id);
	if(it!=chunk_list_.cend()){
		printf("The memory chunk is already existed!\n");
		return false;
	}
	chunk_list_[chunk_id]=chunk_info;
	return true;
}
MemoryChunkStore* MemoryChunkStore::getInstance(){
	if(instance_==0){
		instance_=new MemoryChunkStore();
	}
	return instance_;
}
