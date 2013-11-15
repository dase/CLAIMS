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
	Block(const unsigned& size,const void* start);
	virtual ~Block();
	void* getBlock()const;
	void setBlock(void*);
	unsigned getsize() const;
	unsigned setsize(const unsigned& size) ;
protected:
	unsigned BlockSize;
	char* start;
private:
	/* true, if it references to others, which means that the block does not need
	 * to free the memory reference in destructor.
	 */
	bool isReference_;
};

#endif /* BLOCK_H_ */
