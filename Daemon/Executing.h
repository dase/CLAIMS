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
class Executing {
public:
	Executing();
	virtual ~Executing();
	static ResultSet* run_sql(std::string sql,std::string& error);
};

#endif /* EXECUTING_H_ */
