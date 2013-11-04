/*
 * BlockReadableFixBuffer.h
 *
 *  Created on: 2013-7-8
 *      Author: zhanglei
 */

#ifndef BLOCKREADABLEFIXBUFFER_H_
#define BLOCKREADABLEFIXBUFFER_H_

#include "BlockReadableFix.h"
#include "../Schema/Schema.h"
#include <iostream>
using namespace std;

class BlockReadableFixBuffer {
public:
	BlockReadableFixBuffer(Schema *schema,unsigned BlockSize,unsigned BlockNum,unsigned curBlock);
	virtual ~BlockReadableFixBuffer();
	//when init the BlockBufferWritableFix, Block array list must be allocate
	//memory, Currently, choose non-sequential allocation
	//TODO: support sequential memory allocation
	bool init();
	void* next();
	bool nextBlock();
    unsigned getCurBlock() const;
    BlockReadableFix** getStart() const;
    void setBlockSize(unsigned  BlockSize);
    void setCurBlock(unsigned  curBlock);
private:
	BlockReadableFix** start;
	Schema *schema;
	unsigned BlockSize;
	//Currently, the buffer has only one block corresponding to the lower nodes.
	//TODO: support a buffer which has a "buffer" corresponding to every lower nodes.
	unsigned BlockNum;
	//the block number which is used now
	unsigned curBlock;
};

#endif /* BLOCKREADABLEFIXBUFFER_H_ */
