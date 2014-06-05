/*
 * InsertOptimizedHashTable.h
 *
 *  Created on: Jun 4, 2014
 *      Author: wangli
 */

#ifndef INSERTOPTIMIZEDHASHTABLE_H_
#define INSERTOPTIMIZEDHASHTABLE_H_
#include <vector>
#include "../common/Block/BlockStream.h"
#include "../utility/lock.h"


/**
 * This is an implementation of insert-optimized hash table. Rather than inserting each
 * received tuples into the hash table, all tuples are organized in listed blocks and an
 * pointer is inserted into the corresponding hash bucket for each tuple.
 *
 * The drawback of this implementation is that there are at least two memory accesses to
 * search a target key, one for locating the pointer and one for reading the real tuple.
 */


#define Bucket_Size 64
#define Bucket_Capacity (Bucket_Size-sizeof(meta_data))/sizeof(entry)

class InsertOptimizedHashTable {
public:
	struct entry{
		entry(){};
		entry(void* p):pointer(p){};
		void* pointer;
	};
	struct bucket;
	struct meta_data{
		meta_data():tuple_count(0),lock(),next(0){};
		SpineLock lock;
		uint32_t tuple_count;
		bucket* next;
	};
	struct bucket{
		meta_data head;
		entry entrys[(Bucket_Size-sizeof(head))/sizeof(entry)];
		bool has_space_for_new_entry(){
			return head.tuple_count!=(Bucket_Capacity);
		}
		bool allocate_new_entry(unsigned& new_entry_index){
//			bool ret=(meta_data.tuple_count!=(Bucket_Capacity));
			new_entry_index=head.tuple_count;
			head.tuple_count++;
//			meta_data.tuple_count+=ret;
//			return ret;
		}
	};

	class BucketIterator{
	public:
		BucketIterator(bucket* buck)
		:bucket_(buck),tuple_cur(0){};
		void* nextTuple(){
			if(tuple_cur<bucket_->head.tuple_count)
				return bucket_->entrys[tuple_cur++].pointer;
			else{
				if(bucket_->head.next){
					tuple_cur=0;
					bucket_=bucket_->head.next;
					return nextTuple();
				}
				else
					return 0;
			}
		}
	private:
		unsigned tuple_cur;
		InsertOptimizedHashTable::bucket* bucket_;
	};
	friend class BucketIterator;
public:
	InsertOptimizedHashTable(unsigned nbuckets);
	virtual ~InsertOptimizedHashTable();

	/**
	 * Insert a block stream into the hash table block list.
	 * It is the responsibility of hash table for possible space allocation and release.
	 * If the block is a reference, a new referenced block will be created. Otherwise, a
	 * new block will be allocated with desirable block size and apply deep copy to create
	 * a new block.
	 */
	void insertBlock(BlockStreamBase* block);

	/*insert an entry into the bn-th bucket*/
	void insertEntry(unsigned bn, entry& entry);

	BucketIterator createBucketIterator(unsigned bn){
		return BucketIterator(&buckets_[bn]);
	}
private:
protected:
	std::vector<BlockStreamBase*> block_list_;
	SpineLock block_list_lock_;
	bucket* buckets_;
	unsigned nbuckets_;

	const unsigned long hash_value_mask;
};




#endif /* INSERTOPTIMIZEDHASHTABLE_H_ */
