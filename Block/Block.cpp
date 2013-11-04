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
:BlockSize(BlockSize)
{
	start=memalign(cacheline_size,BlockSize);
}  //cdcd
//zhanglei
Block::~Block() {
	free(start);
	start=0;
}

Block::Block(const Block &block)
{
	this->BlockSize=block.BlockSize;
	this->start=block.start;
}

void* Block::getBlock()
{
	return start;
}
unsigned Block::getsize ()const
{
	return BlockSize;
}
void Block::setBlock(void* addr){
	start=addr;
}
