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
#include "../../common/rename.h"
#include "../../common/Logging.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../Environment.h"
#include "../../common/Logging.h"
#include "../../utility/ThreadSafe.h"
#include "../../common/ids.h"
#include "../../utility/rdtsc.h"
ExpandableBlockStreamExchangeLowerEfficient::ExpandableBlockStreamExchangeLowerEfficient(State state)
:state(state){
	// TODO Auto-generated constructor stub
	assert(state.partition_key_index<100);

}

ExpandableBlockStreamExchangeLowerEfficient::~ExpandableBlockStreamExchangeLowerEfficient() {
	// TODO Auto-generated destructor stub
}
bool ExpandableBlockStreamExchangeLowerEfficient::open(const PartitionOffset&){
	logging_=new ExchangeIteratorEagerLowerLogging();
	logging_->log("[%lld] Exchange lower is created!",state.exchange_id);
	unsigned long long int start=curtick();
	unsigned long long int step1,step2,step3;
	step1=curtick();
	step2=curtick();
	step3=curtick();
	connected_uppers=0;
	connected_uppers_in=0;
	state.child->open(state.partition_offset);

	nuppers=state.upper_ip_list.size();
	partition_function_=PartitionFunctionFactory::createBoostHashFunction(nuppers);
//	printf("<><><><><><><> lower open time:%4.4f (step 1) \n",getSecond(step1));
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

//	buffer->~PartitionedBlockBuffer();
//
//	void* test=malloc(1024*1024*400);
//	memset(test,0,1024*1024*400);
//	free(test);

//	printf(">>>--------Lower:: open %d\n",state.exchange_id);
//	printf("<><><><><><><> lower open time:%4.4f (step 2) \n",getSecond(step2));
	for(unsigned upper_id=0;upper_id<state.upper_ip_list.size();upper_id++){

//		printf("_______ this time connect target=%s (state=%d)\n",state.upper_ip_list[upper_id].c_str(),state.exchange_id);
		struct hostent host;
		std::string tmp_before(state.upper_ip_list[upper_id].c_str());
		if((ThreadSafe::gethostbyname_ts(host,state.upper_ip_list[upper_id].c_str()))==0){
			perror("gethostbyname errors!\n");
			return false;
		}


		if((socket_fd_upper_list[upper_id]=socket(AF_INET, SOCK_STREAM,0))==-1){
			perror("socket creation errors!\n");
			return false;
		}
//		printf("Lower %d is created!\n",socket_fd_upper_list[upper_id]);

		ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
		int upper_port;
		unsigned long long int connect_info=curtick();
		if((upper_port=et->AskForSocketConnectionInfo(ExchangeID(state.exchange_id,upper_id),state.upper_ip_list[upper_id]))==0){
			logging_->elog("Fails to ask %s for socket connection info, the exchange id=%d",state.upper_ip_list[upper_id].c_str(),state.exchange_id);
			assert(false);
		}
//		printf("\n\n\n port=%d\n\n\n",upper_port);
//		printf("_______________socket connect_if time:%4.4f\n",getSecond(connect_info));
		connected_uppers++;

//		if(connected_uppers_list_.find(state.upper_ip_list[upper_id].c_str())!=connected_uppers_list_.end())
//			assert(false);
//		connected_uppers_list_[state.upper_ip_list[upper_id].c_str()]=upper_id;

//		std::string tmp(inet_ntoa(*((struct in_addr*)host.h_addr)));
		unsigned long long int connect=curtick();
		if(ConnectToUpperExchangeWithMulti(socket_fd_upper_list[upper_id],state.upper_ip_list[upper_id],upper_port)==false)
			return false;
//		printf("_______________socket connection time:%4.4f\n",getSecond(connect));

//		assert(connected_uppers<=state.upper_ip_list.size());
//		printf("<<<<<<<<<<<<state %d: connected:%d\n",state.exchange_id,connected_uppers);


	}

//	printf("<><><><><><><> lower open time:%4.4f (step 3) \n",getSecond(step3));
	int error;
	error=pthread_create(&sender_tid,NULL,sender,this);
	if(error!=0){
		logging_->elog("Failed to create the sender thread>>>>>>>>>>>>>>>>>>>>>>>>>>>>@@#@#\n\n.");
		return false;
	}
	else{
//		printf("Exchange(%d,%d) create sender thread %x#################\n",state.exchange_id,state.partition_offset,sender_tid);
	}

//	pthread_create(&debug_tid,NULL,debug,this);
/*debug*/
	readsendedblocks=0;
	hash_test=PartitionFunctionFactory::createBoostHashFunction(4);
//	printf("<><><><><><><> lower open time:%4.4f\n",getSecond(start));
	return true;
}
bool ExpandableBlockStreamExchangeLowerEfficient::next(BlockStreamBase*){
	PartitionFunction* test_hash=PartitionFunctionFactory::createBoostHashFunction(4);
	void* tuple_from_child;
	void* tuple_in_cur_block_stream;
	block_stream_for_asking_->setEmpty();
	if(state.child->next(block_stream_for_asking_)){
//		printf("Lower next retured tuple count=%d\n",block_stream_for_asking_->getTuplesInBlock());
		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator=block_stream_for_asking_->createIterator();
		while((tuple_from_child=traverse_iterator->nextTuple())>0){
			const unsigned partition_id=hash(tuple_from_child);
//			assert(partition_id==state.partition_offset);
//			assert(partition_id==state.partition_offset);
//			if(rand()%10000<3){
//
//				printf("key:%d\n",test_hash->get_partition_value(*(unsigned long*)state.schema->getColumnAddess(1,tuple_from_child)));
//			}
//			printf("Lower partition key:%d\n",partition_id);
			const unsigned bytes=state.schema->getTupleActualSize(tuple_from_child);
			while(!(tuple_in_cur_block_stream=cur_block_stream_list_[partition_id]->allocateTuple(bytes))){
//				printf("cur_block_stream_list:%d",cur_block_stream_list_[partition_id]->getTuplesInBlock());
				cur_block_stream_list_[partition_id]->serialize(*block_for_inserting_to_buffer_);
//				printf("tuple count in [block_for_inserting_to_buffer_] =%d\n",*(int*)((char*)block_for_inserting_to_buffer_->getBlock()+65532));
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
			logging_->log("||||||Fold the last||||||!");
			cur_block_stream_list_[i]->serialize(*block_for_inserting_to_buffer_);


			buffer->insertBlockToPartitionedList(block_for_inserting_to_buffer_,i);

			/* The following lines send an empty block to the upper, indicating that all
			 * the data from current lower has been transmit to the uppers.
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
		logging_->log("Waiting until all the blocks in the buffer is sent!");

		while(!buffer->isEmpty()){
			usleep(10000);

		}
		/*
		 * waiting until all the uppers send the close notification which means that
		 * blocks in the uppers' socket buffer have all been consumed.
		 */
		logging_->log("Waiting for close notification!");
		WaitingForCloseNotification();
		logging_->log("....passed!");

		return false;
	}




}

unsigned ExpandableBlockStreamExchangeLowerEfficient::hash(void* value){
	const void* hash_key_address=state.schema->getColumnAddess(state.partition_key_index,value);
//	if(state.exchange_id==0){
//		const unsigned long v=*(unsigned long*)hash_key_address;
//		const unsigned ret= ((v*16807)%2839+(v*19))%(unsigned long)nuppers;
//
//		const void* test_key=state.schema->getColumnAddess(2,value);
//		const unsigned long vv=*(unsigned long*) test_key;
//		if(rand()%100000<3000000){
////			printf("key:%d\n",((vv*16807)%2839+(vv))%4);
//			state.schema->displayTuple(value,"|");
//			sleep(1);
//			printf("key:%d\n",state.schema->getcolumn(2).operate->getPartitionValue(state.schema->getColumnAddess(2,value),hash_test));
////			printf("key:%d\n",vv%4);
//		}
//		return ret;
//	}

	return state.schema->getcolumn(state.partition_key_index).operate->getPartitionValue(hash_key_address,nuppers);
}

bool ExpandableBlockStreamExchangeLowerEfficient::close(){

//	block_stream_for_asking_
//	block_for_buffer_
//	buffer_for_sending_
//	block_for_inserting_to_buffer_
//	cur_block_stream_list_
//	buffer

//	printf("The sender thread is killed in the close() function! state=%d,%d\n",state.exchange_id,state.partition_offset);
	pthread_cancel(sender_tid);

	void* res;

	pthread_join(sender_tid,&res);
	if(res!=PTHREAD_CANCELED)
		printf("thread is not canceled!\n");
	sender_tid=0;
//	pthread_cancel(debug_tid);

	/* close the socket connections to the uppers */
	for(unsigned i=0;i<state.upper_ip_list.size();i++){
//		FileClose(socket_fd_upper_list[i]);
//		printf("Lower %d is closed!\n",socket_fd_upper_list[i]);
	}
	state.child->close();
	buffer->~PartitionedBlockBuffer();
	block_stream_for_asking_->~BlockStreamBase();
	block_for_sending->~BlockContainer();
	block_for_inserting_to_buffer_->~BlockWritable();
	buffer_for_sending_->~PartitionedBlockContainer();
	block_for_buffer_->~BlockContainer();
	for(unsigned i=0;i<nuppers;i++){
		cur_block_stream_list_[i]->~BlockStreamBase();
	}

	delete [] cur_block_stream_list_;
	delete [] socket_fd_upper_list;

	partition_function_->~PartitionFunction();
	return true;
}

bool ExpandableBlockStreamExchangeLowerEfficient::ConnectToUpperExchangeWithMulti(int &sock_fd,struct hostent* host,int port){
	struct sockaddr_in serv_add;
	serv_add.sin_family=AF_INET;
	serv_add.sin_port=htons(port);
	serv_add.sin_addr=*((struct in_addr*)host->h_addr);
	bzero(&(serv_add.sin_zero),8);

	int returnvalue;

	std::string tmp(inet_ntoa(serv_add.sin_addr));
//	printf("||||Lower(state=%d) try to connect to  %s:%d\n",state.exchange_id,inet_ntoa(serv_add.sin_addr),port);

	if(connected_uppers_list_in.find(tmp)!=connected_uppers_list_in.end())
		assert(false);
	connected_uppers_list_in[tmp]=connected_uppers_in++;


	if((returnvalue=connect(sock_fd,(struct sockaddr *)&serv_add, sizeof(struct sockaddr)))==-1)
	{
		logging_->elog("Fails to connect remote socket: %s:%d",inet_ntoa(serv_add.sin_addr),port);
		return false;
	}
//	printf("connected to the Master socket %s:%d\n",inet_ntoa(serv_add.sin_addr),port);
	//printf("connected to the Master socket %d !\n",returnvalue);
	return true;
}
bool ExpandableBlockStreamExchangeLowerEfficient::ConnectToUpperExchangeWithMulti(int &sock_fd,const std::string ip,int port){
	struct sockaddr_in serv_add;
	serv_add.sin_family=AF_INET;
	serv_add.sin_port=htons(port);
	serv_add.sin_addr.s_addr=inet_addr(ip.c_str());
	bzero(&(serv_add.sin_zero),8);
//
//	int returnvalue;
//
//	std::string tmp(inet_ntoa(serv_add.sin_addr));
//	printf("||||Lower(state=%d) try to connect to  %s:%d\n",state.exchange_id,inet_ntoa(serv_add.sin_addr),port);
//
//	if(connected_uppers_list_in.find(tmp)!=connected_uppers_list_in.end())
//		assert(false);
//	connected_uppers_list_in[tmp]=connected_uppers_in++;


	if((connect(sock_fd,(struct sockaddr *)&serv_add, sizeof(struct sockaddr)))==-1)
	{
		logging_->elog("Fails to connect remote socket: %s:%d",inet_ntoa(serv_add.sin_addr),port);
		return false;
	}
//	printf("connected to the Master socket %s:%d\n",ip.c_str(),port);
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
//		if((recvbytes=recv(socket_fd_upper_list[i],&byte,sizeof(char),0))==0){
			perror("recv error!\n");
		}
		FileClose(socket_fd_upper_list[i]);
//		printf("Lower %d is closed!\n",socket_fd_upper_list[i]);
//		printf("Receive the close notifaction from the upper[%s], the byte='%c' state=%d\n",state.upper_ip_list[i].c_str(),byte,state.exchange_id);
		logging_->log("Receive the close notifaction from the upper[%s], the byte='%c'",state.upper_ip_list[i].c_str(),byte);
	}


}
void* ExpandableBlockStreamExchangeLowerEfficient::sender(void* arg){
	ExpandableBlockStreamExchangeLowerEfficient* Pthis=(ExpandableBlockStreamExchangeLowerEfficient*)arg;


		Pthis->buffer_for_sending_->Initialized();

		try{
	while(true){
//		usleep(100000);
		pthread_testcancel();
		bool consumed=false;
		int partition_id=Pthis->buffer_for_sending_->getBlockForSending(Pthis->block_for_sending);
//		assert(partition_id>-1);
		if(partition_id>=0){
			pthread_testcancel();
			if(Pthis->block_for_sending->GetRestSize()>0){
				int recvbytes;
				recvbytes=send(Pthis->socket_fd_upper_list[partition_id],(char*)Pthis->block_for_sending->getBlock()+Pthis->block_for_sending->GetCurSize(),Pthis->block_for_sending->GetRestSize(),MSG_DONTWAIT);
				if(recvbytes==-1){
//					Logging_ExchangeIteratorEagerLower("Send fail~~~~!");
					if (errno == EAGAIN){
						continue;
					}
					printf("Error=%d,fd=%d\n",errno,Pthis->socket_fd_upper_list[partition_id]);
					perror("Send error!\n");
//					printf("fd=%d",Pthis->socket_fd_upper_list[partition_id]);
					break;
				}
				else{
					if(recvbytes<Pthis->block_for_sending->GetRestSize()){
						/* the block is not entirely sent. */
						Pthis->logging_->log("**not entire sent! bytes=%d, rest size=%d",recvbytes,Pthis->block_for_sending->GetRestSize());
						Pthis->block_for_sending->IncreaseActualSize(recvbytes);
						continue;
					}
					else{
//						Pthis->block_for_sending->getBlock()
						Pthis->logging_->log("A block is sent bytes=%d, rest size=%d",recvbytes,Pthis->block_for_sending->GetRestSize());
						Pthis->block_for_sending->IncreaseActualSize(recvbytes);
						/* the block is sent completely.*/
						Pthis->logging_->log("Send the new block to [%s]",Pthis->state.upper_ip_list[partition_id].c_str());
						Pthis->sendedblocks++;
//						Logging_ExchangeIteratorEagerLower("Waiting the connection notification from [%s]",Pthis->state.upper_ip_list[partition_id].c_str());
						Pthis->readsendedblocks++;
//						Logging_ExchangeIteratorEagerLower("The block is received the upper[%s].",Pthis->state.upper_ip_list[partition_id].c_str());

						Pthis->logging_->log("sent blocks=%d",Pthis->readsendedblocks);
						consumed=true;
//						const int tuples=*(int*)((char*)Pthis->block_for_sending->getBlock()+Pthis->block_for_sending->getsize()-2*sizeof(int));
//						printf("Send the new block to [%s,fd=%d], number of tuples=%d\n",Pthis->state.upper_ip_list[partition_id].c_str(),Pthis->socket_fd_upper_list[partition_id],tuples);
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
			pthread_testcancel();
			if(Pthis->buffer->getBlock(*Pthis->block_for_buffer_,partition_id)){
				Pthis->block_for_buffer_->reset();
				Pthis->buffer_for_sending_->insert(partition_id,Pthis->block_for_buffer_);
			}
			else{
				usleep(1);
			}

		}

	}
		}
		catch(std::exception e){
			pthread_testcancel();
		}
}
void* ExpandableBlockStreamExchangeLowerEfficient::debug(void* arg){
	ExpandableBlockStreamExchangeLowerEfficient* Pthis=(ExpandableBlockStreamExchangeLowerEfficient*)arg;
	while(true){
//		printf("Lower: %d blocks in buffer.\n",Pthis->buffer->getBlockInBuffer());
		usleep(100000);
	}
}
