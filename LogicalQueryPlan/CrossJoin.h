/*
 * CrossJoin.h
 *
 *  Created on: Jul 14, 2014
 *      Author: fzhedu, wangli1426
 */

#ifndef CROSSJOIN_H_
#define CROSSJOIN_H_

#include "LogicalOperator.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"

class CrossJoin: public LogicalOperator {
public:
	enum join_police{left_broadcast, right_broadcast, local_join};
	CrossJoin();
	CrossJoin(LogicalOperator* left_input,LogicalOperator* right_input);
	virtual ~CrossJoin();
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned& blocksize);
	void print(int level=0)const;
private:

	/** return true if the nest-loop-join can be conducted locally**/
	bool canLocalJoin(Dataflow &left, Dataflow &right) const;

	void generateChildPhysicalQueryPlan(
			BlockStreamIteratorBase* & left_child_iterator_tree,
			BlockStreamIteratorBase* & right_child_iterator_tree, const unsigned & blocksize);

	LogicalOperator* left_child_;
	LogicalOperator* right_child_;
	Dataflow* dataflow_;
	join_police join_police_;
};

#endif /* CROSSJOIN_H_ */
