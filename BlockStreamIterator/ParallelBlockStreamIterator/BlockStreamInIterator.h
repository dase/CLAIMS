/*
 * BlockStreamInIterator.h
 *
 *  Created on: Mar 4, 2014
 *      Author: scdong
 */

#ifndef BLOCKSTREAMINITERATOR_H_
#define BLOCKSTREAMINITERATOR_H_

#include "../BlockStreamIteratorBase.h"
#include "../../Debug.h"
#include "../../utility/rdtsc.h"
#include "../../common/hashtable.h"

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "../../utility/lock.h"
#include "../../common/hash.h"
using namespace std;

class BlockStreamInIterator :public BlockStreamIteratorBase {
public:
	struct remaining_block{
		remaining_block(BlockStreamBase *bsb_right,BlockStreamBase::BlockStreamTraverseIterator *bsti)
		:bsb_in_(bsb_right),blockstream_iterator(bsti){};
		remaining_block():bsb_in_(0),blockstream_iterator(0){};
		remaining_block(const remaining_block&r){
			bsb_in_=r.bsb_in_;
			blockstream_iterator=r.blockstream_iterator;
		}
		BlockStreamBase *bsb_in_;
		BlockStreamBase::BlockStreamTraverseIterator *blockstream_iterator;
	};

	class State{
		friend class BlockStreamInIterator;
	public:
		State(BlockStreamIteratorBase *child_set,
				       BlockStreamIteratorBase *child_in,
				       Schema *schema_child_set,
				       Schema *schema_child_in,
				       unsigned index_child_set,
				       unsigned index_child_in,
				       unsigned block_size,
				       unsigned ht_nbuckets,
				       unsigned ht_bucketsize);
		State(BlockStreamIteratorBase *child_set,
				       BlockStreamIteratorBase *child_in,
				       Schema *schema_child_set,
				       Schema *schema_child_in,
				       unsigned index_child_set,
				       unsigned index_child_in,
				       unsigned block_size);

		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & child_set & child_in & schema_child_set & schema_child_in & index_child_set & index_child_in
			& ht_nbuckets & ht_bucketsize & block_size_;
		}
	public:
		//input and output
		BlockStreamIteratorBase *child_set,*child_in;
		Schema *schema_child_set,*schema_child_in;

		//how to choose the legal record
		unsigned index_child_set;
		unsigned index_child_in;

		//hash table
		unsigned ht_nbuckets;
		unsigned ht_bucketsize;

		unsigned block_size_;
	};

	BlockStreamInIterator(State state);
	BlockStreamInIterator();
	virtual ~BlockStreamInIterator();

	bool open(const PartitionOffset& partition_offset = 0);
	bool next(BlockStreamBase* block);
	bool close();

private:
	bool atomicPopRemainingBlock(remaining_block & rb);
	void atomicPushRemainingBlock(remaining_block rb);
	BlockStreamBase* AtomicPopFreeBlockStream();
	void AtomicPushFreeBlockStream(BlockStreamBase* block);
	BlockStreamBase* AtomicPopFreeHtBlockStream();
	void AtomicPushFreeHtBlockStream(BlockStreamBase* block);

	State state_;

	PartitionFunction* hash;
	BasicHashTable* hashtable;

	std::list<remaining_block> remaining_block_list_;
	std::list<BlockStreamBase *> free_block_stream_list_;
	std::list<BlockStreamBase *> ht_free_block_stream_list_;

	semaphore sema_open_;
	volatile bool open_finished_;
	unsigned reached_end;
	Lock lock_;
	Barrier *barrier_;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
            ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
    }
};

#endif /* BLOCKSTREAMINITERATOR_H_ */
