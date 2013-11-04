/*
 * BlockWritableFix.h
 *
 *  Created on: Jun 24, 2013
 *      Author: wangli
 */

#ifndef BLOCKWRITABLEFIX_H_
#define BLOCKWRITABLEFIX_H_
#include "BlockWritable.h"
#include "../Schema/SchemaFix.h"
class BlockWritableFix : public BlockWritable {
public:
	BlockWritableFix(unsigned BlockSize, Schema* schema);
	virtual ~BlockWritableFix();
	inline bool insert(void * tuple){
		unsigned tuplesize;
		if((char*)free+schema->getTupleMaxSize()<=(char*)start+BlockSize-2*sizeof(tuplecount))
		{
			tuplesize=schema->copyTuple(tuple,free);
			free=free+tuplesize;
			tuplecount++;
			return true;
		}
		return false;
	}
	inline bool insert(void* & tuple, SchemaFix &schemafix){
		if((char*)free+schemafix.getTupleSize()<=(char*)start+BlockSize-2*sizeof(tuplecount))
		{
			const unsigned tuplesize=schemafix.copy(tuple,(void*)free);
			free=free+tuplesize;
			tuplecount++;
			return true;
		}
		return false;
	}
	void foldLastBlock();
	bool isLastBlock();
	void fold();
	void reset();
	void setEmpty();
	unsigned getTupleCount();
//	void* allocate(){};
//	bool copyTuple(void* desc){};
private:
	bool hasSpace();
private:
	Schema* schema;
	char* free;
	unsigned tuplecount;
};

#endif /* BLOCKWRITABLEFIX_H_ */
