/*
 * DataflowPartitioner.h
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#ifndef DATAFLOWPARTITIONER_H_
#define DATAFLOWPARTITIONER_H_
#include <vector>
#include "../Catalog/Partitioner.h"
#include "DataflowPartition.h"
/**
 * describe how the data flow are partitioned and the partition property between them.
 */
class DataflowPartitionDescriptor {
public:
	DataflowPartitionDescriptor():partition_function_(0),partition_key_(0){};
	DataflowPartitionDescriptor(const Partitioner& partitoiner);
	virtual ~DataflowPartitionDescriptor();
	Attribute getPartitionKey()const;
	bool hasSamePartitionLocation(const DataflowPartitionDescriptor& target)const;
	unsigned getAggregatedDatasize()const;
	PartitionFunction::partition_fashion getPartitionFashion()const;
	unsigned getNumberOfPartitions()const;
	 DataflowPartition* getPartition(unsigned index)const;
private:
	std::vector<DataflowPartition> partition_list_;
	PartitionFunction* partition_function_;
	Attribute* partition_key_;
};

#endif /* DATAFLOWPARTITIONER_H_ */
