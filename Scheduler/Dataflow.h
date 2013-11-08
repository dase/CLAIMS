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
class Dataflow {
	/* describe the properties of the data flow*/
	struct property{
		Partitioner* partitoiner;
		unsigned commnication_cost;
	};
public:
	Dataflow();
	virtual ~Dataflow();
private:
	std::vector<Attribute> attribute_list_;
	property property_;

};

#endif /* DATAFLOW_H_ */
