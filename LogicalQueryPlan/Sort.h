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
#include "../Catalog/Catalog.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamSortIterator.h"

class LogicalSort: public LogicalOperator{
public:
	struct OrderByAttr{
		OrderByAttr(const char *t,const char *a){
			tbl_=t;attr_=a;
		}
		const char *tbl_;
		const char *attr_;
	};

	LogicalSort(LogicalOperator *child, vector<OrderByAttr*> oba);
	virtual ~LogicalSort();

	Dataflow getDataflow();
	BlockStreamIteratorBase *getIteratorTree(const unsigned& blocksize);

	int getOrderByKey(const char *,const char *);

	virtual bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024){};
	virtual void print(int level=0)const{};

private:
	vector<OrderByAttr*> oba_;
	Dataflow dataflow_;
	LogicalOperator *child_;
};

#endif /* SORT_H_ */
