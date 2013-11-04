/*
 * BlockWritableFixBuffer.cpp
 *
 *  Created on: 2013-7-8
 *      Author: zhanglei
 */

#include "BlockWritableFixBuffer.h"

BlockWritableFixBuffer::BlockWritableFixBuffer(Schema *schema,unsigned BlockSize,unsigned BlockNum,unsigned curBlock)
:BlockNum(BlockNum),BlockSize(BlockSize),schema(schema),curBlock(curBlock){
	// TODO Auto-generated constructor stub
	//allocate the memory to the points' array
	start=(BlockWritableFix**)malloc(sizeof(BlockWritableFix*));
	for(int i=0;i<BlockNum;i++){
		start[i]=0;
	}
}

BlockWritableFixBuffer::~BlockWritableFixBuffer() {
	// TODO Auto-generated destructor stub
}

bool BlockWritableFixBuffer::init(){
	for(int i=0;i<BlockNum;i++){
		start[i]=new BlockWritableFix(BlockSize,schema);
	}
	curBlock=0;
	return true;
}

//Currently, a simple hash function is given here, it's somewhat ugly
//TODO: support a Partition factory to define some HashPartition, ModulePatition .etc
unsigned BlockWritableFixBuffer::hash(void *tuple){
	unsigned hp;
	char* h = reinterpret_cast<char *>(tuple);
    uint64_t nbucket = 14695981039346656037ULL;
    while (*h != 0)
    	nbucket = (nbucket ^ (size_t)(*(h++))) * 1099511628211ULL;
	hp=nbucket%BlockNum;
	return hp;
}

unsigned BlockWritableFixBuffer::insert(void *tuple){
	//if none block is full, return BlockNum
	//if a block is full, return the number of the block
	if(start[hash(tuple)]->insert(tuple))
		return BlockNum;
	else{
		return hash(tuple);
	}
}

BlockWritableFix** BlockWritableFixBuffer::getStart() const
{
    return start;
}

unsigned BlockWritableFixBuffer::getBlockNum() const
{
    return BlockNum;
}
