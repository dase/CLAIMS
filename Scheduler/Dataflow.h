/*
 * DataFlow.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#ifndef DATAFLOW_H_
#define DATAFLOW_H_
#include <vector>
#include "../Catalog/Column.h"
#include "../Catalog/Partitioner.h"
#include "DataflowPartitionDescriptor.h"
class Dataflow {
	/* describe the properties of the data flow*/
	struct property{
		DataflowPartitionDescriptor partitioner;
		unsigned commnication_cost;
		property(const property& r){
			partitioner=r.partitioner;
			commnication_cost=r.commnication_cost;
		}
	};
public:
	Dataflow();
	Dataflow(const Dataflow& r);
	virtual ~Dataflow();
	unsigned getAggregatedDatasize()const;
private:
	std::vector<Attribute> attribute_list_;
	property property_;

};

#endif /* DATAFLOW_H_ */
