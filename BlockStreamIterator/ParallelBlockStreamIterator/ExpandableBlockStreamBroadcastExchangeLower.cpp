/*
 * ExpandableBlockStreamBroadcastExchangeLower.cpp
 *
 *  Created on: 2013-9-3
 *      Author: casa
 */

#include "ExpandableBlockStreamBroadcastExchangeLower.h"

#include <malloc.h>
#include "../../configure.h"

#include "../../Block/BlockWritableFix.h"
#include "../../Block/BlockReadable.h"
#include "../../common/rename.h"
#include "../../common/Logging.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../Environment.h"
#include "../../common/Logging.h"

//#define BROADCAST 3//1011

ExpandableBlockStreamBroadcastExchangeLower::ExpandableBlockStreamBroadcastExchangeLower(State state_)
:state(state_){
	// TODO 自动生成的构造函数存根

}

ExpandableBlockStreamBroadcastExchangeLower::~ExpandableBlockStreamBroadcastExchangeLower() {
	// TODO 自动生成的析构函数存根
}

bool ExpandableBlockStreamBroadcastExchangeLower::open(const PartitionOffset&){
	state.child->open(state.partition_offset);
	//upper节点的个数
	nuppers=state.upper_ip_list.size();
	//upper节点的socket数组
	socket_fd_upper_list=new int[nuppers];
	//向下端的iterator要的block
	block_stream_for_asking_=BlockStreamBase::createBlock(state.schema,state.block_size);
	//要发送走的block
	block_for_sending=new BlockWritableFix(block_stream_for_asking_->getSerializedBlockSize(),state.schema);
	//序列化完之后插入到buffer中的block
	block_for_inserting_to_buffer_=new BlockWritableFix(block_stream_for_asking_->getSerializedBlockSize(),state.schema);

	cur_block_stream_list_=BlockStreamBase::createBlock(state.schema,state.block_size);
	buffer=new PartitionedBlockBuffer(nuppers,block_stream_for_asking_->getSerializedBlockSize());


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
/*debug*/
//	readsendedblocks=0;

	return true;
}


bool ExpandableBlockStreamBroadcastExchangeLower::next(BlockStreamBase *){
	void* tuple_from_child;
	void* tuple_in_cur_block_stream;
	block_stream_for_asking_->setEmpty();
	if(state.child->next(block_stream_for_asking_)){
		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator=block_stream_for_asking_->createIterator();
		while((tuple_from_child=traverse_iterator->nextTuple())>0){
//			const unsigned partition_id=hash(tuple_from_child);
			const unsigned bytes=state.schema->getTupleActualSize(tuple_from_child);
			while(!(tuple_in_cur_block_stream=cur_block_stream_list_->allocateTuple(bytes))){
				cur_block_stream_list_->serialize(*block_for_inserting_to_buffer_);
				buffer->insertBlockToPartitionedList(block_for_inserting_to_buffer_,0);
				cur_block_stream_list_->setEmpty();
			}
			state.schema->copyTuple(tuple_from_child,tuple_in_cur_block_stream);
		}
		return true;
	}
	else{
		/* the child iterator is exhausted. We add the cur block steram block into the buffer*/
//		for(unsigned i=0;i<nuppers;i++){//1011
			Logging_ExchangeIteratorEagerLower("||||||Fold the last||||||!");
			cur_block_stream_list_->serialize(*block_for_inserting_to_buffer_);
			buffer->insertBlockToPartitionedList(block_for_inserting_to_buffer_,0);

			/* the following stupid three lines send a block with last block bytes to the buffer.
			 * TODO: making any change to the three lines would make them less stupid.
			 */
			if(!cur_block_stream_list_->Empty()){//1011
				cur_block_stream_list_->setEmpty();
				cur_block_stream_list_->serialize(*block_for_inserting_to_buffer_);
				buffer->insertBlockToPartitionedList(block_for_inserting_to_buffer_,0);
			}//1011
//		}//1011
		/*
		 * waiting until all the block in the buffer has been transformed to the uppers.
		 */
		Logging_ExchangeIteratorEagerLower("Waiting until all the blocks in the buffer is sent!");

		while(!buffer->isEmpty()){
			usleep(1);

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

bool ExpandableBlockStreamBroadcastExchangeLower::close(){


	Logging_ExchangeIteratorEagerLower("The sender thread is killed in the close() function!");
	pthread_cancel(sender_tid);
	/* close the socket connections to the uppers */
//	for(unsigned i=0;i<state.upper_ip_list.size();i++){
//		FileClose(socket_fd_upper_list[i]);
//	}
	state.child->close();
	block_stream_for_asking_->~BlockStreamBase();
	block_for_sending->~BlockWritable();
	block_for_inserting_to_buffer_->~BlockWritable();
	cur_block_stream_list_->~BlockStreamBase();
	buffer->~PartitionedBlockBuffer();
	delete [] socket_fd_upper_list;

	return true;
}

bool ExpandableBlockStreamBroadcastExchangeLower::ConnectToUpperExchangeWithMulti(int &sock_fd,struct hostent* host,int port){
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

void ExpandableBlockStreamBroadcastExchangeLower::WaitingForNotification(int target_socket_fd){
	char byte;
	int recvbytes;
	if((recvbytes=recv(target_socket_fd,&byte,sizeof(char),0))==-1){
		perror("recv error!\n");
	}

}

void ExpandableBlockStreamBroadcastExchangeLower::WaitingForCloseNotification(){
	for(unsigned i=0;i<nuppers;i++){
		char byte;
		int recvbytes;
		if((recvbytes=recv(socket_fd_upper_list[i],&byte,sizeof(char),0))==-1){
			perror("recv error!\n");
		}

		Logging_ExchangeIteratorEagerLower("Receive the close notifaction from the upper[%s], the byte='%c'",state.upper_ip_list[i].c_str(),byte);
	}


}

void* ExpandableBlockStreamBroadcastExchangeLower::sender(void* arg){
	ExpandableBlockStreamBroadcastExchangeLower* Pthis=(ExpandableBlockStreamBroadcastExchangeLower*)arg;
	while(true){
		int partition_id=Pthis->buffer->getBlock(*Pthis->block_for_sending);
		assert(partition_id>-1);

		for(int k=0;k<Pthis->nuppers;k++){
		int sendtotal=0;
			while(sendtotal<Pthis->block_for_sending->getsize()){
				int recvbytes;
				if((recvbytes=send(Pthis->socket_fd_upper_list[k],(char*)Pthis->block_for_sending->getBlock()+sendtotal,Pthis->block_for_sending->getsize()-sendtotal,0))==-1){
										perror("recv error!\n");
										return false;
				}
				sendtotal+=recvbytes;
			}
		Logging_ExchangeIteratorEagerLower("Send the new block to [%s]",Pthis->state.upper_ip_list[k].c_str());
		}


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
//			Pthis->sendedblocks++;
////		if(!Pthis->block_for_sending->isLastBlock()){
//			Logging_ExchangeIteratorEagerLower("Waiting the connection notification from [%s]",Pthis->state.upper_ip_list[partition_id].c_str());
//			Pthis->WaitingForNotification(Pthis->socket_fd_upper_list[partition_id]);
//			Pthis->readsendedblocks++;
//			Logging_ExchangeIteratorEagerLower("The block is received the upper[%s].",Pthis->state.upper_ip_list[partition_id].c_str());
////		}
//			Logging_ExchangeIteratorEagerLower("sent blocks=%d",Pthis->readsendedblocks);
	}
}

