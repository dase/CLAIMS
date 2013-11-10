/*
 * DataflowBranch.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#include "DataflowPartition.h"

DataflowPartition::DataflowPartition(unsigned partition_id,int datasize,NodeID location)
:partition_id_(partition_id),datasize_(datasize),location_(location){

}

DataflowPartition::~DataflowPartition() {
	// TODO Auto-generated destructor stub
}
DataflowPartition::DataflowPartition(const DataflowPartition& part){
	this->partition_id_=part.partition_id_;
	this->location_=part.location_;
	this->datasize_=part.datasize_;
}

