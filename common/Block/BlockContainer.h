/*
 * BlockContainer.h
 *
 *  Created on: Sep 7, 2013
 *      Author: wangli
 */

#ifndef BLOCKCONTAINER_H_
#define BLOCKCONTAINER_H_
#include "Block.h"
class BlockContainer:public Block {
public:
	BlockContainer(unsigned size);
	virtual ~BlockContainer();
	unsigned GetMaxSize()const;
	unsigned GetCurSize()const;
	unsigned GetRestSize()const;
	void reset();
	void IncreaseActualSize(unsigned size);
private:
	unsigned actual_size_;
};

#endif /* BLOCKCONTAINER_H_ */
