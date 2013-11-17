/*
 * MemoryStore.cpp
 *
 *  Created on: 2013-10-11
 *      Author: casa
 */

#include <iostream>
#include <memory.h>
#include "MemoryStore.h"
#include "../BufferManager/BufferManager.h"
#include "../configure.h"
using namespace std;
MemoryChunkStore* MemoryChunkStore::instance_=0;
MemoryChunkStore::MemoryChunkStore(){
	cout<<"in the memorystroage initialize"<<endl;
}

MemoryChunkStore::~MemoryChunkStore() {

}
bool MemoryChunkStore::applyChunk(ChunkID chunk_id, HdfsInMemoryChunk& chunk_info){
	boost::unordered_map<ChunkID,HdfsInMemoryChunk>::const_iterator it=chunk_list_.find(chunk_id);
	if(it!=chunk_list_.cend()){
		return false;
	}
	if(!BufferManager::getInstance()->applyStorageDedget(chunk_info.length)){
		return false;
	}
	chunk_info.hook=memalign(cacheline_size,chunk_info.length);
	chunk_list_[chunk_id]=chunk_info;
	return true;
}

bool MemoryChunkStore::getChunk(const ChunkID& chunk_id,HdfsInMemoryChunk& chunk_info)const{
	boost::unordered_map<ChunkID,HdfsInMemoryChunk>::const_iterator it=chunk_list_.find(chunk_id);
	if(it!=chunk_list_.cend()){
		chunk_info=it->second;
		return true;
	}
	return false;
}
MemoryChunkStore* MemoryChunkStore::getInstance(){
	if(instance_==0){
		instance_=new MemoryChunkStore();
	}
	return instance_;
}
