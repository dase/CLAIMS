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

#include "../Parsetree/runparsetree.h"
#include "../Parsetree/parsetree2logicalplan.cpp"
#include "../Parsetree/ExecuteLogicalQueryPlan.h"
#include "../logical_operator/logical_query_plan_root.h"
class Executing {
public:
	Executing();
	virtual ~Executing();
	static ResultSet* run_sql(std::string sql,std::string& error);
	static void run_sql(
			const std::string &cmd,
			ResultSet *&result,
			bool &status,
			std::string &error_info,
			std::string &info,
			int fd
		);

};

#endif /* EXECUTING_H_ */
