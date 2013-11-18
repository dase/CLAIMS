/*
 * LogicalQueryPlanRoot.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#include "LogicalQueryPlanRoot.h"
#include "../Resource/NodeTracker.h"
#include "../IDsGenerator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/BlockStreamPrint.h"
#include "../PerformanceMonitor/BlockStreamPerformanceMonitorTop.h"
LogicalQueryPlanRoot::LogicalQueryPlanRoot(NodeID collecter,LogicalOperator* child,const outputFashion& fashion)
:collecter_(collecter),child_(child),fashion_(fashion){
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
	state.schema=schema;
	state.upper_ip_list.push_back(node_tracker->getNodeIP(collecter_));
	state.partition_key_index=0;
	std::vector<NodeID> lower_id_list=getInvolvedNodeID(dataflow.property_.partitioner);
	for(unsigned i=0;i<lower_id_list.size();i++){
		const std::string ip=node_tracker->getNodeIP(lower_id_list[i]);
		assert(ip!="");
		state.lower_ip_list.push_back(ip);
	}

	BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(state);

	BlockStreamIteratorBase* ret;
	switch(fashion_){
		case PRINT:{
			BlockStreamPrint::State print_state(schema,exchange,block_size);
			ret=new BlockStreamPrint(print_state);
			break;
		}
		case PERFORMANCE:{
			BlockStreamPerformanceMonitorTop::State performance_state(schema,exchange,block_size);
			ret=new BlockStreamPerformanceMonitorTop(performance_state);
			break;
		}
	}

	return ret;
}
Dataflow LogicalQueryPlanRoot::getDataflow(){
	return child_->getDataflow();
}
