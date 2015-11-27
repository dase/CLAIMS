/*
 * StatManager.h
 *
 *  Created on: 2014年1月18日
 *      Author: volt
 */

#ifndef STATMANAGER_H_
#define STATMANAGER_H_

//#include <boost/unordered_map.hpp>

#include <boost/unordered_map.hpp>

#include "../../catalog/stat/Statistic.h"
#include "../../catalog/stat/TableStatistic.h"
#include "../../common/ids.h"
#include "../attribute.h"
using namespace claims::catalog;

class StatManager {
 public:
  static StatManager* getInstance();
  virtual ~StatManager();

  Histogram* getHistogram(const AttributeID attID);
  Histogram* getStat(const AttributeID attID, const PartitionID partID);

  void addStat(const AttributeID attId, Histogram* stat);
  void addStat(const AttributeID attId, const PartitionID partId,
               const Histogram* stat);

  // TODO update method
  TableStatistic* getTableStatistic(const TableID& table_id) const;

  AttributeStatistics* getAttributeStatistic(const AttributeID& attrId) const;
  AttributeStatistics* getAttributeStatistic(const Attribute& attr) const;

  void setTableStatistic(const TableID& table_id, TableStatistic* tab_stat);

 private:
  StatManager();

  static StatManager* m_singleton;
  //	boost::unordered_map<AttributeID, StatisticOnTable*> m_stat;

  boost::unordered_map<TableID, TableStatistic*> tab_stat_list_;
};

#endif /* STATMANAGER_H_ */
