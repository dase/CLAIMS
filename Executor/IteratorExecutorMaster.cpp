/*
 * IteratorExecutorMaster.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#include "IteratorExecutorMaster.h"
#include "../Environment.h"
#include "../common/TimeOutReceiver.h"
#include "../utility/rdtsc.h"
IteratorExecutorMaster* IteratorExecutorMaster::_instance=0;


IteratorExecutorMaster::IteratorExecutorMaster(){
	_instance=this;
	endpoint=Environment::getInstance()->getEndPoint();
	framework=new Theron::Framework(*endpoint);
	framework->SetMaxThreads(1);
//	framework->SetMinThreads(5);
	logging_=new IteratorExecutorMasterLogging();
}

IteratorExecutorMaster::~IteratorExecutorMaster() {
	_instance=0;
	delete framework;
}

IteratorExecutorMaster* IteratorExecutorMaster::getInstance()
{
	if(_instance==0)
	{
		return new IteratorExecutorMaster();
	}
	else
		return _instance;
}

bool IteratorExecutorMaster::ExecuteBlockStreamIteratorsOnSites(PhysicalOperatorBase* it, std::vector<std::string> ip_list){

	PhysicalQueryPlan im(it);

	Message4K str= PhysicalQueryPlan::serialize4K(im);

	TimeOutReceiver receiver(endpoint);

	Theron::Catcher<int> resultCatcher;

	receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<int>::Push);
	for(unsigned slave_filter_id=0;slave_filter_id<ip_list.size();slave_filter_id++){
		ostringstream ip_port;
		ip_port<<"IteratorExecutorActor://"<<ip_list[slave_filter_id];
		cout<<"actname: "<<ip_port.str()<<endl;
//		framework->Send(int(1),receiver.GetAddress(),Theron::Address(ip_port.str().c_str()));
		framework->Send(str,receiver.GetAddress(),Theron::Address(ip_port.str().c_str()));
		logging_->log("send!\n********** please rebounce!\n");
	}
	unsigned feedback_count=0;
	feedback_count=receiver.TimeOutWait(ip_list.size(),5000);

	if(feedback_count<ip_list.size()){
		logging_->elog("Time out! only received %d feedbacks \n",feedback_count);
		return true;
	}

	return true;
}
bool IteratorExecutorMaster::ExecuteBlockStreamIteratorsOnSite(PhysicalOperatorBase* it,NodeID target_id){
	PhysicalQueryPlan* im = new PhysicalQueryPlan(it);

//	GETCURRENTTIME(s);
	Message4K str= PhysicalQueryPlan::serialize4K(*im);
//	cout<<"Yu debug: serialize message use:"<<GetElapsedTime(s)<<endl;

	// if target is this node, send plan to Slave directly other than by network
	if (target_id == 0) {
		PhysicalQueryPlan* new_plan = new PhysicalQueryPlan(PhysicalQueryPlan::deserialize4K(str));
		Environment::getInstance()->getIteratorExecutorSlave()->createNewThreadAndRun(new_plan);
		logging_->log("The iterator tree has been sent to local slave.\n");
		return true;
	}
//	TimeOutReceiver receiver(endpoint);
//
//	Theron::Catcher<Message256> resultCatcher;
//	receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<Message256>::Push);
	ostringstream actor_name;

	actor_name<<"IteratorExecutorActor://"<<target_id;
//	cout<<"actname: "<<ip_port.str()<<endl;
//	framework->Send(str,receiver.GetAddress(),Theron::Address(ip_port.str().c_str()));
//	GETCURRENTTIME(t);
//	printf("Yu debug:time when to send message: %ld.%ld\n", t.tv_sec, t.tv_usec);
	framework->Send(str,Theron::Address(),Theron::Address(actor_name.str().c_str()));
	logging_->log("The serialized iterator tree has been sent to %s.\n",actor_name.str().c_str());
//
//	unsigned feedback_count=0;
//	feedback_count=receiver.TimeOutWait(1,5000);
//
//	if(feedback_count!=1){
//		logging_->elog("Time out! no feedback received! \n");
//		return true;
//	}
//	logging_->log("Received the confirm feedback from %s",ip_port.str().c_str());
	return true;
}
bool IteratorExecutorMaster::Propogation(const int count,std::string target){
	printf("Master:%d\n",count);
	ostringstream ip_port;

	ip_port<<"IteratorExecutorActor://"<<target;

	framework->Send(count,Theron::Address(),Theron::Address(ip_port.str().c_str()));
	sleep(100);
}
