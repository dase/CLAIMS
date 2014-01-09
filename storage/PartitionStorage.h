/*
 * ProjectionStorage.h
 *
 *  Created on: Nov 14, 2013
 *      Author: wangli
 */

#ifndef PARTITIONSTORAGE_H_
#define PARTITIONSTORAGE_H_

#include "../ids.h"
#include "ChunkStorage.h"
#include "StorageLevel.h"
#include "../Block/synch.h"
#include "PartitionReaderIterator.h"


class PartitionStorage {
public:
	class PartitionReaderItetaor{
	public:
		PartitionReaderItetaor();
		PartitionReaderItetaor(PartitionStorage* partition_storage);
		virtual ~PartitionReaderItetaor();
		virtual ChunkReaderIterator* nextChunk();
	protected:
		PartitionStorage* ps;
		unsigned chunk_cur_;
	};
	class AtomicPartitionReaderIterator:public PartitionReaderItetaor{
	public:
		AtomicPartitionReaderIterator();
		AtomicPartitionReaderIterator(PartitionStorage* partition_storage):PartitionReaderItetaor(partition_storage){};
		virtual ~AtomicPartitionReaderIterator();
		ChunkReaderIterator* nextChunk();
	private:
		Lock lock_;
	};

	friend class PartitionReaderItetaor;
	PartitionStorage(const PartitionID &partition_id,const unsigned &number_of_chunks,const StorageLevel&);
	virtual ~PartitionStorage();
	void addNewChunk();
	PartitionReaderItetaor* createReaderIterator();
	PartitionReaderItetaor* createAtomicReaderIterator();
protected:
	PartitionID partition_id_;
	unsigned number_of_chunks_;
	std::vector<ChunkStorage*> chunk_list_;
	StorageLevel desirable_storage_level_;
};

#endif /* PARTITIONSTORAGE_H_ */
