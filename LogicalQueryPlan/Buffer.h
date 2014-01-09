/*
 * Buffer.h
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "LogicalOperator.h"

class Buffer: public LogicalOperator {
public:
	Buffer(LogicalOperator* child);
	virtual ~Buffer();
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned &);
private:
	LogicalOperator* child_;
	Dataflow dataflow_;
};

#endif /* BUFFER_H_ */
