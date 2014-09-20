/*
 * ResourceManagerSlave.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "ResourceManagerSlave.h"
#include "../Environment.h"
#include "../common/TimeOutReceiver.h"
#define ResourceManagerMasterName "ResourceManagerMaster"
InstanceResourceManager::InstanceResourceManager() {
	framework_=new Theron::Framework(*Environment::getInstance()->getEndPoint());
	logging_=new ResourceManagerMasterLogging();
}

InstanceResourceManager::~InstanceResourceManager() {
	delete framework_;
	delete logging_;
}
NodeID InstanceResourceManager::Register(){
	NodeID ret=10;
	TimeOutReceiver receiver(Environment::getInstance()->getEndPoint());
	Theron::Catcher<NodeID> resultCatcher;
	receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<NodeID>::Push);

	std::string ip=Environment::getInstance()->getIp();
	unsigned port=Environment::getInstance()->getPort();
	NodeRegisterMessage message(ip,port);

	framework_->Send(message,receiver.GetAddress(),Theron::Address("ResourceManagerMaster"));
	Theron::Address from;
	if(receiver.TimeOutWait(1,1000)==1){

		resultCatcher.Pop(ret,from);
		logging_->log("Successfully registered to the master, the allocated id =%d.",ret);
		return ret;
	}
	else{
		logging_->elog("Failed to get NodeId from the master.");
		return -1;
	}
}
void InstanceResourceManager::ReportStorageBudget(StorageBudgetMessage& message){
	framework_->Send(message,Theron::Address(),Theron::Address(ResourceManagerMasterName));
}

void InstanceResourceManager::setStorageBudget(unsigned long memory,
		unsigned long disk) {

}
