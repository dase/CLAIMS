/*
 * BufferManager.h
 *
 *  Created on: Nov 16, 2013
 *      Author: wangli
 */

#ifndef BUFFERMANAGER_H_
#define BUFFERMANAGER_H_
#include "../storage/MemoryStore.h"
#include "../Block/synch.h"
#include "../Logging.h"
class BufferManager {
public:
	static BufferManager* getInstance();
	virtual ~BufferManager();
	bool applyStorageDedget(unsigned long size);
	unsigned getStorageMemoryBudegeInMilibyte()const;
private:
	BufferManager();
	unsigned long int getTotalUsed()const;
private:
	unsigned long int totol_capacity_;
	unsigned long int storage_budget_max_;
	unsigned long int storage_used_;
	unsigned long int storage_budget_min_;
	unsigned long int intermediate_buffer_budget_max_;
	unsigned long int intermediate_buffer_budget_min_;
	unsigned long int intermediate_buffer_used_;
	MemoryChunkStore* memory_storage_;
	static BufferManager* instance_;
	Lock lock_;
	Logging* logging_;
};

#endif /* BUFFERMANAGER_H_ */
