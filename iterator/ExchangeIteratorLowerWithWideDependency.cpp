/*
 * ExchangeIteratorLowerWithWideDependency.cpp
 *
 *  Created on: 2013-6-30
 *      Author: zhanglei
 */

#include "ExchangeIteratorLowerWithWideDependency.h"
#include "../Block/BlockWritableFixBuffer.h"
#include "../rename.h"
#include "../Logging.h"
#include "../Executor/ExchangeTracker.h"
#include "../Environment.h"
#include "../configure.h"
#include <libconfig.h++>
#include <stdlib.h>
#include <error.h>
#include <string>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>



ExchangeIteratorLowerWithWideDependency::ExchangeIteratorLowerWithWideDependency(State state):state(state) {
	// TODO Auto-generated constructor stub
	for(int i=0;i<upper_count;i++){
		eventQueue[i]=0;
		sock_fd[i]=0;
	}
}

ExchangeIteratorLowerWithWideDependency::~ExchangeIteratorLowerWithWideDependency() {
	// TODO Auto-generated destructor stub
}

bool ExchangeIteratorLowerWithWideDependency::open(){
	state.child->open();
	upper_count=state.upper_ip_port_array.size();
	bwfb=new BlockWritableFixBuffer(state.schema,state.block_size,state.upper_ip_port_array.size(),0);
	bwfb->init();
	over=1;
	sendcount=0;
	empty_sem=new semaphore(upper_count);
	full_sem=new semaphore();
	lo=new Lock();
	eventQueue=new int[upper_count];
	sock_fd=new int[upper_count];

	tuple=memalign(cacheline_size,state.schema->getTupleMaxSize());
	//set upper_ip_port block and send the blocks out
	for(int upper_ip_port_num=0;upper_ip_port_num<state.upper_ip_port_array.size();upper_ip_port_num++){

		struct hostent* host;
		if((host=gethostbyname(state.upper_ip_port_array[upper_ip_port_num].c_str()))==0){
			perror("gethostbyname errors!\n");
			return false;
		}

//		struct sockaddr_in temp;
//		temp.sin_family=AF_INET;
//		temp.sin_port=htons(100);
//		temp.sin_addr=*((struct in_addr*)host->h_addr);

		if((sock_fd[upper_ip_port_num]=socket(AF_INET, SOCK_STREAM,0))==-1){
			perror("socket creation errors!\n");
			return false;
		}

		ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
		int upper_port;
		if((upper_port=et->AskForSocketConnectionInfo(state.exchange_id,state.upper_ip_port_array[upper_ip_port_num]),upper_port)==0){
			Logging_ExchangeIteratorLowerWithWideDependency("Fails to ask %s for socket connection info, the exchange id=%d",state.upper_ip_port_array[upper_ip_port_num].c_str(),state.exchange_id);
		}

		if(ConnectToUpperExchangeWithMulti(sock_fd[upper_ip_port_num],host,upper_port)==false)
			return false;

	}


	//Currently, the producer and prochase function are created in the open func
	//TODO: when create them, abstract a thread from the threads pool to do the two func
	pthread_t prochaseId;
	int error;
	error=pthread_create(&prochaseId,NULL,prochaseLower,this);
	if(error!=0){
		cout<<"create threads error!"<<endl;
	}
	cout<<"==================: "<<over<<endl;
	//when there is no tuple in the file, give a value 0 to the variable "over"
	while(over==1){
		Logging_ExchangeIteratorLowerWithWideDependency("in the while loop to listen the fd set");
		//getchar();
		fd_set socketfds;
		FD_ZERO(&socketfds);
		//Currently, here must adopt the "select" syscall to monitor the socket_fd, for example, there are
		//2 upper hash partitions on the upper, the buffer size can be 2*n. the fd_set can be use to monitor
		//the 2 socket_fd, when one of the fd occurred a event, will invoke the PrepareForTheNewBlock
		int sock_fd_max=0;
		for(int i=0;i<upper_count;i++){
			FD_SET(sock_fd[i],&socketfds);
			if(sock_fd[i]>sock_fd_max)
				sock_fd_max=sock_fd[i];
		}
		//Currently, we come up with a algorithm, the main meaning is as follows:
		//---------------------------------------------
		//if(nth block in blockwritablebuffer full)
		//	if(n belongs to socket fd_set)
		//		send the n block out;
		//	else
		//		int returnvalue=0;
		//		wai(returnvalue=t for the socket fd come into fd_set)
		//---------------------------------------------
		//TODO:	now, the time which waiting for socket fd is maybe long, the longest
		//time is one block handling long, and multi block in the buffer can be used
		//to decrease the time consuming here
		if(select(sock_fd_max+1,&socketfds,(fd_set*)NULL,(fd_set*)NULL,(struct timeval*)NULL)>0){
			for(int j=0;j<upper_count;j++){
				if(FD_ISSET(sock_fd[j],&socketfds)){
					empty_sem->wait();
					lo->acquire();
					eventQueue[j]=1;
//					cout<<"in the select"<<endl;
					lo->release();
					full_sem->post();
				}
			}
		}
	}

	return true;
}

