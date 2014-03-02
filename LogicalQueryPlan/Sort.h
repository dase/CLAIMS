/*
 * Sort.h
 *
 *  Created on: 2014-3-1
 *      Author: casa
 */

#ifndef SORT_H_
#define SORT_H_

#include "../LogicalQueryPlan/LogicalOperator.h"
#include "../Scheduler/Dataflow.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamSortIterator.h"

class Sort: public LogicalOperator{
public:
	Sort(LogicalOperator *child, int sortByKey);
	virtual ~Sort();

	Dataflow getDataflow();
	BlockStreamIteratorBase *getIteratorTree(const unsigned& blocksize);

private:
	LogicalOperator *child_;
	int sortByKey_;
};

#endif /* SORT_H_ */
