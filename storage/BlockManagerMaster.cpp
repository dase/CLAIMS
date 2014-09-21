/*
 * BlockManagerMaster.cpp
 *
 *  Created on: 2013-10-15
 *      Author: casa
 */

#include <sstream>

#include "BlockManagerMaster.h"
#include "../Environment.h"
#include "../common/TimeOutReceiver.h"
#include "../common/Message.h"
#include "../utility/print_tool.h"
BlockManagerMaster *BlockManagerMaster::master_=0;

BlockManagerMaster::BlockManagerMaster() {

	framework_=new Theron::Framework(*Environment::getInstance()->getEndPoint());
	actor_=new BlockManagerMasterActor(framework_,"blockManagerMasterActor");
	logging_=new StorageManagerMasterLogging();
}

BlockManagerMaster::~BlockManagerMaster() {
	master_=0;
	delete actor_;
	delete framework_;
	delete logging_;
}

void BlockManagerMaster::initialize(){
	abi_=AllBlockInfo::getInstance();
	testForPoc();
}

void BlockManagerMaster::testForPoc(){
	bm_=BlanceMatcher::getInstance();
//	string file_name="/home/hayue/input/3_64m";
	string file_name="/home/claims/cj_/prj2/hs";
	list<string> projs;
	projs.push_back("/home/claims/cj_/prj2/hs_1");
	projs.push_back("/home/claims/cj_/prj2/hs_2");
	projs.push_back("/home/claims/cj_/prj2/hs_3");
	projs.push_back("/home/claims/cj_/prj2/hs_4");
	projs.push_back("/home/claims/cj_/prj2/hs_5");
	bm_->projectsInput(file_name.c_str(),projs);
}

BlockManagerMaster::BlockManagerMasterActor::BlockManagerMasterActor(Theron::Framework* framework,const char *name)
:Actor(*(framework),name){
	RegisterHandler(this,&BlockManagerMasterActor::workerRegister);
	RegisterHandler(this,&BlockManagerMasterActor::heartbeatReceiver);
	RegisterHandler(this,&BlockManagerMasterActor::blockStatusReceiver);
	RegisterHandler(this,&BlockManagerMasterActor::matcherReceiver);
//	cout<<"the workerRegister is ready"<<endl;
}

BlockManagerMaster::BlockManagerMasterActor::~BlockManagerMasterActor() {

}

void BlockManagerMaster::BlockManagerMasterActor::workerRegister(const StorageBudgetMessage &message,const Theron::Address from){
	// 加到blockInfo中
	cout<<"I receive message: nodeid"<<message.nodeid<<" and other information "<<from.AsString()<<endl;
	string respond="ok";
	RegisterStorageRespond rsr(respond.c_str());
	Send(rsr,from);
}

void BlockManagerMaster::BlockManagerMasterActor::heartbeatReceiver(const HeartBeatMessage &message,const Theron::Address from){
	cout<<"receive heartbeat from node "<<from.AsString()<<" shows:"<<message.mText<<endl;
	string respond="ok";
	HeartBeatRespond hbr(respond.c_str());
	Send(hbr,from);
}

void BlockManagerMaster::BlockManagerMasterActor::blockStatusReceiver(const BlockStatusMessage &message,const Theron::Address from){
	cout<<"receive a block "<<from.AsString()<<" shows:"<<message.mText<<endl;
	AllBlockInfo *abi=AllBlockInfo::getInstance();
	abi->put(from.AsString(),message.mText);
}

void BlockManagerMaster::BlockManagerMasterActor::matcherReceiver(const MatcherMessage &message,const Theron::Address from){
	cout<<"I want the proj "<<from.AsString()<<" shows:"<<message.filenameText<<" from "<<message.bmiText<<endl;
	BlockManagerId *bmi=new BlockManagerId(message.bmiText);
	BlanceMatcher *bm=BlanceMatcher::getInstance();
	string res=bm->matcher(message.filenameText,*bmi);
	MatcherRespond resp(res.c_str());
	cout<<"I will send the proj "<<res.c_str()<<" to "<<from.AsString()<<endl;
	Send(resp,from);
}
std::string BlockManagerMaster::generateSlaveActorName(const NodeID & node_id)const{
	std::ostringstream str;
	str<<"blockManagerWorkerActor://"<<NodeTracker::getInstance()->getNodeIP(node_id);
	return str.str();
}

bool BlockManagerMaster::SendBindingMessage(const PartitionID& partition_id, const unsigned& number_of_chunks, const StorageLevel& desirable_storage_level,const NodeID& target)const{
	TimeOutReceiver receiver(Environment::getInstance()->getEndPoint());

	Theron::Catcher<int> resultCatcher;
	receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<int>::Push);

	PartitionBindingMessage message(partition_id,number_of_chunks,desirable_storage_level);
	logging_->log("Sending the binding message to [%s]",generateSlaveActorName(target).c_str());
	framework_->Send(message,receiver.GetAddress(),Theron::Address(generateSlaveActorName(target).c_str()));
	if(receiver.TimeOutWait(1,200000)==0){
		logging_->elog("The node[%s] fails to receiver the partition binding message! target actor name=%s",NodeTracker::getInstance()->getNodeIP(target).c_str(),generateSlaveActorName(target).c_str());
	}

	return true;
}

//updateBindingMessage {}
bool BlockManagerMaster::UpdateBindingMessage(const PartitionID&, const unsigned& number_of_blocks, const StorageLevel& desirable_storage_level, const NodeID& target)const
{

}
