/*
 * Block.cpp
 *
 *  Created on: Jun 22, 2013
 *      Author: wangli
 */
#include <malloc.h>
#include <memory.h>
//hello
#include "Block.h"
#include "../../configure.h"

int allocate=0;
Block::Block(unsigned BlockSize)
:BlockSize(BlockSize),isReference_(false)
{
//	start=(char*)memalign(cacheline_size,BlockSize);
	start=(char*)malloc(BlockSize);
	allocate++;
//	printf("allocate %d-->%d\n",allocate-1,allocate);
	/*the following memset is just for debugging the memory leak*/
//	memset(start,0,BlockSize);
}
Block::Block(const unsigned& size,const void* start)
:BlockSize(size),start((char*)start),isReference_(true){

}

//zhanglei
Block::~Block() {
	if(!isReference_)
		free(start);
	start=0;
	allocate--;
//	printf("allocate %d-->%d\n",allocate+1,allocate);
}

Block::Block(const Block &block)
{
	this->BlockSize=block.BlockSize;
	this->start=block.start;
	this->isReference_=block.isReference_;
}

void* Block::getBlock()const
{
	return start;
}
unsigned Block::getsize ()const
{
	return BlockSize;
}
unsigned Block::setsize(const unsigned& size) {
	BlockSize=size;
}
void Block::setBlock(void* addr){
	start=(char*)addr;
}

void Block::setIsReference(bool isReference) {
	if(!isReference){
		delete start;
	}
	isReference = true;
}
