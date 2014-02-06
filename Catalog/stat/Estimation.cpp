/*
 * Estimation.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: volt
 */

#include "Estimation.h"

#include "../../data_type.h"
//#include "../../ids.h"
#include "../Attribute.h"
#include "../Catalog.h"
#include "../table.h"
#include "Statistic.h"
#include "StatManager.h"
#include <cassert>

Estimation::Estimation() {
	// TODO Auto-generated constructor stub

}

Estimation::~Estimation() {
	// TODO Auto-generated destructor stub
}

unsigned long Estimation::estEqualOper(AttributeID attrID, void *para) {
	unsigned long ret = 0;

	const Histogram *stat = StatManager::getInstance()->getStat(attrID);

	//look up in the end-biased statistic
	void **valueList;
	double *selList;

	valueList = stat->m_staValues2;
	selList = stat->m_staNumbers2;

	assert(valueList!=0&&selList!=0);

	Attribute attr=Catalog::getInstance()->getTable(attrID.table_id)->getAttribute(
			attrID.offset);

	Operate *op =
			attr.attrType->operate;

	for (unsigned i = 0; i < stat->m_bucketCnt; ++i) {

		if (op->equal(valueList[i], para)) {
			return stat->getTupleCount() * selList[i];
		}
	}

	double sel = 0;
	unsigned i;
	for (i = 0; i < stat->m_bucketCnt; ++i) {

		/**
		 * 如果找到最小的 i 满足 para < valueList[i]，那么para必然落在i-1号bucket中。
		 * 如果始终找不到，那么para必然是最大值，落在最后一个bucket中。
		 */
		if (op->compare(para, stat->m_staValues1[i]) < 0)
			break;
	}

	if( i == 0 ) return 0;

	/**
	 * 最大值，属于上一个边界。因为，唯独最后一个bucket两边都是闭区间。
	 */
	if( op->compare(para, stat->m_staValues1[stat->m_bucketCnt-1]) == 0 ) --i;

	--i;

	assert(i < stat->m_bucketCnt);

	unsigned j;
	for (j = 0; j < stat->m_bucketCnt; ++j)
		if (stat->m_staNumbers1[j] == 0)
			break;

	ret = (1.0 * stat->getTupleCount()) / (j * stat->m_staNumbers1[i]);

	return ret;
}

/**
 * compute the cover ratio
 */
double ratio(void *bucketLow, void *bucketUp, void *paraLow, void *paraUp,
		column_type *type) {
	//TODO ADD support for new data types
	double ret = 0;

	void *low = new char[type->get_length()];
	void *up = new char[type->get_length()];

	(type->operate->assignment(low, bucketLow));
	(type->operate->assignment)(up, bucketUp);
	(type->operate->GetMINFunction())(up, paraUp);
	(type->operate->GetMAXFunction())(low, paraLow);

	if (type->operate->compare(low, up) < 0) {

		switch (type->type) {
		case t_int:
			ret = (*(int*) up - *(int*) low)*1.0
					/ (*(int*) bucketUp - *(int*) bucketLow);
			break;
		case t_float:
			ret = (*(float*) up - *(float*) low)
					/ (*(float*) bucketUp - *(float*) bucketLow);
			break;
		case t_double:
			ret = (*(double*) up - *(double*) low)
					/ (*(double*) bucketUp - *(double*) bucketLow);
			break;
		case t_string:
			ret = 0.5;
			break;
		case t_u_long:
			ret =
					(*(unsigned long*) up - *(unsigned long*) low)*1.0
							/ (*(unsigned long*) bucketUp
									- *(unsigned long*) bucketLow);
			break;
		default:
			ret = 0.5;
			break;
		}
	}
	delete low;
	delete up;
	return ret;
}

/**
 * assume both lowPara and upperPara is legal
 * lowPara:
 * upperPara:
 * range is used with < and >, instead of =
 * for <=, it decomposed into < and =
 */
unsigned long Estimation::estRangeOper(AttributeID attrID, void *lowPara,
		void *upperPara) {
	unsigned long ret = 0;

	const Histogram *stat = StatManager::getInstance()->getStat(attrID);
	column_type* type =
			Catalog::getInstance()->getTable(attrID.table_id)->getAttribute(
					attrID.offset).attrType;
	Operate *op = type->operate;

	double sel = 0;
	for (unsigned i = 0; i < stat->m_bucketCnt - 1; ++i) {

		if ((op->compare(lowPara, stat->m_staValues1[i]) <= 0)
				&& (op->compare(stat->m_staValues1[i + 1], upperPara) <= 0)) {
			sel += 1;
			//the lowest value of the bucket is unreachable
			if (op->equal(lowPara, stat->m_staValues1[i]))
				sel -= 1.0 / stat->m_staNumbers1[i];
		} else {	//intersect
			double r = ratio(stat->m_staValues1[i], stat->m_staValues1[i + 1],
					lowPara, upperPara, type);
			sel += r;
		}
	}
	ret = sel * stat->m_staCount;
	return ret;
}
