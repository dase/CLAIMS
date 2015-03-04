#include "qnode.h"
#include"queryfunc.h"
#include "../../common/Logging.h"
/*
 * queryfunc.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: imdb
 */
ExecFunc ExectorFunction::operator_function[DATA_TYPE_NUM][OPER_TYPE_NUM];
AvgDivide ExectorFunction::avg_divide[DATA_TYPE_NUM];//the avg_divide array store the AvgDivide function according to the datatype
