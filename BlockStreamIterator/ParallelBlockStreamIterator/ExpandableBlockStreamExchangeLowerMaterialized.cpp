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
#include "../../common/Logging.h"

ExpandableBlockStreamExchangeLowerMaterialized::ExpandableBlockStreamExchangeLowerMaterialized(State state)
:state_(state),BlockStreamExchangeLowerBase(){

}

ExpandableBlockStreamExchangeLowerMaterialized::~ExpandableBlockStreamExchangeLowerMaterialized() {
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

	/** the lengths of the disk files, each corresponding to a single file **/
	disk_file_length_list_=new unsigned[nuppers_];

	/** record the current read position of the on-disk partitioned file **/
	disk_file_cur_list_=new unsigned[nuppers_];

	/** initialize a temporary structure that hold the space for calling child's next() **/
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
		if(!ConnectToUpper(ExchangeID(state_.exchange_id_,0),state_.upper_ip_list_[upper_id],socket_fd_upper_list_[upper_id])){
			return false;
		}
		printf("New socket id %d\n",socket_fd_upper_list_[upper_id]);
	}

	/** create the sender thread **/
	createWorkerThread();

	return true;
}
bool ExpandableBlockStreamExchangeLowerMaterialized::next(BlockStreamBase*){
	void* tuple_from_child;
	void* tuple_in_cur_block_stream;
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

			/** inser the tuple into the corresponding partitioned block **/
			while(!(tuple_in_cur_block_stream=partitioned_block_stream_[partition_id]->allocateTuple(bytes))){
				/** if the destination block is full, we insert the block into the buffer **/

				partitioned_block_stream_[partition_id]->serialize(*block_for_serialization_);
				partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,partition_id);
				partitioned_block_stream_[partition_id]->setEmpty();
			}
			/** thread arriving here means that the space for the tuple is successfully allocated, so we copy the tuple **/
			state_.schema_->copyTuple(tuple_from_child,tuple_in_cur_block_stream);
		}
		return true;
	}
	else{
		/* the child iterator is exhausted. We add the remaining data in partitioned data blocks into the buffer*/
		for(unsigned i=0;i<nuppers_;i++){
			Logging_ExpandableBlockStreamExchangeLM("||||||Fold the last||||||!");
			partitioned_block_stream_[i]->serialize(*block_for_serialization_);
			partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,i);
			/* the following stupid three lines send a block with last block bytes to the buffer.
			 * TODO: making any change to the three lines would make them less stupid.
			 */
			partitioned_block_stream_[i]->setEmpty();
			partitioned_block_stream_[i]->serialize(*block_for_serialization_);
			partitioned_data_buffer_->insertBlockToPartitionedList(block_for_serialization_,i);
		}
		/*
		 * waiting until all the block in the buffer has been transformed to the uppers.
		 */
		Logging_ExpandableBlockStreamExchangeLM("Waiting until all the blocks in the buffer is sent!");
		child_exhausted_=true;

		while(!partitioned_data_buffer_->isEmpty()){
			usleep(1);
		}
		/*
		 * waiting until all the uppers send the close notification which means that
		 * blocks in the uppers' socket buffer have all been consumed.
		 */
		Logging_ExpandableBlockStreamExchangeLM("Waiting for close notification!");

		for(unsigned i=0;i<nuppers_;i++){

			WaitingForCloseNotification(socket_fd_upper_list_[i]);
		}
		Logging_ExpandableBlockStreamExchangeLM("....passed!");

		return false;
	}
}

