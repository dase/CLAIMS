/*
 * Environment.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: wangli
 */

#include "Environment.h"
#include <libconfig.h++>
#include <iostream>
#include <sstream>
#include <string>
#include "./Debug.h"
#include "./Config.h"
#include "common/Logging.h"
#include "common/TypePromotionMap.h"
#include "common/TypeCast.h"
#include "common/Expression/queryfunc.h"
#include "common/error_define.h"
#include "codegen/CodeGenerator.h"
using namespace claims::common;

Environment* Environment::_instance = 0;

Environment::Environment(bool ismaster) : ismaster_(ismaster) {
  _instance = this;
  Config::getInstance();
  CodeGenerator::getInstance();
  logging_ = new EnvironmentLogging();
  readConfigFile();
  initializeExpressionSystem();
  portManager = PortManager::getInstance();

  if (ismaster) {
    logging_->log("Initializing the Coordinator...");
    initializeCoordinator();
    catalog_ = Catalog::getInstance();
    if (kSuccess != catalog_->restoreCatalog()) {
      cerr << "restore catalog failed" << endl;
      exit(kFailure);
    }
  }

  if (true == g_thread_pool_used) {
    logging_->log("Initializing the ThreadPool...");
    if (false == initializeThreadPool()) {
      logging_->elog("initialize ThreadPool failed");
    }
  }
  logging_->log("Initializing the AdaptiveEndPoint...");
  initializeEndPoint();
  /**
   * TODO:
   * DO something in AdaptiveEndPoint such that the construction function does
          not return until the connection is completed. If so, the following
   sleep()
          dose not needed.

          This is done in Aug.18 by Li :)
   */

  /*Before initializing Resource Manager, the instance ip and port should be
   * decided.*/

  initializeResourceManager();

  initializeStorage();

  initializeBufferManager();

  logging_->log("Initializing the ExecutorMaster...");
  iteratorExecutorMaster = new IteratorExecutorMaster();

  logging_->log("Initializing the ExecutorSlave...");
  iteratorExecutorSlave = new IteratorExecutorSlave();

  exchangeTracker = new ExchangeTracker();
  expander_tracker_ = ExpanderTracker::getInstance();
  if (ismaster) {
    initializeClientListener();
  }
}

Environment::~Environment() {
  _instance = 0;
  delete expander_tracker_;
  delete logging_;
  delete portManager;
  delete catalog_;
  delete coordinator;
  if (ismaster_) {
    delete iteratorExecutorMaster;
    delete resourceManagerMaster_;
    delete blockManagerMaster_;
    destoryClientListener();
  }
  delete iteratorExecutorSlave;
  delete exchangeTracker;
  delete resourceManagerSlave_;
  delete blockManager_;
  delete bufferManager_;
  delete endpoint;
}
Environment* Environment::getInstance(bool ismaster) {
  if (_instance == 0) {
    new Environment(ismaster);
  }
  return _instance;
}
std::string Environment::getIp() { return ip; }
unsigned Environment::getPort() { return port; }

ThreadPool* Environment::getThreadPool() const { return thread_pool_; }

void Environment::readConfigFile() {
  libconfig::Config cfg;
  cfg.readFile(Config::config_file.c_str());
  ip = (const char*)cfg.lookup("ip");
}
void Environment::initializeEndPoint() {
  //	libconfig::Config cfg;
  //	cfg.readFile("/home/claims/config/wangli/config");
  //	std::string endpoint_ip=(const char*)cfg.lookup("ip");
  //	std::string endpoint_port=(const char*)cfg.lookup("port");
  std::string endpoint_ip = ip;
  int endpoint_port;
  if ((endpoint_port = portManager->applyPort()) == -1) {
    logging_->elog("The ports in the PortManager is exhausted!");
  }
  port = endpoint_port;
  logging_->log("Initializing the AdaptiveEndPoint as EndPoint://%s:%d.",
                endpoint_ip.c_str(), endpoint_port);
  std::ostringstream name, port_str;
  port_str << endpoint_port;
  name << "EndPoint://" << endpoint_ip << ":" << endpoint_port;

  endpoint =
      new AdaptiveEndPoint(name.str().c_str(), endpoint_ip, port_str.str());
}
void Environment::initializeCoordinator() { coordinator = new Coordinator(); }
void Environment::initializeStorage() {
  if (ismaster_) {
    blockManagerMaster_ = BlockManagerMaster::getInstance();
    blockManagerMaster_->initialize();
  }
  /*both master and slave node run the BlockManager.*/
  //		BlockManagerId *bmid=new BlockManagerId();
  //		string
  // actorname="blockManagerWorkerActor_"+bmid->blockManagerId;
  //		cout<<actorname.c_str()<<endl;
  //		BlockManager::BlockManagerWorkerActor
  //*blockManagerWorkerActor=new
  // BlockManager::BlockManagerWorkerActor(endpoint,framework_storage,actorname.c_str());

  blockManager_ = BlockManager::getInstance();
  blockManager_->initialize();
}

void Environment::initializeResourceManager() {
  if (ismaster_) {
    resourceManagerMaster_ = new ResourceManagerMaster();
  }
  resourceManagerSlave_ = new InstanceResourceManager();
  nodeid = resourceManagerSlave_->Register();
}
void Environment::initializeBufferManager() {
  bufferManager_ = BufferManager::getInstance();
}

void Environment::initializeIndexManager() {
  indexManager_ = IndexManager::getInstance();
}

AdaptiveEndPoint* Environment::getEndPoint() { return endpoint; }
ExchangeTracker* Environment::getExchangeTracker() { return exchangeTracker; }
ResourceManagerMaster* Environment::getResourceManagerMaster() {
  return resourceManagerMaster_;
}
InstanceResourceManager* Environment::getResourceManagerSlave() {
  return resourceManagerSlave_;
}
NodeID Environment::getNodeID() const { return nodeid; }
Catalog* Environment::getCatalog() const { return catalog_; }

void Environment::initializeClientListener() {
  listener_ = new ClientListener(Config::client_listener_port);
  listener_->configure();
}

void Environment::initializeExpressionSystem() {
  initialize_arithmetic_type_promotion_matrix();
  initialize_type_cast_functions();
  initialize_operator_function();
}

void Environment::destoryClientListener() {
  listener_->shutdown();
  delete listener_;
}

bool Environment::initializeThreadPool() {
  thread_pool_ = new ThreadPool();
  //	return thread_pool_->Thread_Pool_init(2*sysconf(_SC_NPROCESSORS_CONF));
  return thread_pool_->Thread_Pool_init(100);
}

IteratorExecutorSlave* Environment::getIteratorExecutorSlave() const {
  return iteratorExecutorSlave;
}
