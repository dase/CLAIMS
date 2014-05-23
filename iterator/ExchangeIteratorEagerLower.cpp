/*
 * ExchangeIteratorEagerLower.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wangli
 */

#include "ExchangeIteratorEagerLower.h"
#include <malloc.h>
#include "../configure.h"
#include "ExchangeIteratorLowerWithWideDependency.h"
#include "../Block/BlockWritableFix.h"
#include "../Block/BlockReadable.h"
#include "../common/rename.h"
#include "../common/Logging.h"
#include "../Executor/ExchangeTracker.h"
#include "../Environment.h"
ExchangeIteratorEagerLower::ExchangeIteratorEagerLower(State state)
:state(state){
	// TODO Auto-generated constructor stub

}

ExchangeIteratorEagerLower::~ExchangeIteratorEagerLower() {
	// TODO Auto-generated destructor stub
}

bool ExchangeIteratorEagerLower::open(){
	state.child->open();
	nuppers=state.upper_ip_list.size();
	tuple=memalign(cacheline_size,state.schema->getTupleMaxSize());
	socket_fd_upper_list=new int[nuppers];
	buffer=new PartitionedBlockBuffer(nuppers,state.block_size);
	block_for_sending=new BlockWritableFix(state.block_size,state.schema);
	cur_block_list= new BlockWritable*[nuppers];

	for(unsigned i=0;i<nuppers;i++){
		cur_block_list[i]=new BlockWritableFix(state.block_size,state.schema);
	}

	for(unsigned upper_id=0;upper_id<state.upper_ip_list.size();upper_id++){
		struct hostent* host;
		if((host=gethostbyname(state.upper_ip_list[upper_id].c_str()))==0){
			perror("gethostbyname errors!\n");
			return false;
		}

		if((socket_fd_upper_list[upper_id]=socket(AF_INET, SOCK_STREAM,0))==-1){
			perror("socket creation errors!\n");
			return false;
		}

		ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
		int upper_port;
		if((upper_port=et->AskForSocketConnectionInfo(ExchangeID(state.exchange_id,upper_id),state.upper_ip_list[upper_id]),upper_port)==0){
			Logging_ExchangeIteratorEagerLower("Fails to ask %s for socket connection info, the exchange id=%d",state.upper_ip_list[upper_id].c_str(),state.exchange_id);
		}

		if(ConnectToUpperExchangeWithMulti(socket_fd_upper_list[upper_id],host,upper_port)==false)
			return false;
	}

	int error;
	error=pthread_create(&sender_tid,NULL,sender,this);
	if(error!=0){
		Logging_ExchangeIteratorEagerLower("Failed to create the sender thread.");
		return false;
	}

//	pthread_create(&debug_tid,NULL,debug,this);

	return true;
}
bool ExchangeIteratorEagerLower::next(void*){
	if(state.child->next(tuple)){
		unsigned partition_id=hash(tuple);
		while(!cur_block_list[partition_id]->insert(tuple)){
			//the block does not have enough space for insertion.
			generated_blocks++;
			cur_block_list[partition_id]->fold();
			assert(!cur_block_list[partition_id]->isLastBlock());
			if(!buffer->insertBlockToPartitionedList(cur_block_list[partition_id],partition_id)){
				Logging_ExchangeIteratorEagerLower("Error occurs when inserting record into the Partitioned Block Buffer");
				return false;
			}
			cur_block_list[partition_id]->setEmpty();
		}
		return true;
	}
	else{//the child iterator is exhausted, add the cur blocks into the buffer
		Logging_ExchangeIteratorEagerLower("The child iterator is exhausted!\n");
		for(unsigned i=0;i<nuppers;i++){
//			if(cur_block_list[i]->getTupleCount()!=0){
				generated_blocks++;
				Logging_ExchangeIteratorEagerLower("||||||Fold the last||||||!");
				cur_block_list[i]->foldLastBlock();
				assert(cur_block_list[i]->isLastBlock());
				if(!buffer->insertBlockToPartitionedList(cur_block_list[i],i)){
					Logging_ExchangeIteratorEagerLower("Error occurs when inserting record into the Partitioned Block Buffer");
					return false;
				}
//			}

		}
		/*
		 * waiting until all the block in the buffer has been transformed to the uppers.
		 */
		Logging_ExchangeIteratorEagerLower("Waiting until all the blocks in the buffer is sent!");

		while(!buffer->isEmpty()){
			sleep(1);

		}
		/*
		 * waiting until all the uppers send the close notification which means that
		 * blocks in the uppers' socket buffer have all been consumed.
		 */
		Logging_ExchangeIteratorEagerLower("Waiting for close notification!");
		WaitingForCloseNotification();
		Logging_ExchangeIteratorEagerLower("....passed!");

		return false;
	}

}

