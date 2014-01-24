/*
 * Statistic.h
 *
 *  Created on: 2014年1月18日
 *      Author: volt
 */

#ifndef STATISTIC_H_
#define STATISTIC_H_


//#include <boost/unordered_map.hpp>

#include "../../../../../../imdb/supports/boost_1_53_0/boost/unordered_map.hpp"
#include "../../data_type.h"
#include "../../ids.h"

typedef void* ValuePtr;

//enum OperationType {
//	EQUAL, LESS
//};
class Estimation;

class Statistic {
	friend class Estimation;
public:
	Statistic();
	Statistic(unsigned bucketCnt);
	void setTupleCount(unsigned long tupleCount);
	void setValueCount(unsigned long valueCount);
	void setMostCommonValues(void **mcvList, double *seleList);
	void setEquithDepthBound(void **boundList, unsigned long* bucketValueCount);

	unsigned long getTupleCount() const;
	unsigned long getValueCount() const;
	unsigned getBucketCnt() const;

	void print(column_type type);

	void destory();
	virtual ~Statistic();

private:

	unsigned m_bucketCnt;

	unsigned long m_staDistinct;		//distinct value count
	unsigned long m_staCount;	//tuple count;

	double m_staNullFrac;	//null value fraction

	ValuePtr* m_staValues1;	//value bound for the equi-depth, first is the minimum and last is the maximum
	ValuePtr* m_staValues2;	//most common value

	unsigned long* m_staNumbers1;	//distinct values in the equi-depth
	double* m_staNumbers2;	//frequency of the most common value

};

class StatisticOnTable : public Statistic{

public:
	StatisticOnTable();
	StatisticOnTable(unsigned bucketCnt);
	Statistic* getPartStat(const PartitionID partID);
	void destory();
	virtual ~StatisticOnTable();

private:
	boost::unordered_map < PartitionID, Statistic* > m_partStat;

};

#endif /* STATISTIC_H_ */
