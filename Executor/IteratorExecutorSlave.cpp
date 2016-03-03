/*
 * IteratorExecutorSlave.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#include "IteratorExecutorSlave.h"
#include "../Environment.h"
#include "../common/Logging.h"
#include "../Resource/CPUResource.h"
#include "../utility/print_tool.h"
#include <iosfwd>


IteratorExecutorSlave::IteratorExecutorSlave(){


	logging_=new IteratorExecutorSlaveLogging();
	endpoint=Environment::getInstance()->getEndPoint();

	framework=new Theron::Framework(*endpoint);
	framework->SetMaxThreads(1);
//	framework->SetMinThreads(5);
	logging_->log("Minimum thread is set to be %d",framework->GetMinThreads());
	std::ostringstream str;
	str<<"IteratorExecutorActor://"<<Environment::getInstance()->getNodeID();
	execute_iterator_actor=new ExecuteIteratorActor(this,*framework,str.str().c_str());
	logging_->log("Actor created with name: IteratorExecutorActor://%d",Environment::getInstance()->getNodeID());

}

IteratorExecutorSlave::~IteratorExecutorSlave() {
	delete execute_iterator_actor;
	delete framework;
	delete logging_;
}

IteratorExecutorSlave::ExecuteIteratorActor::ExecuteIteratorActor(IteratorExecutorSlave* ies, Theron::Framework &framework, const char *const slaveID)
	:Theron::Actor(framework, slaveID),ies(ies)
{
	RegisterHandler(this, &IteratorExecutorSlave::ExecuteIteratorActor::Handler256);
	RegisterHandler(this, &IteratorExecutorSlave::ExecuteIteratorActor::Handler4K);
	RegisterHandler(this, &IteratorExecutorSlave::ExecuteIteratorActor::progation);
}

void IteratorExecutorSlave::ExecuteIteratorActor::Handler256(const Message256 &message,const Theron::Address from)
{

	ies->logging_->log("New iterator tree received!\n");
	Message256 msg;
	Send(msg,from);
	PhysicalQueryPlan im=PhysicalQueryPlan::deserialize(message);
	printf("_-_\n");
	im.run();

	ies->logging_->log("iterator tree is successfully executed!");

}
void IteratorExecutorSlave::ExecuteIteratorActor::Handler4K(const Message4K &message,const Theron::Address from)
{
//	ies->logging_->log("New iterator tree received!\n");
//
//	Send(int(0),from);
//	ies->logging_->log("Sent the response message to the Receiver!");
//	IteratorMessage im=IteratorMessage::deserialize4K(message);
//
//	im.run();
//	ies->logging_->log("iterator tree is successfully executed!");
//	printf("serialized size:%d\n\n\n\n\n\n",message.length);
	ies->logging_->log("New iterator tree received!\n");


	ies->logging_->log("Sent the response message to the Receiver!");
//	GETCURRENTTIME(s);
//	printf("Yu debug:time when received message: %ld.%ld\n", s.tv_sec, s.tv_usec);
	PhysicalQueryPlan* runable_iterator_message=new PhysicalQueryPlan();

//	GETCURRENTTIME(t);
	*runable_iterator_message=PhysicalQueryPlan::deserialize4K(message);
//	cout<<"Yu debug:deserialize message use :"<<GetElapsedTime(t)<<endl;
	ies->createNewThreadAndRun(runable_iterator_message);
//	Send(int(0),from);
	ies->logging_->log("iterator tree is added to the running queue");
}
void IteratorExecutorSlave::ExecuteIteratorActor::progation(const int &message,const Theron::Address from){
	printf("Slave:%d\n",message);
	sleep(1);
	IteratorExecutorMaster::getInstance()->Propogation(message+1,"127.0.0.1");
}


void IteratorExecutorSlave::createNewThreadAndRun(PhysicalQueryPlan* it){

	pthread_t thread;
	void** arg=new void*[2];
	arg[0]=it;
	arg[1]=this;
	pthread_create(&thread,NULL,run_iterator,arg);
	lock_.acquire();
	busy_thread_list_.insert(thread);
	lock_.release();

	logging_->log("A new Running thread is created!");
}
void* IteratorExecutorSlave::run_iterator(void* arg){
	pthread_detach(pthread_self());
	PhysicalQueryPlan* it=(PhysicalQueryPlan*)(*(void**)arg);
	IteratorExecutorSlave* Pthis=(IteratorExecutorSlave*)(*((void**)arg+1));

//	p_green("A new thread (%lx) is created.\n",pthread_self());
//	printf("--------\n Before apply:");
//	CPUResourceManager::getInstance()->print();
//	int core=CPUResourceManager::getInstance()->applyCore();
//	it->run();
//	CPUResourceManager::getInstance()->freeCore(core);
//	printf("--------\n After apply:");


	executePhysicalQueryPlan(*it);

//	CPUResourceManager::getInstance()->print();
	delete it;
	Pthis->logging_->log("A iterator tree is successfully executed!\n");
	assert(Pthis->busy_thread_list_.find(pthread_self())!=Pthis->busy_thread_list_.end());
	Pthis->lock_.acquire();
	Pthis->busy_thread_list_.erase(pthread_self());
	Pthis->lock_.release();
//	p_green("Job in thread (%lx) finished.\n",pthread_self());
	delete[] ((void**)arg);
}

void IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan plan) {
//	int core=CPUResourceManager::getInstance()->applyCore();
//	printf("--------\n After apply:");
//	CPUResourceManager::getInstance()->print();
	plan.run();
//	CPUResourceManager::getInstance()->freeCore(core);
}
