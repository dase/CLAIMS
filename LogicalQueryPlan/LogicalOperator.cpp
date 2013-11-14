/*
 * LogicalOperator.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#include "LogicalOperator.h"
#include "../Resource/NodeTracker.h"

//LogicalOperator::LogicalOperator() {
//	// TODO Auto-generated constructor stub
//
//}
//
//LogicalOperator::~LogicalOperator() {
//	// TODO Auto-generated destructor stub
//}
//
Schema* LogicalOperator::getSchema(const std::vector<Attribute> &attributes)const{
	std::vector<column_type> columns;
	for(unsigned i=0;i<attributes.size();i++){
		columns.push_back(*attributes[i].attrType);
	}
	return new SchemaFix(columns);

}
Schema* LogicalOperator::getSchema(const std::vector<Attribute>& attributes_left,const std::vector<Attribute>& attributes_right)const{
	std::vector<column_type> columns;
	for(unsigned i=0;i<attributes_left.size();i++){
		columns.push_back(*attributes_left[i].attrType);
	}
	for(unsigned i=0;i<attributes_right.size();i++){
		columns.push_back(*attributes_right[i].attrType);
	}
	return new SchemaFix(columns);
}
std::vector<NodeID> LogicalOperator::getInvolvedNodeID(const DataflowPartitionDescriptor& part)const{
	std::vector<NodeID> ret;
	for(unsigned i=0;i<part.getNumberOfPartitions();i++){
		ret.push_back(part.getPartition(i)->getLocation());
	}
	return ret;
}
std::vector<NodeIP> LogicalOperator::convertNodeIDListToNodeIPList(const std::vector<NodeID>& node_id_list)const{
	std::vector<NodeIP> ret;
	NodeTracker* nodetracker=NodeTracker::getInstance();
	for(unsigned i=0;i<node_id_list.size();i++){
		ret.push_back(nodetracker->getNodeIP(node_id_list[i]));
	}
	return ret;
}
