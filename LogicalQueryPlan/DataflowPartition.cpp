/*
 * DataflowBranch.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#include "DataflowPartition.h"

DataflowPartition::DataflowPartition(unsigned partition_id,int datasize,NodeID location)
:partition_id_(partition_id),datasize_(datasize),location_(location),filtered_(false){

}

DataflowPartition::~DataflowPartition() {
	// TODO Auto-generated destructor stub
}
DataflowPartition::DataflowPartition(const DataflowPartition& part){
	this->partition_id_=part.partition_id_;
	this->location_=part.location_;
	this->datasize_=part.datasize_;
	this->filtered_=part.filtered_;
}
unsigned DataflowPartition::getDataSize()const{
	if(filtered_)
		return 0;
	return datasize_;
}
void DataflowPartition::setDataSize(const unsigned & size){
	datasize_=size;
}
bool DataflowPartition::isAvaliable()const{
	return !filtered_;
}
void DataflowPartition::setFiltered(){
	filtered_=true;
	datasize_=0;
}
NodeID DataflowPartition::getLocation()const{
	return location_;
}
