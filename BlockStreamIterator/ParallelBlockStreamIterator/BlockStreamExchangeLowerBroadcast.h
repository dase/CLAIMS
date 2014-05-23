/*
 * BlockStreamExchangeLowerBroadcast.h
 *
 *  Created on: 2013-9-7
 *      Author: casa
 */

#ifndef BLOCKSTREAMEXCHANGELOWERBROADCAST_H_
#define BLOCKSTREAMEXCHANGELOWERBROADCAST_H_

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
#include "../../common/Schema/Schema.h"
#include "../../Block/BlockReadable.h"
#include "../../Block/BlockWritable.h"
#include "../../Block/BlockReadableFixBuffer.h"
#include "../../Executor/IteratorExecutorMaster.h"
#include "../../Block/PartitionedBlockBuffer.h"
#include "../../Block/BlockStream.h"
#include "../../Block/BlockStreamBuffer.h"

#include "../BlockStreamExchangeLowerBase.h"

class BlockStreamExchangeLowerBroadcast: public BlockStreamExchangeLowerBase{
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

	BlockStreamExchangeLowerBroadcast(State state);
	BlockStreamExchangeLowerBroadcast();
	virtual ~BlockStreamExchangeLowerBroadcast();

	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* );
	bool close();

	static void* sender(void* arg);


private:
	State state_;
	unsigned nuppers_;
	int* socket_fd_upper_list_;
	PartitionedBlockBuffer* buffer_;

	/* one BlockStream for each uppers, the tuples from the child
	 * iterator are fed to the cur_block_stream_list_ according to their
	 * partition key.
	 */
	BlockStreamBase* cur_block_stream_list_;
	BlockWritable* block_for_sending_;
	BlockWritable* block_for_inserting_to_buffer_;
	BlockStreamBase* block_stream_for_asking_;
	pthread_t sender_tid_;
	pthread_t debug_tid_;

private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
                ar & boost::serialization::base_object<BlockStreamExchangeLowerBase>(*this) & state_;
        }
};

#endif /* BLOCKSTREAMEXCHANGELOWERBROADCAST_H_ */
