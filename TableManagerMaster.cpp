/*
 * TableManagerMaster.cpp
 *
 *  Created on: May 12, 2013
 *      Author: wangli
 */

#include "TableManagerMaster.h"
#include "TimeOutReceiver.h"
TableManagerMaster::TableManagerMaster():table_cursor(0) {
	// TODO Auto-generated constructor stub
	_instance=this;

	libconfig::Config cfg;
	cfg.readFile(CONFIG);

	std::string master_ip=(const char*)cfg.lookup("TableManager.Master.ip");
	std::string master_port=(const char*)cfg.lookup("TableManager.Master.port");

//	slave_id_list.push_back("10.11.1.224:6000");
	endpoint=new Theron::EndPoint((master_ip+":"+master_port).c_str(),("tcp://"+master_ip+":"+master_port).c_str());
	endpoint->Connect("tcp://10.11.1.224:6000");
	framework=new Theron::Framework(*endpoint);
	framework->SetMaxThreads(3);
	register_slave_actor=new RegisterSlaveActor();
	//receive_massage_actor= new ReceiveMessageActor(this,*framework,"Master");


}

TableManagerMaster::~TableManagerMaster() {
	// TODO Auto-generated destructor stub
}

void TableManagerMaster::add_slave(std::string ip,std::string port)
{
	slave_id_list.push_back(ip+":"+port);
}


void TableManagerMaster::create_table_on_all_slaves(std::string table_name,std::vector<data_type> list)
{
	if(tables.find(table_name)!=tables.end())
	{
		printf("Table %s already exists!\n",table_name.c_str());
		return;
	}
	tables[table_name]=table_cursor++;
	printf("Command: Create Table %s\n", table_name.c_str());
	CreateTableMessage CTM(tables[table_name],list);
	Message256 str= CreateTableMessage::serializae(CTM);


	TimeOutReceiver receiver(endpoint,"asdfasdfasf");




	Theron::Catcher<Message256> resultCatcher;
	receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<Message256>::Push);
		printf("I send to %s!\n",slave_id_list[0].c_str());
	 framework->Send(
			 	 	 str,
		            //Theron::Address("Master"),
			 	 	 receiver.GetAddress(),
		            Theron::Address(("CreateTableActor:"+slave_id_list[0]).c_str()));
	 uint timeout=10;
	 if(receiver.TimeOutWait(1,1000)==0)
		 printf("Time out!\n");
	 else
	 {
		 Message256 mes;

		 Theron::Address from;
		 if(!resultCatcher.Empty())
		 {
			 resultCatcher.Pop(mes,from);
			 CreateTableRespond CTR=CreateTableRespond::deserialize(mes);
			 if(CTR.result)
			 {
				 printf("Receive Message: succeed!\n");
			 }
			 else
			 {
				 printf("Receive Message: failed!");
			 }
		 }
	 }


}
TableManagerMaster* TableManagerMaster::_instance=0;
TableManagerMaster* TableManagerMaster::instance()
{
	if(_instance==0)
	{
		return new TableManagerMaster();
	}
	else
		return _instance;
}

//TableManagerMaster::ReceiveMessageActor::ReceiveMessageActor(TableManagerMaster* tmm,Theron::Framework &framework, const char *const MasterName)
//:Theron::Actor(framework,MasterName),tmm(tmm)
//{
//	RegisterHandler(this,&TableManagerMaster::ReceiveMessageActor::Handle);
//}
//void TableManagerMaster::ReceiveMessageActor::Handle(const Message256 &message,const Theron::Address from)
//{
//	CreateTableRespond CTR=CreateTableRespond::deserialize(message);
//	if(CTR.result)
//	{
//		printf("Create table Succeeds!\n");
//	}
//	else
//	{
//		printf("Fail to Create table~!\n");
//	}
//	return;
//}

TableManagerMaster::RegisterSlaveActor::RegisterSlaveActor()
:Actor(*(TableManagerMaster::instance()->framework),"RegisterSlaveActor")
{
	printf("asfasf\n");
	tmm=TableManagerMaster::instance();
	if(tmm)
	{
		printf("tmm is not null!\n");
	}
	else
	{
		printf("tmm is null!\n");
	}
//	Actor(*(tmm->framework),"RegisterSlaveActor");
	RegisterHandler(this,&TableManagerMaster::RegisterSlaveActor::Handle);

}
void TableManagerMaster::RegisterSlaveActor::Handle(const Message256 &message,const Theron::Address from)
{
	RegisterSlaveMessage RSM=RegisterSlaveMessage::deserialize(message);
	printf("Receive Register request from %s : %s\n",RSM.ip.c_str(),RSM.port.c_str());

	tmm->slave_id_list.push_back(RSM.ip+":"+RSM.port);

	tmm->framework->Send(0,Theron::Address(),Theron::Address("RegisterReplyReceiver"));
}
