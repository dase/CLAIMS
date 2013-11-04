/*
 * BlockFix.cpp
 *
 *  Created on: 2013-8-23
 *      Author: zhanglei
 */

#include "BlockFix.h"
BlockFix::BlockFix(unsigned BlockSize,SchemaFix* _schema):
schema(_schema),_free((char*)start),tuplecount(0),BlockSize(BlockSize) {
	// TODO 自动生成的构造函数存根
	start=(char*)memalign(cacheline_size,BlockSize);
	cur=(char *)start;

	tuple_size=schema->getTupleSize();

	//zhanglei
}
BlockFix::BlockFix(Block &block,SchemaFix* schema)
:schema(schema){
	start=(char*)block.getBlock();
	BlockSize=block.getsize();
	cur=(char *)start;
	tuple_size=schema->getTupleSize();
}
BlockFix::~BlockFix() {
	// TODO 自动生成的析构函数存根
//	free(start);
}

