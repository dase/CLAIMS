/*
 * BlockReadableFixBuffer.cpp
 *
 *  Created on: 2013-7-8
 *      Author: zhanglei
 */

#include "BlockReadableFixBuffer.h"

BlockReadableFixBuffer::BlockReadableFixBuffer(Schema *schema,unsigned BlockSize,unsigned BlockNum,unsigned curBlock)
:BlockNum(BlockNum),BlockSize(BlockSize),schema(schema),curBlock(curBlock){
	// TODO Auto-generated constructor stub
	//allocate the memory to store the points' array
	start=(BlockReadableFix**)malloc(sizeof(BlockReadableFix*));
	for(int i=0;i<BlockNum;i++){
		start[i]=0;
	}
}

BlockReadableFixBuffer::~BlockReadableFixBuffer() {
	// TODO Auto-generated destructor stub
}

bool BlockReadableFixBuffer::init(){
	for(int i=0;i<BlockNum;i++){
		start[i]=new BlockReadableFix(BlockSize,schema);
	}
	curBlock=0;
	return true;
}

void* BlockReadableFixBuffer::next(){
	char* ret=start[curBlock]->getCur();
	if(ret+schema->getTupleMaxSize()<start[curBlock]->getend()){
		start[curBlock]->setCur(start[curBlock]->getCur()+schema->getTupleMaxSize());
		return ret;
	}
	else{
		nextBlock();
		return 0;
	}
}

bool BlockReadableFixBuffer::nextBlock(){
	curBlock++;
	curBlock=curBlock%BlockNum;
	start[curBlock]->reset();
	return true;
}

unsigned BlockReadableFixBuffer::getCurBlock() const
{
    return curBlock;
}

BlockReadableFix** BlockReadableFixBuffer::getStart() const
{
    return start;
}

void BlockReadableFixBuffer::setBlockSize(unsigned  BlockSize)
{
    this->BlockSize = BlockSize;
}

void BlockReadableFixBuffer::setCurBlock(unsigned  curBlock)
{
    this->curBlock = curBlock;
}


