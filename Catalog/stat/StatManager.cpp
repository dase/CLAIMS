/*
 * StatManager.cpp
 *
 *  Created on: 2014年1月18日
 *      Author: volt
 */

#include "StatManager.h"

#include <boost/unordered/detail/fwd.hpp>
#include <boost/unordered/unordered_map.hpp>

#include "AttributeStatistics.h"

StatManager *StatManager::m_singleton = 0;
//BlockManager *BlockManager::blockmanager_=0;

StatManager::StatManager() {

}

StatManager* StatManager::getInstance() {
	if (m_singleton == 0) {
		m_singleton = new StatManager();
	}
	return m_singleton;
}

Histogram* StatManager::getHistogram(const AttributeID attID) {
	AttributeStatistics* att_stat = getAttributeStatistic(attID);
	if (att_stat != 0) {
		return att_stat->getHistogram();
	}
	return 0;
//	return m_singletonstat[attID];
}

Histogram* StatManager::getStat(const AttributeID attID,
		const PartitionID partID) {

	return 0;
//	return m_stat[attID]->getPartStat(partID);
}

//void StatManager::addStat(AttributeID attrID,  Histogram *stat){
//
//	m_stat[attrID] = (StatisticOnTable*)stat;
//}

TableStatistic* StatManager::getTableStatistic(const TableID& table_id) const {
	if (tab_stat_list_.find(table_id) == tab_stat_list_.cend())
		return 0;
	return tab_stat_list_.at(table_id);
}

AttributeStatistics* StatManager::getAttributeStatistic(const AttributeID &attrId) const{

	TableStatistic* table_stat = getTableStatistic(attrId.table_id);
	return table_stat == 0 ?
			0 : table_stat->getAttributeStatistics(attrId);
}

AttributeStatistics* StatManager::getAttributeStatistic(
		const Attribute& attr) const {
	TableStatistic* table_stat = getTableStatistic(attr.table_id_);
	return table_stat == 0 ?
			0 : table_stat->getAttributeStatistics(attr.getID());
}
StatManager::~StatManager() {
}
void StatManager::setTableStatistic(const TableID& table_id,
		TableStatistic* tab_stat) {
	tab_stat_list_[table_id] = tab_stat;
}
