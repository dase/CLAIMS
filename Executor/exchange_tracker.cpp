/*
 * exchange_tracker.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: wangli
 */

#include <string>
#include <sstream>
#include "../common/Logging.h"
#include "../Environment.h"
#include "../common/TimeOutReceiver.h"
#include "../utility/rdtsc.h"
#include "../common/ids.h"
#include <iosfwd>

#include "exchange_tracker.h"
ExchangeTracker::ExchangeTracker() {
	endpoint=Environment::getInstance()->getEndPoint();
	framework=new Theron::Framework(*endpoint);
	framework->SetMaxThreads(1);
	logging_=new ExchangeTrackerLogging();
	std::ostringstream name;
	name<<"ExchangeTrackerActor://"<<Environment::getInstance()->getNodeID();
	actor=new ExchangeTrackerActor(this,framework,name.str().c_str());
	logging_->log("%s created!",name.str().c_str());


}

ExchangeTracker::~ExchangeTracker() {
	delete logging_;
	delete actor;
	delete framework;
	// TODO Auto-generated destructor stub
}
bool ExchangeTracker::RegisterExchange(ExchangeID id, std::string port){
	lock_.acquire();
	if(id_to_port.find(id)!=id_to_port.end()){
		logging_->log("RegisterExchange fails because the exchange id has already existed.");
		lock_.release();
		return false;
	}
	id_to_port[id]=port;
	logging_->log("New exchange with id=%d (port %s)is successfully registered!",id.exchange_id,port.c_str());
	lock_.release();
	return true;
}
void ExchangeTracker::LogoutExchange(const ExchangeID &id){
	lock_.acquire();
	boost::unordered_map<ExchangeID,std::string> ::const_iterator it=id_to_port.find(id);
//	if(it==id_to_port.cend()){
//		printf("Print:\n");
//		this->printAllExchangeId();
//		printf("Printed!\n");
//	}
	assert(it!=id_to_port.cend());
	id_to_port.erase(it);
	lock_.release();
	logging_->log("Exchange with id=(%d,%d) is logged out!",id.exchange_id,id.partition_offset);
}

bool ExchangeTracker::AskForSocketConnectionInfo(ExchangeID exchange_id,NodeID target_id,NodeAddress & node_addr){
	unsigned long long int step1,step2;
//	return 17002;
	step1=curtick();
	step2=curtick();
	TimeOutReceiver* receiver=new TimeOutReceiver(endpoint);
	Theron::Catcher<NodeRegisterMessage> ResultCatcher;
	receiver->RegisterHandler(&ResultCatcher,&Theron::Catcher<NodeRegisterMessage>::Push);
	std::ostringstream str;
	str<<"ExchangeTrackerActor://"<<target_id;
	bool send_result=framework->Send(exchange_id,receiver->GetAddress(),Theron::Address(str.str().c_str()));
	unsigned Timeout=30000;	//timeout in millisecond

//	if(receiver->TimeOutWait(1,Timeout)==0){
//		/**
//		 * TODO: In current version, the request is only tried once. In the future,
//		 * the request should be sent repeatedly until the reply is received or the
//		 * times of timeouts exceeds some threshold.
//		 */
//		logging_->elog("Timeout when asking node[%s] for the connection info, the request Exchange ID is %u",target_ip.c_str(),exchange_id);
//		assert(false);
//		return 0;
//	}


//	receiver.Wait(1);

	Message256 feedback;
	Theron::Address from;
//	ResultCatcher.Pop(feedback,from);
	NodeRegisterMessage received("0",0);
//	printf("OOOOOOOOOOOO step 1:%4.4f\n",getSecond(step1));
	while(!ResultCatcher.Pop(received,from));
	node_addr.ip=received.get_ip();
	std::ostringstream str1;
	str1<<received.port;
	node_addr.port=str1.str();
//	ResultCatcher.Pop(port,from);
//	NodeConnectionMessage NCM=NodeConnectionMessage::deserialize(feedback);
//	logging_->log("Receive Socket connection info from <%s>, content: %s:%s",from.AsString(),NCM.ip.c_str(),NCM.port.c_str());
//	receiver.~Receiver();
//	return atoi(NCM.port.c_str());
//	printf("OOOOOOOOOOOO step 2:%4.4f\n",getSecond(step2));
	receiver->~TimeOutReceiver();
	return received.ip!=0;
}

ExchangeTracker::ExchangeTrackerActor::ExchangeTrackerActor(ExchangeTracker* et,Theron::Framework* framework,const char* Name)
:et(et),Actor(*framework,Name){
	RegisterHandler(this,&ExchangeTracker::ExchangeTrackerActor::AskForConnectionInfo);
}

void ExchangeTracker::ExchangeTrackerActor::AskForConnectionInfo(const ExchangeID &exchange_id, const Theron::Address from){
	et->logging_->log("%s is asking for the socket connecton info!",from.AsString());
	et->lock_.acquire();
	NodeRegisterMessage node_addr("0",0);
	if(et->id_to_port.find(exchange_id)!=et->id_to_port.cend()){

//		NodeConnectionMessage myNCM(Environment::getInstance()->getIp(),et->id_to_port[exchange_id]);
		node_addr.set_ip(Environment::getInstance()->getIp());
		node_addr.port=atoi(et->id_to_port[exchange_id].c_str());
		Send(node_addr,from);
//		Send(NodeConnectionMessage::serialize(myNCM),from);
		et->logging_->log("The ask is answered!");
	}
	else{

//		Send(NodeConnectionMessage::serialize(NodeConnectionMessage("0","0")),from);
		Send(node_addr,from);
		et->logging_->log("No exchange matched for %lld!",exchange_id.exchange_id);
	}
	et->lock_.release();

}

void ExchangeTracker::printAllExchangeId() const {
	for(boost::unordered_map<ExchangeID,std::string>::const_iterator it=id_to_port.cbegin();it!=id_to_port.cend();it++){
		printf("(%ld,%ld) --->%s\n",it->first.exchange_id,it->first.partition_offset,it->second.c_str());
	}
}
