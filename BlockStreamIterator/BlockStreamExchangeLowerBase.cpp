/*
 * BlockStreamExchangeLowerBase.cpp
 *
 *  Created on: Sep 4, 2013
 *      Author: wangli
 */
#include <netdb.h>
#include <arpa/inet.h>
#include "BlockStreamExchangeLowerBase.h"
#include "../Environment.h"
#include "../common/Logging.h"
BlockStreamExchangeLowerBase::BlockStreamExchangeLowerBase() {

}

BlockStreamExchangeLowerBase::~BlockStreamExchangeLowerBase() {

}

bool BlockStreamExchangeLowerBase::ConnectToUpper(const ExchangeID &exchange_id,const std::string &ip,int &sock_fd) const {
	struct hostent* host;
	if((host=gethostbyname(ip.c_str()))==0){
		perror("gethostbyname errors!\n");
		return false;
	}

	if((sock_fd=socket(AF_INET, SOCK_STREAM,0))==-1){
		perror("socket creation errors!\n");
		return false;
	}
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	int upper_port;
	if((upper_port=et->AskForSocketConnectionInfo(exchange_id,ip))==0){
		Logging_BlockStreamExchangeLowerBase("Fails to ask %s for socket connection info, the exchange id=%d\n",ip.c_str(),exchange_id);
	}

	struct sockaddr_in serv_add;
	serv_add.sin_family=AF_INET;
	serv_add.sin_port=htons(upper_port);
	serv_add.sin_addr=*((struct in_addr*)host->h_addr);
	bzero(&(serv_add.sin_zero),8);

	int returnvalue;

	if((returnvalue=connect(sock_fd,(struct sockaddr *)&serv_add, sizeof(struct sockaddr)))==-1)
	{
		Logging_BlockStreamExchangeLowerBase("Fails to connect remote socket: %s:%d",inet_ntoa(serv_add.sin_addr),upper_port);
		return false;
	}
	Logging_BlockStreamExchangeLowerBase("connected to the Master socket :"+returnvalue);

	return true;

}

void BlockStreamExchangeLowerBase::WaitingForNotification(const int &target_socket_fd) const {
	char byte;
	int recvbytes;
	if((recvbytes=recv(target_socket_fd,&byte,sizeof(char),0))==-1){
		perror("recv error!\n");
	}

}

void BlockStreamExchangeLowerBase::WaitingForCloseNotification(const int & target_socket_fd) const {

		char byte;
		int recvbytes;
		if((recvbytes=recv(target_socket_fd,&byte,sizeof(char),0))==-1){
			perror("recv error!\n");
		}
		FileClose(target_socket_fd);
}

unsigned BlockStreamExchangeLowerBase::hash(void* input_tuple, Schema* schema,
		unsigned partition_key_index, unsigned nuppers) {
		const void* hash_key_address=schema->getColumnAddess(partition_key_index,input_tuple);
		return schema->getcolumn(partition_key_index).operate->getPartitionValue(hash_key_address,nuppers);

}
