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
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "BlockStream.h"
#include "../../common/Schema/Schema.h"
#include "../../common/Block/MonitorableBuffer.h"
#include "../../utility/lock.h"
class BlockStreamBuffer : public MonitorableBuffer {
 public:
  BlockStreamBuffer(unsigned block_size, unsigned block_count, Schema* schema);
  virtual ~BlockStreamBuffer();
  void insertBlock(BlockStreamBase* block);
  bool InsertOneBlock(Block* const block);
  void InsertOneBlock(BlockStreamBase*& block);
  bool getBlock(BlockStreamBase& block);
  unsigned getBlockInBuffer();
  bool Empty();
  bool ReturnEmptyBlock(BlockStreamBase*& block);
  double getBufferUsage();
  long getReceivedDataSizeInKbytes();
  bool getEmptyBlock(BlockStreamBase*& block);

 private:
  std::list<BlockStreamBase*> block_stream_empty_list_;
  std::list<BlockStreamBase*> block_stream_used_list_;
  semaphore sema_empty_block_;
  Lock lock_;

  unsigned total_block_count_;
  unsigned block_size_;
  unsigned long received_block_count_;
};

#endif /* BLOCKSTREAMBUFFER_H_ */