unsigned ExchangeIteratorEagerLower::hash(void* value){
	return *(int*)value%nuppers;
}

bool ExchangeIteratorEagerLower::close(){


	Logging_ExchangeIteratorEagerLower("The sender thread is killed in the close() function!");
	pthread_cancel(sender_tid);
	state.child->close();
	return true;
}

bool ExchangeIteratorEagerLower::ConnectToUpperExchangeWithMulti(int &sock_fd,struct hostent* host,int port){
	struct sockaddr_in serv_add;
	serv_add.sin_family=AF_INET;
	serv_add.sin_port=htons(port);
	serv_add.sin_addr=*((struct in_addr*)host->h_addr);
	bzero(&(serv_add.sin_zero),8);

	int returnvalue;

	if((returnvalue=connect(sock_fd,(struct sockaddr *)&serv_add, sizeof(struct sockaddr)))==-1)
	{
		Logging_ExchangeIteratorEagerLower("Fails to connect remote socket: %s:%d",inet_ntoa(serv_add.sin_addr),port);
		return false;
	}
	Logging_ExchangeIteratorEagerLower("connected to the Master socket :"+returnvalue);
	//printf("connected to the Master socket %d !\n",returnvalue);
	return true;
}
void ExchangeIteratorEagerLower::WaitingForNotification(int target_socket_fd){
	char byte;
	int recvbytes;
	if((recvbytes=recv(target_socket_fd,&byte,sizeof(char),0))==-1){
		perror("recv error!\n");
	}

}
void ExchangeIteratorEagerLower::WaitingForCloseNotification(){
	for(unsigned i=0;i<nuppers;i++){
		char byte;
		int recvbytes;
		if((recvbytes=recv(socket_fd_upper_list[i],&byte,sizeof(char),0))==-1){
			perror("recv error!\n");
		}

		Logging_ExchangeIteratorEagerLower("Receive the close notifaction from the upper[%s], the byte='%c'",state.upper_ip_list[i].c_str(),byte);
	}


}
void* ExchangeIteratorEagerLower::sender(void* arg){
	ExchangeIteratorEagerLower* Pthis=(ExchangeIteratorEagerLower*)arg;
	while(true){
		int partition_id=Pthis->buffer->getBlock(*Pthis->block_for_sending);
		assert(partition_id>-1);
		Logging_ExchangeIteratorEagerLower("Send the new block to [%s]",Pthis->state.upper_ip_list[partition_id].c_str());

		int sendtotal=0;

		while(sendtotal<Pthis->block_for_sending->getsize()){
			int recvbytes;
			if((recvbytes=send(Pthis->socket_fd_upper_list[partition_id],(char*)Pthis->block_for_sending->getBlock()+sendtotal,Pthis->block_for_sending->getsize()-sendtotal,MSG_WAITALL))==-1){
									perror("recv error!\n");
									return false;
			}
			sendtotal+=recvbytes;
//			printf("send=%d\n",sendtotal);
		}

		usleep(1);
//		int error;
//		if((error=send(Pthis->socket_fd_upper_list[partition_id],
//						Pthis->block_for_sending->getBlock(),
//						Pthis->block_for_sending->getsize(),
//						0))==-1
//				)
//		{
//			perror("Send error!\n");
//			return false;
//		}
			Pthis->sendedblocks++;
//		if(!Pthis->block_for_sending->isLastBlock()){
			Logging_ExchangeIteratorEagerLower("Waiting the connection notification from [%s]",Pthis->state.upper_ip_list[partition_id].c_str());
//			Pthis->WaitingForNotification(Pthis->socket_fd_upper_list[partition_id]);
			Pthis->readsendedblocks++;
			Logging_ExchangeIteratorEagerLower("The block is received the upper[%s].",Pthis->state.upper_ip_list[partition_id].c_str());
//		}
			Logging_ExchangeIteratorEagerLower("sent blocks=%d",Pthis->readsendedblocks);
	}
}

void* ExchangeIteratorEagerLower::debug(void* arg){
	ExchangeIteratorEagerLower* Pthis=(ExchangeIteratorEagerLower*)arg;
	while(true){
		printf("%d blocks in buffer.\n",Pthis->buffer->getBlockInBuffer());
		usleep(100000);
	}
}


