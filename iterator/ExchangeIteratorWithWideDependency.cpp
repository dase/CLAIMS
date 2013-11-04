/*
 * ExchangeIteratorWithWideDependency.cpp
 *
 *  Created on: 2013-7-1
 *      Author: zhanglei
 */

#include "ExchangeIteratorWithWideDependency.h"
#include "../Executor/IteratorExecutorMaster.h"
#include "../Block/BlockReadableFixBuffer.h"
#include "../rename.h"
#include "../Logging.h"

#include <libconfig.h++>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include "../PortManager.h"
#include "../Environment.h"
#include "../Executor/ExchangeTracker.h"

ExchangeIteratorWithWideDependency::ExchangeIteratorWithWideDependency(State state)
:state(state){
	// TODO Auto-generated constructor stub
	port=0;
	sock_fd=0;
	in=0;
	out=0;
	collect=0;
	recvcount=0;
	printcount=0;
	spy1=0;
//	empty_sem=new semaphore(BLOCKNUM);
//	full_sem=new semaphore(0);
//	lo=new lock();
//	brfb=0;
//	for(int i=0;i<BLOCKNUM;i++){
//		eventQueue[i]=0;
//		sock_fd_lower_array[i]=0;
//		nullQueue[i]=0;
//	}
}
ExchangeIteratorWithWideDependency::ExchangeIteratorWithWideDependency()
{
	// TODO Auto-generated constructor stub
	port=0;
	sock_fd=0;
	in=0;
	out=0;
	collect=0;
	recvcount=0;
	printcount=0;
	spy1=0;

	brfb=0;
//	empty_sem=new semaphore(BLOCKNUM);
//	full_sem=new semaphore(0);
//	lo=new lock();
//	for(int i=0;i<BLOCKNUM;i++){
//		eventQueue[i]=0;
//		sock_fd_lower_array[i]=0;
//		nullQueue[i]=0;
//	}
}
ExchangeIteratorWithWideDependency::~ExchangeIteratorWithWideDependency() {
	// TODO Auto-generated destructor stub
}

bool ExchangeIteratorWithWideDependency::open(){


	empty_sem=new semaphore(BLOCKNUM);
	full_sem=new semaphore(0);
	lo=new Lock();
	for(int i=0;i<BLOCKNUM;i++){
		eventQueue[i]=0;
		sock_fd_lower_array[i]=0;
		nullQueue[i]=0;
	}

	brfb=new BlockReadableFixBuffer(state.schema,state.block_size,state.lower_ip_port_array.size(),0);
	brfb->init();

	if(PrepareTheSocket()==false)
		return false;

	if(RegisterExchange()==false){
		Logging_ExchangeIteratorWithWideDependency("Register Exchange with ID=%l fails!",state.exchange_id);
	}

	if(isMaster()){
		if(SerializeAndSendToMulti()==false)
			return false;
	}

	if(WaitForConnectionFromLowerExchangeAllReached(state.lower_ip_port_array.size())==false){
		return false;
	}

	pthread_t producerId;
	int error;
	error=pthread_create(&producerId,NULL,producerUpper,this);
	if(error!=0){
		cout<<"create threads error!"<<endl;
	}

	return true;
}

/**
 * This next function seems never to return a false value.
 */
bool ExchangeIteratorWithWideDependency::next(void *tuple){
	//maybeerror: out or out++?
	if(recvcount<1)
		usleep(1000);
	recvcount++;
	void *tuple_in_one_of_blocks_in_buffer=brfb->getStart()[out]->next();

	if(tuple_in_one_of_blocks_in_buffer>0){
//		cout<<"cao in buffer"<<recvcount++<<endl;
	//	cout<<"has tuple"<<endl;
		state.schema->copyTuple(tuple_in_one_of_blocks_in_buffer,tuple);
		return true;
	}
	else{
		//getchar();
		if(collect>=3){
			lo->destroy();
			empty_sem->destroy();
			full_sem->destroy();
			return false;
		}
//		cout<<"cao call for buffer"<<recvcount++<<endl;
		Logging_ExchangeIteratorWithWideDependency("Call for buffer in the next() method!");
		eventQueue[out]=0;

		full_sem->wait();

		lo->acquire();
		//printf("------------------------------acquire()----------------------------\n");
		out++;


		out=out%BLOCKNUM;
		lo->release();
		//printf("------------------------------release()----------------------------\n");
		empty_sem->post();
		return next(tuple);
	}
}

