/*
 * BlockStreamExchangeBase.cpp
 *
 *  Created on: Sep 4, 2013
 *      Author: wangli
 */

#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "BlockStreamExchangeBase.h"
#include "../Environment.h"
#include "../Executor/ExchangeTracker.h"
#include "../common/rename.h"
#include "../common/Logging.h"
BlockStreamExchangeBase::BlockStreamExchangeBase(const std::vector<std::string> &upper_ip_list,const unsigned &nlowers)
:upper_ip_list_(upper_ip_list),nlowers_(nlowers){
	socket_fd_lower_list_=new int[nlowers_];
	lower_ip_array_=new std::string[nlowers_];
}

BlockStreamExchangeBase::~BlockStreamExchangeBase() {
	delete[] socket_fd_lower_list_;
	delete[] lower_ip_array_;
}

bool BlockStreamExchangeBase::PrepareTheSocket()
{
	struct sockaddr_in my_addr;

	//sock_fd is the socket of this node
	if((sock_fd_=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket creation error!\n");
		return false;
	}
	my_addr.sin_family=AF_INET;

	/* apply for the port dynamicaly.*/
	if((socket_port_=PortManager::getInstance()->applyPort())==0){
		Logging_BlockStreamExchangeBase("Fails to apply a port for the socket. Reason: the PortManager is exhausted!");
	}
	Logging_BlockStreamExchangeBase("The applied port for socket is %d",socket_port_);

	my_addr.sin_port=htons(socket_port_);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	/* Enable address reuse */
	int on=1;
	setsockopt(sock_fd_, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));
//	int size=1024*1024*64;
//	setsockopt(sock_fd,SOL_SOCKET,SO_RCVBUF,(const char*)&size,sizeof(int));



	if(bind(sock_fd_,(struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
	{
		perror("bind errors!\n");
		return false;
	}

	if(listen(sock_fd_, nlowers_)==-1)
	{
		perror("listen errors!\n");
		return false;
	}

	Logging_BlockStreamExchangeBase("socket created as: %s:%d",inet_ntoa(my_addr.sin_addr),socket_port_);

	return true;
}

bool BlockStreamExchangeBase::RegisterExchange(ExchangeID exchange_id){
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	std::ostringstream port_str;
	port_str<<socket_port_;
	return et->RegisterExchange(exchange_id,port_str.str());
}
bool BlockStreamExchangeBase::isMaster(){
	return Environment::getInstance()->getIp()==upper_ip_list_[0];
}
bool BlockStreamExchangeBase::WaitForConnectionFromLowerExchanges(){
	//wait for the lower nodes send the connection information
	socklen_t sin_size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	unsigned count=0;
	while(1){
		if(count>=nlowers_){
			return true;
		}
		Logging_BlockStreamExchangeBase("Waiting for the socket connection from the lower exchange..");
		if((socket_fd_lower_list_[count]=accept(sock_fd_,(struct sockaddr*)&remote_addr,&sin_size))!=-1)
		{
			lower_ip_array_[count]=inet_ntoa(remote_addr.sin_addr);
			Logging_ExchangeIteratorEager("The lower exchange <%s> is connected to the socket.",lower_ip_array_[count].c_str());
			count++;
		}
	}

	return true;
}
void BlockStreamExchangeBase::CloseTheSocket(){
	/* colse the sockets of the lowers*/
	for(unsigned i=0;i<nlowers_;i++){
		FileClose(socket_fd_lower_list_[i]);
		std::cout<<"in "<<__FILE__<<":"<<__LINE__;printf("-----for debug: close fd %d.\n", socket_fd_lower_list_[i]);
	}

	/* close the socket of this exchange*/
	FileClose(sock_fd_);
	std::cout<<"in "<<__FILE__<<":"<<__LINE__;printf("-----for debug: close fd %d.\n", sock_fd_);

	/* return the applied port to the port manager*/
	PortManager::getInstance()->returnPort(socket_port_);
}
void BlockStreamExchangeBase::SendBlockBufferedNotification(int target_socket_fd){
	char content='c';
	if(send(target_socket_fd,&content,sizeof(char),0)==-1){
		perror("Send error!\n");
		return ;
	}

}
void BlockStreamExchangeBase::SendBlockAllConsumedNotification(int target_socket_fd){
	char content='e';
	if(send(target_socket_fd,&content,sizeof(char),MSG_WAITALL)==-1){
		perror("Send error!\n");
		return ;
	}
}
