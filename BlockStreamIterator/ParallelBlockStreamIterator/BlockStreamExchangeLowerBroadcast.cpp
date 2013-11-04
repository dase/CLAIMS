/*
 * BlockStreamExchangeLowerBroadcast.cpp
 *
 *  Created on: 2013-9-7
 *      Author: casa
 */

#include "BlockStreamExchangeLowerBroadcast.h"

#define BROADCAST 1

#include <malloc.h>
#include "../../configure.h"

#include "../../Block/BlockWritableFix.h"
#include "../../Block/BlockReadable.h"
#include "../../rename.h"
#include "../../Logging.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../Environment.h"
#include "../../Logging.h"

BlockStreamExchangeLowerBroadcast::BlockStreamExchangeLowerBroadcast(State state)
:state_(state),BlockStreamExchangeLowerBase() {

}

BlockStreamExchangeLowerBroadcast::BlockStreamExchangeLowerBroadcast()
:BlockStreamExchangeLowerBase() {

}

BlockStreamExchangeLowerBroadcast::~BlockStreamExchangeLowerBroadcast() {
	// TODO 自动生成的析构函数存根
}

void* BlockStreamExchangeLowerBroadcast::sender(void* arg){
	BlockStreamExchangeLowerBroadcast* Pthis=(BlockStreamExchangeLowerBroadcast*)arg;
	while(true){
		int partition_id=Pthis->buffer_->getBlock(*Pthis->block_for_sending_);
		assert(partition_id>-1);
		Logging_ExchangeIteratorEagerLower("Send the new block to [%s]",Pthis->state_.upper_ip_list[partition_id].c_str());

		for(int k=0;k<Pthis->nuppers_;k++){
		int sendtotal=0;
			while(sendtotal<Pthis->block_for_sending_->getsize()){
				int recvbytes;
				if((recvbytes=send(Pthis->socket_fd_upper_list_[k],(char*)Pthis->block_for_sending_->getBlock()+sendtotal,Pthis->block_for_sending_->getsize()-sendtotal,0))==-1){
										perror("recv error!\n");
										return false;
				}
				sendtotal+=recvbytes;
			}
		}
	}
}

bool BlockStreamExchangeLowerBroadcast::open(){
	state_.child->open();
	//upper节点的个数
	nuppers_=state_.upper_ip_list.size();
	//upper节点的socket数组
	socket_fd_upper_list_=new int[nuppers_];
	//向下端的iterator要的block
	block_stream_for_asking_=BlockStreamBase::createBlock(state_.schema,state_.block_size);
	//要发送走的block
	block_for_sending_=new BlockWritableFix(block_stream_for_asking_->getSerializedBlockSize(),state_.schema);
	//序列化完之后插入到buffer中的block
	block_for_inserting_to_buffer_=new BlockWritableFix(block_stream_for_asking_->getSerializedBlockSize(),state_.schema);

	cur_block_stream_list_=BlockStreamBase::createBlock(state_.schema,state_.block_size);
	buffer_=new PartitionedBlockBuffer(BROADCAST,block_stream_for_asking_->getSerializedBlockSize());


	for(unsigned upper_id=0;upper_id<state_.upper_ip_list.size();upper_id++){
		struct hostent* host;
		if((host=gethostbyname(state_.upper_ip_list[upper_id].c_str()))==0){
			perror("gethostbyname errors!\n");
			return false;
		}

		if((socket_fd_upper_list_[upper_id]=socket(AF_INET, SOCK_STREAM,0))==-1){
			perror("socket creation errors!\n");
			return false;
		}

		ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
		int upper_port;
		if((upper_port=et->AskForSocketConnectionInfo(state_.exchange_id,state_.upper_ip_list[upper_id]),upper_port)==0){
			Logging_ExchangeIteratorEagerLower("Fails to ask %s for socket connection info, the exchange id=%d",state_.upper_ip_list[upper_id].c_str(),state_.exchange_id);
		}

        for(unsigned upper_id=0;upper_id<state_.upper_ip_list.size();upper_id++){
                if(!ConnectToUpper(state_.exchange_id,state_.upper_ip_list[upper_id],socket_fd_upper_list_[upper_id])){
                        return false;
                }
        }

	}

	int error;
	error=pthread_create(&sender_tid_,NULL,sender,this);
	if(error!=0){
		Logging_ExchangeIteratorEagerLower("Failed to create the sender thread.");
		return false;
	}

//	pthread_create(&debug_tid,NULL,debug,this);
/*debug*/
//	readsendedblocks=0;

	return true;
}


