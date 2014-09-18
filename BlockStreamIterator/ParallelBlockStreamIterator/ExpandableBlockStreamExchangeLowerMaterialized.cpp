/*
 * ExpandableBlockStreamExchangeLower.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */

#include "ExpandableBlockStreamExchangeLowerMaterialized.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <malloc.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../../configure.h"

#include "../../common/rename.h"
#include "../../common/Logging.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../Environment.h"

ExpandableBlockStreamExchangeLowerMaterialized::ExpandableBlockStreamExchangeLowerMaterialized(State state)
:state_(state),BlockStreamExchangeLowerBase(){
	log_=new ExchangeIteratorSenderMaterialized();
}

ExpandableBlockStreamExchangeLowerMaterialized::~ExpandableBlockStreamExchangeLowerMaterialized() {
	log_=new ExchangeIteratorSenderMaterialized();
}

ExpandableBlockStreamExchangeLowerMaterialized::ExpandableBlockStreamExchangeLowerMaterialized() {
	log_=new ExchangeIteratorSenderMaterialized();
}
bool ExpandableBlockStreamExchangeLowerMaterialized::open(const PartitionOffset& ){

	state_.child_->open(state_.partition_offset);

	/** get the number of mergers **/
	nuppers_=state_.upper_ip_list_.size();

	/** set the child exhausted bit **/
	child_exhausted_=false;

	/** socket fd for mergers, each corresponding to one merger. **/
	socket_fd_upper_list_=new int[nuppers_];

	/** fd for local partition on disk. **/
	disk_fd_list_=new int[nuppers_];

	/** initialize a temporary structure that holds the space for calling child's next() **/
	block_stream_for_asking_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);

	/** initialize a block that is used for sending **/
	block_for_sending_=new Block(block_stream_for_asking_->getSerializedBlockSize());

	/** initialize a block that is used as the intermediate structure for block serialization**/
	block_for_serialization_=new Block(block_stream_for_asking_->getSerializedBlockSize());

	/** initialize a partitioned block streams to accumulate the tuples obtained from child iterator's next() **/
	partitioned_block_stream_= new BlockStreamBase*[nuppers_];
	for(unsigned i=0;i<nuppers_;i++){
		partitioned_block_stream_[i]=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	}
	partitioned_data_buffer_=new PartitionedBlockBuffer(nuppers_,block_stream_for_asking_->getSerializedBlockSize(),1000);


	/** connect to the mergers **/
	for(unsigned upper_id=0;upper_id<state_.upper_ip_list_.size();upper_id++){
		if(!ConnectToUpper(ExchangeID(state_.exchange_id_,upper_id),state_.upper_ip_list_[upper_id],socket_fd_upper_list_[upper_id],log_)){
			return false;
		}
	}

	/** create the sender thread **/
	createWorkerThread();

	return true;
}
bool ExpandableBlockStreamExchangeLowerMaterialized::next(BlockStreamBase*){
	void* tuple_from_child;
	void* tuple_in_cur_block_stream;
	while(true){
		block_stream_for_asking_->setEmpty();
		if(state_.child_->next(block_stream_for_asking_)){
			/** a new block is obtained from child iterator **/
			BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator=block_stream_for_asking_->createIterator();
			while((tuple_from_child=traverse_iterator->nextTuple())>0){
				/** for each tuple in the newly obtained block, insert the tuple to one partitioned block according to the
				 * partition hash value**/
				const unsigned partition_id=hash(tuple_from_child,state_.schema_,state_.partition_key_index_,nuppers_);

				/** calculate the tuple size for the current tuple **/
				const unsigned bytes=state_.schema_->getTupleActualSize(tuple_from_child);

				/** insert the tuple into the corresponding partitioned block **/
				while(!(tuple_in_cur_block_stream=partitioned_block_stream_[partition_id]->allocateTuple(bytes))){
					/** if the destination block is full, we insert the block into the buffer **/

					partitioned_block_stream_[partition_id]->serialize(*block_for_serialization_);
					partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,partition_id);
					partitioned_block_stream_[partition_id]->setEmpty();
				}
				/** thread arriving here means that the space for the tuple is successfully allocated, so we copy the tuple **/
				state_.schema_->copyTuple(tuple_from_child,tuple_in_cur_block_stream);
			}
//			block_stream_for_asking_->serialize(*block_for_serialization_);
//			for(unsigned i=0;i<nuppers_;i++){
//				partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,i);
//			}
		}
		else{
			/* the child iterator is exhausted. We add the remaining data in partitioned data blocks into the buffer*/
			for(unsigned i=0;i<nuppers_;i++){
				Logging_ExpandableBlockStreamExchangeLM("||||||Fold the last||||||!");
				partitioned_block_stream_[i]->serialize(*block_for_serialization_);
				partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,i);
				/* The following lines send an empty block to the upper, indicating that all
								 * the data from current sent has been transmit to the uppers.
								 */
				if(!partitioned_block_stream_[i]->Empty()){
					partitioned_block_stream_[i]->setEmpty();
					partitioned_block_stream_[i]->serialize(*block_for_serialization_);
					partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,i);
				}
			}
			/*
			 * waiting until all the block in the buffer has been transformed to the uppers.
			 */
			log_->log("Waiting until all the blocks in the buffer is sent!");
			child_exhausted_=true;

			while(!partitioned_data_buffer_->isEmpty()){
				usleep(1);
			}
			/*
			 * waiting until all the uppers send the close notification which means that
			 * blocks in the uppers' socket buffer have all been consumed.
			 */
			log_->log("Waiting for close notification!");

			for(unsigned i=0;i<nuppers_;i++){

				WaitingForCloseNotification(socket_fd_upper_list_[i]);
			}
			log_->log("....passed!");

			return false;
		}
	}
}

