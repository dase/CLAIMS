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
#include "Debug.h"
#include "Logging.h"
Environment* Environment::_instance=0;
Environment::Environment(bool ismaster):ismaster_(ismaster) {
	_instance=this;
	logging_=new EnvironmentLogging();
	Initialize();
	portManager=PortManager::getInstance();
	catalog_=new Catalog();
	if(ismaster){
		logging_->log("Initializing the Coordinator...");
		InitializeCoordinator();

	}

	logging_->log("Initializing the AdaptiveEndPoint...");
	InitializeEndPoint();
/**
 * TODO:
 * DO something in AdaptiveEndPoint such that the construction function does
	not return until the connection is completed. If so, the following sleep()
	dose not needed.

	This is done in Aug.18 by Li :)
 */
//	InitializeStorage(ismaster);

	InitializeResourceManager();

	logging_->log("Initializing the ExecutorMaster...");
	iteratorExecutorMaster=new IteratorExecutorMaster();

	logging_->log("Initializing the ExecutorSlave...");
	iteratorExecutorSlave=new IteratorExecutorSlave();

	exchangeTracker =new ExchangeTracker();
}

Environment::~Environment() {
	// TODO Auto-generated destructor stub
}
Environment* Environment::getInstance(bool ismaster){
	if(_instance==0){
			new Environment(ismaster);
	}
	return _instance;
}
std::string Environment::getIp(){
	return ip;
}
void Environment::Initialize(){
	libconfig::Config cfg;
	cfg.readFile(CONFIG);
	ip=(const char*)cfg.lookup("ip");
}
void Environment::InitializeEndPoint(){
//	libconfig::Config cfg;
//	cfg.readFile("/home/imdb/config/wangli/config");
//	std::string endpoint_ip=(const char*)cfg.lookup("ip");
//	std::string endpoint_port=(const char*)cfg.lookup("port");
	std::string endpoint_ip=ip;
	int endpoint_port;
	if((endpoint_port=portManager->applyPort())==0){
		logging_->elog("The ports in the PortManager is exhausted!");
	}

	logging_->log("Initializing the AdaptiveEndPoint as EndPoint://%s:%d.",endpoint_ip.c_str(),endpoint_port);
	std::ostringstream name,port;
	port<<endpoint_port;
	name<<"EndPoint://"<<endpoint_ip<<":"<<port;

	endpoint=new AdaptiveEndPoint(name.str().c_str(),endpoint_ip,port.str());
}
void Environment::InitializeCoordinator(){
	coordinator=new Coordinator();
}
//void Environment::InitializeStorage(bool ismaster){
//	Theron::Framework *framework_storage=new Theron::Framework(*endpoint);
//	if(ismaster){
//		BlockManagerMaster::BlockManagerMasterActor *blockManagerMasterActor=new BlockManagerMaster::BlockManagerMasterActor(endpoint,framework_storage,"blockManagerMasterActor");
//		BlockManagerMaster* blockManagerMaster=BlockManagerMaster::getInstance(blockManagerMasterActor);
//		blockManagerMaster->initialize();
//	}else{
//		BlockManagerId *bmid=new BlockManagerId();
//		string actorname="blockManagerWorkerActor_"+bmid->blockManagerId;
//		cout<<actorname.c_str()<<endl;
//		BlockManager::BlockManagerWorkerActor *blockManagerWorkerActor=new BlockManager::BlockManagerWorkerActor(endpoint,framework_storage,actorname.c_str());
//		BlockManager *blockManager=BlockManager::getInstance(blockManagerWorkerActor);
//		blockManager->initialize();
//	}
//}

void Environment::InitializeResourceManager(){
	if(ismaster_){
		resourceManagerMaster_=new ResourceManagerMaster();
	}
}

AdaptiveEndPoint* Environment::getEndPoint(){
	return endpoint;
}
ExchangeTracker* Environment::getExchangeTracker(){
	return exchangeTracker;
}
ResourceManagerMaster* Environment::getResourceManagerMaster(){
	return resourceManagerMaster_;
}
Catalog* Environment::getCatalog()const{
	return catalog_;
}
