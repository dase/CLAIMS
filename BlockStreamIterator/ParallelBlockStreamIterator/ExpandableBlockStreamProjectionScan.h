/*
 * ExpandableBlockStreamSingleColumnScan.h
 * In the current implementation, for simplicity, we assume that the underlying storage
 * is arranged in blocks, each of which is the same as the size of the
 * block in the parameter of the next function.
 * TODO: adjust or rewrite this iterator if the consumption above does not hold.
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMPROJECTIONSCAN_H_
#define EXPANDABLEBLOCKSTREAMPROJECTIONSCAN_H_
#include <string>
#include <list>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "../BlockStreamIteratorBase.h"
#include "../../Schema/Schema.h"
#include "../../Block/synch.h"
#include "../../ids.h"
#include "../../storage/ChunkStorage.h"
#include "../../storage/PartitionStorage.h"
class ExpandableBlockStreamProjectionScan:public BlockStreamIteratorBase {
public:
	struct allocated_block{
		char* start;
		unsigned length;
	};
	class State{
	friend class ExpandableBlockStreamProjectionScan;
	public:
		State(ProjectionID projection_id,Schema* schema,unsigned block_size);
		State(){};
	public:
		Schema* schema_;
		ProjectionID projection_id_;
		unsigned block_size_;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & projection_id_ & block_size_;
		}
	};
	ExpandableBlockStreamProjectionScan(State state);
	ExpandableBlockStreamProjectionScan();
	virtual ~ExpandableBlockStreamProjectionScan();
	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* block);
	bool close();
private:

	void atomicPushChunkReaderIterator(ChunkReaderIterator*);
	bool atomicPopChunkReaderIterator(ChunkReaderIterator*&);
private:

	State state_;

	/* the semaphore used to guarantee that only one thread does the real work
	 * in the open function the open function
	 */
	semaphore sema_open_;

	volatile bool open_finished_;
	semaphore sema_open_finished_;

	Lock cursor_lock_;

	PartitionStorage::PartitionReaderItetaor* partition_reader_iterator_;
	std::list<ChunkReaderIterator*> remaining_chunk_reader_iterator_list_;
	Lock chunk_reader_container_lock_;
	/*
	 * The following code is for boost serialization.
	 */
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}

};

#endif /* EXPANDABLEBLOCKSTREAMSINGLECOLUMNSCAN_H_ */
