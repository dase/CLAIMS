/*
 * StatManager.cpp
 *
 *  Created on: 2014年1月18日
 *      Author: volt
 */

#include "StatManager.h"

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

Histogram* StatManager::getStat(const AttributeID attID){

	return m_stat[attID];
}

Histogram* StatManager::getStat(const AttributeID attID, const PartitionID partID){

	return m_stat[attID]->getPartStat(partID);
}

void StatManager::addStat(AttributeID attrID,  Histogram *stat){

	m_stat[attrID] = (StatisticOnTable*)stat;
}

TableStatistic* StatManager::getTableStatistic(const TableID& table_id)const{
	if(tab_stat_list_.find(table_id)==tab_stat_list_.cend())
		return 0;
	return tab_stat_list_.at(table_id);
}
StatManager::~StatManager() {
}
void StatManager::setTableStatistic(const TableID& table_id,TableStatistic* tab_stat){
	tab_stat_list_[table_id]=tab_stat;
}
