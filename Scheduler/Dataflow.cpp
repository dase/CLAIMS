/*
 * DataFlow.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "Dataflow.h"
#include "../common/Schema/SchemaFix.h"

Dataflow::Dataflow() {
	// TODO Auto-generated constructor stub
}

Dataflow::~Dataflow() {
	// TODO Auto-generated destructor stub
}
Dataflow::Dataflow(const Dataflow& r){
	attribute_list_=r.attribute_list_;
	property_=r.property_;
}
unsigned long Dataflow::getAggregatedDatasize()const{
	const unsigned tuple_size=getTupleSize();
	return property_.partitioner.getAggregatedDataCardinality()*tuple_size;
}
unsigned long Dataflow::getAggregatedDataCardinality()const{
	return property_.partitioner.getAggregatedDataCardinality();
}
bool Dataflow::isHashPartitioned()const{
	return property_.partitioner.getPartitionFashion()==PartitionFunction::hash_f;
}
Schema* Dataflow::getSchema()const{
	/**
	 * Only fixed schema is supported now.
	 * TODO: support other schemas.
	 */
	std::vector<column_type> columns;
	for(unsigned i=0;i<attribute_list_.size();i++){
		columns.push_back(*attribute_list_[i].attrType);
	}
	return new SchemaFix(columns);
}
unsigned Dataflow::getTupleSize()const{
	unsigned ret=0;
	for(unsigned i=0;i<attribute_list_.size();i++){
		ret+=attribute_list_[i].attrType->get_length();
	}
	return ret;
}
Attribute Dataflow::getAttribute(std::string name)const{
	for(unsigned i=0;i<attribute_list_.size();i++){
		if(attribute_list_[i].attrName==name){
			return attribute_list_[i];
		}
	}
	printf("Failed to find attribute [%s]\n",name.c_str());
	assert(false);
	return Attribute(ATTRIBUTE_NULL);
}

Attribute Dataflow::getAttribute(std::string tbname,std::string colname)const{
	for(unsigned i=0;i<attribute_list_.size();i++){
		if(attribute_list_[i].attrName==colname){
			return attribute_list_[i];
		}
	}
	printf("Failed to find attribute [%s]\n",colname.c_str());

	assert(false);
	return Attribute(ATTRIBUTE_NULL);
}
