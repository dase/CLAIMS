/*
 * AttributeStatistics.h
 *
 *  Created on: Feb 2, 2014
 *      Author: wangli
 */

#ifndef ATTRIBUTESTATISTICS_H_
#define ATTRIBUTESTATISTICS_H_
#include <map>
#include "Statistic.h"
class AttributeStatistics {
public:
	AttributeStatistics();
	virtual ~AttributeStatistics();
	void setDistinctCardinality(unsigned long int value);
	void setHistogram(Histogram* his);
	void print()const;
private:
	unsigned long distinct_cardinality_;
	void* max_;
	void* min_;
	Histogram* histogram_;
};

#endif /* ATTRIBUTESTATISTICS_H_ */
