/*
 * Sort.h
 *
 *  Created on: 2014-3-1
 *      Author: casa
 */

#ifndef LOGICALSORT_H_
#define LOGICALSORT_H_

#include "../LogicalQueryPlan/LogicalOperator.h"
#include "../Scheduler/Dataflow.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamSortIterator.h"

class LogicalSort: public LogicalOperator{
public:
	LogicalSort(LogicalOperator *child, int sortByKey);
	virtual ~LogicalSort();

	Dataflow getDataflow();
	BlockStreamIteratorBase *getIteratorTree(const unsigned& blocksize);

	virtual bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024){};
	virtual void print(int level=0)const{};

private:
	LogicalOperator *child_;
	int sortByKey_;
};

#endif /* SORT_H_ */
