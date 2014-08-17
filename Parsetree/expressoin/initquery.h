/*
 * initquery.h
 *
 *  Created on: Aug 10, 2014
 *      Author: imdb
 */

#ifndef INITQUERY_H_
#define INITQUERY_H_

#include"qnode.h"
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<string>
#include<map>
#include"execfunc.h"
#include "queryfunc.h"
#include "../../Parsetree/sql_node_struct.h"
#include "../../common/data_type.h"
#include "../../common/TypePromotionMap.h"
#include "../../common/Logging.h"
#include "../../Environment.h"
using namespace std;
QNode * transformqual(Node *node);
void initqual(QNode *&node,data_type r_type,map<string,int>&colindex);

#endif /* INITQUERY_H_ */
