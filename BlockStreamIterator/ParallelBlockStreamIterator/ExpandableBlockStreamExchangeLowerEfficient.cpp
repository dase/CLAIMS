/*
 * ExpandableBlockStreamExchangeLowerEfficient.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */

#include "ExpandableBlockStreamExchangeLowerEfficient.h"


#include <malloc.h>
#include "../../configure.h"

#include "../../common/rename.h"
#include "../../common/Logging.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../Environment.h"
#include "../../common/Logging.h"
#include "../../utility/ThreadSafe.h"
#include "../../common/ids.h"
#include "../../utility/rdtsc.h"
ExpandableBlockStreamExchangeLowerEfficient::ExpandableBlockStreamExchangeLowerEfficient(State state)
:state(state),logging_(0){
	// TODO Auto-generated constructor stub
	assert(state.partition_key_index<100);

}

ExpandableBlockStreamExchangeLowerEfficient::~ExpandableBlockStreamExchangeLowerEfficient() {
	// TODO Auto-generated destructor stub
	delete logging_;
}
bool ExpandableBlockStreamExchangeLowerEfficient::open(const PartitionOffset&){
	logging_=new ExchangeIteratorEagerLowerLogging();
	logging_->log("[%lld] Exchange lower is created!",state.exchange_id);

	debug_connected_uppers=0;
	debug_connected_uppers_in=0;
	state.child->open(state.partition_offset);

	nuppers=state.upper_ip_list.size();
	partition_function_=PartitionFunctionFactory::createBoostHashFunction(nuppers);
	socket_fd_upper_list=new int[nuppers];

	/** initialize the block stream that is used to accumulate the block obtained by calling child iterator's next() **/
	block_stream_for_asking_=BlockStreamBase::createBlock(state.schema,state.block_size);

	/** buffer stores the tuples received from child iterator. Note the tuples are partitioned and stored. **/
	partitioned_data_buffer_=new PartitionedBlockBuffer(nuppers,block_stream_for_asking_->getSerializedBlockSize());

	/** the temporary block that is used to transfer a block from partitioned data buffer into sending buffer.**/
	block_for_buffer_=new BlockContainer(block_stream_for_asking_->getSerializedBlockSize());

	/** Initialize the buffer that is used to hold the blocks being sent. There are nuppers block, each corresponding
	 * to a merger. **/
	sending_buffer_=new PartitionedBlockContainer(nuppers,block_stream_for_asking_->getSerializedBlockSize());

	/** Initialized the temporary block to hold the serialized block stream. **/
	block_for_serialization_=new Block(block_stream_for_asking_->getSerializedBlockSize());

	/** Initialize the blocks that are used to accumulate the tuples from child so that the insertion to the buffer
	 * can be conducted at the granularity of blocks rather than tuples.
	 */
	partitioned_blockstream_= new BlockStreamBase*[nuppers];
	for(unsigned i=0;i<nuppers;i++){
		partitioned_blockstream_[i]=BlockStreamBase::createBlock(state.schema,state.block_size);
	}


	/** connect to all the mergers **/
	for(unsigned upper_id=0;upper_id<state.upper_ip_list.size();upper_id++){


		if(ConnectToUpper(ExchangeID(state.exchange_id,state.partition_offset),state.upper_ip_list[upper_id],socket_fd_upper_list[upper_id])!=true)
			return false;
	}

	/** create the sender thread **/
	int error;
	error=pthread_create(&sender_tid,NULL,sender,this);
	if(error!=0){
		logging_->elog("Failed to create the sender thread>>>>>>>>>>>>>>>>>>>>>>>>>>>>@@#@#\n\n.");
		return false;
	}

//	pthread_create(&debug_tid,NULL,debug,this);
/*debug*/
	return true;
}
bool ExpandableBlockStreamExchangeLowerEfficient::next(BlockStreamBase*){
	void* tuple_from_child;
	void* tuple_in_cur_block_stream;
	block_stream_for_asking_->setEmpty();
	if(state.child->next(block_stream_for_asking_)){
		/** if a blocks is obtained from child, we partition the tuples in the block. **/
		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator=block_stream_for_asking_->createIterator();
		while((tuple_from_child=traverse_iterator->nextTuple())>0){
//			const unsigned partition_id=hash(tuple_from_child);
			const unsigned partition_id=hash(tuple_from_child,state.schema,state.partition_key_index,nuppers);

			/** calculate the length of the tuple **/
			const unsigned bytes=state.schema->getTupleActualSize(tuple_from_child);

			while(!(tuple_in_cur_block_stream=partitioned_blockstream_[partition_id]->allocateTuple(bytes))){
				/** if there is not enough space to hold the new tuple, we insert the block into the data buffer **/
				partitioned_blockstream_[partition_id]->serialize(*block_for_serialization_);
				partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,partition_id);
				partitioned_blockstream_[partition_id]->setEmpty();
			}
			/** Inser the tuple, if there is enough space for the new tuple**/
			state.schema->copyTuple(tuple_from_child,tuple_in_cur_block_stream);
		}
		return true;
	}
	else{
		/* the child iterator is exhausted. We add the cur block steram block into the buffer*/
		for(unsigned i=0;i<nuppers;i++){
			partitioned_blockstream_[i]->serialize(*block_for_serialization_);
			partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,i);

			/* The following lines send an empty block to the upper, indicating that all
			 * the data from current sent has been transmit to the uppers.
			 */
			if(!partitioned_blockstream_[i]->Empty()){
				partitioned_blockstream_[i]->setEmpty();
				partitioned_blockstream_[i]->serialize(*block_for_serialization_);
				partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,i);
			}
		}

		/*
		 * waiting until all the block in the buffer has been transformed to the uppers.
		 */
		logging_->log("Waiting until all the blocks in the buffer is sent!");
		while(!partitioned_data_buffer_->isEmpty()){
			usleep(1);
		}

		/*
		 * waiting until all the uppers send the close notification which means that
		 * blocks in the uppers' socket buffer have all been consumed.
		 */
		logging_->log("Waiting for close notification!");
		for(unsigned i=0;i<nuppers;i++){
			WaitingForCloseNotification(socket_fd_upper_list[i]);
		}

		return false;
	}
}

