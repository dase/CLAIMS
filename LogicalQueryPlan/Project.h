/*
 * Project.h
 *
 *  Created on: 2014-2-21
 *      Author: casa
 */

#ifndef LOGICALPROJECT_H_
#define LOGICALPROJECT_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/Mapping.h"
#include "../common/ExpressionItem.h"
#include "../common/TypePromotionMap.h"
#include "../common/TypeCast.h"
#include "../Catalog/Catalog.h"
#include "LogicalOperator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamProjectIterator.h"
#include <vector>
#include <sstream>
#include <map>
#include "../common/Expression/qnode.h"

class LogicalProject:public LogicalOperator{
public:
	LogicalProject(LogicalOperator *child, std::vector<std::vector<ExpressionItem> > &exprArray);
	LogicalProject(LogicalOperator *child, vector<QNode *>exprTree);

	virtual ~LogicalProject();

	Dataflow GetDataflow();
	BlockStreamIteratorBase *GetIteratorTree(const unsigned& blocksize);

	virtual bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024){};
	void printProjSchema()const;
	void Print(int level=0)const;

private:
	Mapping getMapping();
	Schema *getOutputSchema();
	int getColumnSeq(ExpressionItem &ei);
	string recovereyName(Expression ei);
	bool getcolindex(Dataflow dataflow);
private:
	Mapping mappings_;
	Dataflow *dataflow_;
	LogicalOperator *child_;
	std::vector<Expression> exprArray_;

	std::vector<QNode *>exprTree_;
	std::map<std::string,int>colindex_;
};

#endif /* LOGICALPROJECT_H_ */
