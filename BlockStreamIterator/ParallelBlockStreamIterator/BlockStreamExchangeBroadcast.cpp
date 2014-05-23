/*
 * BlockStreamExchangeBroadcast.cpp
 *
 *  Created on: 2013-9-7
 *      Author: casa
 */

#include "BlockStreamExchangeBroadcast.h"
#include "BlockStreamExchangeLowerBroadcast.h"

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

#include "../../Block/BlockReadableFix.h"
#include "../../common/Logging.h"
#include "../../Environment.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../configure.h"
#include "../../common/rename.h"

BlockStreamExchangeBroadcast::BlockStreamExchangeBroadcast(State state)
:BlockStreamExchangeBase(state.upper_ip_list,state.lower_ip_list.size()),state_(state),open_finished_(false){
	cout<<"in the BlockStreamExchangeBase"<<state_.upper_ip_list[0]<<"::"<<endl;
	//这里应该是state而不是state_，因为构造函数还没有执行完
	sema_open_.set_value(1);
}

BlockStreamExchangeBroadcast::BlockStreamExchangeBroadcast()
:BlockStreamExchangeBase(),open_finished_(false){
	sema_open_.set_value(1);
}

BlockStreamExchangeBroadcast::~BlockStreamExchangeBroadcast() {
	// TODO 自动生成的析构函数存根
}


bool BlockStreamExchangeBroadcast::open(const PartitionOffset& partition){

	if(sema_open_.try_wait()){
		cout<<"----------------------------------------------------------------"<<endl;
		nexhausted_lowers_=0;
		nlowers_=state_.lower_ip_list.size();

		for(unsigned i=0;i<nlowers_;i++){
			received_block_[i]=0;
		}

		cout<<"-----------------+++++++++++++++++++++++++-----------------------"<<endl;
		socket_fd_lower_list_=new int[nlowers_];
		lower_ip_array_=new std::string[nlowers_];
	//	buffer=new PartitionedBlockBuffer(nlowers,state.block_size);
		buffer_=new BlockStreamBuffer(state_.block_size,nlowers_*10,state_.schema);
		received_block_stream_=BlockStreamBase::createBlock(state_.schema,state_.block_size);
		block_for_socket_=new BlockReadableFix(received_block_stream_->getSerializedBlockSize(),state_.schema);
		if(PrepareTheSocket()==false)
			return false;
		cout<<"-----------------+++++++++++++++++++++++++-----------------------"<<endl;
		if(RegisterExchange(ExchangeID(state_.exchange_id,0))==false){
			Logging_ExchangeIteratorEager("Register Exchange with ID=%l fails!",state_.exchange_id);
		}
		cout<<"-----------------+++++++++++++++++++++++++-----------------------"<<endl;

		if(isMaster()){
			cout<<"reach the ismaster"<<endl;
			Logging_ExchangeIteratorEager("This exchange is the master one, serialize the iterator subtree to the children...");
			cout<<"-----------------+++++++++++++++++++++++++-----------------------"<<endl;
			if(SerializeAndSendToMulti()==false)
				return false;
		}

		cout<<"not reach the ismaster"<<endl;
		if(WaitForConnectionFromLowerExchanges()==false){
			return false;
		}

		if(CreateReceiverThread()==false){
			return false;
		}
		open_finished_=true;
		cout<<"-----------------+++++++++++++++++++++++++-----------------------"<<endl;
		return true;

	}
	else{
		while(!open_finished_){
			usleep(1);
		}
		return true;
	}
}

bool BlockStreamExchangeBroadcast::next(BlockStreamBase* block){
	while(nexhausted_lowers_<nlowers_){
		//3                       4
		if(buffer_->getBlock(*block)){
			return true;
		}
	}
	/* all the lowers exchange are exhausted.*/
	return buffer_->getBlock(*block);
}

bool BlockStreamExchangeBroadcast::close(){
	CancelReceiverThread();

	for(unsigned i=0;i<nlowers_;i++){
		FileClose(this->socket_fd_lower_list_[i]);
	}

	block_for_socket_->~BlockReadable();
	received_block_stream_->~BlockStreamBase();
	buffer_->~BlockStreamBuffer();

	delete[] socket_fd_lower_list_;
	delete[] lower_ip_array_;
	CloseTheSocket();

	Environment::getInstance()->getExchangeTracker()->LogoutExchange(ExchangeID(state_.exchange_id,0));

	return true;
}

