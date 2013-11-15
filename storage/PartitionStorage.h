/*
 * ProjectionStorage.h
 *
 *  Created on: Nov 14, 2013
 *      Author: wangli
 */

#ifndef PARTITIONSTORAGE_H_
#define PARTITIONSTORAGE_H_
#include "../ids.h"
class PartitionStorage {
public:
	PartitionStorage();
	virtual ~PartitionStorage();
private:
	PartitionID partition_id_;
	unsigned number_of_chunks;
};

#endif /* PARTITIONSTORAGE_H_ */
