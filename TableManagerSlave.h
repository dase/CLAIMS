/*
 * TableManagerSlave.h
 *
 *  Created on: May 10, 2013
 *      Author: wangli
 */

#ifndef TABLEMANAGERSLAVE_H_
#define TABLEMANAGERSLAVE_H_

#include <Theron/Theron.h>
#include <iostream>
#include <sstream>
#include "Message.h"
#include "TableManager.h"
#include <libconfig.h++>
#include "Debug.h"
//THERON_REGISTER_MESSAGE(TextMessages);
// In order to be sent over the network, message types must be registered.


class TableManagerSlave:public TableManager {
public:
	TableManagerSlave(std::string port,std::string ip);
	virtual ~TableManagerSlave();


public:

	class CreateTableActor: public Theron::Actor
	{
	public:
		CreateTableActor(TableManagerSlave *tms,Theron::Framework &framework, const char *const slaveID);

	private:
		void Handler(const Message256 &message,const Theron::Address from);
		void Handlerint(const int &message,const Theron::Address from);

	private:
		TableManagerSlave* tms;
	};

private:
	CreateTableActor* create_table_actor;
	Theron::Framework* framework;
	Theron::EndPoint* endpoint;
	std::string slave_id;
	std::string ip;
	std::string port;




};

#endif /* TABLEMANAGERSLAVE_H_ */
