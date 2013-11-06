/*
 * BlockManagerMaster.cpp
 *
 *  Created on: 2013-10-15
 *      Author: casa
 */



#include "BlockManagerMaster.h"

BlockManagerMaster *BlockManagerMaster::master_=0;

BlockManagerMaster::BlockManagerMaster(BlockManagerMasterActor *driverActor) {
	driverActor_=driverActor;
}

BlockManagerMaster::~BlockManagerMaster() {

}

void BlockManagerMaster::initialize(){
	cout<<"initialize the blockManagerMaster"<<endl;
	abi_=AllBlockInfo::getInstance();
	testForPoc();
}

void BlockManagerMaster::testForPoc(){
	bm_=BlanceMatcher::getInstance();
//	string file_name="/home/hayue/input/3_64m";
	string file_name="/home/imdb/cj_/prj2/hs";
	list<string> projs;
	projs.push_back("/home/imdb/cj_/prj2/hs_1");
	projs.push_back("/home/imdb/cj_/prj2/hs_2");
	projs.push_back("/home/imdb/cj_/prj2/hs_3");
	projs.push_back("/home/imdb/cj_/prj2/hs_4");
	projs.push_back("/home/imdb/cj_/prj2/hs_5");
	bm_->projectsInput(file_name.c_str(),projs);
}

BlockManagerMaster::BlockManagerMasterActor::BlockManagerMasterActor(Theron::EndPoint *endpoint,Theron::Framework* framework,const char *name)
:Actor(*(framework),name),endpoint_(endpoint),framework_(framework){
	RegisterHandler(this,&BlockManagerMasterActor::workerRegister);
	RegisterHandler(this,&BlockManagerMasterActor::heartbeatReceiver);
	RegisterHandler(this,&BlockManagerMasterActor::blockStatusReceiver);
	RegisterHandler(this,&BlockManagerMasterActor::matcherReceiver);
	cout<<"the workerRegister is ready"<<endl;
}

BlockManagerMaster::BlockManagerMasterActor::~BlockManagerMasterActor() {

}

void BlockManagerMaster::BlockManagerMasterActor::workerRegister(const RegisterStorageMessage &message,const Theron::Address from){
	cout<<"I am in the workerRegister!"<<endl;
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