//unsigned ExpandableBlockStreamExchangeLowerEfficient::hash(void* value){
//	const void* hash_key_address=state.schema->getColumnAddess(state.partition_key_index,value);
//	return state.schema->getcolumn(state.partition_key_index).operate->getPartitionValue(hash_key_address,nuppers);
//}
bool ExpandableBlockStreamExchangeLowerEfficient::close(){

	cancelSenderThread();

	state.child->close();

	/** free temporary space **/
	delete partitioned_data_buffer_;
	delete block_stream_for_asking_;
	delete block_for_serialization_;
	delete sending_buffer_;
	delete block_for_buffer_;
	for(unsigned i=0;i<nuppers;i++){
		delete partitioned_blockstream_[i];
	}

	delete [] partitioned_blockstream_;
	delete [] socket_fd_upper_list;

	delete partition_function_;
	return true;
}

void* ExpandableBlockStreamExchangeLowerEfficient::sender(void* arg){
	ExpandableBlockStreamExchangeLowerEfficient* Pthis=(ExpandableBlockStreamExchangeLowerEfficient*)arg;

	Pthis->sending_buffer_->Initialized();
	try{
		while(true){
			pthread_testcancel();
			bool consumed=false;
			BlockContainer* block_for_sending;
			int partition_id=Pthis->sending_buffer_->getBlockForSending(block_for_sending);
			if(partition_id>=0){
				pthread_testcancel();
				if(block_for_sending->GetRestSize()>0){
					int recvbytes;
					recvbytes=send(Pthis->socket_fd_upper_list[partition_id],(char*)block_for_sending->getBlock()+block_for_sending->GetCurSize(),block_for_sending->GetRestSize(),MSG_DONTWAIT);
					if(recvbytes==-1){
						if (errno == EAGAIN){
							continue;
						}
						printf("Error=%d,fd=%d\n",errno,Pthis->socket_fd_upper_list[partition_id]);
						Pthis->logging_->elog("Send error!\n");
						break;
					}
					else{
						if(recvbytes<block_for_sending->GetRestSize()){
							/* the block is not entirely sent. */
							Pthis->logging_->log("**not entire sent! bytes=%d, rest size=%d",recvbytes,block_for_sending->GetRestSize());
							block_for_sending->IncreaseActualSize(recvbytes);
							continue;
						}
						else{
							/** the block is sent in entirety. **/
							Pthis->logging_->log("A block is sent bytes=%d, rest size=%d",recvbytes,block_for_sending->GetRestSize());
							block_for_sending->IncreaseActualSize(recvbytes);
							Pthis->logging_->log("Send the new block to [%s]",Pthis->state.upper_ip_list[partition_id].c_str());
							Pthis->sendedblocks++;
							consumed=true;
						}
					}
				}
				else{
					consumed=true;
				}
			}
			else{
				/* "partition_id<0" means that block_for_sending is empty, so we get one block from the buffer into the block_for_sending_*/
				unsigned index=Pthis->partitioned_data_buffer_->getBlock(*Pthis->block_for_buffer_);
				Pthis->block_for_buffer_->reset();
				Pthis->sending_buffer_->insert(index,Pthis->block_for_buffer_);
			}
			if(consumed==true){
				/* In the current loop, we have sent an entire block to the receiver, so we should get a new block
				 * into the block_for_sender_*/
				pthread_testcancel();
				if(Pthis->partitioned_data_buffer_->getBlock(*Pthis->block_for_buffer_,partition_id)){
					Pthis->block_for_buffer_->reset();
					Pthis->sending_buffer_->insert(partition_id,Pthis->block_for_buffer_);
				}
				else{
					/**TODO: test the effort of the following sleeping statement and consider
					 * whether it should be replaced by conditioned wait **/
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
		usleep(100000);
	}
}

void ExpandableBlockStreamExchangeLowerEfficient::cancelSenderThread() {
	pthread_cancel(sender_tid);
	void* res;
	pthread_join(sender_tid,&res);
	if(res!=PTHREAD_CANCELED)
		printf("thread is not canceled!\n");
	sender_tid=0;
}
