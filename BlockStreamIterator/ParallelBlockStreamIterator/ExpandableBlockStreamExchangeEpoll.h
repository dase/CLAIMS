/*
 * ExpandableBlockStreamExchange.h
 *	This class is implemented based on the ExchangeIteratorEager.
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMEXCHANGEEPOLL_H_
#define EXPANDABLEBLOCKSTREAMEXCHANGEEPOLL_H_



#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include <string>
#include <pthread.h>
#include <map>
#include "../BlockStreamIteratorBase.h"
#include "../../utility/lock.h"
#include "../../Executor/IteratorExecutorMaster.h"

#include "../../common/Logging.h"
#include "../../common/ExpandedThreadTracker.h"

#include "../../common/Schema/Schema.h"

#include "../../common/Block/BlockContainer.h"
#include "../../common/Block/PartitionedBlockBuffer.h"
#include "../../common/Block/BlockStream.h"
#include "../../common/Block/BlockStreamBuffer.h"

#include "../ExpandableBlockStreamIteratorBase.h"

class ExpandableBlockStreamExchangeEpoll:public ExpandableBlockStreamIteratorBase {
public:
	struct State{
		Schema* schema;
		BlockStreamIteratorBase* child;
		unsigned block_size;
		unsigned long long int exchange_id;
		std::vector<std::string> lower_ip_list;
		std::vector<std::string> upper_ip_list;
		unsigned partition_key_index;
		State(Schema* schema, BlockStreamIteratorBase* child,unsigned block_size,std::vector<std::string> lower_ip_list,std::vector<std::string> upper_ip_list,unsigned long long int exchange_id,unsigned partition_index=0)
		:schema(schema),child(child),block_size(block_size),exchange_id(exchange_id),lower_ip_list(lower_ip_list),upper_ip_list(upper_ip_list),partition_key_index(partition_index){}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema & child & block_size & exchange_id & lower_ip_list & upper_ip_list & partition_key_index;
		}

	};
	ExpandableBlockStreamExchangeEpoll(State state);
	ExpandableBlockStreamExchangeEpoll();
	virtual ~ExpandableBlockStreamExchangeEpoll();
	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* block);
	bool close();
	void print();
private:
	bool PrepareTheSocket();
	bool RegisterExchange();
	bool checkOtherUpperRegistered();
	bool isMaster();
	bool SerializeAndSendToMulti();
	bool WaitForConnectionFromLowerExchanges();
	bool CreateReceiverThread();
	void CancelReceiverThread();
	void SendBlockBufferedNotification(int target_socket_fd);
	void SendBlockAllConsumedNotification(int target_socket_fd);
	void CloseTheSocket();
	bool SetSocketNonBlocking(int socket_fd);

	void createPerformanceInfo();
	/* this function is called to reset status of the iterator instance, so that
	 * the following calling of open() and next() can act correctly.
	 */
	void resetStatus();


	static void* receiver(void* arg);
	static void* debug(void* arg);
private:
	State state;
	BlockContainer** block_for_socket_;
	BlockStreamBase* received_block_stream_;
	int sock_fd;
	unsigned socket_port;
	unsigned nlowers;
	int epoll_fd_;
	int* socket_fd_lower_list;
	std::vector<std::string>  lower_ip_array;
	pthread_t receiver_tid;
	pthread_t debug_tid;
	unsigned nexhausted_lowers;
	unsigned partition_offset;
	BlockStreamBuffer* buffer;

	semaphore sem_new_block_or_eof_;

	/*the lower socket fd to the index*/
	std::map<int,int> lower_sock_fd_to_index;
	Logging* logging_;

	PerformanceInfo * perf_info_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<ExpandableBlockStreamIteratorBase>(*this) & state;
	}
private:
	//debug
	unsigned debug_consumed_block[100];
	unsigned debug_received_block[100];
	unsigned debug_winner_thread;

};

#endif /* EXPANDABLEBLOCKSTREAMEXCHANGEEPOLL_H_ */
