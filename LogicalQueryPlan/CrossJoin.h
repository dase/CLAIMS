/*
 * CrossJoin.h
 *
 *  Created on: Jul 14, 2014
 *      Author: imdb
 */

#ifndef CROSSJOIN_H_
#define CROSSJOIN_H_

#include "LogicalOperator.h"

class CrossJoin: public LogicalOperator {
public:
	CrossJoin();
	CrossJoin(LogicalOperator* left_input,LogicalOperator* right_input);
	virtual ~CrossJoin();
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned& blocksize);
	void print(int level=0)const;
private:
	LogicalOperator* left_child_;
	LogicalOperator* right_child_;
	Dataflow* dataflow_;
};

#endif /* CROSSJOIN_H_ */
