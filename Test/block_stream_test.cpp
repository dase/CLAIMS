/*
 * block_stream_test.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */
#include "../common/Schema/SchemaFix.h"
#include "../Block/BlockStream.h"
int mainasfasdf22(){
	std::vector<column_type> column_list;
	column_list.push_back(column_type(t_int));

	Schema* input=new SchemaFix(column_list);

	BlockStreamBase* bs=new BlockStreamFix(4096,4);



	void* tuple;
	int value=0;
	int i=0;
	while(tuple=bs->allocateTuple(4)){
		*(int*)tuple=value++;
		printf("insert %d\n",i++);
	}
	BlockStreamBase::BlockStreamTraverseIterator* bsti=bs->createIterator();
	i=0;
	while(tuple=bsti->nextTuple()){
		printf("%d: Tuple=%d\n",i++,*(int*)tuple);
	}




}


