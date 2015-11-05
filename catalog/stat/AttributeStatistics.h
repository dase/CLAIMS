/*
 * AttributeStatistics.h
 *
 *  Created on: Feb 2, 2014
 *      Author: wangli
 */

#ifndef ATTRIBUTESTATISTICS_H_
#define ATTRIBUTESTATISTICS_H_
#include <map>

#include "../../catalog/stat/Statistic.h"
class AttributeStatistics {
public:
	AttributeStatistics();
	virtual ~AttributeStatistics();
	void setDistinctCardinality(unsigned long int value);
	void setHistogram(Histogram* his);
	Histogram* getHistogram()const;
	void print()const;
	unsigned long getDistinctCardinality()const;
private:
	unsigned long distinct_cardinality_;
	void* max_;
	void* min_;
	Histogram* histogram_;
};

#endif /* ATTRIBUTESTATISTICS_H_ */
