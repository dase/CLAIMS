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
#include "../Message.h"
#include "../iterator.hpp"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../Logging.h"
class IteratorExecutorMaster {
public:
	IteratorExecutorMaster();
	virtual ~IteratorExecutorMaster();
	static IteratorExecutorMaster* getInstance();
	bool ExecuteIteratorsOnSlave(Iterator* it);
	bool ExecuteIteratorsOnSites(Iterator* it, std::vector<std::string> ip_list);
	bool ExecuteBlockStreamIteratorsOnSites(BlockStreamIteratorBase* it,std::vector<std::string> ip_list);
	bool ExecuteBlockStreamIteratorsOnSite(BlockStreamIteratorBase* it,std::string target_ip);
	bool Propogation(const int,std::string target);
private:
	static IteratorExecutorMaster* _instance;
	Theron::EndPoint* endpoint;
	Theron::Framework* framework;
	Logging* logging_;
};

#endif /* ITERATOREXECUTORMASTER_H_ */