bool BlockStreamExchangeBroadcast::SerializeAndSendToMulti(){
	//TODO: the support for partition_offset is not implemented yet!
	IteratorExecutorMaster* IEM=IteratorExecutorMaster::getInstance();
	BlockStreamExchangeLowerBroadcast::State EIELstate(state_.schema,state_.child,state_.upper_ip_list,state_.block_size,state_.exchange_id);
	BlockStreamIteratorBase *EIEL=new BlockStreamExchangeLowerBroadcast(EIELstate);
	for(unsigned i=0;i<state_.lower_ip_list.size();i++){
		cout<<"lower ip: "<<state_.lower_ip_list[i]<<endl;
	}
	cout<<"((((((((((((((((((((((((((((((((((((((((()))))))))))))))))))))))))))))))))))))))))"<<endl;
	if(IEM->ExecuteBlockStreamIteratorsOnSites(EIEL,state_.lower_ip_list)==false){
		printf("Cannot send the serialized iterator tree to the remote node!\n");
		return false;
	}
	cout<<"((((((((((((((((((((((((((((((((((((((((()))))))))))))))))))))))))))))))))))))))))"<<endl;
	return true;
}

bool BlockStreamExchangeBroadcast::CreateReceiverThread(){
	int error;
	error=pthread_create(&receiver_tid_,NULL,receiver,this);
	if(error!=0){
		Logging_ExchangeIteratorEager("Failed to create receiver thread.");
		return false;
	}
	return true;
}

void BlockStreamExchangeBroadcast::CancelReceiverThread(){
	pthread_cancel(receiver_tid_);
}

void* BlockStreamExchangeBroadcast::receiver(void* arg){
	BlockStreamExchangeBroadcast* Pthis=(BlockStreamExchangeBroadcast*)arg;

	unsigned subset=3<Pthis->nlowers_?3:Pthis->nlowers_;

	while(true){

		int old_seed=rand()%Pthis->nlowers_;

		fd_set socketfds;
		FD_ZERO(&socketfds);
		int sock_fd_max=0;
		for(int i=0;i<Pthis->nlowers_;i++){
			FD_SET(Pthis->socket_fd_lower_list_[i],&socketfds);
			if(Pthis->socket_fd_lower_list_[i]>sock_fd_max)
				sock_fd_max=Pthis->socket_fd_lower_list_[i];
		}
		/**
		 * After the select() returned, there is a loop to check which socket fd has newly coming message.
		 * The check should be in random order rather than in the increasing order, otherwise, the socket fd
		 * which is in the smaller index has larger chance to be checked than other socket fds.
		 */
		unsigned seed=rand()%Pthis->nlowers_;

		if(select(sock_fd_max+1,&socketfds,(fd_set*)NULL,(fd_set*)NULL,(struct timeval*)NULL)>0){

			for(unsigned j=0;j<subset;j++){
				if(FD_ISSET(Pthis->socket_fd_lower_list_[seed],&socketfds)){

					int recvtotal=0;

					while(recvtotal<Pthis->block_for_socket_->getsize()){
						int recvbytes;//recvtotal=3120,recvbytes=3120
						              //Pthis->block_for_socket_->getsize()=4104
						if((recvbytes=recv(Pthis->socket_fd_lower_list_[seed],(char*)Pthis->block_for_socket_->getBlock()+recvtotal,Pthis->block_for_socket_->getsize()-recvtotal,0))==-1){
												perror("recv error!\n");
												return false;
						}
						recvtotal+=recvbytes;
					}

//					Pthis->SendBlockBufferedNotification(Pthis->socket_fd_lower_list_[seed]);
					Logging_ExchangeIteratorEager("The %d-th block is received from Lower[%s]",Pthis->received_block_[seed],Pthis->lower_ip_array_[seed].c_str());
					Pthis->received_block_[seed]++;

					Pthis->received_block_stream_->deserialize(Pthis->block_for_socket_);
					const bool isLastBlock=Pthis->block_for_socket_->IsLastBlock();
					Pthis->buffer_->insertBlock(Pthis->received_block_stream_);

					if(isLastBlock){
						Logging_ExchangeIteratorEager("*****This block is the last one.");
						Pthis->nexhausted_lowers_++;
						Logging_ExchangeIteratorEager("<<<<<<<<<<<<<<<<nexhausted_lowers=%d>>>>>>>>>>>>>>>>",Pthis->nexhausted_lowers_);
						Pthis->SendBlockAllConsumedNotification(Pthis->socket_fd_lower_list_[seed]);
						Logging_ExchangeIteratorEager("This notification (all the blocks in the socket buffer are consumed) is send to the lower[%s].",Pthis->lower_ip_array_[seed].c_str());

					}

				}
				seed=(seed+1)%Pthis->nlowers_;
			}
		}

	}
}

