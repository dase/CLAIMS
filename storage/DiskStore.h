/*
 * DiskStore.h
 *
 *  Created on: 2013-10-11
 *      Author: casa
 */

#ifndef DISKSTORE_H_
#define DISKSTORE_H_

/*
 * 由于存取这个过程还没有经历过，所以放在以后补充
 * */
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

#include "BlockStore.h"

#include <sys/stat.h>
#include <sys/types.h>

class DiskStore{
public:
	DiskStore(string rootDirs);
	virtual ~DiskStore();

	// 存到磁盘上的时候是用blockId来命名的
	bool putValue(string blockId,void *value){
		return true;
	}
	void *getChunk(string blockId){
		// todo: 将blockId对应的文件mmap到内存中，然后返回内存的指针
	}

	bool remove(string blockId){
		return true;
	}
	bool contains(string blockId){
		return true;
	}
	unsigned getSize(string blockId){
		return 0;
	}

	bool getFile(string blockId){
		return true;
	}
	// 创建给定路径的目录
	bool createDirs(string rootDirs);

private:
	string rootDirs_;
};

#endif /* DISKSTORE_H_ */
