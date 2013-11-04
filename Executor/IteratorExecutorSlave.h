/*
 * IteratorExecutorSlave.h
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#ifndef ITERATOREXECUTORSLAVE_H_
#define ITERATOREXECUTORSLAVE_H_
#include <Theron/Theron.h>
#include <string>
#include <libconfig.h++>
#include <iostream>
#include "../Message.h"
#include "../Logging.h"
using namespace std;


class IteratorExecutorSlave {
public:
	friend class ExecuteIteratorActor;
	class ExecuteIteratorActor: public Theron::Actor
	{
	public:
		ExecuteIteratorActor(IteratorExecutorSlave *ies,Theron::Framework &framework, const char *const slaveID);

	private:
		void Handler256(const Message256 &message,const Theron::Address from);
		void Handler4K(const Message4K &message,const Theron::Address from);
	private:
		IteratorExecutorSlave* ies;
	};
	IteratorExecutorSlave();

	virtual ~IteratorExecutorSlave();
protected:
	Logging * logging_;
private:
	ExecuteIteratorActor* execute_iterator_actor;
	Theron::Framework* framework;
	Theron::EndPoint* endpoint;
	std::string slave_id;

};

#endif /* ITERATOREXECUTORSLAVE_H_ */
