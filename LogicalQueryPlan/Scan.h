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
class LogicalScan:public LogicalOperator {
public:
	LogicalScan();
	virtual ~LogicalScan();
	LogicalProjection getLogcialProjection()const;
private:
	LogicalProjection logical_projection_;
};

#endif /* SCAN_H_ */
