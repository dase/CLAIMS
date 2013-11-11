/*
 * DataFlow.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "Dataflow.h"

Dataflow::Dataflow() {
	// TODO Auto-generated constructor stub

}

Dataflow::~Dataflow() {
	// TODO Auto-generated destructor stub
}
Dataflow::Dataflow(const Dataflow& r){
	this->attribute_list_=r.attribute_list_;
	this->property_=r.property_;
}
unsigned Dataflow::getAggregatedDatasize()const{
	return property_.partitioner.getAggregatedDatasize();
}

bool Dataflow::isHashPartitioned()const{
	return property_.partitioner.getPartitionFashion()==PartitionFunction::hash_f;
}
