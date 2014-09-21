/*
 * filter_test.h
 *
 *  Created on: Aug 17, 2014
 *      Author: wangli
 */

#ifndef FILTER_TEST_H_
#define FILTER_TEST_H_
#include "../../utility/generator.h"
#include "../../common/AttributeComparator.h"



int filter_test(){


//	ColumnDistribution* column = new UniformInt(0,100);


//	std::vector<column_metadata> columns;
//	columns.push_back(column_metadata(t_int,column));

	RandomRow rr;
	rr.addIntUniformColumn(0,1024*1024*1024,3);
	rr.addIntUniformColumn(0,1024*1024*1024,4);
	Schema* schema;

	const unsigned long tuple_count=1024*1024;
	DynamicBlockBuffer* dataset=generateBlockStreamBuffer(rr.getColumns(),tuple_count,&schema);


	DynamicBlockBuffer::Iterator it=dataset->createIterator();

	unsigned long passed_count=0;

	int va=100;
	AttributeComparator ac(column_type(t_int), Comparator::EQ,0,&va);



	BlockStreamBase* data_block;
	ticks start=curtick();
	while((data_block=it.nextBlock())!=0){
		BlockStreamBase::BlockStreamTraverseIterator* it=data_block->createIterator();
		void * tuple;
		while((tuple=it->nextTuple())!=0){
//			if(*(int*)tuple==100){
			if(ac.filter(tuple)){
				passed_count++;
			}
		}
		delete it;
	}

	printf("Filter condition avg. cycles: %d\n",(curtick()-start)/tuple_count);

	delete schema;
	delete dataset;

}

#endif /* FILTER_TEST_H_ */
