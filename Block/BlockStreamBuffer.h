/*
 * BlockStreamBuffer.h
 *	This class is the buffer of several BlockStreams.
 *	The access to this buffer is thread-safe.
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMBUFFER_H_
#define BLOCKSTREAMBUFFER_H_
#include <list>
#include "BlockStream.h"
#include "../Schema/Schema.h"
#include "synch.h"
class BlockStreamBuffer {
public:
	BlockStreamBuffer(unsigned block_size, unsigned block_count, Schema* schema);
	virtual ~BlockStreamBuffer();
	void insertBlock(BlockStreamBase* block);
	bool getBlock(BlockStreamBase &block);
	unsigned getBlockInBuffer();
	bool Empty() ;
private:
	std::list<BlockStreamBase*> block_stream_empty_list_;
	std::list<BlockStreamBase*> block_stream_used_list_;
	semaphore sema_empty_block_;
	Lock lock_;
};

#endif /* BLOCKSTREAMBUFFER_H_ */
