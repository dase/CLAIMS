/*
 * InsertOptimizedHashTable.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: wangli
 */
#include <boost/pool/singleton_pool.hpp>
#include <boost/atomic.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include "InsertOptimizedHashTable.h"
#include "../utility/atomics.h"
//#define __INSERT_HASH_TABLE_MEMORY_POOL__


#ifdef __INSERT_HASH_TABLE_MEMORY_POOL__
	struct pool_tag{};
	typedef boost::singleton_pool<pool_tag, sizeof(InsertOptimizedHashTable::bucket)> pool;
#endif
InsertOptimizedHashTable::InsertOptimizedHashTable(unsigned nbuckets):nbuckets_(nbuckets),hash_value_mask(nbuckets-1)
{
//	initialize();
}
void InsertOptimizedHashTable::initialize(){
	buckets_=new bucket[nbuckets_];
//	memset(buckets_,0,sizeof(bucket)*nbuckets_);
}

InsertOptimizedHashTable::~InsertOptimizedHashTable() {

	/* free all the block in block list */
	for(unsigned i=0;i<block_list_.size();i++){
		delete block_list_[i];
	}

	for(unsigned i=0;i<nbuckets_;i++){
		bucket* buck=buckets_[i].head.next;
		while(buck){
			bucket* tmp_buck=buck;
			buck=buck->head.next;
#ifdef __INSERT_HASH_TABLE_MEMORY_POOL__
			pool::free(tmp_buck);
#else
			delete tmp_buck;
#endif
		}
	}
	delete[] buckets_;
}

void InsertOptimizedHashTable::insertBlock(BlockStreamBase* block) {
	BlockStreamBase* new_block=block->createBlockAndDeepCopy();
	block_list_lock_.acquire();
	block_list_.push_back(new_block);
	block_list_lock_.release();
}

void InsertOptimizedHashTable::insertEntry(unsigned bn, entry& entry){
	bucket& buck=buckets_[bn];
	buck.head.lock.acquire();
	unsigned new_entry_index;
	if(!buck.has_space_for_new_entry()){
#ifdef __INSERT_HASH_TABLE_MEMORY_POOL__
		bucket* new_buck=(bucket*)pool::malloc();
#else
		bucket* new_buck=new bucket();
#endif
		*new_buck=buck;
		buck.head.next=new_buck;
		buck.head.tuple_count=0;
	}
	buck.allocate_new_entry(new_entry_index);
	buck.entrys[new_entry_index]=entry;
	buck.head.lock.release();
}
