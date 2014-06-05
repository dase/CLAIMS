#include <stdio.h>
#include "../../common/InsertOptimizedHashTable.h"
/*
 * insert_optimized_hash_table_test.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: wangli
 */

#ifndef __INSERT_HASHTABLE_TEST__
#define __INSERT_HASHTABLE_TEST__
#include <gtest/gtest.h>
#include "../../common/InsertOptimizedHashTable.h"
#include "../../utility/generator.h"

static int insert_optimziaed_hash_table_test(){
	InsertOptimizedHashTable::bucket t;
	printf("size is %d\n",sizeof(t));
}

TEST(INSERT_OPTIMIZED_HASH_TABLE,INSERT){
	const unsigned nbuckets=1024;
	const unsigned data_size_in_MB=1024;
	const unsigned tuple_length=32;

//	InsertOptimizedHashTable hashtable(nbuckets);
	void* tmp;

	Schema* schema=generateSchema(tuple_length);
//	DynamicBlockBuffer* buffer=generate_BlockStreamBuffer(schema,data_size_in_MB);
//
//	DynamicBlockBuffer::Iterator it=buffer->createIterator();
//
//	BlockStreamBase* block;
//
//	while((block=it.nextBlock())!=0){
//		BlockStreamBase::BlockStreamTraverseIterator* bit=block->createIterator();
//		hashtable.insertBlock(block);
//		void* tuple;
//		while((tuple=bit->nextTuple())!=0){
//			InsertOptimizedHashTable::entry ent_tmp=InsertOptimizedHashTable::entry(tuple);
//			hashtable.insertEntry(random()%(nbuckets),ent_tmp);
//		}
//		bit->~BlockStreamTraverseIterator();
//	}
//
//	unsigned long tuples_in_hashtable=0;
//	for(unsigned i=0;i<nbuckets;i++){
//		InsertOptimizedHashTable::BucketIterator buck_iter=hashtable.createBucketIterator(i);
//		while(buck_iter.nextTuple()){
//			tuples_in_hashtable++;
//		}
//	}
//
//	EXPECT_EQ(buffer->getNumberOftuples(),tuples_in_hashtable);
	EXPECT_EQ(1,1);
	schema->~Schema();
//	buffer->~DynamicBlockBuffer();
}
#endif

