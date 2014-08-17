/*
 * generator.h
 *
 *  Created on: Jun 5, 2014
 *      Author: wangli
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include <stdarg.h>
#include "../common/Schema/SchemaFix.h"
#include "../common/Schema/Schema.h"
#include "../Debug.h"
#include "data_distribution_generator.h"



struct column_metadata{
	column_type type;
	ColumnDistribution* dis;
	column_metadata(column_type type,ColumnDistribution* dis=0):type(type),dis(dis){
	}
	column_metadata():dis(0){

	}
	column_metadata(const column_metadata& r){
		this->type=r.type;
		this->dis=r.dis;
	}
};

class RandomRow{
public:
	RandomRow(){

	}
	void addIntUniformColumn(int min=0, int max=1024*1024, int seed=0){
		column_metadata column;
		column.type=column_type(t_int);
		column.dis=new UniformInt(min,max,seed);
		columns_.push_back(column);
	}
	~RandomRow(){
		for(unsigned i=0;i<columns_.size();i++){
			delete columns_[i].dis;
		}
	}
	std::vector<column_metadata> getColumns(){
		return columns_;
	}
private:
	std::vector<column_metadata> columns_;
};

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

inline DynamicBlockBuffer* generateBlockStreamBuffer(vector<column_metadata> columns, unsigned long tuple_count, Schema** return_schema=0){
	std::vector<column_type> column_types;
	for(unsigned i=0;i<columns.size();i++){
		column_types.push_back(columns[i].type);
	}
	Schema* schema=generateSchema(column_types);

	const unsigned tuple_length=schema->getTupleMaxSize();
	unsigned block_num=tuple_count*schema->getTupleMaxSize()/(BLOCK_SIZE);
	void* tmp_tuple=malloc(schema->getTupleMaxSize());

	DynamicBlockBuffer* buffer=new DynamicBlockBuffer();
	int value=0;
	for(unsigned i=0;i<tuple_count;){
		void* tuple;
		BlockStreamBase* new_block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(schema,BLOCK_SIZE);
		new_block->setEmpty();
		while(tuple=new_block->allocateTuple(schema->getTupleMaxSize())){

			for(unsigned i=0;i<columns.size();i++){
				if(columns[i].dis!=0){
					columns[i].dis->giveRandomValue(schema->getColumnAddess(i,tuple));
				}
			}

//			memcpy(tuple,tmp_tuple,tuple_length);
//			schema->columns[0].operate->assignment(&value,tuple);
//			value=random();
//			value++;
			i++;
			if(i==tuple_count)
				break;
		}
//		new_block->setIsReference(true);
		buffer->appendNewBlock(new_block);
	}
	free(tmp_tuple);

	if(return_schema!=0){
		*return_schema=schema->duplicateSchema();
	}
	delete schema;
	return buffer;
}

inline DynamicBlockBuffer* generate_BlockStreamBuffer(Schema* schema,unsigned long total_data_size_in_MB,std::vector<ColumnDistribution*> random_col=std::vector<ColumnDistribution*>()){


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
//		new_block->setIsReference(true);
		buffer->appendNewBlock(new_block);
	}
	free(tmp_tuple);
	return buffer;

}

#endif /* GENERATOR_H_ */
