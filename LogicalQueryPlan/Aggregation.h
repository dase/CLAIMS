/*
 * Aggregation.h
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#ifndef AGGREGATION_H_
#define AGGREGATION_H_
#include <vector>
#include "LogicalOperator.h"
#include "../Catalog/Attribute.h"
#include "../iterator/AggregationIterator.h"
class Aggregation: public LogicalOperator {
public:
	Aggregation(std::vector<Attribute> group_by_attribute,std::vector<Attribute> aggregation_attribute,std::vector<AggregationIterator::State::aggregation> aggregation_list,LogicalOperator* child);
	virtual ~Aggregation();
	Dataflow getDataflow();
private:
	bool canLeverageHashPartition(const Dataflow& child_dataflow)const;
private:
	LogicalOperator* child_;
	std::vector<Attribute> group_by_attribute_list_;
	std::vector<Attribute> aggregation_attribute_list_;
	std::vector<AggregationIterator::State::aggregation> aggregation_list_;
	Dataflow* dataflow_;
};

#endif /* AGGREGATION_H_ */