void *ExchangeIteratorWithWideDependency::producerUpper(void *arg){
	ExchangeIteratorWithWideDependency *Pthis=(ExchangeIteratorWithWideDependency *)arg;
	//getchar();
	while(Pthis->collect<BLOCKNUM){
//		cout<<"collect: "<<Pthis->collect<<endl;
		Pthis->empty_sem->wait();

		Pthis->lo->acquire();
		//printf("------------------------------acquire()----------------------------\n");
		if(Pthis->nullQueue[Pthis->in]==1){
			Logging_ExchangeIteratorWithWideDependency("offset %d is null, so in++",Pthis->in);
			Pthis->in++;
			Pthis->in=(Pthis->in)%BLOCKNUM;
			Pthis->lo->release();
			//printf("------------------------------release()----------------------------\n");
			Pthis->full_sem->post();
			continue;
		}
		//maybeerror: in or in++?
		Logging_ExchangeIteratorWithWideDependency("Ask [%s] for the new block..",Pthis->lower_ip_array[Pthis->in].c_str());
		if(Pthis->AskForNewBlock(Pthis->in)){
			Logging_ExchangeIteratorWithWideDependency("Received!");
			Pthis->eventQueue[Pthis->in]=1;
			//maybeerror: in can be increased?
			Pthis->in++;
			Pthis->in=Pthis->in%BLOCKNUM;
		}
		else{
			FileClose(Pthis->sock_fd_lower_array[Pthis->in]);
			Pthis->nullQueue[Pthis->in]=1;
			cout<<"Pthis->in: "<<Pthis->in<<endl;
			Pthis->in++;
			Pthis->in=Pthis->in%BLOCKNUM;
		}
		Pthis->lo->release();
		Pthis->full_sem->post();
	}
	pthread_exit(NULL);
	return (void *)(1);
}

bool ExchangeIteratorWithWideDependency::close()
{
	//for(int i=0;i<BLOCKNUM;i++){
	//	SendRequest(close_iterator,sock_fd_lower_array[i]);
	//}
	cout<<"recvcount: "<<recvcount<<endl;
	FileClose(sock_fd);
	//maybeerror: no need to close the socket map fd
	//for(int sock_fd_Lower=0;sock_fd_Lower<BLOCKNUM;sock_fd_Lower++){
	//	FileClose(sock_fd_lower_array[sock_fd_Lower]);
	//}

	return true;
}

