/*
 * Block.h
 *
 *  Created on: Jun 22, 2013
 *      Author: wangli
 */

#ifndef BLOCK_H_
#define BLOCK_H_

class Block {
public:
	Block(unsigned BlockSize);
	Block(const Block &block);
	virtual ~Block();
	void* getBlock();
	void setBlock(void*);
	unsigned getsize() const;
protected:
	unsigned BlockSize;
	void* start;
};

#endif /* BLOCK_H_ */
