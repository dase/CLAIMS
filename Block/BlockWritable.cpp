/*
 * BlockWritable.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: wangli
 */

#include "BlockWritable.h"
#include <memory.h>
#include <malloc.h>
BlockWritable::BlockWritable(unsigned BlockSize)
:Block(BlockSize)
{

}

BlockWritable::~BlockWritable() {

}

bool BlockWritable::copyBlock(void* src)
{
	memcpy(start,src,BlockSize);
	return true;
}

