/*
 * MemoryStore.h
 *
 *  Created on: 2013-10-11
 *      Author: casa
 */

#ifndef MEMORYSTORE_H_
#define MEMORYSTORE_H_
/*
 * 是否应该有个手动清楚内存的方式？
 */

#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>

#include <string>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>
#include <iostream>
using namespace std;

#include "BlockStore.h"
#include "../rename.h"
#include "../Block/synch.h"
#include "../Debug.h"
#include <boost/pool/pool.hpp>

using boost::pool;

/* 在此的block内的模式信息以及有多少个tuple是不会存储在这里的，那个就
 * 是catalog来管理的，所以这个地方只是负责存取，知道那些信息也没用，那
 * 些信息是做优化的
 * */
struct HdfsBlock{
//	/*可以直接使用hdfs的blockid*/
//	unsigned blockId;
	HdfsBlock():hook(0),length(0){}
	HdfsBlock(void* add,int length):hook(add),length(length){}
	/*是将block mmap操作之后返回的内存地址*/
	void *hook;
	/*记录每个block大小也就是文件长度*/
	int length;
	// 是否被序列化过
};

typedef HdfsBlock HdfsInMemoryChunk;

/*
 * memorystore只是负责数据的存取，而和数据的管理和为什么存储是没有关系的，
 * 在负责数据存取的同时，put的时候还要看看内存够不够，如果不够就要开始内存
 * 空间的移除，在此有很多的策略选择，要将memstore写成单例模式
 * */
class MemoryChunkStore{
public:
	static MemoryChunkStore* getInstance();
	MemoryChunkStore();
	virtual ~MemoryChunkStore();

	/* todo:这里还有可能是直接存储对象或者存储将对象序列化之后的结果两种
	 * 在spark中要估计结果，所以有一个hdfsBlock中的length变量，在此留接口
	 * 如果我们要估计结果，要写一个类来做估计
	 * */
	bool putValue(string chunkId,void *value){
		// todo: 在此如果放不下了怎么办？采用什么样的交换策略，留接口，tryToPut中应该还有个接口是估计出来的值
		tryToPut(chunkId,value);
		return true;
	};


	bool applyChunk(ChunkID chunk_id,void*& start_address);

	void returnChunk(const ChunkID& chunk_id);

	void *getChunk(string blockId){
		map<string, HdfsBlock>::iterator it_;
		it_=bufferpool_.find(blockId);
		if(it_!=bufferpool_.end()){
			return it_->second.hook;
		}
	};
	bool getChunk(const ChunkID& chunk_id,HdfsInMemoryChunk& chunk_info)const;

	bool putChunk(const ChunkID& chunk_id,HdfsInMemoryChunk& chunk_info);

	bool remove(string blockId){
		return true;
	};

	bool contains(string blockId){
		return false;
	};

	unsigned getSize(string blockId){
		return 0;
	};

	/*
	 * 将block为单位放到buffer pool中
	 * */
	bool tryToPut(string chunkId,void *value){
		if(ensureFreeSpace()){
			lock_.acquire();
			HdfsBlock chunkin;
			chunkin.hook=(char *)value;
			// todo:接口，就先等于这个吧
			chunkin.length=CHUNK_SIZE;
			bufferpool_[chunkId.c_str()]=chunkin;
			lock_.release();
		}
		else{

		}
		return true;
	}

	bool ensureFreeSpace(){
		// todo: 基于LRU的column-based交换
		return true;
	}

	/* 有这个函数提供一个文件到block的映射,这个地方可以用iterator模式将其从
	 * master端获取，因为做iterator的节点肯定不是主节点，下面为调试用
	 * */
	vector<string> getFileLocation(string partition_file_name){
		vector<string> block_set;
		block_set.push_back("/home/casa/storage/data/1");
		block_set.push_back("/home/casa/storage/data/2");
		return block_set;
	}


private:
	map<string, HdfsBlock> bufferpool_;
	boost::unordered_map<ChunkID,HdfsInMemoryChunk> chunk_list_;
	// 本节点能使用的最大的内存，以兆为单位
	long maxMemory_;
	// 现在使用了多少内存？以兆为单位
	long currentMemory_;
	// 在存储进去buffer pool的时候要枷锁
	Lock lock_;

	pool<> chunk_pool_;
	pool<> block_pool_;

	static MemoryChunkStore* instance_;
};

#endif /* MEMORYSTORE_H_ */
