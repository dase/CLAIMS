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
#include <boost/functional/hash.hpp>

static int insert_optimziaed_hash_table_test(){
	InsertOptimizedHashTable::bucket t;
	printf("size is %d\n",sizeof(t));
}

struct parallel_insert_args{
	DynamicBlockBuffer::Iterator* buffer_it;
	InsertOptimizedHashTable* hashtable;
	unsigned nbuckets;
};
void* insert_into_insert_optimized_hash_table(void* args){
	parallel_insert_args* arg=(parallel_insert_args*)args;

	BlockStreamBase* block;

	while((block=arg->buffer_it->atomicNextBlock())!=0){
		BlockStreamBase::BlockStreamTraverseIterator* bit=block->createIterator();
		arg->hashtable->insertBlock(block);
		void* tuple;
		while((tuple=bit->nextTuple())!=0){
			InsertOptimizedHashTable::entry ent_tmp=InsertOptimizedHashTable::entry(tuple);
			arg->hashtable->insertEntry(boost::hash_value(*(int*)tuple)%(arg->nbuckets),ent_tmp);
		}
		delete bit;
	}
}

TEST(INSERT_OPTIMIZED_HASH_TABLE,INSERT){
	const unsigned nbuckets=1024;
	const unsigned data_size_in_MB=1024;
	const unsigned tuple_length=32;

	InsertOptimizedHashTable hashtable(nbuckets);

	Schema* schema=generateSchema(tuple_length);
	DynamicBlockBuffer* buffer=generate_BlockStreamBuffer(schema,data_size_in_MB);

	DynamicBlockBuffer::Iterator it=buffer->createIterator();

	BlockStreamBase* block;

	unsigned long long int start=curtick();
	while((block=it.nextBlock())!=0){
		BlockStreamBase::BlockStreamTraverseIterator* bit=block->createIterator();
		hashtable.insertBlock(block);
		void* tuple;
		while((tuple=bit->nextTuple())!=0){
			InsertOptimizedHashTable::entry ent_tmp=InsertOptimizedHashTable::entry(tuple);
			hashtable.insertEntry(boost::hash_value(*(int*)tuple)%(nbuckets),ent_tmp);
		}
		delete bit;
	}
	unsigned long long int total_cycles=curtick()-start;

	unsigned long tuples_in_hashtable=0;
	for(unsigned i=0;i<nbuckets;i++){
		InsertOptimizedHashTable::BucketIterator buck_iter=hashtable.createBucketIterator(i);
		while(buck_iter.nextTuple()){
			tuples_in_hashtable++;
		}
	}
	printf("%d cycles per tuple!\n",total_cycles/tuples_in_hashtable);

	EXPECT_EQ(buffer->getNumberOftuples(),tuples_in_hashtable);
	delete schema;
	delete buffer;
}

TEST(INSERT_OPTIMIZED_HASH_TABLE,INSERT_PARALLEL){
	const unsigned nbuckets=1024;
	const unsigned data_size_in_MB=1024;
	const unsigned tuple_length=32;
	const unsigned nthreads=4;

	InsertOptimizedHashTable hashtable(nbuckets);

	Schema* schema=generateSchema(tuple_length);
	DynamicBlockBuffer* buffer=generate_BlockStreamBuffer(schema,data_size_in_MB);

	DynamicBlockBuffer::Iterator it=buffer->createIterator();

	parallel_insert_args para;
	para.buffer_it=&it;
	para.hashtable=&hashtable;
	para.nbuckets=nbuckets;
	pthread_t pid[20];
	for(unsigned i=0;i<nthreads;i++){

		pthread_create(&pid[i],0,insert_into_insert_optimized_hash_table,&para);
	}

	for(unsigned i=0;i<nthreads;i++){
		pthread_join(pid[i],0);
	}

	unsigned long tuples_in_hashtable=0;
	for(unsigned i=0;i<nbuckets;i++){
		InsertOptimizedHashTable::BucketIterator buck_iter=hashtable.createBucketIterator(i);
		while(buck_iter.nextTuple()){
			tuples_in_hashtable++;
		}
	}

	EXPECT_EQ(buffer->getNumberOftuples(),tuples_in_hashtable);
	delete schema;
	delete buffer;
}
#endif

