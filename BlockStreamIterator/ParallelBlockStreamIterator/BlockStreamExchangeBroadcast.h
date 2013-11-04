/*
 * BlockStreamExchangeBroadcast.h
 *
 *  Created on: 2013-9-7
 *      Author: casa
 */

#ifndef BLOCKSTREAMEXCHANGEBROADCAST_H_
#define BLOCKSTREAMEXCHANGEBROADCAST_H_

#include "../BlockStreamExchangeBase.h"

#include <string>
#include <pthread.h>

#include "../../Schema/Schema.h"
#include "../../Block/BlockReadable.h"
#include "../../Block/BlockReadableFixBuffer.h"
#include "../../Executor/IteratorExecutorMaster.h"
#include "../../Block/synch.h"
#include "../../Block/PartitionedBlockBuffer.h"
#include "../../Block/BlockStream.h"
#include "../../Block/BlockStreamBuffer.h"

class BlockStreamExchangeBroadcast:public BlockStreamExchangeBase{
public:
	struct State{
			Schema* schema;
			BlockStreamIteratorBase* child;
			unsigned block_size;
			unsigned long long int exchange_id;
			std::vector<std::string> lower_ip_list;
			std::vector<std::string> upper_ip_list;
			State(Schema* schema, BlockStreamIteratorBase* child,unsigned block_size,std::vector<std::string> lower_ip_list,std::vector<std::string> upper_ip_list,unsigned long long int exchange_id)
			:schema(schema),child(child),block_size(block_size),exchange_id(exchange_id),lower_ip_list(lower_ip_list),upper_ip_list(upper_ip_list){}
			State(){};
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int version){
				ar & schema & child & block_size & exchange_id & lower_ip_list & upper_ip_list;
			}

		};

	BlockStreamExchangeBroadcast(State state);
	BlockStreamExchangeBroadcast();
	virtual ~BlockStreamExchangeBroadcast();

	bool open();
	bool next(BlockStreamBase* block);
	bool close();

protected:
	bool SerializeAndSendToMulti();
	bool CreateReceiverThread();
	void CancelReceiverThread();
	static void* receiver(void* arg);

private:
	State state_;
//	BlockReadable* curBlock;
	BlockReadable* block_for_socket_;
	BlockStreamBase* received_block_stream_;
	pthread_t receiver_tid_;
//	semaphore empty_nslots;
//	semaphore used_nslots;
//	semaphore *produced_list;// one per lower exchange.
	unsigned nexhausted_lowers_;
	semaphore sema_open_;
	volatile bool open_finished_;

	BlockStreamBuffer* buffer_;
private:
	//debug
	unsigned consumed_block_[100];
	unsigned received_block_[100];

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
            ar & boost::serialization::base_object<BlockStreamExchangeBase>(*this) & state_;
    }
};

#endif /* BLOCKSTREAMEXCHANGEBROADCAST_H_ */
