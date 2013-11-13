/*
 * LogicalQueryPlanRoot.h
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#ifndef LOGICALQUERYPLANROOT_H_
#define LOGICALQUERYPLANROOT_H_
#include "LogicalOperator.h"
#include "../ids.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
class LogicalQueryPlanRoot: public LogicalOperator {
public:
	LogicalQueryPlanRoot(NodeID collecter,LogicalOperator* child);
	virtual ~LogicalQueryPlanRoot();
	Dataflow getDataflow()=0;
	BlockStreamIteratorBase* getIteratorTree(const unsigned&);
private:
	NodeID collecter_;
	LogicalOperator* child_;

};

#endif /* LOGICALQUERYPLANROOT_H_ */
