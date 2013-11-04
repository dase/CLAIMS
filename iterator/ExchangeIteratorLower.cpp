/*
 * ExchangeIteratorLower.cpp
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
#include <string>

#include "ExchangeIteratorLower.h"
#include "../Block/BlockWritableFix.h"
#include "../rename.h"
//typedef int (f)(int);
//f *file_close=close;
ExchangeIteratorLower::ExchangeIteratorLower(State state)
:state(state),block(0),sock_fd(0)
{
	// TODO Auto-generated constructor stub

}

ExchangeIteratorLower::~ExchangeIteratorLower() {
	// TODO Auto-generated destructor stub
}

bool ExchangeIteratorLower::open()
{
	state.child->open();
	block=new BlockWritableFix(state.block_size,state.schema);
	tuple=malloc(state.schema->getTupleMaxSize());

	libconfig::Config cfg;
	cfg.readFile("/home/imdb/config/config");
	unsigned short port=atoi((const char*)cfg.lookup("Exchange.Upper.port"));
	std::string ip=(const char*)cfg.lookup("Exchange.Upper.ip");

	struct hostent* host;

	if((host=gethostbyname(ip.c_str()))==0)
	{
		perror("gethostbyname errors!\n");
		return false;
	}
	if((sock_fd = socket(AF_INET, SOCK_STREAM,0))==-1)
	{
		perror("socket craetion errors!\n");
		return 1;
	}

	if(ConnectToUpperExchange(host,port)==false)
		return false;


	int recvbytes;
	char buf[2];
	while(1)
	{
		if((recvbytes=recv(sock_fd,buf,1,0))==-1)
		{
			perror("recv error!\n");
			return false;
		}
		printf("Recevied bytes: %d",recvbytes);
		if(buf[0]=='n')
		{
			if(PrepareForTheNewBlock())
			{
				SendTheNewBlock();
			}
			else
			{
				SendTheFinishNotification();
			}
		}
		else
		{
			printf("Received the close command from the Upper Exchange!\n");
			return true;
		}
	}
	return true;
}

bool ExchangeIteratorLower::next(void* tuple)
{
	return false;
}

bool ExchangeIteratorLower::ConnectToUpperExchange(hostent* host, unsigned short port)
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(port);
	serv_addr.sin_addr=*((struct in_addr*)host->h_addr);
	bzero(&(serv_addr.sin_zero),8);
	if(connect(sock_fd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr))==-1)
	{
		perror("connection errors!\n");
		return false;
	}
	printf("connected to the Master socket !\n");
	return true;
}

/**
 * return true if new block is generated, return false if the
 * child iterator is exhausted before inserting any tuple into the block.
 */
bool ExchangeIteratorLower::PrepareForTheNewBlock()
{
	block->reset();
	while(state.child->next(tuple))
	{
		if(!block->insert(tuple))
		{
			break;
		}
	}
	if(block->getTupleCount()==0)
		return false;

	block->fold();
	return true;
}

bool ExchangeIteratorLower::SendTheNewBlock()
{
	if(send(sock_fd,block->getBlock(),block->getsize(),0)==-1)
	{
		perror("Send error!\n");
		return false;
	}
	printf("I have sent the new block(%d bytes).\n",block->getsize() );
	return true;
}

bool ExchangeIteratorLower::SendTheFinishNotification()
{
	if(send(sock_fd,"f",1,0)==-1)
	{
		perror("Send error!\n");
		return false;
	}
	printf("The input iterator is exhausted. I have sent the notification message. \n");
	return true;
}

bool ExchangeIteratorLower::close()
{
	free(tuple);
	block->~BlockWritable();
	state.child->close();
	FileClose(sock_fd);
	return true;
}
