/*
 * Estimation.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: volt
 */

#include "../../catalog/stat/Estimation.h"
#include "../../common/data_type.h"
//#include "../../ids.h"
#include "../table.h"
#include <cassert>
#include "../../catalog/attribute.h"
#include "../../catalog/catalog.h"
#include "../../catalog/stat/Statistic.h"
#include "../../catalog/stat/StatManager.h"

using namespace claims::catalog;

Estimation::Estimation() {
  // TODO Auto-generated constructor stub
}

Estimation::~Estimation() {
  // TODO Auto-generated destructor stub
}

unsigned long Estimation::estEqualOper(AttributeID attrID, void *para) {
  unsigned long ret = 0;

  const Histogram *stat = StatManager::getInstance()->getHistogram(attrID);

  // look up in the end-biased statistic
  void **valueList;
  double *selList;

  valueList = stat->m_staValues2;
  selList = stat->m_staNumbers2;

  assert(valueList != 0 && selList != 0);

  Attribute attr =
      Catalog::getInstance()->getTable(attrID.table_id)->getAttribute(
          attrID.offset);

  Operate *op = attr.attrType->operate;

  for (unsigned i = 0; i < stat->m_bucketCnt; ++i) {
    if (op->equal(valueList[i], para)) {
      return stat->getTupleCount() * selList[i];
    }
  }

  double sel = 0;
  unsigned i;
  for (i = 0; i < stat->m_bucketCnt; ++i) {
    /**
     * 如果找到最小的 i 满足 para <
     * valueList[i]，那么para必然落在i-1号bucket中。
     * 如果始终找不到，那么para必然是最大值，落在最后一个bucket中。
     */
    if (op->compare(para, stat->m_staValues1[i]) < 0) break;
  }

  if (i == 0) return 0;

  /**
   * 最大值，属于上一个边界。因为，唯独最后一个bucket两边都是闭区间。
   */
  if (op->compare(para, stat->m_staValues1[stat->m_bucketCnt - 1]) == 0) --i;

  --i;

  assert(i < stat->m_bucketCnt);

  unsigned j;
  for (j = 0; j < stat->m_bucketCnt; ++j)
    if (stat->m_staNumbers1[j] == 0) break;

  ret = (1.0 * stat->getTupleCount()) / (j * stat->m_staNumbers1[i]);

  return ret;
}

/**
 * compute the cover ratio
 */
double ratio(const void *sourLow, const void *sourUp, const void *paraLow,
             const void *paraUp, const column_type *type) {
  // TODO ADD support for new data types
  double ret = 0;

  void *low = new char[type->get_length()];   // new
  void *up = new char[type->get_length()];    // new
  void *pLow = new char[type->get_length()];  // new
  void *pUp = new char[type->get_length()];   // new

  (type->operate->assignment(sourLow, low));
  (type->operate->assignment)(sourUp, up);
  (type->operate->assignment(paraLow, pLow));
  (type->operate->assignment)(paraUp, pUp);

  (type->operate->GetMINFunction())(up, pUp);
  (type->operate->GetMAXFunction())(low, pLow);

  if (type->operate->compare(low, up) < 0) {
    switch (type->type) {
      case t_int:
        ret = (*(int *)up - *(int *)low) * 1.0 /
              (*(int *)sourUp - *(int *)sourLow);
        break;
      case t_float:
        ret = (*(float *)up - *(float *)low) /
              (*(float *)sourUp - *(float *)sourLow);
        break;
      case t_double:
        ret = (*(double *)up - *(double *)low) /
              (*(double *)sourUp - *(double *)sourLow);
        break;
      case t_string:
        ret = 0.5;
        break;
      case t_u_long:
        ret = (*(unsigned long *)up - *(unsigned long *)low) * 1.0 /
              (*(unsigned long *)sourUp - *(unsigned long *)sourLow);
        break;
      default:
        ret = 0.5;  // For data type like String, the intersect ration
                    // computation does not exist.
        // Hence, we use the magic number to guess the ratio of values fallen
        // into the both intervals.
        break;
    }
  }
  delete low;
  delete up;
  delete pLow;
  delete pUp;
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

  const Histogram *stat = StatManager::getInstance()->getHistogram(attrID);

  Attribute attr =
      Catalog::getInstance()->getTable(attrID.table_id)->getAttribute(
          attrID.offset);
  Operate *op = attr.attrType->operate;

  //	column_type* type =
  //			Catalog::getInstance()->getTable(attrID.table_id)->getAttribute(
  //					attrID.offset).attrType;
  //	Operate *op = type->operate;

  double sel = 0;
  for (unsigned i = 0; i < stat->m_bucketCnt - 1; ++i) {
    if ((op->compare(lowPara, stat->m_staValues1[i]) <= 0) &&
        (op->compare(stat->m_staValues1[i + 1], upperPara) <= 0)) {
      sel += 1;
      // the lowest value of the bucket is unreachable
      if (op->equal(lowPara, stat->m_staValues1[i]))
        sel -= 1.0 / stat->m_staNumbers1[i];
    } else {  // intersect
      double r = ratio(stat->m_staValues1[i], stat->m_staValues1[i + 1],
                       lowPara, upperPara, attr.attrType);
      sel += r;
    }
  }
  sel = sel / (stat->m_bucketCnt - 1);
  ret = sel * stat->m_staCount;
  return ret;
}

