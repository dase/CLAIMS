/*
 * ExchangeIteratorEager.cpp
 *
 *  Created on: Aug 15, 2013
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
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <assert.h>
//#include <linux/tcp.h>
#include <netinet/tcp.h>
#include "ExchangeIteratorEager.h"
#include "../Block/BlockReadableFix.h"
#include "../Logging.h"
#include "../PortManager.h"
#include "../Environment.h"
#include "../Executor/ExchangeTracker.h"
#include "ExchangeIteratorEagerLower.h"
#include "../configure.h"
ExchangeIteratorEager::ExchangeIteratorEager(State state)
:state(state){
	curBlock=0;
	// TODO Auto-generated constructor stub

}

ExchangeIteratorEager::~ExchangeIteratorEager() {
	// TODO Auto-generated destructor stub
}

bool ExchangeIteratorEager::open(){

	curBlock=new BlockReadableFix(state.block_size,state.schema);
	receiving_block=new BlockReadableFix(state.block_size,state.schema);
	nlowers=state.lower_ip_list.size();
	socket_fd_lower_list=new int[nlowers];
	lower_ip_array=new std::string[nlowers];
//	produced_list=new semaphore[nlowers];
//	empty_nslots.set_value(nlowers*3);
//	used_nslots.set_value(0);//=new semaphore(0);
	buffer=new PartitionedBlockBuffer(nlowers,state.block_size);
	if(PrepareTheSocket()==false)
		return false;

	if(RegisterExchange()==false){
		Logging_ExchangeIteratorEager("Register Exchange with ID=%l fails!",state.exchange_id);
	}

	if(isMaster()){
		Logging_ExchangeIteratorEager("This exchange is the master one, serialize the iterator subtree to the children...");
		if(SerializeAndSendToMulti()==false)
			return false;
	}


	if(WaitForConnectionFromLowerExchanges()==false){
		return false;
	}

	if(CreateReceiverThread()==false){
		return false;
	}



	return true;
}

bool ExchangeIteratorEager::close(){
	CancelReceiverThread();
	curBlock->~BlockReadable();
	for(unsigned i=0;i<nlowers;i++){
		FileClose(this->socket_fd_lower_list[i]);
	}

	//TODO::complete the close function.
	return true;
}


bool ExchangeIteratorEager::PrepareTheSocket()
{
	struct sockaddr_in my_addr;

	//sock_fd is the socket of this node
	if((sock_fd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket creation error!\n");
		return false;
	}
	my_addr.sin_family=AF_INET;

	/* apply for the port dynamicaly.*/
	if((socket_port=PortManager::getInstance()->applyPort())==0){
		Logging_ExchangeIteratorEager("Fails to apply a port for the socket. Reason: the PortManager is exhausted!");
	}
	Logging_ExchangeIteratorEager("The applied port for socket is %d",socket_port);

	my_addr.sin_port=htons(socket_port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	/* Enable address reuse */
	int on=1;
	setsockopt(sock_fd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));
	int size=1024*1024*50;
	setsockopt(sock_fd,SOL_SOCKET,SO_RCVBUF,(const char*)&size,sizeof(int));

	int flag=1;
	setsockopt(sock_fd,IPPROTO_TCP,TCP_NODELAY, (char *)&flag, sizeof(flag));


	if(bind(sock_fd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
	{
		perror("bind errors!\n");
		return false;
	}

	if(listen(sock_fd, nlowers)==-1)
	{
		perror("listen errors!\n");
		return false;
	}

	Logging_ExchangeIteratorEager("socket created as: %s:%d",inet_ntoa(my_addr.sin_addr),socket_port);

	return true;
}

bool ExchangeIteratorEager::RegisterExchange(){
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	std::ostringstream port_str;
	port_str<<socket_port;
	return et->RegisterExchange(state.exchange_id,port_str.str());
}

bool ExchangeIteratorEager::isMaster(){
	Logging_ExchangeIteratorEager("master ip=%s, self ip=%s",state.upper_ip_list[0].c_str(),Environment::getInstance()->getIp().c_str());
	return Environment::getInstance()->getIp()==state.upper_ip_list[0];
}
bool ExchangeIteratorEager::SerializeAndSendToMulti(){
	IteratorExecutorMaster* IEM=IteratorExecutorMaster::instance();
	ExchangeIteratorEagerLower::State EIELstate(state.schema,state.child,state.upper_ip_list,state.block_size,state.exchange_id);
	ExchangeIteratorEagerLower *EIEL=new ExchangeIteratorEagerLower(EIELstate);

	if(IEM->ExecuteIteratorsOnSites(EIEL,state.lower_ip_list)==false){
		printf("Cannot send the serialized iterator tree to the remote node!\n");
		return false;
	}
	return true;
}

bool ExchangeIteratorEager::WaitForConnectionFromLowerExchanges(){
	//wait for the lower nodes send the connection information
	socklen_t sin_size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	unsigned count=0;
	while(1){
		if(count>=nlowers){
			return true;
		}
		Logging_ExchangeIteratorEager("Waiting for the socket connection from the lower exchange..");
		if((socket_fd_lower_list[count]=accept(sock_fd,(struct sockaddr*)&remote_addr,&sin_size))!=-1)
		{
			lower_ip_array[count]=inet_ntoa(remote_addr.sin_addr);
			Logging_ExchangeIteratorEager("The lower exchange <%s> is connected to the socket.",lower_ip_array[count].c_str());
			count++;
		}
	}

	return true;
}

bool ExchangeIteratorEager::CreateReceiverThread(){
	int error;
	error=pthread_create(&receiver_tid,NULL,receiver,this);
	if(error!=0){
		Logging_ExchangeIteratorEager("Failed to create receiver thread.");
		return false;
	}
	return true;
}
void ExchangeIteratorEager::CancelReceiverThread(){
	pthread_cancel(receiver_tid);
}

void* ExchangeIteratorEager::receiver(void* arg){
	ExchangeIteratorEager* Pthis=(ExchangeIteratorEager*)arg;

//	unsigned subset=2<Pthis->nlowers?2:Pthis->nlowers;
	unsigned subset=Pthis->nlowers;
//	unsigned subset=1;
	int time=0;

	while(true){

//		int old_seed=rand()%Pthis->nlowers;
//		int old_seed=(time++)%Pthis->nlowers;
//		printf("rand=%d",old_seed);
		int old_seed=0;

		fd_set socketfds;
		FD_ZERO(&socketfds);
		int sock_fd_max=0;
//		for(int i=0;i<Pthis->nlowers;i++){
//			FD_SET(Pthis->socket_fd_lower_list[i],&socketfds);
//			if(Pthis->socket_fd_lower_list[i]>sock_fd_max)
//				sock_fd_max=Pthis->socket_fd_lower_list[i];
//		}
		for(int i=0,seed=old_seed;i<subset;i++){
			FD_SET(Pthis->socket_fd_lower_list[seed],&socketfds);
			if(Pthis->socket_fd_lower_list[seed]>sock_fd_max)
				sock_fd_max=Pthis->socket_fd_lower_list[seed];
			seed=(seed+1)%Pthis->nlowers;
		}
		/**
		 * After the select() returned, there is a loop to check which socket fd has newly coming message.
		 * The check should be in random order rather than in the increasing order, otherwise, the socket fd
		 * which is in the smaller index has larger chance to be checked than other socket fds.
		 */
//		unsigned seed=rand()%Pthis->nlowers;
		unsigned seed=old_seed;

		if(select(sock_fd_max+1,&socketfds,(fd_set*)NULL,(fd_set*)NULL,(struct timeval*)NULL)>0){

			for(unsigned j=0;j<subset;j++){
//			for(unsigned j=0;j<Pthis->nlowers;j++){
//				printf("Check [%s]",Pthis->lower_ip_array[seed].c_str());
				if(FD_ISSET(Pthis->socket_fd_lower_list[seed],&socketfds)){
//

//					int recvbytes;
//					if((recvbytes=recv(Pthis->socket_fd_lower_list[seed],Pthis->receiving_block->getBlock(),Pthis->receiving_block->getsize(),MSG_WAITALL))==-1){
//						perror("recv error!\n");
//						return false;
//					}
//					if(recvbytes==0){
//						break;
//					}
//					if(recvbytes<Pthis->receiving_block->getsize()){
//						perror("The received bytes are smaller than a block size, some data is lost during the transmission!");
//						printf("Only %d bytes are received!\n",recvbytes);
//						Logging_ExchangeIteratorEager("Error occurs when fetching a block (from %s) into the curBlock",Pthis->lower_ip_array[seed].c_str());
//						return false;
//					}

					int recvtotal=0;

					while(recvtotal<Pthis->receiving_block->getsize()){
						int recvbytes;
						if((recvbytes=recv(Pthis->socket_fd_lower_list[seed],(char*)Pthis->receiving_block->getBlock()+recvtotal,Pthis->receiving_block->getsize()-recvtotal,MSG_WAITALL))==-1){
												perror("recv error!\n");
												return false;
						}
						recvtotal+=recvbytes;
					}




					Logging_ExchangeIteratorEager("The %d-th block is received from Lower[%s]",Pthis->received_block[seed],Pthis->lower_ip_array[seed].c_str());
					Pthis->received_block[seed]++;

					const bool isLastBlock=Pthis->receiving_block->IsLastBlock();
					Pthis->buffer->insertBlockToPartitionedList(Pthis->receiving_block,seed);
//					Pthis->produced_list[seed].post();
//					Pthis->used_nslots.post();
					//send notification to the sender.

//					Pthis->SendBlockBufferedNotification(Pthis->socket_fd_lower_list[seed]);

					if(isLastBlock){
						Logging_ExchangeIteratorEager("*****This block is the last one.");
//						assert(produced_list[lower_to_consume].get_value()==0);
						Pthis->nexhausted_lowers++;
						Logging_ExchangeIteratorEager("<<<<<<<<<<<<<<<<nexhausted_lowers=%d>>>>>>>>>>>>>>>>",Pthis->nexhausted_lowers);
						Pthis->SendBlockAllConsumedNotification(Pthis->socket_fd_lower_list[seed]);
						Logging_ExchangeIteratorEager("This notification (all the blocks in the socket buffer are consumed) is send to the lower[%s].",Pthis->lower_ip_array[seed].c_str());

					}
					break;
				}
//				printf("....passed!\n");
//				break;
				seed=(seed+1)%Pthis->nlowers;
			}
		}

	}
}

void ExchangeIteratorEager::SendBlockBufferedNotification(int target_socket_fd){
	char content='c';
	if(send(target_socket_fd,&content,sizeof(char),0)==-1){
		perror("Send error!\n");
		return ;
	}

}
void ExchangeIteratorEager::SendBlockAllConsumedNotification(int target_socket_fd){
	char content='e';
	if(send(target_socket_fd,&content,sizeof(char),MSG_WAITALL)==-1){
		perror("Send error!\n");
		return ;
	}
}
bool ExchangeIteratorEager::ReadBlockInSocketBuffer(){
	if(nexhausted_lowers==nlowers&&buffer->isEmpty()){
		return false;
	}
//	used_nslots.wait();
//	int lower_to_consume=PickLowerOffset();
//	if(lower_to_consume<0){
//		return false;//all the lowers are exhausted!
//	}

	int index=buffer->getBlock(*curBlock);

//	produced_list[index].wait();
	/*copy the block in the socket buffer into the curBlock*/
//	int recvbytes;
//	if((recvbytes=recv(socket_fd_lower_list[lower_to_consume],curBlock->getBlock(),curBlock->getsize(),MSG_WAITALL))==-1){
//		perror("recv error!\n");
//		return false;
//	}
//	if(recvbytes<curBlock->getsize()){
//		perror("The received bytes are smaller than a block size, some data is lost during the transmission!");
//		printf("Only %d bytes are received!\n",recvbytes);
//		Logging_ExchangeIteratorEager("Error occurs when fetching a block (from %s) into the curBlock",lower_ip_array[lower_to_consume].c_str());
//		return false;
//	}

//	if(curBlock->IsLastBlock()){
//		Logging_ExchangeIteratorEager("*****This block is the last one.");
//		assert(produced_list[lower_to_consume].get_value()==0);
//		nexhausted_lowers++;
//		SendBlockAllConsumedNotification(socket_fd_lower_list[lower_to_consume]);
//		Logging_ExchangeIteratorEager("This notification (all the blocks in the socket buffer are consumed) is send to the lower[%s].",lower_ip_array[lower_to_consume].c_str());
//	}


	curBlock->reset();
	Logging_ExchangeIteratorEager("a block (from %s) is fetched into the curBlock from the socket buffer",lower_ip_array[index].c_str());
	Logging_ExchangeIteratorEager("tuples=%d",curBlock->getMaxTupleCount());
	Logging_ExchangeIteratorEager("blockcount[%d]=%d",index,consumed_block[index]++);
//	empty_nslots.post();

	return true;

}
//int ExchangeIteratorEager::PickLowerOffset(){
//	unsigned seed;
//	seed=used_nslots.get_value()>0?rand()%used_nslots.get_value():0;
//	unsigned accum=0;
//	if(nexhausted_lowers==nlowers)
//		return -1;
//	for(unsigned i=0;i<nlowers;i++){
//		accum+=produced_list[i].get_value();
//		if(seed<accum)
//		{
//			return i;
//		}
//	}
//	Logging_ExchangeIteratorEager("PickLowerOffset() is abnormal, try again!");
//	return PickLowerOffset();
//}