void *ExchangeIteratorLowerWithWideDependency::prochaseLower(void *arg){
	ExchangeIteratorLowerWithWideDependency *Pthis=(ExchangeIteratorLowerWithWideDependency *)arg;
	int recvbytes,returnBlock,currentBlock;
	char buf[2];

	while(1){
		//getchar();
		Logging_ExchangeIteratorLowerWithWideDependency("Waiting for producing new block.");
		Pthis->full_sem->wait();
		Pthis->lo->acquire();
		Logging_ExchangeIteratorLowerWithWideDependency("Received the semaphore that a new block is produced!");
		if((returnBlock=Pthis->PrepareForTheNewBlock())<Pthis->bwfb->getBlockNum()){
			Logging_ExchangeIteratorLowerWithWideDependency("In <If> cause");
			//the fold function is different from lower fold
			Pthis->bwfb->getStart()[returnBlock]->fold();
			//while loop make when eventQueue[returnBlock]==1, go on
			Logging_ExchangeIteratorLowerWithWideDependency("Waiting for enentQueue[%d]==1",returnBlock);
			while(Pthis->eventQueue[returnBlock]==0);
			//somewhat ugly code
			Logging_ExchangeIteratorLowerWithWideDependency("Waiting for the request from [%s]~!",Pthis->state.upper_ip_port_array[returnBlock].c_str());

			if((recvbytes=recv(Pthis->sock_fd[returnBlock],buf,1,0))==-1){
				perror("recv error!\n");
				return false;
			}
			Logging_ExchangeIteratorLowerWithWideDependency("Request received!");
			if(buf[0]=='n'){


				Logging_ExchangeIteratorLowerWithWideDependency("Send the new block to [%s]",Pthis->state.upper_ip_port_array[returnBlock].c_str());
				Pthis->SendTheNewBlock(Pthis->sock_fd[returnBlock],returnBlock);

				Pthis->lo->release();

				Pthis->empty_sem->post();

			}
			else{
				printf("thread 1   :Receiveinet_ntoa(remote_addr.sin_addr)d the close command from the Upper Exchange!\n");
				return (void *)(1);
			}
		}
		else{
			Logging_ExchangeIteratorLowerWithWideDependency("In <Else> cause");
			for(int k=0;k<Pthis->upper_count;k++){
				//the fold here is different from the upper fold
				Pthis->bwfb->getStart()[k]->foldLastBlock();
				Logging_ExchangeIteratorLowerWithWideDependency("in the last block");

				while(Pthis->eventQueue[k]==0);
				Logging_ExchangeIteratorLowerWithWideDependency("pass the loop");
				//somewhat ugly code
				if((recvbytes=recv(Pthis->sock_fd[k],buf,1,0))==-1){
					perror("recv error!\n");
					return false;
				}
				//Currently, only one block in the buffer, and 0 determines that only one block
				//TODO: send every block which is not full in the write buffer to the upper
				if(buf[0]=='n'){
					Pthis->SendTheNewBlock(Pthis->sock_fd[k],k);
					Pthis->lo->release();
					Pthis->empty_sem->post();
				}
				else{
					Logging_ExchangeIteratorLowerWithWideDependency("thread 1   :Received the close command from the Upper Exchange!\n");

					return (void *)(1);
				}
			}
			//destroy the semaphore and kill itself
			Pthis->over=0;
			Pthis->lo->destroy();
			Pthis->empty_sem->destroy();
			Pthis->full_sem->destroy();
			Logging_ExchangeIteratorLowerWithWideDependency("?????????????");
			//maybeerror? pthread_exit() function's arguments is (void *)2
			pthread_exit(NULL);
		}
	}
	return (void *)(1);
}

