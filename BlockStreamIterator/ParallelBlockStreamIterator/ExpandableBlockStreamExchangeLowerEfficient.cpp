/*
 * ExpandableBlockStreamExchangeLowerEfficient.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */

#include "ExpandableBlockStreamExchangeLowerEfficient.h"


#include <malloc.h>
#include "../../configure.h"

#include "../../Block/BlockWritableFix.h"
#include "../../Block/BlockReadable.h"
#include "../../rename.h"
#include "../../Logging.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../Environment.h"
#include "../../Logging.h"
ExpandableBlockStreamExchangeLowerEfficient::ExpandableBlockStreamExchangeLowerEfficient(State state)
:state(state){
	// TODO Auto-generated constructor stub

}

ExpandableBlockStreamExchangeLowerEfficient::~ExpandableBlockStreamExchangeLowerEfficient() {
	// TODO Auto-generated destructor stub
}

bool ExpandableBlockStreamExchangeLowerEfficient::open(){
	state.child->open();
	nuppers=state.upper_ip_list.size();

	socket_fd_upper_list=new int[nuppers];
	block_stream_for_asking_=BlockStreamBase::createBlock(state.schema,state.block_size);
//	block_for_sending=new BlockContainer(block_stream_for_asking_->getSerializedBlockSize());
	block_for_buffer_=new BlockContainer(block_stream_for_asking_->getSerializedBlockSize());
	buffer_for_sending_=new PartitionedBlockContainer(nuppers,block_stream_for_asking_->getSerializedBlockSize());
	block_for_inserting_to_buffer_=new BlockWritableFix(block_stream_for_asking_->getSerializedBlockSize(),state.schema);

	cur_block_stream_list_= new BlockStreamBase*[nuppers];
	for(unsigned i=0;i<nuppers;i++){
		cur_block_stream_list_[i]=BlockStreamBase::createBlock(state.schema,state.block_size);
	}
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
		if((upper_port=et->AskForSocketConnectionInfo(state.exchange_id,state.upper_ip_list[upper_id]))==0){
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

	pthread_create(&debug_tid,NULL,debug,this);
/*debug*/
	readsendedblocks=0;

	return true;
}
bool ExpandableBlockStreamExchangeLowerEfficient::next(BlockStreamBase*){

	void* tuple_from_child;
	void* tuple_in_cur_block_stream;
	block_stream_for_asking_->setEmpty();
	if(state.child->next(block_stream_for_asking_)){
		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator=block_stream_for_asking_->createIterator();
		while((tuple_from_child=traverse_iterator->nextTuple())>0){
			const unsigned partition_id=hash(tuple_from_child);
			const unsigned bytes=state.schema->getTupleActualSize(tuple_from_child);
			while(!(tuple_in_cur_block_stream=cur_block_stream_list_[partition_id]->allocateTuple(bytes))){
				cur_block_stream_list_[partition_id]->serialize(*block_for_inserting_to_buffer_);
				buffer->insertBlockToPartitionedList(block_for_inserting_to_buffer_,partition_id);
				cur_block_stream_list_[partition_id]->setEmpty();
			}
			state.schema->copyTuple(tuple_from_child,tuple_in_cur_block_stream);

		}
		return true;
	}
	else{
		/* the child iterator is exhausted. We add the cur block steram block into the buffer*/
		for(unsigned i=0;i<nuppers;i++){
			Logging_ExchangeIteratorEagerLower("||||||Fold the last||||||!");
			cur_block_stream_list_[i]->serialize(*block_for_inserting_to_buffer_);


			buffer->insertBlockToPartitionedList(block_for_inserting_to_buffer_,i);

			/* the following stupid lines send a block with last block bytes to the buffer.
			 * TODO: making any change to the three lines would make them much less stupid.
			 */
			if(!cur_block_stream_list_[i]->Empty()){
				cur_block_stream_list_[i]->setEmpty();
				cur_block_stream_list_[i]->serialize(*block_for_inserting_to_buffer_);
				buffer->insertBlockToPartitionedList(block_for_inserting_to_buffer_,i);
			}

		}
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

unsigned ExpandableBlockStreamExchangeLowerEfficient::hash(void* value){
	return *(int*)value%nuppers;
}

bool ExpandableBlockStreamExchangeLowerEfficient::close(){


	Logging_ExchangeIteratorEagerLower("The sender thread is killed in the close() function!");
	pthread_cancel(sender_tid);
	pthread_cancel(debug_tid);
	/* close the socket connections to the uppers */
	for(unsigned i=0;i<state.upper_ip_list.size();i++){
		FileClose(socket_fd_upper_list[i]);
	}
	state.child->close();
	block_stream_for_asking_->~BlockStreamBase();
//	block_for_sending->~BlockContainer();
	block_for_inserting_to_buffer_->~BlockWritable();
	buffer_for_sending_->~PartitionedBlockContainer();
	for(unsigned i=0;i<nuppers;i++){
		cur_block_stream_list_[i]->~BlockStreamBase();
	}
	buffer->~PartitionedBlockBuffer();
	delete [] cur_block_stream_list_;
	delete [] socket_fd_upper_list;

	return true;
}

bool ExpandableBlockStreamExchangeLowerEfficient::ConnectToUpperExchangeWithMulti(int &sock_fd,struct hostent* host,int port){
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
	Logging_ExchangeIteratorEagerLower("connected to the Master socket %s:%d\n",inet_ntoa(serv_add.sin_addr),port);
	//printf("connected to the Master socket %d !\n",returnvalue);
	return true;
}
void ExpandableBlockStreamExchangeLowerEfficient::WaitingForNotification(int target_socket_fd){
	char byte;
	int recvbytes;
	if((recvbytes=recv(target_socket_fd,&byte,sizeof(char),0))==-1){
		perror("recv error!\n");
	}

}
void ExpandableBlockStreamExchangeLowerEfficient::WaitingForCloseNotification(){
	for(unsigned i=0;i<nuppers;i++){
		char byte;
		int recvbytes;
		if((recvbytes=recv(socket_fd_upper_list[i],&byte,sizeof(char),0))==-1){
			perror("recv error!\n");
		}

		Logging_ExchangeIteratorEagerLower("Receive the close notifaction from the upper[%s], the byte='%c'",state.upper_ip_list[i].c_str(),byte);
	}


}
void* ExpandableBlockStreamExchangeLowerEfficient::sender(void* arg){
	ExpandableBlockStreamExchangeLowerEfficient* Pthis=(ExpandableBlockStreamExchangeLowerEfficient*)arg;


		Pthis->buffer_for_sending_->Initialized();


	while(true){
		pthread_testcancel();
		bool consumed=false;
		int partition_id=Pthis->buffer_for_sending_->getBlockForSending(Pthis->block_for_sending);
//		assert(partition_id>-1);
		if(partition_id>=0){
			if(Pthis->block_for_sending->GetRestSize()>0){
				int recvbytes;
				recvbytes=send(Pthis->socket_fd_upper_list[partition_id],(char*)Pthis->block_for_sending->getBlock()+Pthis->block_for_sending->GetCurSize(),Pthis->block_for_sending->GetRestSize(),MSG_DONTWAIT);
				if(recvbytes==-1){
					Logging_ExchangeIteratorEagerLower("Send fail~~~~!");
					if (errno == EAGAIN){
						continue;
					}
					printf("Error=%d\n",errno);
					perror("Send error!\n");
					break;
				}
				else{
					if(recvbytes<Pthis->block_for_sending->GetRestSize()){
						/* the block is not entirely sent. */
						Logging_ExchangeIteratorEagerLower("**not entire sent! bytes=%d, rest size=%d",recvbytes,Pthis->block_for_sending->GetRestSize());
						Pthis->block_for_sending->IncreaseActualSize(recvbytes);
						continue;
					}
					else{
						Logging_ExchangeIteratorEagerLower("A block is sent bytes=%d, rest size=%d",recvbytes,Pthis->block_for_sending->GetRestSize());
						Pthis->block_for_sending->IncreaseActualSize(recvbytes);
						/* the block is sent completely.*/
						Logging_ExchangeIteratorEagerLower("Send the new block to [%s]",Pthis->state.upper_ip_list[partition_id].c_str());
						Pthis->sendedblocks++;
//						Logging_ExchangeIteratorEagerLower("Waiting the connection notification from [%s]",Pthis->state.upper_ip_list[partition_id].c_str());
						Pthis->readsendedblocks++;
//						Logging_ExchangeIteratorEagerLower("The block is received the upper[%s].",Pthis->state.upper_ip_list[partition_id].c_str());

						Logging_ExchangeIteratorEagerLower("sent blocks=%d",Pthis->readsendedblocks);
						consumed=true;
					}

				}
			}
			else{
				consumed=true;
			}
		}
		else{
			/*block_for_sending is empty, so we get one block from the buffer into the block_for_sending_*/
			unsigned index=Pthis->buffer->getBlock(*Pthis->block_for_buffer_);
			Pthis->buffer_for_sending_->insert(index,Pthis->block_for_buffer_);
		}
		if(consumed==true){
			/* In the current loop, we have sent an entire to the receiver, so we should get a new block
			 * into the block_for_sender_*/
			//the if cause in version 1.2 is delete as getBlock will not retain until a block is successfully got.
			if(Pthis->buffer->getBlock(*Pthis->block_for_buffer_,partition_id)){
				Pthis->block_for_buffer_->reset();
				Pthis->buffer_for_sending_->insert(partition_id,Pthis->block_for_buffer_);
			}

		}

	}
}
void* ExpandableBlockStreamExchangeLowerEfficient::debug(void* arg){
	ExpandableBlockStreamExchangeLowerEfficient* Pthis=(ExpandableBlockStreamExchangeLowerEfficient*)arg;
	while(true){
//		printf("Lower: %d blocks in buffer.\n",Pthis->buffer->getBlockInBuffer());
		usleep(100000);
	}
}
