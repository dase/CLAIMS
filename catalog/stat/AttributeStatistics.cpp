/*
 * AttributeStatistics.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: wangli
 */

#include "../../catalog/stat/AttributeStatistics.h"

AttributeStatistics::AttributeStatistics():min_(0),max_(0),histogram_(0),distinct_cardinality_(-1) {
	// TODO Auto-generated constructor stub

}

AttributeStatistics::~AttributeStatistics() {
	// TODO Auto-generated destructor stub
}

void AttributeStatistics::setDistinctCardinality(unsigned long int value){
	distinct_cardinality_=value;
}
void AttributeStatistics::print()const{
	printf("Distinct value: %d\n",distinct_cardinality_);
}
void AttributeStatistics::setHistogram(Histogram* his){
	histogram_=his;
}
Histogram* AttributeStatistics::getHistogram()const{
	return histogram_;
}
unsigned long AttributeStatistics::getDistinctCardinality()const{
	return distinct_cardinality_;
}
