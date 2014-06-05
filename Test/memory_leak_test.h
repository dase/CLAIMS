/*
 * memory_leak_test.h
 *
 *  Created on: Jun 5, 2014
 *      Author: wangli
 */

#ifndef MEMORY_LEAK_TEST_H_
#define MEMORY_LEAK_TEST_H_
#include <assert.h>


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

int memory_leak_test(){
	test_schema_allocate_and_deallocate(1);
	assert(COUNTER::count==0);
//	sleep(100);
}

#endif /* MEMORY_LEAK_TEST_H_ */
