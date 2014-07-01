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
	int32_t pid;
	DynamicBlockBuffer::Iterator* buffer_it;
	InsertOptimizedHashTable* hashtable;
	unsigned nbuckets;
	Barrier *barrier;
};
void* insert_into_insert_optimized_hash_table(void* args){
//	printf("****\n");
//	int seed=random()%10;
	parallel_insert_args* arg=(parallel_insert_args*)args;

	if(arg->pid==0)
		arg->hashtable->initialize();
	arg->barrier->Arrive();

	BlockStreamBase* block;
	const unsigned nbuckets=arg->nbuckets;

	while((block=arg->buffer_it->atomicNextBlock())!=0){
		BlockStreamBase::BlockStreamTraverseIterator* bit=block->createIterator();
		arg->hashtable->insertBlock(block);
		void* tuple;
		while((tuple=bit->nextTuple())!=0){
			InsertOptimizedHashTable::entry ent_tmp=InsertOptimizedHashTable::entry(tuple);
			arg->hashtable->insertEntry(boost::hash_value(*(int*)tuple)%(nbuckets),ent_tmp);
//			printf("%d---->%d\n",seed,boost::hash_value(*(int*)tuple)%(arg->nbuckets));
		}
		delete bit;
	}
}


TEST(INSERT_OPTIMIZED_HASH_TABLE,INSERT){
	const unsigned nbuckets=1024;
	const unsigned data_size_in_MB=1024;
	const unsigned tuple_length=32;

	InsertOptimizedHashTable hashtable(nbuckets);
	hashtable.initialize();

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

unsigned long long parallel_insert_to_insert_optimized_hash(InsertOptimizedHashTable& hashtable,unsigned nbuckets,DynamicBlockBuffer* buffer,DynamicBlockBuffer::Iterator* it,int nthreads){
	parallel_insert_args tmp_arg;
	parallel_insert_args paras[20];
	tmp_arg.buffer_it=it;
	tmp_arg.hashtable=&hashtable;
	tmp_arg.nbuckets=nbuckets;
	tmp_arg.barrier=new Barrier(nthreads);
	pthread_t pid[20];
	unsigned long long ret=curtick();
	for(unsigned i=0;i<nthreads;i++){
		paras[i]=tmp_arg;
		paras[i].pid=i;
		pthread_create(&pid[i],0,insert_into_insert_optimized_hash_table,&paras[i]);
	}

	for(unsigned i=0;i<nthreads;i++){
		pthread_join(pid[i],0);
	}
	return curtick()-ret;
}
TEST(INSERT_OPTIMIZED_HASH_TABLE,INSERT_PARALLEL){
	const unsigned nbuckets=1024*1024;
	const unsigned data_size_in_MB=1024;
	const unsigned tuple_length=32;
	const unsigned nthreads=4;

	InsertOptimizedHashTable hashtable(nbuckets);

	Schema* schema=generateSchema(tuple_length);
	DynamicBlockBuffer* buffer=generate_BlockStreamBuffer(schema,data_size_in_MB);

	DynamicBlockBuffer::Iterator it=buffer->createIterator();

	parallel_insert_to_insert_optimized_hash(hashtable,nbuckets,buffer,&it,nthreads);

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

