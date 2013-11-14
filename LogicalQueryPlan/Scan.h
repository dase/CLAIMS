/*
 * Scan.h
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#ifndef SCAN_H_
#define SCAN_H_
#include "LogicalOperator.h"
#include "../Schema/LogicalProjection.h"
class LogicalScan: public LogicalOperator {
public:
	LogicalScan(std::vector<Attribute> attribute_list);
	virtual ~LogicalScan();
	LogicalProjection getLogcialProjection()const;
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned &){}

private:
	LogicalProjection logical_projection_;
//	Partitioner* partition_info_;
//	Dataflow* dataflow_;
	std::vector<Attribute> scan_attribute_list_;
};

#endif /* SCAN_H_ */
