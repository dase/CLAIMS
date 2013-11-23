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
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
class Aggregation: public LogicalOperator {
public:
	enum fashion{no_repartition,repartition};
	Aggregation(std::vector<Attribute> group_by_attribute,std::vector<Attribute> aggregation_attribute,std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_list,LogicalOperator* child);
	virtual ~Aggregation();
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned & block_size);
	std::vector<unsigned> getInvolvedIndexList(const std::vector<Attribute>& attribute_list,const Dataflow& dataflow )const;
private:
	bool canLeverageHashPartition(const Dataflow& child_dataflow)const;
	float predictSelectivity()const;
private:
	LogicalOperator* child_;
	std::vector<Attribute> group_by_attribute_list_;
	std::vector<Attribute> aggregation_attribute_list_;
	std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_list_;
	Dataflow* dataflow_;
	fashion fashion_;
};

#endif /* AGGREGATION_H_ */
