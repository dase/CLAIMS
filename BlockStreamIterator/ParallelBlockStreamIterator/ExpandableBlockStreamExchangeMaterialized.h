/*
 * ExpandableBlockStreamExchangeMaterialized.h
 *
 *  Created on: Sep 4, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMEXCHANGEMATERIALIZED_H_
#define EXPANDABLEBLOCKSTREAMEXCHANGEMATERIALIZED_H_
#include <pthread.h>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../BlockStreamExchangeBase.h"
#include "../../common/Schema/Schema.h"
#include "../../utility/lock.h"
#include "../../common/Block/BlockStreamBuffer.h"
class ExpandableBlockStreamExchangeMaterialized: public BlockStreamExchangeBase {
public:
	struct State{
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
		unsigned long long int exchange_id_;
		std::vector<std::string> lower_ip_list_;
		std::vector<std::string> upper_ip_list_;
		State(Schema* schema, BlockStreamIteratorBase* child,unsigned block_size,std::vector<std::string> lower_ip_list,std::vector<std::string> upper_ip_list,unsigned long long int exchange_id)
		:schema_(schema),child_(child),block_size_(block_size),exchange_id_(exchange_id),lower_ip_list_(lower_ip_list),upper_ip_list_(upper_ip_list){}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ & block_size_ & exchange_id_ & lower_ip_list_ & upper_ip_list_;
		}

	};
	ExpandableBlockStreamExchangeMaterialized(State state);
	ExpandableBlockStreamExchangeMaterialized(){};
	virtual ~ExpandableBlockStreamExchangeMaterialized();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase* block);
	bool close();
private:
	bool SerializeAndSendToMulti();
	static void* receiver(void* arg);
private:
	State state_;
	semaphore sem_open_;
	volatile bool open_finished_;
	unsigned nexhausted_lowers_;
	Block* block_for_socket_;
	BlockStreamBase* received_block_stream_;
	BlockStreamBuffer* buffer;
	pthread_t receiver_tid;
};

#endif /* EXPANDABLEBLOCKSTREAMEXCHANGEMATERIALIZED_H_ */
