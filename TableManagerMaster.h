/*
 * TableManagerMaster.h
 *
 *  Created on: May 12, 2013
 *      Author: wangli
 */

#ifndef TABLEMANAGERMASTER_H_
#define TABLEMANAGERMASTER_H_
#include <map>
#include <Theron/Theron.h>
#include <Theron/Defines.h>
#include <vector>
#include <string>
#include <libconfig.h++>
#include "Message.h"
#include "data_type.h"
#include "Debug.h"

class TableManagerMaster {
public:

	virtual ~TableManagerMaster();
	void add_slave(std::string ip,std::string port);
	void create_table_on_all_slaves(std::string table_name,std::vector<data_type> list);
//	class ReceiveMessageActor:public Theron::Actor
//	{
//	public:
//		ReceiveMessageActor(TableManagerMaster* tmm,Theron::Framework &framework, const char *const MasterName);
//		void Handle(const Message256 &message,const Theron::Address from);
//	private:
//		TableManagerMaster* tmm;
//	};s
	class RegisterSlaveActor:public Theron::Actor
	{
	public:
		RegisterSlaveActor();
		void Handle(const Message256 &message,const Theron::Address from);
	private:
		TableManagerMaster* tmm;
	};


	static TableManagerMaster* instance();
protected:
	TableManagerMaster();
private:
	static TableManagerMaster* _instance;
	Theron::EndPoint* endpoint;
	Theron::Framework* framework;
	std::vector<std::string> slave_id_list;
	std::map<std::string, unsigned> tables;
	unsigned table_cursor;
	RegisterSlaveActor* register_slave_actor;
	//ReceiveMessageActor* receive_massage_actor;
};

#endif /* TABLEMANAGERMASTER_H_ */
