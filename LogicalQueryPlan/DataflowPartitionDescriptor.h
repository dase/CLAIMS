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
class DataflowPartitioningDescriptor {
public:
	DataflowPartitioningDescriptor():partition_function_(0){};
	DataflowPartitioningDescriptor(const Partitioner& partitoiner);
	DataflowPartitioningDescriptor(const DataflowPartitioningDescriptor&);
	virtual ~DataflowPartitioningDescriptor();
	Attribute getPartitionKey()const;
	bool hasSamePartitionLocation(const DataflowPartitioningDescriptor& target)const;
	unsigned getAggregatedDatasize()const;
	PartitionFunction::partition_fashion getPartitionFashion()const;
	unsigned getNumberOfPartitions()const;
	DataflowPartition* getPartition(unsigned index)const;

	std::vector<DataflowPartition> getPartitionList()const;
	void setPartitionList(const std::vector<DataflowPartition>&);


	void setPartitionFunction(PartitionFunction* partition_function);
	PartitionFunction* getPartitionFunction()const;

	void setPartitionKey(const Attribute& partitionkey);
	void addShadowPartitionKey(const Attribute& partitionkey);
	bool hasShadowPartitionKey()const;
	std::vector<Attribute> getShadowAttributeList()const;

private:
	std::vector<DataflowPartition> partition_list_;
	PartitionFunction* partition_function_;
	Attribute partition_key_;

	/*
	 * After the no-partitioning EqualHashJoin where the left join input is partitioned on
	 * A1 and the right join input is partitioned on A2, we can say that the generated
	 * data flow is either partitioned on A1 or partitioned on A2. In such cases, each
	 * attribute is a SHADOW PARTITION KEY to the other.*/
	std::vector<Attribute> shadow_partition_key_list_;
};

#endif /* DATAFLOWPARTITIONER_H_ */
