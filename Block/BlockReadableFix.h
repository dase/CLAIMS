/*
 * BlockReadbaleFix.h
 *
 *  Created on: Jun 25, 2013
 *      Author: wangli
 */

#ifndef BLOCKREADBALEFIX_H_
#define BLOCKREADBALEFIX_H_
#include "BlockReadable.h"
#include "../common/Schema/Schema.h"
class BlockReadableFix:public BlockReadable {
public:
	BlockReadableFix(Block block, Schema* schema);
	BlockReadableFix(unsigned BlockSize, Schema* schema);
	virtual ~BlockReadableFix();
	void reset();
	void* next();
	unsigned getMaxTupleCount();
	unsigned getActualTupleCount();
	bool IsLastBlock();
	unsigned getS();
    char *getCur() const;
//private:
	char* getend();
    void setCur(char *cur);
private:
	char* cur;
	char* end;
	Schema* schema;
};

#endif /* BLOCKREADBALEFIX_H_ */