bool ExchangeIteratorLowerWithWideDependency::next(void* tuple)
{
	return false;
}

bool ExchangeIteratorLowerWithWideDependency::ConnectToUpperExchangeWithMulti(int &sock_fd,struct hostent* host,int port){
	struct sockaddr_in serv_add;
	serv_add.sin_family=AF_INET;
	serv_add.sin_port=htons(port);
	serv_add.sin_addr=*((struct in_addr*)host->h_addr);
	bzero(&(serv_add.sin_zero),8);

	int returnvalue;

	if((returnvalue=connect(sock_fd,(struct sockaddr *)&serv_add, sizeof(struct sockaddr)))==-1)
	{
		Logging_ExchangeIteratorLowerWithWideDependency("Fails to connect remote socket: %s:%d",inet_ntoa(serv_add.sin_addr),port);
		return false;
	}
	Logging_ExchangeIteratorLowerWithWideDependency("connected to the Master socket :"+returnvalue);
	//printf("connected to the Master socket %d !\n",returnvalue);
	return true;
}

bool ExchangeIteratorLowerWithWideDependency::close()
{
	free(tuple);
	cout<<"sendcount: "<<sendcount<<endl;
	state.child->close();
	for(unsigned int i=0;i<state.upper_ip_port_array.size();i++){
		FileClose(sock_fd[i]);
	}
	return true;
}

//the PrepareForNewBlock return a value that is the number of the block
int ExchangeIteratorLowerWithWideDependency::PrepareForTheNewBlock()
{
	//in this function, if prepareForNewBlock is ok, return false(0)
	//else if prepareForNewBlock is not ok, return true(0)
	//fold() is called when the block is not null
//	int i=0;
//	bwfb->getStart()[i]->reset();
//	while(state.child->next(tuple)){
//		if((i=bwfb->insert(tuple))==bwfb->getBlockNum()){
//			continue;
//		}
//		else{
//			bwfb->getStart()[i]->fold();
//			if(bwfb->getStart()[i]->getTupleCount()==0)
//				return 1;
//			return i;
//		}
//	}
//	return 0;
	int i;
	i=bwfb->getBlockNum();
//	bwfb->getStart()[0]->reset();
	while(state.child->next(tuple)){
		if((i=bwfb->insert(tuple))!=bwfb->getBlockNum()){
			//a block is full, return "i" as the number of the block
			//break to send the full block to the upper
			return i;
		}
		else{
			continue;
		}
	}
	Logging_ExchangeIteratorLowerWithWideDependency("The child iterator is exhausted!");
	return bwfb->getBlockNum();

//	if(bwfb->getStart()[i]->getTupleCount()==0)
//		return false;
//
//	bwfb->getStart()[i]->fold();
//	return true;
	//if(bwfb->getStart()[i]->getTupleCount()==0)
	//	return 1;
}

//send the block which is full
bool ExchangeIteratorLowerWithWideDependency::SendTheNewBlock(int sockfd,int socket_fd)
{
	int error=0;
	if((error=send(sockfd,bwfb->getStart()[socket_fd]->getBlock(),bwfb->getStart()[socket_fd]->getsize(),0))==-1)
	{
		perror("Send error!\n");
		return false;
	}
	void *p=0;
	//p=bwfb->getStart()[socket_fd]->getBlock();
	//for(int i=0;i<4;i++){
	//	cout<<"::"<<*(reinterpret_cast<int *>(p)+i)<<endl;
	//}
	bwfb->getStart()[socket_fd]->reset();
	Logging_ExchangeIteratorLowerWithWideDependency("I have sent the new block(%d bytes).\n",bwfb->getStart()[socket_fd]->getsize());
	sendcount++;
	return true;
}

bool ExchangeIteratorLowerWithWideDependency::SendBusySignal(int sockfd){
	int error=0;
	char s='b';
	if((error=send(sockfd,&s,1,0))==-1)
	{
		perror("Send error!\n");
		return false;
	}

}

//and send the notification to the other sides
bool ExchangeIteratorLowerWithWideDependency::SendTheFinishNotification(int socket_fd){
	if(send(sock_fd[socket_fd],"f",1,0)==-1)
	{
		perror("Send error!\n");
		return false;
	}
	Logging_ExchangeIteratorLowerWithWideDependency("The input iterator is exhausted. I have sent the notification message. \n");
	return true;
}
