/*
 * Executing.h
 *
 *  Created on: Feb 20, 2014
 *      Author: wangli
 */

#ifndef EXECUTING_H_
#define EXECUTING_H_
#include <string>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/Block/ResultSet.h"

#include "../Parsetree/runparsetree.cpp"
#include "../Parsetree/parsetree2logicalplan.cpp"
#include "../Parsetree/ExecuteLogicalQueryPlan.h"
#include "../LogicalQueryPlan/LogicalQueryPlanRoot.h"
class Executing {
public:
	Executing();
	virtual ~Executing();
	static ResultSet* run_sql(std::string sql,std::string& error);
	static void run_sql(std::string cmd, ResultSet *result, bool &status, std::string &error_info, std::string info);

};

#endif /* EXECUTING_H_ */
