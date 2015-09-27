/*
 * DataflowBranch.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#include "../logical_query_plan/DataflowPartition.h"

DataflowPartition::DataflowPartition(unsigned partition_id,int datasize,NodeID location)
:partition_id_(partition_id),cardinality_(datasize),location_(location),filtered_(false){

}

DataflowPartition::~DataflowPartition() {
	// TODO Auto-generated destructor stub
}
DataflowPartition::DataflowPartition(const DataflowPartition& part){
	this->partition_id_=part.partition_id_;
	this->location_=part.location_;
	this->cardinality_=part.cardinality_;
	this->filtered_=part.filtered_;
}
unsigned DataflowPartition::getDataCardinality()const{
	if(filtered_)
		return 0;
	return cardinality_;
}
void DataflowPartition::setDataCardinality(const unsigned & size){
	cardinality_=size;
}
bool DataflowPartition::isAvaliable()const{
	return !filtered_;
}
void DataflowPartition::setFiltered(){
	filtered_=true;
	cardinality_=0;
}
NodeID DataflowPartition::getLocation()const{
	return location_;
}
