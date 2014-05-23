/*
 * CSBIndexBuilding.h
 *
 *  Created on: Mar 17, 2014
 *      Author: scdong
 */

#ifndef CSBINDEXBUILDING_H_
#define CSBINDEXBUILDING_H_
#include <boost/serialization/base_object.hpp>
#include <map>
#include "../BlockStreamIterator/ExpandableBlockStreamIteratorBase.h"
#include "../common/ids.h"
#include "../common/Schema/Schema.h"
#include "../storage/PartitionStorage.h"
#include "../Block/BlockStream.h"
#include "../storage/ChunkStorage.h"
#include "../common/data_type.h"
#include "CSBPlusTree.h"

template<typename T>
CSBPlusTree<T>* indexBuilding(Schema* schema, vector<void*> chunk_tuples);

class bottomLayerCollecting :public ExpandableBlockStreamIteratorBase {
public:
	struct remaining_block {
		remaining_block() : block(0), iterator(0), chunk_offset(0), block_offset(0), tuple_offset(0) {}
		remaining_block(BlockStreamBase* block, BlockStreamBase::BlockStreamTraverseIterator* iterator, unsigned short chunk_offset, unsigned short block_offset, unsigned short tuple_offset)
		:block(block), iterator(iterator), chunk_offset(chunk_offset), block_offset(block_offset), tuple_offset(tuple_offset) {}
		BlockStreamBase::BlockStreamTraverseIterator* iterator;
		BlockStreamBase* block;
		ChunkOffset chunk_offset;
		unsigned short block_offset;
		unsigned short tuple_offset;
	};
	class State {
		friend class bottomLayerCollecting;
	public:
		State() {}
		State(ProjectionID projection_id, Schema* schema, unsigned key_indexing, unsigned block_size);
	public:
		Schema* schema_;
		ProjectionID projection_id_;
		unsigned key_indexing_;
		unsigned block_size_;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & projection_id_ & block_size_& key_indexing_;
		}
	};

public:
	bottomLayerCollecting();
	bottomLayerCollecting(State state);
	virtual ~bottomLayerCollecting();
	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* block);
	bool close();
	void print(){
		printf("CCSBIndexingBuilding\n");
	}

private:
	State state_;
	PartitionStorage::PartitionReaderItetaor* partition_reader_iterator_;
	ChunkReaderIterator* chunk_reader_iterator_;
	std::list<remaining_block> remaining_block_list_;
	std::list<BlockStreamBase*> block_stream_list_;

	Schema* output_schema_;

	ChunkOffset chunk_offset_;
	unsigned short block_offset_;
//	unsigned short tuple_offset_;

	Lock lock_;

	void atomicPushRemainingBlock(remaining_block rb);
	bool atomicPopRemainingBlock(remaining_block& rb);
	void AtomicPushBlockStream(BlockStreamBase* block);
	BlockStreamBase* AtomicPopBlockStream();

	bool askForNextBlock(BlockStreamBase* & block, remaining_block& rb);

	void computeOutputSchema();
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<ExpandableBlockStreamIteratorBase>(*this) & state_;
	}
};




class bottomLayerSorting :public ExpandableBlockStreamIteratorBase {
public:
	class State {
		friend class bottomLayerSorting;
	public:
		State() {}
		State(Schema* schema, BlockStreamIteratorBase* child, const unsigned block_size, ProjectionID projection_id, unsigned key_indexing, std::string index_name);
	public:
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
		//The following two variable is for registering to the IndexManager
		//similar to its child's projection id and key indexing
		ProjectionID projection_id_;
		unsigned key_indexing_;
		std::string index_name_;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned version) {
			ar & schema_ & child_ & block_size_ & projection_id_ & key_indexing_ & index_name_;
		}
	};

public:
	typedef struct compare_node {
		Schema* vector_schema_;
		void* tuple_;
		Operate* op_;
	} compare_node;

	bottomLayerSorting();
	bottomLayerSorting(State state);
	virtual ~bottomLayerSorting();

	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* block);
	bool close();

private:
	static bool compare(const compare_node* a, const compare_node* b);

	template<typename T>
	CSBPlusTree<T>* indexBuilding(vector<compare_node*> chunk_tuples);

	void computeVectorSchema();

	State state_;

	Schema* vector_schema_;
	std::map <ChunkOffset, vector<compare_node*> > tuples_in_chunk_; //ChunkID->tuples_in_chunk

	PartitionID partition_id_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<ExpandableBlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* CSBINDEXBUILDING_H_ */