bool ExpandableBlockStreamExchangeLowerMaterialized::close(){
	Logging_ExpandableBlockStreamExchangeLM("The sender thread is killed in the close() function!");

	/* close the files*/
	closeDiskFiles();
	/* Delete the files */
//	deleteDiskFiles();

	state_.child_->close();

	delete block_stream_for_asking_;
	delete block_for_sending_;
	delete block_for_serialization_;
	for(unsigned i=0;i<nuppers_;i++){
		delete partitioned_block_stream_[i];
	}
	delete partitioned_data_buffer_;
	delete [] partitioned_block_stream_;
	delete [] socket_fd_upper_list_;

	return true;
}
int ExpandableBlockStreamExchangeLowerMaterialized::getBlockFromFile(Block &block){
	unsigned seed=rand()%nuppers_;
	for(unsigned i=0;i<nuppers_;i++){
			const int read_size=read(disk_fd_list_[seed],block.getBlock(),block.getsize());
			if(read_size==-1){
				printf("Error occurs when read file. Reason: %s\n",strerror(errno));
			}
			else if(read_size==0){
				seed=(seed+1)%nuppers_;
				continue;
			}
			else if(!(read_size==block.getsize())){
				printf("read_size=%d, block.getsize()=%d\n",read_size,block.getsize());
				assert(false);
			}
			return seed;
	}
	return -1;
}
void ExpandableBlockStreamExchangeLowerMaterialized::Send(){

	/* open the file*/
	log_->log("open the file!\n");
	for(unsigned i=0;i<nuppers_;i++){
		disk_fd_list_[i]=FileOpen(getPartititionedFileName(i).c_str(),O_RDONLY);
		if(disk_fd_list_[i]==-1){
			printf("Cannot open file %s! Reason: %s\n",getPartititionedFileName(i).c_str(),strerror(errno));
			return;
		}
		/** set the read cursor at the beginner **/
	}
	log_->log("the file is opened!\n");
	while(true){
		int partition_id=getBlockFromFile(*block_for_sending_);
		if(partition_id<0){
			break;
		}
		int sendtotal=0;
		while(sendtotal<block_for_sending_->getsize()){
			int recvbytes;
			if((recvbytes=send(socket_fd_upper_list_[partition_id],(char*)block_for_sending_->getBlock()+sendtotal,block_for_sending_->getsize()-sendtotal,MSG_WAITALL))==-1){
				perror("recv error!\n");
				return ;
			}
			sendtotal+=recvbytes;
		}
		log_->log("Waiting the connection notification from [%s]",state_.upper_ip_list_[partition_id].c_str());
		log_->log("The block is received the upper[%s].",state_.upper_ip_list_[partition_id].c_str());
	}
}
bool ExpandableBlockStreamExchangeLowerMaterialized::Materialize(){
	/* create the file*/
	for(unsigned i=0;i<nuppers_;i++){
		disk_fd_list_[i]=FileOpen(getPartititionedFileName(i).c_str(),O_RDWR|O_APPEND|O_TRUNC|O_CREAT,S_IWUSR|S_IRUSR);
		if(disk_fd_list_[i]==-1){
			printf("Cannot open file %s! Reason: %s\n",getPartititionedFileName(i).c_str(),strerror(errno));
			printf("Hint: the most possible problem is that you do not have directory /tmp/claims/exchange/. Please create the directory and retry.\n");
			return false;
		}

	}
	log_->log("The intermediate file is created!\n");
	Block* block_for_storing=new Block(block_stream_for_asking_->getSerializedBlockSize());
//	ticks start=curtick();
	int counter=0;
	while(!child_exhausted_){
		if(partitioned_data_buffer_->getBlockInBuffer()>0){
			const unsigned partition_id=partitioned_data_buffer_->getBlock(*block_for_storing);
			const unsigned write_bytes=write(disk_fd_list_[partition_id],block_for_storing->getBlock(),block_for_storing->getsize());
			if(write_bytes!=block_for_storing->getsize()){
				printf("Error occurs when writing the block into the disk, reason:%s\n",strerror(errno));
			}
		}
	}
//	printf("Materialized %4.4f\n",getSecond(start));

	/*send the remaining block in the buffer*/
	while(partitioned_data_buffer_->getBlockInBuffer()>0){
		const unsigned partition_id=partitioned_data_buffer_->getBlock(*block_for_storing);
		const unsigned write_bytes=write(disk_fd_list_[partition_id],block_for_storing->getBlock(),block_for_storing->getsize());
		if(write_bytes!=block_for_storing->getsize()){
			printf("Error occurs when writing the block into the disk, reason:%s\n",strerror(errno));
		}
	}

	/* the child iterator is exhausted and the all the block in the buffer have been store in the file.*/
	log_->log("closing the local file!");

	/* close the file*/
	closeDiskFiles();
	log_->log("the local files are closed!");
	return true;

}
void* ExpandableBlockStreamExchangeLowerMaterialized::materialize_and_send(void* arg){
	ExpandableBlockStreamExchangeLowerMaterialized* Pthis=(ExpandableBlockStreamExchangeLowerMaterialized*)arg;
	if(!Pthis->Materialize()){
		perror("Error occurs when materializing the intermediate result!\n");
	}
	Pthis->log_->log("The intermediate is completely stored in the disk!\n");
	Pthis->Send();
	return 0;
}
void* ExpandableBlockStreamExchangeLowerMaterialized::debug(void* arg){
	ExpandableBlockStreamExchangeLowerMaterialized* Pthis=(ExpandableBlockStreamExchangeLowerMaterialized*)arg;
	while(true){
		printf("%d blocks in buffer.\n",Pthis->partitioned_data_buffer_->getBlockInBuffer());
		usleep(100000);
	}
}

