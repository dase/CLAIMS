/*
 * ExpandableBlockStreamBroadcastExchangeLower.h
 *
 *  Created on: 2013-9-3
 *      Author: casa
 */

#ifndef EXPANDABLEBLOCKSTREAMBROADCASTEXCHANGELOWER_H_
#define EXPANDABLEBLOCKSTREAMBROADCASTEXCHANGELOWER_H_

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

class ExpandableBlockStreamBroadcastExchangeLower:public BlockStreamIteratorBase {
public:
	struct State{
		Schema* schema;
		BlockStreamIteratorBase* child;
		unsigned long long int exchange_id;
		//Currently, support give the ip vector.
		//TODO: support ip vector provided by scheduler
		std::vector<std::string> upper_ip_list;
		unsigned block_size;
		PartitionOffset partition_offset;
		State(Schema *schema, BlockStreamIteratorBase* child, std::vector<std::string> upper_ip_list, unsigned block_size,
						unsigned long long int exchange_id=0)
		:schema(schema),child(child),upper_ip_list(upper_ip_list),block_size(block_size),exchange_id(exchange_id),partition_offset(0)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema & child & exchange_id & upper_ip_list &block_size&partition_offset;
		}
	};

	ExpandableBlockStreamBroadcastExchangeLower(State state_);
	ExpandableBlockStreamBroadcastExchangeLower(){};
	virtual ~ExpandableBlockStreamBroadcastExchangeLower();

	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* );
	bool close();

private:
	static void* sender(void* arg);

	/* the thread for outputing debug information*/
//	static void* debug(void* arg);
//	unsigned hash(void*);
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
	BlockStreamBase* cur_block_stream_list_;
	BlockWritable* block_for_sending;
	BlockWritable* block_for_inserting_to_buffer_;
	BlockStreamBase* block_stream_for_asking_;
	pthread_t sender_tid;
	pthread_t debug_tid;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state;
	}
};

#endif /* EXPANDABLEBLOCKSTREAMBROADCASTEXCHANGELOWER_H_ */
