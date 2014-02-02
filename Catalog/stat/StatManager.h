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
#include "../../ids.h"
#include "Statistic.h"

class StatManager {
public:

	static StatManager *getInstance();
	virtual ~StatManager();

	Statistic* getStat(const AttributeID attID);
	Statistic* getStat(const AttributeID attID, const PartitionID partID);

	void addStat(const AttributeID attId, Statistic *stat);
	void addStat(const AttributeID attId, const PartitionID partId,
			const Statistic *stat);

	//TODO update method

private:

	StatManager();

	static StatManager *m_singleton;
	boost::unordered_map<AttributeID, StatisticOnTable*> m_stat;

};

#endif /* STATMANAGER_H_ */
