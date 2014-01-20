/*
 * DynamicBlockBuffer.h
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */

#ifndef DYNAMICBLOCKBUFFER_H_
#define DYNAMICBLOCKBUFFER_H_
#include <vector>
#include "Block.h"
#include "synch.h"
/**
 * A container which buffers serialized blocks. The number of blocks in the buffer is not known when constructing
 * and is growing dynamically on the fly.
 */



class DynamicBlockBuffer {
public:
	class Iterator{
	public:
		Iterator(const DynamicBlockBuffer* dbb);
		Iterator(const Iterator &);
		Iterator();
		Block* nextBlock();
		Block* atomicNextBlock();
	private:
		unsigned cur_;
		const DynamicBlockBuffer* dbb_;
		Lock lock_;
	};
	DynamicBlockBuffer();
	virtual ~DynamicBlockBuffer();

	bool appendNewBlock(Block* new_block);
	bool atomicAppendNewBlock(Block* new_block);
	Block* getBlock(unsigned index)const;
	Iterator createIterator()const;


	unsigned getNumberOfBlocks();
	/* release the memory in block list.
	 * NOTE: this function should be used carefully, as the block memory is not allocated by this class.
	 */
	void destory();
private:
	std::vector<Block*> block_list_;
	Lock lock_;
};

#endif /* DYNAMICBLOCKBUFFER_H_ */
