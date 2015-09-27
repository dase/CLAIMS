/*
 * Buffer.h
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */

#ifndef BUFFER_H_
#define BUFFER_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "LogicalOperator.h"

class Buffer: public LogicalOperator {
public:
	Buffer(LogicalOperator* child);
	virtual ~Buffer();
	Dataflow GetDataflow();

	BlockStreamIteratorBase* GetIteratorTree(const unsigned &);
	void print(int level=0)const{

	}
private:
	LogicalOperator* child_;
	Dataflow dataflow_;
};

#endif /* BUFFER_H_ */
