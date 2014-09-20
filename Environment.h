/*
 * Environment.h
 *
 *  Created on: Aug 10, 2013
 *      Author: wangli
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include "Executor/Coordinator.h"
#include "Executor/IteratorExecutorMaster.h"
#include "Executor/IteratorExecutorSlave.h"
#include "Executor/ExchangeTracker.h"
#include "storage/BlockManager.h"
#include "storage/BlockManagerMaster.h"
#include "Resource/ResourceManagerMaster.h"
#include "Resource/ResourceManagerSlave.h"
#include "Catalog/Catalog.h"
#include "IndexManager/IndexManager.h"
#include "Executor/ExpanderTracker.h"
#include "Executor/AdaptiveEndPoint.h"
#include "Executor/PortManager.h"
#include "common/Logging.h"
#include "Client/ClaimsServer.h"
#include "Resource/BufferManager.h"

class Environment {
public:
	virtual ~Environment();
	static Environment* getInstance(bool ismaster=0);
	std::string getIp();
	unsigned getPort();
	AdaptiveEndPoint* getEndPoint();
	ExchangeTracker* getExchangeTracker();
	ResourceManagerMaster* getResourceManagerMaster();
	InstanceResourceManager* getResourceManagerSlave();
	NodeID getNodeID()const;
	Catalog* getCatalog()const;
	Environment(bool ismaster=false);
private:
	void Initialize();
	void InitializeEndPoint();
	void InitializeCoordinator();
	void InitializeStorage();
	void InitializeResourceManager();
	void InitializeBufferManager();
	void InitializeIndexManager();
	void InitializeClientListener();

	void destoryClientListener();
private:
	static Environment* _instance;
	PortManager* portManager;
	AdaptiveEndPoint* endpoint;
	Coordinator* coordinator;
	std::string ip;
	unsigned port;
	IteratorExecutorSlave* iteratorExecutorSlave;
	IteratorExecutorMaster* iteratorExecutorMaster;
	ExchangeTracker* exchangeTracker;
	Logging* logging_;
	bool ismaster_;
	ResourceManagerMaster* resourceManagerMaster_;
	InstanceResourceManager* resourceManagerSlave_;
	Catalog* catalog_;
	/* the globally unique node id*/
	NodeID nodeid;
	BlockManagerMaster* blockManagerMaster_;
	BlockManager*	blockManager_;
	BufferManager* bufferManager_;
	IndexManager* indexManager_;
	ExpanderTracker* expander_tracker_;
	ClientListener* listener_;

	/**
	 * TODO: the master and slave pair, such as ResouceManagerMaster and ResourceManagerSlave, should have a
	 * base class which provides the access methods and is derived by ResouceManagerMaster and ResourceManagerSlave.
	 */
};

#endif /* ENVIRONMENT_H_ */
