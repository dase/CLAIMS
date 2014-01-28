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
	enum outputFashion{PRINT,PERFORMANCE,RESULTCOLLECTOR};
	LogicalQueryPlanRoot(NodeID collecter,LogicalOperator* child,const outputFashion& fashion=PERFORMANCE);
	virtual ~LogicalQueryPlanRoot();
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned&);
	bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024);
private:
	void print(int level=0)const;
	std::vector<std::string> getAttributeName(const Dataflow& dataflow)const;
private:
	NodeID collecter_;
	LogicalOperator* child_;
	outputFashion fashion_;

};

#endif /* LOGICALQUERYPLANROOT_H_ */
