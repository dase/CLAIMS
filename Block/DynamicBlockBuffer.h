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
#include "BlockStream.h"
/**
 * A container which buffers serialized blocks. The number of blocks in the buffer is not known when constructing
 * and is growing dynamically on the fly.
 */



class DynamicBlockBuffer {
public:
	class Iterator {
	public:
		Iterator(const DynamicBlockBuffer* dbb);
		Iterator(const Iterator &);
		Iterator();
		BlockStreamBase* nextBlock();
		BlockStreamBase* atomicNextBlock();
	private:
		unsigned cur_;
		const DynamicBlockBuffer* dbb_;
		Lock lock_;
	};
	DynamicBlockBuffer();
	DynamicBlockBuffer(const DynamicBlockBuffer& r);
	virtual ~DynamicBlockBuffer();

	bool appendNewBlock(BlockStreamBase* new_block);
	bool atomicAppendNewBlock(BlockStreamBase* new_block);
	BlockStreamBase* getBlock(unsigned index) const;
	Iterator createIterator() const;
	


	unsigned getNumberOfBlocks();
	/* release the memory in block list.
	 * NOTE: this function should be used carefully, as the block memory is not allocated by this class.
	 */
	void destory();
	unsigned long getNumberOftuples()const;
private:
	std::vector<BlockStreamBase*> block_list_;
	Lock lock_;
};

#endif /* DYNAMICBLOCKBUFFER_H_ */
