/*
 * ProjectionStorage.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: wangli
 */

#include "PartitionStorage.h"
#include "../Debug.h"
PartitionStorage::PartitionStorage(const PartitionID &partition_id,const unsigned &number_of_chunks,const StorageLevel& storage_level)
:partition_id_(partition_id),number_of_chunks_(number_of_chunks),desirable_storage_level_(storage_level){
	for(unsigned i=0;i<number_of_chunks_;i++){
		chunk_list_.push_back(new ChunkStorage(ChunkID(partition_id_,i),BLOCK_SIZE,desirable_storage_level_));
	}
}

PartitionStorage::~PartitionStorage() {
	for(unsigned i=0;i<chunk_list_.size();i++){
		chunk_list_[i]->~ChunkStorage();
	}
	chunk_list_.clear();
}

void PartitionStorage::addNewChunk(){
	number_of_chunks_++;
}
PartitionStorage::PartitionReaderItetaor* PartitionStorage::createReaderIterator()const{
	return new PartitionReaderItetaor();
}
PartitionStorage::PartitionReaderItetaor* PartitionStorage::createAtomicReaderIterator()const{
	return new AtomicPartitionReaderIterator();
}



PartitionStorage::PartitionReaderItetaor::PartitionReaderItetaor(PartitionStorage::PartitionStorage* partition_storage)
:ps(partition_storage),chunk_cur_(0){

}
PartitionStorage::PartitionReaderItetaor::PartitionReaderItetaor():chunk_cur_(0){

}
PartitionStorage::PartitionReaderItetaor::~PartitionReaderItetaor(){

}
ChunkReaderIterator* PartitionStorage::PartitionReaderItetaor::nextChunk(){
	if(chunk_cur_<ps->number_of_chunks_)
		return ps->chunk_list_[chunk_cur_++]->createChunkReaderIterator();
	else
		return 0;
}
PartitionStorage::AtomicPartitionReaderIterator::AtomicPartitionReaderIterator():PartitionReaderItetaor(){

}
PartitionStorage::AtomicPartitionReaderIterator::~AtomicPartitionReaderIterator(){

}
ChunkReaderIterator* PartitionStorage::AtomicPartitionReaderIterator::nextChunk(){
	lock_.acquire();
	ChunkReaderIterator* ret;
	if(chunk_cur_<ps->number_of_chunks_)
		ret= ps->chunk_list_[chunk_cur_++]->createChunkReaderIterator();
	else
		ret= 0;
	lock_.release();
	return ret;
}