bool BlockStreamExchangeLowerBroadcast::next(BlockStreamBase *){
	void* tuple_from_child;
	void* tuple_in_cur_block_stream;
	block_stream_for_asking_->setEmpty();
	if(state_.child->next(block_stream_for_asking_)){
		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator=block_stream_for_asking_->createIterator();
		while((tuple_from_child=traverse_iterator->nextTuple())>0){
//			const unsigned partition_id=hash(tuple_from_child);
			const unsigned bytes=state_.schema->getTupleActualSize(tuple_from_child);
			while(!(tuple_in_cur_block_stream=cur_block_stream_list_->allocateTuple(bytes))){
				cur_block_stream_list_->serialize(*block_for_inserting_to_buffer_);
				buffer_->insertBlockToPartitionedList(block_for_inserting_to_buffer_,0);
				cur_block_stream_list_->setEmpty();
			}
			state_.schema->copyTuple(tuple_from_child,tuple_in_cur_block_stream);
		}
		return true;
	}
	else{
		/* the child iterator is exhausted. We add the cur block steram block into the buffer*/
		for(unsigned i=0;i<nuppers_;i++){
			Logging_ExchangeIteratorEagerLower("||||||Fold the last||||||!");
			cur_block_stream_list_->serialize(*block_for_inserting_to_buffer_);
			buffer_->insertBlockToPartitionedList(block_for_inserting_to_buffer_,0);

			/* the following stupid three lines send a block with last block bytes to the buffer.
			 * TODO: making any change to the three lines would make them less stupid.
			 */
			cur_block_stream_list_->setEmpty();
			cur_block_stream_list_->serialize(*block_for_inserting_to_buffer_);
			buffer_->insertBlockToPartitionedList(block_for_inserting_to_buffer_,0);

		}
		/*
		 * waiting until all the block in the buffer has been transformed to the uppers.
		 */
		Logging_ExchangeIteratorEagerLower("Waiting until all the blocks in the buffer is sent!");

		while(!buffer_->isEmpty()){
			usleep(1);

		}
		/*
		 * waiting until all the uppers send the close notification which means that
		 * blocks in the uppers' socket buffer have all been consumed.
		 */
		Logging_ExchangeIteratorEagerLower("Waiting for close notification!");
		for(unsigned i=0;i<nuppers_;i++){
			WaitingForCloseNotification(socket_fd_upper_list_[i]);
		}
		Logging_ExchangeIteratorEagerLower("....passed!");

		return false;
	}
}

bool BlockStreamExchangeLowerBroadcast::close(){


	Logging_ExchangeIteratorEagerLower("The sender thread is killed in the close() function!");
	pthread_cancel(sender_tid_);
	/* close the socket connections to the uppers */
//	for(unsigned i=0;i<state.upper_ip_list.size();i++){
//		FileClose(socket_fd_upper_list[i]);
//	}
	state_.child->close();
	block_stream_for_asking_->~BlockStreamBase();
	block_for_sending_->~BlockWritable();
	block_for_inserting_to_buffer_->~BlockWritable();
	cur_block_stream_list_->~BlockStreamBase();
	buffer_->~PartitionedBlockBuffer();
	delete [] socket_fd_upper_list_;

	return true;
}
