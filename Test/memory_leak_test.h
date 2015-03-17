/*
 * memory_leak_test.h
 *
 *  Created on: Jun 5, 2014
 *      Author: wangli
 */

#ifndef MEMORY_LEAK_TEST_H_
#define MEMORY_LEAK_TEST_H_
#include <assert.h>
#include "../common/hashtable.h"


void test_schema_allocate_and_deallocate(unsigned long repeat=10000){

	volatile Schema* schema;
	for(unsigned i=0;i<repeat;i++){
		Schema* schema=generateSchema(4);
		delete schema;
		if(i%10000==1){
			printf("%d\n",i);
		}
	}
}
void init_insert_and_destroy(unsigned long nbuckets,unsigned long bucket_size, unsigned tuple_size, unsigned long size){
	BasicHashTable* hashtable=new BasicHashTable(nbuckets,bucket_size,tuple_size);

	for(int i=1; i<size/tuple_size; i++){
		void * tuple=hashtable->allocate(rand()%nbuckets);
		memset(tuple,0,tuple_size);
	}

	delete hashtable;
}
void test_hash_table(unsigned long repeat=1000){
	for(int i=0; i< repeat;i++){
		init_insert_and_destroy(1024*1024,128,64,1024*1024*1024);
	}
}

int memory_leak_test(){
//	test_schema_allocate_and_deallocate(100000000);
	test_hash_table();
	sleep(1000);
}

#endif /* MEMORY_LEAK_TEST_H_ */
