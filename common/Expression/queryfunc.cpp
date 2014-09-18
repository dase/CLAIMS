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
