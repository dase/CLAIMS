/*
 * BlockReadable.h
 *
 *  Created on: Jun 24, 2013
 *      Author: wangli
 */

#ifndef BLOCKREADABLE_H_
#define BLOCKREADABLE_H_
#include "Block.h"
class BlockReadable:public Block {
public:
	BlockReadable(Block block);
	BlockReadable(unsigned BlockSize);
	virtual ~BlockReadable();
	virtual void reset()=0;
	virtual void* next()=0;
	virtual bool IsLastBlock()=0;
	virtual unsigned getMaxTupleCount()=0;
	virtual unsigned getActualTupleCount()=0;
};

#endif /* BLOCKREADABLE_H_ */
