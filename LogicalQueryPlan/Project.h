/*
 * Project.h
 *
 *  Created on: 2014-2-21
 *      Author: casa
 */

#ifndef LOGICALPROJECT_H_
#define LOGICALPROJECT_H_

#include "../common/Mapping.h"
#include "../common/ExpressionItem.h"
#include "../common/TypePromotionMap.h"
#include "../common/TypeCast.h"
#include "../Catalog/Catalog.h"
#include "LogicalOperator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamProjectIterator.h"
#include <vector>

class LogicalProject:public LogicalOperator{
public:
	LogicalProject(LogicalOperator *child, std::vector<std::vector<ExpressionItem> > &exprArray);
	virtual ~LogicalProject();

	Dataflow getDataflow();
	BlockStreamIteratorBase *getIteratorTree(const unsigned& blocksize);

	virtual bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024){};
	virtual void print(int level=0)const{};

private:
	Mapping getMapping();
	Schema *getOutputSchema();
	int getColumnSeq(ExpressionItem &ei);

private:
	Mapping mappings_;
	Dataflow dataflow_;
	LogicalOperator *child_;
	std::vector<Expression> exprArray_;
};

#endif /* LOGICALPROJECT_H_ */
