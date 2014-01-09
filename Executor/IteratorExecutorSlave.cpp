/*
 * IteratorExecutorSlave.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#include "IteratorExecutorSlave.h"
#include "../Environment.h"
#include "../Logging.h"


IteratorExecutorSlave::IteratorExecutorSlave(){


	logging_=new IteratorExecutorSlaveLogging();
	endpoint=Environment::getInstance()->getEndPoint();

	framework=new Theron::Framework(*endpoint);
	framework->SetMaxThreads(1);
//	framework->SetMinThreads(5);
	logging_->log("Minimum thread is set to be %d",framework->GetMinThreads());
	execute_iterator_actor=new ExecuteIteratorActor(this,*framework,("IteratorExecutorActor://"+Environment::getInstance()->getIp()).c_str());
	logging_->log("Actor created with name: IteratorExecutorActor://%s",Environment::getInstance()->getIp().c_str());

}

IteratorExecutorSlave::~IteratorExecutorSlave() {

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
	IteratorMessage im=IteratorMessage::deserialize(message);

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
	printf("serialized size:%d\n\n\n\n\n\n",message.length);
	ies->logging_->log("New iterator tree received!\n");


	ies->logging_->log("Sent the response message to the Receiver!");
	IteratorMessage* runable_iterator_message=new IteratorMessage();
	*runable_iterator_message=IteratorMessage::deserialize4K(message);
	ies->createNewThreadAndRun(runable_iterator_message);
//	Send(int(0),from);
	ies->logging_->log("iterator tree is added to the running queue");
}
void IteratorExecutorSlave::ExecuteIteratorActor::progation(const int &message,const Theron::Address from){
	printf("Slave:%d\n",message);
	sleep(1);
	IteratorExecutorMaster::getInstance()->Propogation(message+1,"127.0.0.1");
}


void IteratorExecutorSlave::createNewThreadAndRun(IteratorMessage* it){

	pthread_t thread;
	void** arg=new void*[2];
	arg[0]=it;
	arg[1]=this;
	pthread_create(&thread,NULL,run_iterator,arg);
	lock_.lock();
	busy_thread_list_.insert(thread);
	lock_.unlock();

	logging_->log("A new Running thread is created!");
}
void* IteratorExecutorSlave::run_iterator(void* arg){
	IteratorMessage* it=(IteratorMessage*)(*(void**)arg);
	IteratorExecutorSlave* Pthis=(IteratorExecutorSlave*)(*((void**)arg+1));
	it->run();
	it->~IteratorMessage();
	printf("A iterator tree is successfully executed!\n");
	assert(Pthis->busy_thread_list_.find(pthread_self())!=Pthis->busy_thread_list_.end());
	Pthis->lock_.lock();
	Pthis->busy_thread_list_.erase(pthread_self());
	Pthis->lock_.unlock();
	free((void**)arg);
}
