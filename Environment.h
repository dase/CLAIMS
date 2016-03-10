/*
 * Environment.h
 *
 *  Created on: Aug 10, 2013
 *      Author: wangli
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include "Executor/IteratorExecutorMaster.h"
#include "Executor/IteratorExecutorSlave.h"
#include "storage/BlockManager.h"
#include "storage/BlockManagerMaster.h"
#include "Resource/ResourceManagerMaster.h"
#include "Resource/ResourceManagerSlave.h"
#include "IndexManager/IndexManager.h"
#include "Executor/PortManager.h"
#include "common/Logging.h"
#include "utility/thread_pool.h"
#include "Client/ClaimsServer.h"
#include "Executor/exchange_tracker.h"
#include "Executor/expander_tracker.h"
#include "node_manager/master_node.h"
#include "node_manager/slave_node.h"
#include "Resource/BufferManager.h"

using claims::catalog::Catalog;
using claims::MasterNode;
using claims::SlaveNode;
class Catalog;

class Environment {
 public:
  virtual ~Environment();
  static Environment* getInstance(bool ismaster = 0);
  std::string getIp();
  unsigned getPort();
  ExchangeTracker* getExchangeTracker();
  ResourceManagerMaster* getResourceManagerMaster();
  InstanceResourceManager* getResourceManagerSlave();
  NodeID getNodeID() const;
  claims::catalog::Catalog* getCatalog() const;
  ThreadPool* getThreadPool() const;
  IteratorExecutorSlave* getIteratorExecutorSlave() const;
  Environment(bool ismaster = false);
  MasterNode* get_master_node() { return master_node_; }
  SlaveNode* get_slave_node() { return slave_node_; }
  BlockManager* get_block_manager() { return blockManager_; }

 private:
  void readConfigFile();
  void initializeStorage();
  void initializeResourceManager();
  void initializeBufferManager();
  void initializeIndexManager();
  void initializeClientListener();
  void initializeExpressionSystem();
  void destoryClientListener();
  bool initializeThreadPool();
  void InitMembership();

 private:
  static Environment* _instance;
  PortManager* portManager;
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
  MasterNode* master_node_;
  SlaveNode* slave_node_;
  /**
   * TODO: the master and slave pair, such as ResouceManagerMaster and
   * ResourceManagerSlave, should have a
   * base class which provides the access methods and is derived by
   * ResouceManagerMaster and ResourceManagerSlave.
   */
};

#endif /* ENVIRONMENT_H_ */
