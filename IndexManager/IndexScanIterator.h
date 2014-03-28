/*
 * IndexScanIterator.h
 *
 *  Created on: Mar 25, 2014
 *      Author: scdong
 */

#ifndef INDEXSCANITERATOR_H_
#define INDEXSCANITERATOR_H_
#include "../BlockStreamIterator/ExpandableBlockStreamIteratorBase.h"
#include "../Schema/Schema.h"
#include "../ids.h"
#include "../Block/BlockStream.h"
#include "CSBPlusTree.h"
#include "../storage/PartitionStorage.h"

class IndexScanIterator :public ExpandableBlockStreamIteratorBase {
public:
	struct remaining_block {
		remaining_block() : result_set(0), iter_result_set(0), block_off(0), block(0), iterator(0) {}
		remaining_block(vector<search_result*> result_set, vector<search_result*>::iterator current_tuple_off, unsigned short block_off, BlockStreamBase* block, BlockStreamBase::BlockStreamTraverseIterator* iterator)
		:result_set(result_set), iter_result_set(current_tuple_off), block_off(block_off), block(block), iterator(iterator) {}

		vector<search_result*> result_set;
		vector<search_result*>::iterator iter_result_set;
		unsigned short block_off;

		BlockStreamBase* block;
		BlockStreamBase::BlockStreamTraverseIterator* iterator;
	};
	class State {
		friend class IndexScanIterator;
	public:
		State() {}
		State(ProjectionID projection_id, Schema* schema, unsigned long index_id, void* value_low, void* value_high, unsigned block_size);

	public:
		ProjectionID projection_id_;
		Schema* schema_;
		PartitionStorage::PartitionReaderItetaor* partition_reader_iterator_;
		ChunkReaderIterator* chunk_reader_iterator_;

		unsigned long index_id_;
		void* value_low_;
		void* value_high_;
		unsigned block_size_;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & projection_id_ & schema_ & index_id_ & value_low_ & value_high_ & block_size_;
		}
	};

public:
	IndexScanIterator();
	IndexScanIterator(State state);
	virtual ~IndexScanIterator();
	bool open(const PartitionOffset& partition_off = 0);
	bool next(BlockStreamBase* block);
	bool close();
	void print()
	{
		printf("IndexScanIterator\n");
	}

private:
	State state_;
	PartitionStorage::PartitionReaderItetaor* partition_reader_iterator_;
	ChunkReaderIterator* chunk_reader_iterator_;
	std::map<ChunkID, void*> csb_index_list_;

	std::list<remaining_block> remaining_block_list_;
	std::list<BlockStreamBase*> block_stream_list_;

	Lock lock_;

	void atomicPushRemainingBlock(remaining_block rb);
	bool atomicPopRemainingBlock(remaining_block& rb);
	void AtomicPushBlockStream(BlockStreamBase* block);
	BlockStreamBase* AtomicPopBlockStream();

	bool askForNextBlock(remaining_block& rb);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<ExpandableBlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* INDEXSCANITERATOR_H_ */
