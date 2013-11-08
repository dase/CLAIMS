/*
 * Scan.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#include "Scan.h"

LogicalScan::LogicalScan() {
	// TODO Auto-generated constructor stub

}

LogicalScan::~LogicalScan() {
	// TODO Auto-generated destructor stub
}

LogicalProjection LogicalScan::getLogcialProjection()const{
	return logical_projection_;
}
