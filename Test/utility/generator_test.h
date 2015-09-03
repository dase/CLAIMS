/*
 * generator_test.h
 *
 *  Created on: Jun 5, 2014
 *      Author: wangli
 */

#ifndef GENERATOR_TEST_H_
#define GENERATOR_TEST_H_


#include <vector>
#include <gtest/gtest.h>
#include "../../common/data_type.h"
#include "../../utility/generator.h"
#include "../../Debug.h"

TEST(GENERATOR,SCHEMASS){
	std::vector<column_type> columns;
	columns.push_back(column_type(t_int));
	columns.push_back(column_type(t_float));
	Schema* schema=new SchemaFix(columns);

	Schema* generated_schema=generateSchema(columns);

	EXPECT_TRUE(schema->hasSameSchema(generated_schema));
	schema->~Schema();
	generated_schema->~Schema();

}

TEST(GENERATOR, DATA){
	Schema* schema=generate_schema(48);
	unsigned data_size_in_MB=1024;
	DynamicBlockBuffer* buffer=generate_BlockStreamBuffer(schema,data_size_in_MB);

	BlockStreamBase* block;
	BlockStreamBase::BlockStreamTraverseIterator* it;

	DynamicBlockBuffer::Iterator buffer_it=buffer->createIterator();
	unsigned long total_size=0;
	while((block=buffer_it.nextBlock())!=0){
		it=block->createIterator();
		while(it->nextTuple()){
			total_size+=schema->getTupleMaxSize();
		}
		it->~BlockStreamTraverseIterator();
	}
	EXPECT_EQ(1024*1024*data_size_in_MB/schema->getTupleMaxSize()*schema->getTupleMaxSize(),total_size);

	buffer->~DynamicBlockBuffer();

}

#endif /* GENERATOR_TEST_H_ */
