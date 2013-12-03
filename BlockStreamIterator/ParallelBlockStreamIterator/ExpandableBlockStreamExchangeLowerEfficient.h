/*
 * ExpandableBlockStreamExchangeLower.h
 * This class is the improved version of ExpandableBlockStreamExchangeLower by using
 * non-blocking socket sending.
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMEXCHANGELOWEREFFICIENT_H_
#define EXPANDABLEBLOCKSTREAMEXCHANGELOWEREFFICIENT_H_


#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <string>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include "../../Schema/Schema.h"
#include "../BlockStreamIteratorBase.h"
#include "../../Block/BlockReadable.h"
#include "../../Block/BlockWritable.h"
#include "../../Block/BlockReadableFixBuffer.h"
#include "../../Executor/IteratorExecutorMaster.h"
#include "../../Block/synch.h"
#include "../../Block/PartitionedBlockBuffer.h"
#include "../../Block/BlockStream.h"
#include "../../Block/BlockStreamBuffer.h"
#include "../../Block/PartitionedBlockContainer.h"
#include "../../hash.h"
#include "../../Logging.h"

class ExpandableBlockStreamExchangeLowerEfficient:public BlockStreamIteratorBase {
public:
	friend class ExpandableBlockStreamExchangeEpoll;
	struct State{
		Schema* schema;
		BlockStreamIteratorBase* child;
		unsigned long long int exchange_id;
		//Currently, support give the ip vector.
		//TODO: support ip vector provided by scheduler
		std::vector<std::string> upper_ip_list;
		unsigned block_size;
		unsigned partition_key_index;
		PartitionOffset partition_offset;
		State(Schema *schema, BlockStreamIteratorBase* child, std::vector<std::string> upper_ip_list, unsigned block_size,
						unsigned long long int exchange_id=0,unsigned partition_index=0)
		:schema(schema),child(child),upper_ip_list(upper_ip_list),block_size(block_size),exchange_id(exchange_id),partition_key_index(partition_index),partition_offset(0)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema & child & exchange_id & upper_ip_list &block_size& partition_key_index&partition_offset;
		}
	};
	ExpandableBlockStreamExchangeLowerEfficient(State state);
	ExpandableBlockStreamExchangeLowerEfficient(){};
	virtual ~ExpandableBlockStreamExchangeLowerEfficient();
	bool open(const PartitionOffset& partition_index=0);
	bool next(BlockStreamBase* );
	bool close();
private:
	static void* sender(void* arg);

	/* the thread for outputing debug information*/
	static void* debug(void* arg);
	unsigned hash(void*);
	bool ConnectToUpperExchangeWithMulti(int &sock_fd,struct hostent* host,int port);
	void WaitingForNotification(int target_socket_fd);
	void WaitingForCloseNotification();

private:
	State state;
	unsigned nuppers;
	int* socket_fd_upper_list;
	PartitionedBlockBuffer* buffer;

	/* one BlockStream for each uppers, the tuples from the child
	 * iterator are fed to the cur_block_stream_list_ according to their
	 * partition key.
	 */
	BlockStreamBase** cur_block_stream_list_;
	BlockContainer* block_for_sending;
	BlockContainer* block_for_buffer_;
	PartitionedBlockContainer* buffer_for_sending_;
	BlockWritable* block_for_inserting_to_buffer_;
	BlockStreamBase* block_stream_for_asking_;
	pthread_t sender_tid;
	pthread_t debug_tid;
	PartitionFunction* partition_function_;
	PartitionFunction* hash_test;
	Logging* logging_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state;
	}
private:
	//debug

	unsigned generated_blocks;
	unsigned sendedblocks;
	unsigned readsendedblocks;
};




#endif /* EXPANDABLEBLOCKSTREAMEXCHANGELOWEREFFICIENT_H_ */
