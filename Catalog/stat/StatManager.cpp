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

Statistic* StatManager::getStat(const AttributeID attID){

	return m_stat[attID];
}

Statistic* StatManager::getStat(const AttributeID attID, const PartitionID partID){

	return m_stat[attID]->getPartStat(partID);
}

void StatManager::addStat(AttributeID attrID,  Statistic *stat){

	m_stat[attrID] = (StatisticOnTable*)stat;
}


StatManager::~StatManager() {
}

