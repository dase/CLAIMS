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
//	framework->SetMaxThreads(1);

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
	ies->logging_->log("New iterator tree received!\n");
	Message256 msg;
	Send(int(0),from);
	std::cout<<"Sent the response message to the Receiver!"<<std::endl;
	IteratorMessage im=IteratorMessage::deserialize4K(message);

	im.run();
	ies->logging_->log("iterator tree is successfully executed!");

}

