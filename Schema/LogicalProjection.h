/*
 * LogicalProjection.h
 *
 *  Created on: Nov 8, 2013
 *      Author: wangli
 */

#ifndef LOGICALPROJECTION_H_
#define LOGICALPROJECTION_H_
#include <vector>
#include <map>
#include "../Catalog/table.h"
#include "../Catalog/Column.h"
class LogicalProjection{
public:
	LogicalProjection();
	virtual ~LogicalProjection();
	std::vector<Column*> column_list_;


};

#endif /* LOGICALPROJECTION_H_ */
