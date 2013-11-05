/*
 * BlockStore.h
 *
 *  Created on: 2013-10-20
 *      Author: casa
 */

#ifndef BLOCKSTORE_H_
#define BLOCKSTORE_H_

/*
 * BlockStore中的函数只不过是存取数据，然后有各自地，有达到存取产生的一些附加操作，
 * 比如，内存不足的时候的交换策略，初步是以最近最久未使用的column来交换blockId
 * */
#include <string>
using namespace std;

class BlockStore{
public:
	BlockStore();
	virtual ~BlockStore();

	// 在会有序列化和反序列化的存取操作，但是这是以后测试之后的工作，
	// 首先不考虑序列化的情况，直接存储的是二进制
	virtual bool putValue(string blockId,void *value)=0;
	virtual void* getValue(string blockId)=0;

	virtual bool remove(string blockId)=0;
	virtual bool contains(string blockId)=0;
	virtual unsigned getSize(string blockId)=0;

};

#endif /* BLOCKSTORE_H_ */
