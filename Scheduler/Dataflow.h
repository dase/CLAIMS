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
#include "../LogicalQueryPlan/DataflowPartitionDescriptor.h"
#include "../Schema/SchemaFix.h"
class Dataflow {
	/* describe the properties of the data flow*/
	friend class LogcalOperator;
	struct property{
		DataflowPartitioningDescriptor partitioner;
		unsigned long commnication_cost;
		property():commnication_cost(0){

		}
		property(const property& r){
			partitioner=r.partitioner;
			commnication_cost=r.commnication_cost;
		}
	};
public:
	Dataflow();
	Dataflow(const Dataflow& r);
	virtual ~Dataflow();
	unsigned long getAggregatedDatasize()const;
	unsigned long getAggregatedDataCardinality()const;
	bool isHashPartitioned()const;
	Schema* getSchema()const;
	unsigned getTupleSize()const;
	Attribute getAttribute(std::string name)const;
	Attribute getAttribute(std::string name,std::string attname)const;
public:
	std::vector<Attribute> attribute_list_;
	property property_;

};

#endif /* DATAFLOW_H_ */
