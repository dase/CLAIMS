/*
 * ExpandableBlockStreamExchangeLower.h
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMEXCHANGELOWERMATERIALIZED_H_
#define EXPANDABLEBLOCKSTREAMEXCHANGELOWERMATERIALIZED_H_


#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <string>


#include <pthread.h>
#include <stdio.h>
#include "../../Schema/Schema.h"
#include "../BlockStreamExchangeLowerBase.h"
#include "../../Block/BlockReadable.h"
#include "../../Block/BlockWritable.h"
#include "../../Block/BlockReadableFixBuffer.h"
#include "../../Executor/IteratorExecutorMaster.h"
#include "../../Block/synch.h"
#include "../../Block/PartitionedBlockBuffer.h"
#include "../../Block/BlockStream.h"
#include "../../Block/BlockStreamBuffer.h"

class ExpandableBlockStreamExchangeLowerMaterialized:public BlockStreamExchangeLowerBase {
public:
	struct State{
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned long long int exchange_id_;
		//Currently, support give the ip vector.
		//TODO: support ip vector provided by scheduler
		std::vector<std::string> upper_ip_list_;
		unsigned block_size_;
		PartitionOffset partition_off_;
		State(Schema *schema, BlockStreamIteratorBase* child, std::vector<std::string> upper_ip_list, unsigned block_size,
						unsigned long long int exchange_id=0)
		:schema_(schema),child_(child),upper_ip_list_(upper_ip_list),block_size_(block_size),exchange_id_(exchange_id),partition_off_(0)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ & exchange_id_ & upper_ip_list_ &block_size_&partition_off_;
		}
	};
	ExpandableBlockStreamExchangeLowerMaterialized(State state);
	ExpandableBlockStreamExchangeLowerMaterialized(){

	};
	virtual ~ExpandableBlockStreamExchangeLowerMaterialized();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase* );
	bool close();
private:
	void Send();
	bool Materialize();
	static void* materialize_and_send(void* arg);
	/* the thread for outputing debug information*/
	static void* debug(void* arg);
	unsigned hash(void*);
	int getBlockFromFile(Block &block);
//	bool ConnectToUpperExchangeWithMulti(int &sock_fd,struct hostent* host,int port);
//	void WaitingForNotification(int target_socket_fd);
//	void WaitingForCloseNotification();

private:
	State state_;
	unsigned nuppers_;
	int* socket_fd_upper_list_;
	PartitionedBlockBuffer* buffer_;

	/* one BlockStream for each uppers, the tuples from the child
	 * iterator are fed to the cur_block_stream_list_ according to their
	 * partition key.
	 */
	BlockStreamBase** cur_block_stream_list_;
	BlockWritable* block_for_sending_;
	BlockWritable* block_for_inserting_to_buffer_;
	BlockStreamBase* block_stream_for_asking_;
	pthread_t sender_tid_;
	pthread_t debug_tid_;

	int* disk_fd_list_;
	unsigned* disk_file_length_list_;
	unsigned* disk_file_cur_list_;
//	std::vector<std::ofstream> disk_fd_out_list_;
//	std::vector<std::ifstream> disk_fd_in_list_;
	bool child_exhausted_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamExchangeLowerBase>(*this) & state_;
	}
private:
	//debug

	unsigned generated_blocks;
	unsigned sendedblocks;
	unsigned readsendedblocks;
};




#endif /* EXPANDABLEBLOCKSTREAMEXCHANGELOWER_H_ */
