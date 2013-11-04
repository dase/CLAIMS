/*
 * BlockWritable.h
 *
 *  Created on: Jun 24, 2013
 *      Author: wangli
 */

#ifndef BLOCKWRITABLE_H_
#define BLOCKWRITABLE_H_
#include "Block.h"
class BlockWritable : public Block {
public:
	BlockWritable(unsigned BlockSize);
	virtual ~BlockWritable();
	inline virtual bool insert(void *)=0;
//	virtual void* allocate()=0;
//	virtual bool copyTuple(void*)=0;
	virtual void foldLastBlock()=0;
	virtual bool isLastBlock()=0;
	virtual void fold()=0;
	virtual void reset()=0;
	virtual void setEmpty()=0;
	virtual unsigned getTupleCount()=0;

	bool copyBlock(void* src);

};

#endif /* BLOCKWRITABLE_H_ */
