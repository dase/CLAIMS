/*
 * generator.h
 *
 *  Created on: Jun 5, 2014
 *      Author: wangli
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_
#include "../common/Schema/Schema.h"
#include <stdarg.h>
#include "../common/Schema/SchemaFix.h"
#include "../Debug.h"



inline Schema* generateSchema(unsigned tuple_length){
	std::vector<column_type> columns;
	for(unsigned i=0;i<tuple_length/sizeof(int);i++){
		columns.push_back(column_type(t_int));
	}
	Schema* schema=new SchemaFix(columns);
	return schema;

}

inline Schema* generateSchema(std::vector<column_type> columns){
	return new SchemaFix(columns);

}

inline DynamicBlockBuffer* generate_BlockStreamBuffer(Schema* schema,unsigned long total_data_size_in_MB){


	const unsigned tuple_length=schema->getTupleMaxSize();
	unsigned long tuple_count=total_data_size_in_MB*1024*1024/(tuple_length);
	unsigned block_num=total_data_size_in_MB*1024*1024/(BLOCK_SIZE);
	void* tmp_tuple=malloc(schema->getTupleMaxSize());

	DynamicBlockBuffer* buffer=new DynamicBlockBuffer();
	int value=0;
	for(unsigned i=0;i<tuple_count;){
		void* tuple;
		BlockStreamBase* new_block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(schema,BLOCK_SIZE);
		new_block->setEmpty();
		while(tuple=new_block->allocateTuple(schema->getTupleMaxSize())){
			memcpy(tuple,tmp_tuple,tuple_length);
			schema->columns[0].operate->assignment(&value,tuple);
			value=random();
//			value++;
			i++;
			if(i==tuple_count)
				break;
		}
		new_block->setIsReference(true);
		buffer->appendNewBlock(new_block);
	}
	free(tmp_tuple);
	return buffer;

}

#endif /* GENERATOR_H_ */
