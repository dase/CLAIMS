/*
 * Project.h
 *
 *  Created on: 2014-2-21
 *      Author: casa
 */

#ifndef PROJECT_H_
#define PROJECT_H_

#include "../common/Mapping.h"
#include "../common/ExpressionItem.h"
#include "LogicalOperator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamProjectIterator.h"
#include <vector>

class Project:public LogicalOperator{
public:
	Project(LogicalOperator *child, std::vector<std::vector<ExpressionItem> > &exprArray);
	virtual ~Project();

	Dataflow getDataflow();
	BlockStreamIteratorBase *getIteratorTree(const unsigned& blocksize);

private:
	Mapping* getMapping();
	int getColumnSeq(ExpressionItem *ei);

private:
	LogicalOperator *child_;
	std::vector<std::vector<ExpressionItem> > exprArray_;
};

#endif /* PROJECT_H_ */
