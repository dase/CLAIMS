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
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../BlockStreamIteratorBase.h"
#include "../../common/Schema/Schema.h"
#include "../../storage/ChunkStorage.h"
#include "../../storage/PartitionStorage.h"
#include "../ExpandableBlockStreamIteratorBase.h"
#include "../../common/ExpandedThreadTracker.h"


typedef std::list<ChunkReaderIterator::block_accessor*> assigned_data;
struct input_dataset{
	assigned_data input_data_blocks;
	SpineLock lock;
	bool atomicGet(assigned_data &target,unsigned number_of_block){
		lock.acquire();
		while(number_of_block--&&(!input_data_blocks.empty())){
			target.push_back(input_data_blocks.front());
			input_data_blocks.pop_front();
		}
		lock.release();
		return !target.empty();
	}
	void atomicPut(assigned_data blocks){
		lock.acquire();
		for(assigned_data::iterator it=blocks.begin();it!=blocks.end();it++)
			input_data_blocks.push_front(*it);
		lock.release();
	}
};


class ExpandableBlockStreamProjectionScan:public ExpandableBlockStreamIteratorBase {
public:

	class scan_thread_context:public thread_context{
	public:
		~scan_thread_context(){};
		assigned_data assigned_data_;
	};

	struct allocated_block{
		char* start;
		unsigned length;
	};
	class State{
	friend class ExpandableBlockStreamProjectionScan;
	public:
		State(ProjectionID projection_id,Schema* schema,unsigned block_size,float sample_rate=1);
		State(){};
	public:
		Schema* schema_;
		ProjectionID projection_id_;
		unsigned block_size_;
		float sample_rate_;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & projection_id_ & block_size_&sample_rate_;
		}
	};
	ExpandableBlockStreamProjectionScan(State state);
	ExpandableBlockStreamProjectionScan();
	virtual ~ExpandableBlockStreamProjectionScan();
	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* block);
	bool close();
	void print();
private:

	void atomicPushChunkReaderIterator(ChunkReaderIterator*);
	bool atomicPopChunkReaderIterator(ChunkReaderIterator*&);
	bool passSample()const;
private:

	State state_;

//	/* the semaphore used to guarantee that only one thread does the real work
//	 * in the open function the open function
//	 */
//	semaphore sema_open_;
//
//	volatile bool open_finished_;
//	semaphore sema_open_finished_;

//	Lock cursor_lock_;

	PartitionStorage::PartitionReaderItetaor* partition_reader_iterator_;
	std::list<ChunkReaderIterator*> remaining_chunk_reader_iterator_list_;
	Lock chunk_reader_container_lock_;

	input_dataset input_dataset_;

	/* for debug*/
	unsigned long int return_blocks_;
	Lock lock_;

	PerformanceInfo* perf_info;

	/*
	 * The following code is for boost serialization.
	 */
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<ExpandableBlockStreamIteratorBase>(*this) & state_;
	}

};

#endif /* EXPANDABLEBLOCKSTREAMSINGLECOLUMNSCAN_H_ */
