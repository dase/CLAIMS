/*
 * Analyzer.h
 *
 *  Created on: 2014年1月20日
 *      Author: volt
 */

#ifndef ANALYZER_H_
#define ANALYZER_H_

#include "../../catalog/stat/Statistic.h"
#include "../../common/ids.h"
#include "../../catalog/attribute.h"

using namespace claims::catalog;

class Analyzer {
 public:
  enum analysis_level { a_l_table, a_l_attribute };
  Analyzer();

  static void analyse(const AttributeID &attrID);
  static void analyse(TableID tableID, analysis_level level = a_l_table);
  static void analyse_on_project(ProjectionID projection_id);

  static void compute_histogram(const AttributeID &attr_id);
  static void compute_attribute_stat(const AttributeID &attr_id);
  static void compute_table_stat(const TableID &tab_id);
  //	static void print();

  virtual ~Analyzer();

 private:
  static void mcvAnalyse(void **list, const unsigned long size,
                         const Attribute &attr, Histogram *stat);
  static void equiDepthAnalyse(void **list, const unsigned long size,
                               const Attribute &attr, Histogram *stat);

  static unsigned long getDistinctCardinality(const AttributeID &attr_id);

  static bool isBeneficalFromHistrogram(unsigned distinct_values,
                                        unsigned cardinality);
  /**
   * get histogram if any, otherwise compute it.
   */
  static Histogram *computeHistogram(const AttributeID &attr,
                                     const unsigned nbuckets = 20);

  static unsigned decideNumberOfBucketsForHistogram(
      const int distinct_cardinality, const int cardinality);
};

#endif /* ANALYZER_H_ */
