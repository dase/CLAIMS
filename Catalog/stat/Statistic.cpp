/*
 * Statistic.cpp
 *
 *  Created on: 2014年1月18日
 *      Author: volt
 */

#include "Statistic.h"

Histogram::Histogram() {

	this->m_bucketCnt = 0;
	m_staNumbers1=0;
	m_staNumbers2=0;
}

Histogram::Histogram(unsigned bucketCnt) {

	this->m_bucketCnt = bucketCnt;
}

Histogram::~Histogram() {

}

unsigned Histogram::getBucketCnt() const{
	return m_bucketCnt;
}

unsigned long Histogram::getTupleCount() const{
	return m_staCount;
}

unsigned long Histogram::getValueCount() const{
	return m_staDistinct;
}

void Histogram::setValueCount(unsigned long valueCount) {
	m_staDistinct = valueCount;
}

void Histogram::setTupleCount(unsigned long tupleCount) {
	m_staCount = tupleCount;
}

void Histogram::setEquithDepthBound(void **boundList,
		unsigned long* bucketValueCount) {

	m_staValues1 = boundList;
	m_staNumbers1 = bucketValueCount;
}

void Histogram::setMostCommonValues(void **mcvList, double *seleList) {

	m_staValues2 = mcvList;
	m_staNumbers2 = seleList;
}

void Histogram::Print(column_type type) {

	printf("Tuple Count: %d\n\n", m_staCount);

	printf("Value Count: %d\n", m_staDistinct);

//	printf("Equi-Depth\n");
//	for (unsigned i = 0; i < m_bucketCnt; ++i) {
//		printf("%s ", type.operate->toString(m_staValues1[i]).c_str());
//	}
//	printf("\n");
//	for (unsigned i = 0; i < m_bucketCnt; ++i) {
//		printf("%d ", m_staNumbers1[i]);
//	}
//	printf("\n");
	printf("End-Baised\n");
	for (unsigned i = 0; i < m_bucketCnt; ++i) {
		printf("%s ", type.operate->toString(m_staValues2[i]).c_str());
	}
	printf("\n");
	for (unsigned i = 0; i < m_bucketCnt; ++i) {
		printf("%lf ", m_staNumbers2[i]);
	}
	printf("\n");
}

void Histogram::destory() {

	delete m_staNumbers1;
	delete m_staNumbers2;

	for (unsigned i = 0; i < m_bucketCnt; ++i) {
		delete m_staValues1[i];
		delete m_staValues2[i];
	}

	delete m_staValues1;
	delete m_staValues2;
}

StatisticOnTable::StatisticOnTable(unsigned bucketCnt) :
		Histogram(bucketCnt) {

}

Histogram* StatisticOnTable::getPartStat(const PartitionID partID) {

	return m_partStat[partID];
}

void StatisticOnTable::destory() {

	Histogram::destory();
	for (boost::unordered_map<PartitionID, Histogram*>::iterator it =
			m_partStat.begin(); it != m_partStat.end(); it++) {
		it->second->destory();
	}
}

StatisticOnTable::~StatisticOnTable() {

}
