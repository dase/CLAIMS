/*
 * InsertOptimizedHashTable.h
 *
 *  Created on: Jun 4, 2014
 *      Author: wangli
 */

#ifndef INSERTOPTIMIZEDHASHTABLE_H_
#define INSERTOPTIMIZEDHASHTABLE_H_


/**
 * This is an implementation of insert-optimized hash table. Rather than inserting each
 * received tuples into the hash table, all tuples are organized in listed blocks and an
 * pointer is inserted into the corresponding hash bucket for each tuple.
 *
 * The drawback of this implementation is that there are at least two memory accesses to
 * search a target key, one for locating the pointer and one for reading the real tuple.
 */
class InsertOptimizedHashTable {
public:
	InsertOptimizedHashTable();
	virtual ~InsertOptimizedHashTable();
private:
	void insertBlock(BlockStreamBase* block);
private:
	std::vector<BlockStreamBase*> block_list_;
	SpineLock block_list_lock_;
};

#endif /* INSERTOPTIMIZEDHASHTABLE_H_ */
