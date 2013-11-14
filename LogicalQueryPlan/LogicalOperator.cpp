/*
 * LogicalOperator.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#include "LogicalOperator.h"

//LogicalOperator::LogicalOperator() {
//	// TODO Auto-generated constructor stub
//
//}
//
//LogicalOperator::~LogicalOperator() {
//	// TODO Auto-generated destructor stub
//}
//
Schema* LogicalOperator::getSchema(std::vector<Attribute> &attributes)const{
	std::vector<column_type> columns;
	for(unsigned i=0;i<attributes.size();i++){
		columns.push_back(*attributes[i].attrType);
	}
	return new SchemaFix(columns);

}
std::vector<NodeID> LogicalOperator::getInvolvedNodeID(DataflowPartitionDescriptor& part)const{
	std::vector<NodeID> ret;
	for(unsigned i=0;i<part.getNumberOfPartitions();i++){
		ret.push_back(part.getPartition(i)->getLocation());
	}
	return ret;
}
