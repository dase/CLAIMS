/*
 * Environment.h
 *
 *  Created on: Aug 10, 2013
 *      Author: wangli
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include "AdaptiveEndPoint.h"
#include "Executor/Coordinator.h"
#include "Executor/IteratorExecutorMaster.h"
#include "Executor/IteratorExecutorSlave.h"
#include "Executor/ExchangeTracker.h"
#include "PortManager.h"
#include "Logging.h"
#include "storage/BlockManager.h"
#include "storage/BlockManagerMaster.h"
#include "Resource/ResourceManagerMaster.h"
#include "Resource/ResourceManagerSlave.h"
#include "Catalog/Catalog.h"

class Environment {
public:
	virtual ~Environment();
	static Environment* getInstance(bool ismaster=0);
	std::string getIp();
	AdaptiveEndPoint* getEndPoint();
	ExchangeTracker* getExchangeTracker();
	ResourceManagerMaster* getResourceManagerMaster();
	ResourceManagerSlave* getResourceManagerSlave();
	Catalog* getCatalog()const;
	Environment(bool ismaster=false);
private:
	void Initialize();
	void InitializeEndPoint();
	void InitializeCoordinator();
	void InitializeStorage();
	void InitializeResourceManager();
private:
	static Environment* _instance;
	PortManager* portManager;
	AdaptiveEndPoint* endpoint;
	Coordinator* coordinator;
	std::string ip;
	IteratorExecutorSlave* iteratorExecutorSlave;
	IteratorExecutorMaster* iteratorExecutorMaster;
	ExchangeTracker* exchangeTracker;
	Logging* logging_;
	bool ismaster_;
	ResourceManagerMaster* resourceManagerMaster_;
	ResourceManagerSlave* resourceManagerSlave_;
	Catalog* catalog_;
};

#endif /* ENVIRONMENT_H_ */