bool ExpandableBlockStreamExchangeLowerMaterialized::close(){
	Logging_ExpandableBlockStreamExchangeLM("The sender thread is killed in the close() function!");

	cancelWorkerThread();
	/* close the files*/
	closeDiskFiles();
	/* Delete the files */
	deleteDiskFiles();

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

		if(disk_file_cur_list_[i]+block.getsize()<=disk_file_length_list_[i]){

			const int read_size=read(disk_fd_list_[i],block.getBlock(),block.getsize());
			assert(read_size==block.getsize());
			disk_file_cur_list_[i]+=block.getsize();
			return i;
		}
		seed=(seed=1)%nuppers_;
	}
	return -1;
}
void ExpandableBlockStreamExchangeLowerMaterialized::Send(){

	printf("in Send!\n");
	/* open the file*/
	Logging_ExpandableBlockStreamExchangeLM("open the file!\n");
	std::string temp_file_dir="/tmp/claims/exchange/";
	for(unsigned i=0;i<nuppers_;i++){
		std::ostringstream file_name;
		file_name<<temp_file_dir<<"exchange_"<<state_.exchange_id_<<"_"<<i;
		disk_fd_list_[i]=FileOpen(file_name.str().c_str(),O_RDONLY);
		if(disk_fd_list_[i]==-1){
			printf("Cannot open file %s! Reason: %s\n",file_name.str().c_str(),strerror(errno));
			return;
		}
		/** set the read cursor at the beginner **/
		lseek(disk_fd_list_[i],0,SEEK_SET);
		disk_file_length_list_[i]=lseek(disk_fd_list_[i],0,SEEK_END);
		disk_file_cur_list_[i]=0;
		lseek(disk_fd_list_[i],0,SEEK_SET);
	}
	Logging_ExpandableBlockStreamExchangeLM("the file is opened!\n");

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
			Logging_ExpandableBlockStreamExchangeLM("Waiting the connection notification from [%s]",state_.upper_ip_list_[partition_id].c_str());
			Logging_ExpandableBlockStreamExchangeLM("The block is received the upper[%s].",state_.upper_ip_list_[partition_id].c_str());
	}
}
bool ExpandableBlockStreamExchangeLowerMaterialized::Materialize(){
	/* create the file*/
	std::string temp_file_dir="/tmp/claims/exchange/";
	for(unsigned i=0;i<nuppers_;i++){
		std::ostringstream file_name;
		file_name<<temp_file_dir<<"exchange_"<<state_.exchange_id_<<"_"<<i;
		disk_fd_list_[i]=FileOpen(file_name.str().c_str(),O_RDWR|O_APPEND|O_TRUNC|O_CREAT,S_IWUSR|S_IRUSR);
		if(disk_fd_list_[i]==-1){
			printf("Cannot open file %s! Reason: %s\n",file_name.str().c_str(),strerror(errno));
			printf("Hint: the most possible problem is that you do not have directory /tmp/claims/exchange/. Please create the directory and retry.\n");
			return false;
		}

	}
	Logging_ExpandableBlockStreamExchangeLM("The intermediate file is created!\n");
	Block* block_for_storing=new Block(block_stream_for_asking_->getSerializedBlockSize());
//	ticks start=curtick();
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
//		Logging_ExpandableBlockStreamExchangeLM("->Materialize: a block is written to the file!\n");
	}

	/* the child iterator is exhausted and the all the block in the buffer have been store in the file.*/
	Logging_ExpandableBlockStreamExchangeLM("closing the local file!");

	/* close the file*/
	closeDiskFiles();
	Logging_ExpandableBlockStreamExchangeLM("the local files are closed!");
	return true;

}
void* ExpandableBlockStreamExchangeLowerMaterialized::materialize_and_send(void* arg){
	ExpandableBlockStreamExchangeLowerMaterialized* Pthis=(ExpandableBlockStreamExchangeLowerMaterialized*)arg;
	if(!Pthis->Materialize()){
		perror("Error occurs when materializing the intermediate result!\n");
	}
	Logging_ExpandableBlockStreamExchangeLM("The intermediate is completely stored in the disk!\n");

	Pthis->Send();
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
		Logging_ExpandableBlockStreamExchangeLM("Failed to create the sender thread.");
		return false;
	}
	return true;
}

void ExpandableBlockStreamExchangeLowerMaterialized::cancelWorkerThread() {
	pthread_cancel(sender_tid_);
	void* res;
	pthread_join(sender_tid_,&res);
	if(res!=PTHREAD_CANCELED)
		printf("thread is not canceled!\n");
	sender_tid_=0;
}

void ExpandableBlockStreamExchangeLowerMaterialized::closeDiskFiles() {
	for(unsigned i=0;i<nuppers_;i++){
		FileClose(disk_fd_list_[i]);
	}
}

void ExpandableBlockStreamExchangeLowerMaterialized::deleteDiskFiles() {
	std::string temp_file_dir="/tmp/claims/exchange/";
	for(unsigned i=0;i<nuppers_;i++){
		std::ostringstream file_name;
		file_name<<temp_file_dir<<"exchange_"<<state_.exchange_id_<<"_"<<i;
		unlink(file_name.str().c_str());
	}
}
