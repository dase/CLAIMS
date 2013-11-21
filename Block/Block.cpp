/*
 * Block.cpp
 *
 *  Created on: Jun 22, 2013
 *      Author: wangli
 */
#include <malloc.h>
//hello
#include "Block.h"
#include "../configure.h"
Block::Block(unsigned BlockSize)
:BlockSize(BlockSize),isReference_(false)
{
	start=(char*)memalign(cacheline_size,BlockSize);
}
Block::Block(const unsigned& size,const void* start)
:BlockSize(size),start((char*)start),isReference_(true){

}

//zhanglei
Block::~Block() {
	if(!isReference_)
		free(start);
	start=0;
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

