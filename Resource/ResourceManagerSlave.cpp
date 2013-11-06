/*
 * ResourceManagerSlave.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "ResourceManagerSlave.h"
#include "../Environment.h"
#include "../TimeOutReceiver.h"
#define ResourceManagerMasterName "ResourceManagerMaster"
ResourceManagerSlave::ResourceManagerSlave() {
	framework_=new Theron::Framework(*Environment::getInstance()->getEndPoint());

}

ResourceManagerSlave::~ResourceManagerSlave() {
	framework_->~Framework();
}
NodeID ResourceManagerSlave::Register(std::string ip,unsigned port){
	NodeID ret;
	TimeOutReceiver receiver;
	Theron::Catcher<NodeID> resultCatcher;
	receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<NodeID>::Push);
	NodeRegisterMessage message(ip,port);

	framework_->Send(message,Theron::Address(ResourceManagerMasterName),Theron::Address());
	Theron::Address from;
	if(receiver.TimeOutReceiver(1,1000)==1){
		resultCatcher.Pop(ret,from);
		return ret;
	}
	else{
		return -1;
	}
}
void ResourceManagerSlave::ReportStorageBudget(RegisterStorageMessage& message){
//	receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<int>::Push);
	framework_->Send(message,Theron::Address(ResourceManagerMasterName),Theron::Address());
}
void ResourceManagerSlave::RegisterStorageSlave(){

}
