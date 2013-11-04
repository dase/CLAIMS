/*
 * BlockIterator.h
 *
 *  Created on: Aug 23, 2013
 *      Author: wangli
 */

#ifndef BLOCKITERATORFIX_H_
#define BLOCKITERATORFIX_H_
#include "../Block/BlockFix.h"
class BlockIteratorFix {
public:
	BlockIteratorFix();
	virtual ~BlockIteratorFix();
	virtual bool open()=0;
	virtual bool next(BlockFix &block)=0;
	virtual bool close()=0;
};

#endif /* BLOCKITERATOR_H_ */
