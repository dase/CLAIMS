/*
 * initquery.h
 *
 *  Created on: Aug 10, 2014
 *      Author: imdb
 */

#ifndef INITQUERY_H_
#define INITQUERY_H_

#include "qnode.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <map>
#include "execfunc.h"
#include "queryfunc.h"
#include "../../Parsetree/sql_node_struct.h"
#include "../../common/data_type.h"
#include "../../common/TypePromotionMap.h"
#include "../../common/Logging.h"
#include "../../Environment.h"
#include <algorithm>

#include "../../logical_operator/logical_operator.h"
using namespace claims::logical_operator;
QNode *transformqual(Node *node, LogicalOperator *child);
void InitExprAtPhysicalPlan(QNode *node);
void InitExprAtLogicalPlan(QNode *node, data_type actual_type,
                           map<string, int> &colindex, Schema *schema);
#endif /* INITQUERY_H_ */
