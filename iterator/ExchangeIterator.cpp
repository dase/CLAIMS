/*
 * ExchangeIterator.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: wangli
 */
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
#include <netdb.h>
#include <unistd.h>
#include "ExchangeIterator.h"
#include "../Executor/IteratorExecutorMaster.h"
#include "../Block/BlockReadableFix.h"
#include "../rename.h"
ExchangeIterator::ExchangeIterator(State state)
:state(state)
{


}

ExchangeIterator::~ExchangeIterator() {

}
bool ExchangeIterator::open()
{
	libconfig::Config cfg;
	cfg.readFile("/home/imdb/config/config");
	port=atoi((const char*)cfg.lookup("Exchange.Upper.port"));

	block=new BlockReadableFix(state.block_size,state.schema);

	if(PrepareTheSocket()==false)
		return false;

	if(SerializeAndSend()==false)
		return false;

	if(WaitForConnectionFromLowerExchange()==false)
		return false;

	return true;
}

bool ExchangeIterator::next(void* tuple)
{
	void * tuple_in_block=block->next();
	if(tuple_in_block>0)//get one tuple from the local block
	{
		state.schema->copyTuple(tuple_in_block,tuple);
		return true;
	}
	else// the local block is exhausted.
	{
		if(AskForNewBlock())
		{
			return next(tuple);
		}
		else
			return false;
	}

}
bool ExchangeIterator::close()
{
	SendRequest(close_iterator);
	FileClose(sock_fd);
	FileClose(sock_fd_Lower);
	return true;
}

ExchangeIteratorLower* ExchangeIterator::GenerateLowerExchange()
{
	libconfig::Config cfg;
	cfg.readFile("/home/imdb/config/config");
	std::string Upper_ip=(const char*)cfg.lookup("Exchange.Upper.ip");
	std::string Upper_port=(const char*)cfg.lookup("Exchange.Upper.port");

	ExchangeIteratorLower::State EILstate(state.schema,state.child,Upper_ip,Upper_port,state.block_size);
	return new ExchangeIteratorLower(EILstate);
}

bool ExchangeIterator::AskForNewBlock()
{
	if(!SendRequest(next_block))
		return false;
	if(!WaitNewBlockFromLowerExchange())
		return false;

	return true;
}
bool ExchangeIterator::SendRequest(request req)
{
	switch(req)
	{
		case next_block:
		{
			if(send(sock_fd_Lower,"n",1,0)==-1)
			{
				perror("Send error!\n");
				return false;
			}
			printf("I have sent the request for the new block.\n");
			return true;
		}
		case close_iterator:
		{
			if(send(sock_fd_Lower,"c",1,0)==-1)
			{
				perror("Send error!\n");
				return false;
			}
			printf("I have sent the close command to the Lower Exchange!\n");
			return true;
		}
	}
}
bool ExchangeIterator::PrepareTheSocket()
{
	struct sockaddr_in my_addr;


	if((sock_fd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket creation error!\n");
		return false;
	}
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	/* Enable address reuse */
	int on=1;
	setsockopt(sock_fd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));

	if(bind(sock_fd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
	{
		perror("bind errors!\n");
		return false;
	}

	if(listen(sock_fd, 10)==-1)
	{
		perror("listen errors!\n");
		return false;
	}
	return true;
}
bool ExchangeIterator::SerializeAndSend()
{
//	IteratorExecutorMaster* IEM=IteratorExecutorMaster::instance();
//	ExchangeIteratorLower* EIL=GenerateLowerExchange();//generate the lower iterator tree, the root of which is the ExchangeIteratorLower.
//	if(IEM->ExecuteIteratorsOnSlave(EIL)==false)//serialize the lower iterator tree and execute it.
//	{
//		printf("Cannot send the serialized iterator tree to the remote node!\n");
//		return false;
//	}
	return true;
}
bool ExchangeIterator::WaitForConnectionFromLowerExchange()
{
	socklen_t sin_size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	if((sock_fd_Lower=accept(sock_fd, (struct sockaddr*)&remote_addr,&sin_size))==-1)
	{
		perror("accept errors!\n");
		return false;
	}

	printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));

	return true;
}
bool ExchangeIterator::WaitNewBlockFromLowerExchange()
{
	int recvbytes;
	if((recvbytes=recv(sock_fd_Lower,block->getBlock(),state.block_size,MSG_WAITALL))==-1)
	{
		perror("recv error!\n");
		return false;
	}
	if(recvbytes==state.block_size)
	{

		block->reset();
		return true;
	}
	else
	{
		printf("Received %d bytes. ",recvbytes);
		printf("The child iterator is exhausted!\n");
		return false;
	}
}