bool ExchangeIteratorWithWideDependency::PrepareTheSocket()
{
	struct sockaddr_in my_addr;

	//sock_fd is the socket of this node
	if((sock_fd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket creation error!\n");
		return false;
	}
	my_addr.sin_family=AF_INET;

	//the port is the port of this node

	/* apply for the port dynamicaly.*/
	if((port=PortManager::getInstance()->applyPort())==0){
		Logging_ExchangeIteratorWithWideDependency("Fails to apply a port for the socket. Reason: the PortManager is exhausted!");
	}
	Logging_ExchangeIteratorWithWideDependency("The applied port for socket is %d",port);

	my_addr.sin_port=htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
//	my_addr.sin_addr.s_addr = Environment::getInstance()->getIp();
	bzero(&(my_addr.sin_zero),8);

	/* Enable address reuse */
	int on=1;
	setsockopt(sock_fd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));

	if(bind(sock_fd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
	{
		perror("bind errors!\n");
		return false;
	}
	//Currently, the sock_fd can listen 10 coming connections
	//TODO: support to tune because not know upper nodes' number
	if(listen(sock_fd, 10)==-1)
	{
		perror("listen errors!\n");
		return false;
	}

	Logging_ExchangeIteratorWithWideDependency("socket created as: %s:%d",inet_ntoa(my_addr.sin_addr),port);

	return true;
}

bool ExchangeIteratorWithWideDependency::isMaster(){
	return Environment::getInstance()->getIp()==state.upper_ip_port_array[0];
}

bool ExchangeIteratorWithWideDependency::SerializeAndSendToMulti(){
	IteratorExecutorMaster* IEM=IteratorExecutorMaster::instance();
	ExchangeIteratorLowerWithWideDependency::State EILWWDstate(state.schema,state.child,state.upper_ip_port_array,state.block_size,state.exchange_id);
	ExchangeIteratorLowerWithWideDependency *EILWWD=new ExchangeIteratorLowerWithWideDependency(EILWWDstate);

	if(IEM->ExecuteIteratorsOnSites(EILWWD,state.lower_ip_port_array)==false){
		printf("Cannot send the serialized iterator tree to the remote node!\n");
		return false;
	}
	return true;
}

bool ExchangeIteratorWithWideDependency::WaitForConnectionFromLowerExchangeAllReached(int LowerNum){
	//wait for the lower nodes send the connection information
	socklen_t sin_size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	int count=0;
	while(1){
		if(count>=BLOCKNUM){
			return true;
		}
		Logging_ExchangeIteratorWithWideDependency("Waiting for the socket connection from the lower exchange..");
		if((sock_fd_lower_array[count]=accept(sock_fd,(struct sockaddr*)&remote_addr,&sin_size))!=-1)
		{
			lower_ip_array[count]=inet_ntoa(remote_addr.sin_addr);
			Logging_ExchangeIteratorWithWideDependency("The lower exchange <%s> is connected to the socket.",lower_ip_array[count].c_str());
			count++;
		}
	}

	return true;
}

bool ExchangeIteratorWithWideDependency::AskForNewBlock(int cur){
	
	Logging_ExchangeIteratorWithWideDependency("will get cur block: ");
	if(!SendRequest(next_block,cur))
		return false;

	if(!WaitNewBlockFromLowerExchange(cur))
		return false;

	return true;
}

bool ExchangeIteratorWithWideDependency::SendRequest(request req,int block){
	switch(req)
	{
		case next_block:
		{
//			cout<<"send the new:"<<sock_fd_lower_array[block]<<endl;
			if(send(sock_fd_lower_array[block],"n",1,0)==-1)
			{
				perror("Send error!\n");
				return false;
			}
			return true;
		}
		case close_iterator:
		{
			if(send(sock_fd_lower_array[block],"c",1,0)==-1)
			{
				perror("Send error!\n");
				return false;
			}
			return true;
		}
	}
}

bool ExchangeIteratorWithWideDependency::WaitNewBlockFromLowerExchange(int block)
{
	int recvbytes;
	Logging_ExchangeIteratorWithWideDependency("Waiting...");
	if((recvbytes=recv(sock_fd_lower_array[block],brfb->getStart()[block]->getBlock(),state.block_size,MSG_WAITALL))==-1)
	{
		perror("recv error!\n");
		return false;
	}

//	if(recvbytes<state.block_size)
//	{
//		char* first=(char*)brfb->getStart()[block]->getBlock();
//		if(*first=='b')//b=busy
//		{
//			Logging_ExchangeIteratorWithWideDependency("The lower is busy!");
//			((BlockWritable*)brfb->getStart()[block])->setEmpty();
//			brfb->getStart()[block]->reset();
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}

	//cout<<"hello?: "<<recvbytes<<endl;
	if(brfb->getStart()[block]->getS()==1){
		brfb->getStart()[block]->reset();
	//	cout<<"data: "<<brfb->getStart()[block]<<endl;
		return true;
	}
	else{
		collect++;
		brfb->getStart()[block]->reset();
	//	for(int i=0;i<16;i++){
	//		cout<<collect<<"data: "<<*(reinterpret_cast<int *>(brfb->getStart()[block]->getBlock())+i)<<endl;
	//	}
		return false;
	}
//	if(recvbytes==state.block_size)
//	{
//		cout<<"every block is the block_size block!"<<endl;
//		brfb->getStart()[block]->reset();
//		return true;
//	}
//	else
//	{
//		printf("The child iterator is exhausted!\n");
//		return false;
//	}
}

bool ExchangeIteratorWithWideDependency::RegisterExchange(){
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	std::ostringstream port_str;
	port_str<<port;
	return et->RegisterExchange(state.exchange_id,port_str.str());
}
