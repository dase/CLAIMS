/*
 * BlockReadbaleFix.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: wangli
 */

#include "BlockReadableFix.h"

BlockReadableFix::BlockReadableFix(Block block, Schema *schema)
:BlockReadable(block), schema(schema)
{
	cur=(char*)start;

	end=getend();
}

BlockReadableFix::BlockReadableFix(unsigned BlockSize, Schema* schema)
:BlockReadable(BlockSize),schema(schema)
{
	cur=(char*)start;
	end=(char*)start;//cdcdccdcdcdcdcdcdcdc
}

BlockReadableFix::~BlockReadableFix() {
	// TODO Auto-generated destructor stub
}

void BlockReadableFix::reset()
{
	cur=(char*)start;
	end=getend();
}

void* BlockReadableFix::next()
{
	char* ret=cur;

	if(ret+schema->getTupleMaxSize()<=end)//modify bt zhanglei
	{
		cur+=schema->getTupleMaxSize();
		return ret;
	}
	else
		return 0;
}

unsigned BlockReadableFix::getMaxTupleCount()
{
	return (BlockSize-2*sizeof(unsigned))/schema->getTupleMaxSize();
}

unsigned BlockReadableFix::getActualTupleCount()
{
	unsigned* tuplecount=(unsigned *)((char*)start+BlockSize-2*sizeof(unsigned));
	return *tuplecount;
}
bool BlockReadableFix::IsLastBlock(){
	unsigned* s=(unsigned *)((char *)start+BlockSize-sizeof(unsigned));
	return (*s)==0;
}

unsigned BlockReadableFix::getS(){
	unsigned* s=(unsigned *)((char *)start+BlockSize-sizeof(unsigned));
	return *s;
}

char* BlockReadableFix::getend()
{
	return (char*)start+getActualTupleCount()*schema->getTupleMaxSize();
}

char *BlockReadableFix::getCur() const
{
    return cur;
}

void BlockReadableFix::setCur(char *cur)
{
    this->cur = cur;
}




