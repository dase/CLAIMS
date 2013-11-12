/*
 * Filter.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#ifndef FILTER_H_
#define FILTER_H_
#include <vector>
#include "LogicalOperator.h"
#include "../iterator/FilterIterator.h"
class Filter:public LogicalOperator {
public:
	Filter(std::vector<FilterIterator::AttributeComparator> ComparatorList,LogicalOperator* child );
	virtual ~Filter();
	Dataflow getDataflow();
private:
	bool couldHashPruned(unsigned partition_id,Partitioner* part);
private:
	LogicalOperator* child_;
	vector<FilterIterator::AttributeComparator> comparator_list_;

};

#endif /* FILTER_H_ */
