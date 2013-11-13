/*
 * LogicalQueryPlanRoot.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#include "LogicalQueryPlanRoot.h"
#include "../Resource/NodeTracker.h"
#include "../IDsGenerator.h"
LogicalQueryPlanRoot::LogicalQueryPlanRoot(NodeID collecter,LogicalOperator* child)
:collecter_(collecter),child_(child){
	// TODO Auto-generated constructor stub

}

LogicalQueryPlanRoot::~LogicalQueryPlanRoot() {
	// TODO Auto-generated destructor stub
}

BlockStreamIteratorBase* LogicalQueryPlanRoot::getIteratorTree(const unsigned& block_size){
	BlockStreamIteratorBase* child_iterator=child_->getIteratorTree(block_size);
	Dataflow dataflow=child_->getDataflow();
	Schema* schema=getSchema(dataflow.attribute_list_);
	NodeTracker* node_tracker=NodeTracker::getInstance();
	ExpandableBlockStreamExchangeEpoll::State state;
	state.block_size=block_size;
	state.child=child_iterator;
	state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();

	state.upper_ip_list.push_back(node_tracker->getNodeIP(collecter_));

	std::vector<NodeID> lower_id_list=getInvolvedNodeID(dataflow.property_.partitioner);
	for(unsigned i=0;i<lower_id_list.size();i++){
		const std::string ip=node_tracker->getNodeIP(lower_id_list[i]);
		assert(ip!=NULL);
		state.lower_ip_list.push_back(ip);
	}

	ExpandableBlockStreamExchangeEpoll* exchange=new ExpandableBlockStreamExchangeEpoll(state);
	return exchange;
}
