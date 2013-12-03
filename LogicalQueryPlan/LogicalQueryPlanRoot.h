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
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
class LogicalQueryPlanRoot: public LogicalOperator {
public:
	enum outputFashion{PRINT,PERFORMANCE};
	LogicalQueryPlanRoot(NodeID collecter,LogicalOperator* child,const outputFashion& fashion=PERFORMANCE);
	virtual ~LogicalQueryPlanRoot();
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned&);
private:
	NodeID collecter_;
	LogicalOperator* child_;
	outputFashion fashion_;

};

#endif /* LOGICALQUERYPLANROOT_H_ */
