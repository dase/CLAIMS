/*
 * hash_table_test.h
 *
 *  Created on: Apr 5, 2014
 *      Author: wangli
 */

#ifndef HASH_TABLE_TEST_H_
#define HASH_TABLE_TEST_H_
#include "../../Block/BlockStreamBuffer.h"
#include "../../Catalog/Column.h"
#include "../../hash.h"
#include <boost/functional/hash.hpp>
#define block_size (1024*64)

struct Arg{
	BlockStreamBuffer* buffer;
	BasicHashTable *hash_table;
	Schema* schema;
	PartitionFunction* hash;
};

Schema* generate_schema(unsigned tuple_length){
	std::vector<column_type> columns;
	for(unsigned i=0;i<tuple_length/4;i++){
		columns.push_back(column_type(t_int));
	}
	Schema* schema=new SchemaFix(columns);
	return schema;
}

BasicHashTable* generate_hashtable(Schema* schema, unsigned nbuckets, unsigned bucketsize){
	BasicHashTable* hashtable=new BasicHashTable(nbuckets,bucketsize,schema->getTupleMaxSize());
	return hashtable;
}

BlockStreamBuffer* initial_input_date(Schema* schema,unsigned long total_data_size_in_MB){

	unsigned block_num=total_data_size_in_MB*1024*1024/(block_size);
	printf("%d blocks\n",block_num);

	BlockStreamBuffer* buffer=new BlockStreamBuffer(block_size-4,block_num,schema);
	int value=0;
	BlockStreamBase* new_block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(schema,block_size);
	for(unsigned i=0;i<block_num;i++){
		void* tuple;
		new_block->setEmpty();
		while(tuple=new_block->allocateTuple(schema->getTupleMaxSize())){
			schema->columns[0].operate->assignment(&value,tuple);
			value=random();
//			value++;
		}
		buffer->insertBlock(new_block);
	}
	new_block->~BlockStreamBase();
	return buffer;

}
void* insert_into_hash_table(void * argment){
	Arg arg=*(Arg*)argment;
	BlockStreamBase* fetched_block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(arg.schema,block_size);
	Operate* op=arg.schema->columns[0].operate->duplicateOperator();
	unsigned nbuckets=arg.hash->getNumberOfPartitions();
	while(arg.buffer->getBlock(*fetched_block)){
		void* tuple;
		BlockStreamBase::BlockStreamTraverseIterator* it=fetched_block->createIterator();
		while(tuple=it->nextTuple()){
//			if(boost::hash_value((*(int*)tuple)%1342342)==0){
//				break;
//			}
//			if(op->getPartitionValue(tuple,arg.hash)==0){
//				break;
//			}
			unsigned bn=arg.schema->columns[0].operate->getPartitionValue(tuple,arg.hash);
			void* new_tuple_in_hashtable=arg.hash_table->atomicAllocate(bn);
			arg.schema->copyTuple(tuple,new_tuple_in_hashtable);
		}
	}
}

void create_thread(unsigned nthreads){

}

double fill_hash_table(unsigned degree_of_parallelism){
	Config::getInstance();
	const unsigned nbuckets=1024*1024;
	const unsigned tuple_size=200;
	const unsigned bucketsize=256-8;
	const unsigned long data_size_in_MB=128;
	const unsigned nthreads=degree_of_parallelism;
	double ret;
	printf("nthread=%d\n",nthreads);
	Schema* schema=generate_schema(tuple_size);
	BasicHashTable* hashtable=generate_hashtable(schema,nbuckets,bucketsize);
	BlockStreamBuffer* buffer=initial_input_date(schema,data_size_in_MB);

	Arg arg;
	arg.buffer=buffer;
	arg.hash=PartitionFunctionFactory::createBoostHashFunction(nbuckets);
	arg.hash_table=hashtable;
	arg.schema=schema;
	pthread_t pid[1000];
	unsigned long long int start=curtick();
	for(unsigned i=0;i<nthreads;i++){
		pthread_create(&pid[i],0,insert_into_hash_table,&arg);
	}

	for(unsigned i=0;i<nthreads;i++){
		pthread_join(pid[i],0);
	}
//	hashtable->report_status();
	printf("time:%4.4f\n",getSecond(start));
	ret=getSecond(start);
	int cycles_per_allocate=((double)curtick()-start)/(data_size_in_MB*1024*1024/tuple_size)*nthreads;
	printf("Avg. %d cycles per allocation!\n",cycles_per_allocate);
	buffer->~BlockStreamBuffer();
	hashtable->~BasicHashTable();
	schema->~Schema();

	return ret;

}
double getAverage(int degree_of_parallelism,int repeated_times=10){
	double ret=0;
	for(unsigned i=0;i<repeated_times;i++){
		ret+=fill_hash_table(degree_of_parallelism);
	}
	return ret/repeated_times;
}

int hash_table_test(){
	unsigned int max_degree_of_parallelism=4;
	unsigned repeated_times=3;
	double standard_throughput=0;
	for(unsigned i=1;i<=max_degree_of_parallelism;i++){
		double total_time=getAverage(i,repeated_times);
		if(i==1){
			standard_throughput=1/total_time;
			printf("D=%d\ts=%4.4f scale:1\n",i,total_time,1);
		}
		else{
			printf("D=%d\ts=%4.4f scale:%f\n",i,total_time,1/total_time/standard_throughput);
		}
	}
}

#endif /* HASH_TABLE_TEST_H_ */
