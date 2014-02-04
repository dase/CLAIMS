/*
 * TableStatistic.h
 * This class describe the statistic within a table.
 *  Created on: Feb 2, 2014
 *      Author: wangli
 */

#ifndef TABLESTATISTIC_H_
#define TABLESTATISTIC_H_
#include <boost/unordered_map.hpp>
#include "AttributeStatistics.h"
class TableStatistic {
public:
	TableStatistic();

	virtual ~TableStatistic();


	unsigned long number_of_tuples_;
	void print()const;
private:
//	unsigned long total_size_;
	boost::unordered_map<AttributeID,AttributeStatistics*> att_stat_list_;
};

#endif /* TABLESTATISTIC_H_ */