/**
 * parameter:
 * @lp1 the left bound of the first bucket
 * @rp1	the right bound of the first bucket
 * @d1 	the distinct value in the first bucket
 * @lp2 the left bound of the second bucket
 * @rp2 the right bound of the second bucket
 * @d2 	the distinct value in the second bucket
 */
unsigned long product(const void *lp1, const void *rp1, const int d1,
                      const int c1, const void *lp2, const void *rp2,
                      const int d2, const int c2, const column_type *type) {
  double r1 = ratio(lp1, rp1, lp2, rp2, type);
  double r2 = ratio(lp2, rp2, lp1, rp1, type);

  int numDistinctValue1 = d1 * r1;
  int numDistinctValue2 = d2 * r2;

  int numCombination = numDistinctValue1 < numDistinctValue2
                           ? numDistinctValue1
                           : numDistinctValue2;

  return c1 / d1 * c2 / d2 * numCombination;
}

unsigned long Estimation::estEqualJoin(AttributeID attrID1,
                                       AttributeID attrID2) {
  Histogram *hist1 = StatManager::getInstance()
                         ->getAttributeStatistic(attrID1)
                         ->getHistogram();
  Histogram *hist2 = StatManager::getInstance()
                         ->getAttributeStatistic(attrID2)
                         ->getHistogram();

  //两个属性应该是相同的类型
  Attribute attr1 =
      Catalog::getInstance()->getTable(attrID1.table_id)->getAttribute(
          attrID1.offset);
  Attribute attr2 =
      Catalog::getInstance()->getTable(attrID2.table_id)->getAttribute(
          attrID2.offset);

  if (attr1.attrType->type == attr2.attrType->type) {
    int aveTupleNum1 = hist1->m_staCount / (hist1->m_bucketCnt - 1);
    int aveTupleNum2 = hist2->m_staCount / (hist2->m_bucketCnt - 1);
    unsigned long ret = 0;
    for (int i = 0; i < hist1->m_bucketCnt - 1; ++i) {
      for (int j = 0; j < hist2->m_bucketCnt - 1; ++j) {
        ret += product(hist1->m_staValues1[i], hist1->m_staValues1[i + 1],
                       hist1->m_staNumbers1[i], aveTupleNum1,
                       hist2->m_staValues1[j], hist2->m_staValues1[j + 1],
                       hist2->m_staNumbers1[j], aveTupleNum2, attr1.attrType);
      }
    }
    return ret;
  } else {
    return 0;
  }
}
