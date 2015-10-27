/*
 * IteratorExecutorMaster.h
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#ifndef ITERATOREXECUTORMASTER_H_
#define ITERATOREXECUTORMASTER_H_

#include <Theron/Theron.h>
#include <Theron/Defines.h>
#include <vector>
#include <string>
#include <libconfig.h++>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../common/Logging.h"
#include "../utility/Timer.h"
class IteratorExecutorMaster {
public:
	IteratorExecutorMaster();
	virtual ~IteratorExecutorMaster();
	static IteratorExecutorMaster* getInstance();
	bool ExecuteBlockStreamIteratorsOnSites(BlockStreamIteratorBase* it,std::vector<std::string> ip_list);
	bool ExecuteBlockStreamIteratorsOnSite(BlockStreamIteratorBase* it,NodeID target_ip);
	bool Propogation(const int,std::string target);
private:
	static IteratorExecutorMaster* _instance;
	Theron::EndPoint* endpoint;
	Theron::Framework* framework;
	Logging* logging_;
};

#endif /* ITERATOREXECUTORMASTER_H_ */
