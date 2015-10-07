/*
 * ExpandableBlockStreamExchangeLower.h
 * This class is the improved version of ExpandableBlockStreamExchangeLower by using
 * non-blocking socket sending.
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMEXCHANGELOWEREFFICIENT_H_
#define EXPANDABLEBLOCKSTREAMEXCHANGELOWEREFFICIENT_H_

#include <map>
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
#ifdef DMALLOC
#include "dmalloc.h"
#endif


#include "../../common/Schema/Schema.h"

#include "./BlockStreamIteratorBase.h"

#include "../../Executor/IteratorExecutorMaster.h"

#include "../../common/Block/PartitionedBlockBuffer.h"
#include "../../common/Block/BlockStream.h"
#include "../../common/Block/BlockStreamBuffer.h"
#include "../../common/Block/PartitionedBlockContainer.h"

#include "../../common/hash.h"
#include "../../common/Logging.h"
#include "../physical_query_plan/BlockStreamExchangeLowerBase.h"
#include "../../common/partition_functions.h"

class ExpandableBlockStreamExchangeLowerEfficient:public BlockStreamExchangeLowerBase {
public:
	friend class ExpandableBlockStreamExchangeEpoll;
	struct State{
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned long long int exchange_id_;
		std::vector<NodeID> upper_id_list_;
		unsigned block_size_;
		partition_schema partition_schema_;
		PartitionOffset partition_offset_;
		State(Schema *schema, BlockStreamIteratorBase* child, std::vector<NodeID> upper_id_list, unsigned block_size,
						unsigned long long int exchange_id=0,partition_schema partition_schema=partition_schema::set_hash_partition())
		:schema_(schema),child_(child),upper_id_list_(upper_id_list),block_size_(block_size),exchange_id_(exchange_id),partition_schema_(partition_schema),partition_offset_(0)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ & exchange_id_ & upper_id_list_ &block_size_& partition_schema_&partition_offset_;
		}
	};
	ExpandableBlockStreamExchangeLowerEfficient(State state);
	ExpandableBlockStreamExchangeLowerEfficient();
	virtual ~ExpandableBlockStreamExchangeLowerEfficient();
	bool Open(const PartitionOffset& partition_index=0);
	bool Next(BlockStreamBase* );
	bool Close();
private:
	static void* sender(void* arg);

	/* the thread for outputing debug information*/
	static void* debug(void* arg);

	void cancelSenderThread();
private:
	State state_;
	unsigned nuppers_;
	int* socket_fd_upper_list;
	PartitionedBlockBuffer* partitioned_data_buffer_;

	/* one BlockStream for each uppers, the tuples from the child
	 * iterator are fed to the cur_block_stream_list_ according to their
	 * partition key.
	 */
	BlockStreamBase** partitioned_block_stream_;
	BlockContainer* block_for_sending;
	BlockContainer* block_for_buffer_;
	PartitionedBlockContainer* sending_buffer_;
	Block* block_for_serialization_;
	BlockStreamBase* block_stream_for_asking_;
	pthread_t sender_tid;
	pthread_t debug_tid;
	PartitionFunction* partition_function_;
	PartitionFunction* hash_test;
	Logging* logging_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & debug_connected_uppers, const unsigned int version){
		debug_connected_uppers & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
private:
	//debug

	unsigned generated_blocks;
	unsigned sendedblocks;
	unsigned readsendedblocks;

	unsigned debug_connected_uppers;
	std::map<std::string,int> debug_connected_uppers_list_;

	unsigned debug_connected_uppers_in;
	std::map<std::string,int> debug_connected_uppers_list_in;
};




#endif /* EXPANDABLEBLOCKSTREAMEXCHANGELOWEREFFICIENT_H_ */
