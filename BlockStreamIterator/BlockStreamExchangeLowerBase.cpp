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
#include "../common/ids.h"
BlockStreamExchangeLowerBase::BlockStreamExchangeLowerBase() {

}

BlockStreamExchangeLowerBase::~BlockStreamExchangeLowerBase() {

}

bool BlockStreamExchangeLowerBase::ConnectToUpper(const ExchangeID &exchange_id,const NodeID &id,int &sock_fd, Logging* log) const {
	struct hostent* host;
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	int upper_port;
	NodeAddress upper_addr;
	if(!(et->AskForSocketConnectionInfo(exchange_id,id,upper_addr))){
		log->elog("Fails to ask Node %d for socket connection info, the exchange id=%d\n",id,exchange_id);
		return false;
	}

	if((host=gethostbyname(upper_addr.ip.c_str()))==0){
		perror("gethostbyname errors!\n");
		return false;
	}

	if((sock_fd=socket(AF_INET, SOCK_STREAM,0))==-1){
		perror("socket creation errors!\n");
		return false;
	}
	struct sockaddr_in serv_add;
	serv_add.sin_family=AF_INET;
	serv_add.sin_port=htons(atoi(upper_addr.port.c_str()));
	serv_add.sin_addr=*((struct in_addr*)host->h_addr);
//	serv_add.sin_addr.s_addr=inet_addr(host->h_name);
	bzero(&(serv_add.sin_zero),8);

	int returnvalue;

	if((returnvalue=connect(sock_fd,(struct sockaddr *)&serv_add, sizeof(struct sockaddr)))==-1)
	{
		log->elog("Fails to connect remote socket: %s:%d",inet_ntoa(serv_add.sin_addr),upper_port);
		return false;
	}
	log->log("connected to the Master socket :"+returnvalue);
//	printf("Lower [%ld,%d] will be connect to Upper with target port %s\n",exchange_id.exchange_id,exchange_id.partition_offset,upper_addr.port.c_str());

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
//		std::cout<<"in "<<__FILE__<<":"<<__LINE__;printf("-----for debug: close fd %d.\n", target_socket_fd);
}


unsigned BlockStreamExchangeLowerBase::hash(void* input_tuple, Schema* schema,
		unsigned partition_key_index, unsigned nuppers) {
		const void* hash_key_address=schema->getColumnAddess(partition_key_index,input_tuple);
		return schema->getcolumn(partition_key_index).operate->getPartitionValue(hash_key_address,nuppers);

}
