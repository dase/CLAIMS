/*
 * TableManagerSlave.cpp
 *
 *  Created on: May 10, 2013
 *      Author: wangli
 */



#include "TableManagerSlave.h"
#include "TimeOutReceiver.h"
TableManagerSlave::TableManagerSlave(std::string ip,std::string port)
:TableManager(),port(port),ip(ip)

{
	libconfig::Config cfg;
	cfg.readFile(CONFIG);
	std::string master_ip=(const char*)cfg.lookup("TableManager.Master.ip");
	std::string master_port=(const char*)cfg.lookup("TableManager.Master.port");

	std::string slave_ip=(const char*)cfg.lookup("ip");
	std::string slave_port=(const char*)cfg.lookup("TableManager.Slave.port");

	std::string slave_id="TableManagerSlave:"+slave_ip+":"+slave_port;
	printf("%s\n",slave_id.c_str());



	endpoint=new Theron::EndPoint((slave_ip+":"+slave_port).c_str(),("tcp://"+slave_ip+":"+slave_port).c_str());


	if(!endpoint->Connect(("tcp://"+master_ip+":"+master_port).c_str()))
	{
		 printf("ERROR: Connection failed - check networking is enabled.\n");
	}
	else
	{
		printf("Connect to %s : %s successfully!\n",master_ip.c_str(),master_port.c_str());
	}

	framework=new Theron::Framework(*endpoint);
	framework->SetMaxThreads(3);
	create_table_actor=new CreateTableActor(this,*framework,("CreateTableActor:"+slave_ip+":"+slave_port).c_str());
	printf("ActorName: %s",(slave_ip+":"+slave_port).c_str());

	sleep(1);

	TimeOutReceiver receiver(endpoint,"RegisterReplyReceiver");
	Theron::Catcher<int> resultCatcher;
	receiver.RegisterHandler(&resultCatcher, &Theron::Catcher<int>::Push);




	while(receiver.TimeOutWait(1,1000)==0)
	{
		framework->Send(RegisterSlaveMessage::serialize(RegisterSlaveMessage(slave_ip, slave_port)),receiver.GetAddress(),Theron::Address("RegisterSlaveActor"));
	}

		printf("Register to the master succefully!\n");



}

TableManagerSlave::~TableManagerSlave() {
	// TODO Auto-generated destructor stub
}


TableManagerSlave::CreateTableActor::CreateTableActor(TableManagerSlave* tms, Theron::Framework &framework, const char *const slaveID)
	:Theron::Actor(framework, slaveID),tms(tms)
{
	RegisterHandler(this, &TableManagerSlave::CreateTableActor::Handler);
	RegisterHandler(this, &TableManagerSlave::CreateTableActor::Handlerint);
}
void TableManagerSlave::CreateTableActor::Handler(const Message256 &message,const Theron::Address from)
{

	printf("Create commande recivied!\n");

	CreateTableMessage ctm=CreateTableMessage::deserializae(message);

	bool result=tms->create_table_partition(ctm.table_id,ctm.type_list);

	CreateTableRespond CTR(result);
	tms->framework->Send(CreateTableRespond::serialize(CTR),Theron::Address(),from);


//	tms->framework->Send(RegisterSlaveMessage::serialize(RegisterSlaveMessage("123", "33")),Theron::Address(),Theron::Address("RegisterSlaveActor"));


}
void TableManagerSlave::CreateTableActor::Handlerint(const int &message,const Theron::Address from)
{

	printf("number = %d -_-!\n",message);




//	tms->framework->Send(RegisterSlaveMessage::serialize(RegisterSlaveMessage("123", "33")),Theron::Address(),Theron::Address("RegisterSlaveActor"));


}