bool ExpandableBlockStreamExchangeLowerMaterialized::createWorkerThread() {
	int error;
	error=pthread_create(&sender_tid_,NULL,materialize_and_send,this);
	if(error!=0){
		log_->elog("Failed to create the sender thread.");
		return false;
	}
	return true;
}

void ExpandableBlockStreamExchangeLowerMaterialized::cancelWorkerThread() {
	pthread_cancel(sender_tid_);
	void* res;
	pthread_join(sender_tid_,&res);
	if(res!=PTHREAD_CANCELED||res!=0)
		log_->elog("thread is not canceled!\n");
	sender_tid_=0;
}

void ExpandableBlockStreamExchangeLowerMaterialized::closeDiskFiles() {
	for(unsigned i=0;i<nuppers_;i++){
		FileClose(disk_fd_list_[i]);
	}
}


void ExpandableBlockStreamExchangeLowerMaterialized::deleteDiskFiles() {
	for(unsigned i=0;i<nuppers_;i++){
		unlink(getPartititionedFileName(i).c_str());
	}
}

std::string ExpandableBlockStreamExchangeLowerMaterialized::getPartititionedFileName(
		int partition_index) const {
	std::string temp_file_dir="/tmp/claims/exchange/";
	std::ostringstream file_name;
	file_name<<temp_file_dir<<"exchange_"<<state_.exchange_id_<<"_"<<state_.partition_offset<<"_"<<partition_index;
	return file_name.str();
}
