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
#include "storage/BlockManager.h"
#include "storage/BlockManagerMaster.h"
#include "Resource/ResourceManagerMaster.h"
#include "Resource/ResourceManagerSlave.h"
#include "IndexManager/IndexManager.h"
#include "Executor/AdaptiveEndPoint.h"
#include "Executor/PortManager.h"
#include "common/Logging.h"
#include "utility/ThreadPool.h"
#include "Client/ClaimsServer.h"
#include "Executor/exchange_tracker.h"
#include "Executor/expander_tracker.h"
#include "Resource/BufferManager.h"

using claims::catalog::Catalog;
class Catalog;

class Environment {
 public:
  virtual ~Environment();
  static Environment* getInstance(bool ismaster = 0);
  std::string getIp();
  unsigned getPort();
  AdaptiveEndPoint* getEndPoint();
  ExchangeTracker* getExchangeTracker();
  ResourceManagerMaster* getResourceManagerMaster();
  InstanceResourceManager* getResourceManagerSlave();
  NodeID getNodeID() const;
  Catalog* getCatalog() const;
  ThreadPool* getThreadPool() const;
  IteratorExecutorSlave* getIteratorExecutorSlave() const;
  Environment(bool ismaster = false);

 private:
  void readConfigFile();
  void initializeEndPoint();
  void initializeCoordinator();
  void initializeStorage();
  void initializeResourceManager();
  void initializeBufferManager();
  void initializeIndexManager();
  void initializeClientListener();
  void initializeExpressionSystem();
  void destoryClientListener();
  bool initializeThreadPool();

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
  BlockManager* blockManager_;
  BufferManager* bufferManager_;
  IndexManager* indexManager_;
  ExpanderTracker* expander_tracker_;
  ClientListener* listener_;

  ThreadPool* thread_pool_;

  /**
   * TODO: the master and slave pair, such as ResouceManagerMaster and
   * ResourceManagerSlave, should have a
   * base class which provides the access methods and is derived by
   * ResouceManagerMaster and ResourceManagerSlave.
   */
};

#endif /* ENVIRONMENT_H_ */
