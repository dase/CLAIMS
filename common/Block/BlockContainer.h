/*
 * BlockContainer.h
 *
 *  Created on: Sep 7, 2013
 *      Author: wangli
 */

#ifndef BLOCKCONTAINER_H_
#define BLOCKCONTAINER_H_
#include "Block.h"
#ifdef DMALLOC
#include "dmalloc.h"
#endif
class BlockContainer:public Block {
public:
	BlockContainer(unsigned size);
	virtual ~BlockContainer();
	unsigned GetMaxSize()const;
	unsigned GetCurSize()const;
	unsigned GetRestSizeToHandle()const;
	void reset();
	void IncreaseActualSize(unsigned size);
	/**
	 * Copy the block content and the acutal_size_
	 */
	void copy(BlockContainer& block);
private:
	unsigned actual_size_;
};

#endif /* BLOCKCONTAINER_H_ */
