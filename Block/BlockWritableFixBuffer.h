/*
 * BlockWritableFixBuffer.h
 *
 *  Created on: 2013-7-8
 *      Author: zhanglei
 */

#ifndef BLOCKWRITABLEFIXBUFFER_H_
#define BLOCKWRITABLEFIXBUFFER_H_

#include "BlockWritableFix.h"
#include "../Schema/Schema.h"

class BlockWritableFixBuffer {
public:
	BlockWritableFixBuffer(Schema *schema,unsigned BlockSize,unsigned BlockNum,unsigned curBlock);
	virtual ~BlockWritableFixBuffer();
	//when init the BlockBufferWritableFix, Block array list must be allocate
	//memory, Currently, choose non-sequential allocation
	//TODO: support sequential memory allocation
	bool init();
	//Currently, the hash function is defined as a static function here
	//TODO: support the hash function is transferred from global master
	unsigned hash(void* tuple);
	//in the insert function, insert a bucket which is waiting for being
	//transferred to the upper node
	unsigned insert(void* tuple);
    BlockWritableFix** getStart() const;
    unsigned getBlockNum() const;
private:
	BlockWritableFix** start;
	Schema *schema;
	unsigned BlockSize;
	unsigned BlockNum;
	unsigned curBlock;
};

#endif /* BLOCKWRITABLEFIXBUFFER_H_ */
