/*
 * ExchangeIteratorEager.h
 *
 *  Created on: Aug 15, 2013
 *      Author: wangli
 */

#ifndef EXCHANGEITERATOREAGER_H_
#define EXCHANGEITERATOREAGER_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <string>
#include <pthread.h>
#include "ExchangeIteratorLowerWithWideDependency.h"
#include "../Schema/Schema.h"
#include "../iterator.hpp"
#include "../Block/BlockReadable.h"
#include "../Block/BlockReadableFixBuffer.h"
#include "../Executor/IteratorExecutorMaster.h"
#include "../Block/synch.h"
#include "../Block/PartitionedBlockBuffer.h"

class ExchangeIteratorEager:public Iterator {
public:
	struct State{
		Schema* schema;
		Iterator* child;
		unsigned block_size;
		unsigned long long int exchange_id;
		std::vector<std::string> lower_ip_list;
		std::vector<std::string> upper_ip_list;
		State(Schema* schema, Iterator* child,unsigned block_size,std::vector<std::string> lower_ip_list,std::vector<std::string> upper_ip_list,unsigned long long int exchange_id)
		:schema(schema),child(child),block_size(block_size),exchange_id(exchange_id),lower_ip_list(lower_ip_list),upper_ip_list(upper_ip_list){}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema & child & block_size & exchange_id & lower_ip_list & upper_ip_list;
		}

	};
	ExchangeIteratorEager(State state);
	ExchangeIteratorEager(){};
	virtual ~ExchangeIteratorEager();
	bool open();
	bool next(void* tuple){
		void* tuple_in_block;
		if((tuple_in_block=curBlock->next())>0){
	//		Logging_ExchangeIteratorEager("a tuple is read from curBlock");
			state.schema->copyTuple(tuple_in_block,tuple);
			return true;
		}
		else{
			if(ReadBlockInSocketBuffer()==false){
				return false;//all the lowers are exhausted!
			}
			return next(tuple);
		}
	}
	bool close();
private:
	bool PrepareTheSocket();
	bool RegisterExchange();
	bool isMaster();
	bool SerializeAndSendToMulti();
	bool WaitForConnectionFromLowerExchanges();
	bool CreateReceiverThread();
	void CancelReceiverThread();
	void SendBlockBufferedNotification(int target_socket_fd);
	void SendBlockAllConsumedNotification(int target_socket_fd);
	/* return false if all the exchange lowers are exhausted*/
	bool ReadBlockInSocketBuffer();
//	int PickLowerOffset();
	static void* receiver(void* arg);
private:
	State state;
	BlockReadable* curBlock;
	BlockReadable* receiving_block;
	int sock_fd;
	unsigned socket_port;
	unsigned nlowers;
	int* socket_fd_lower_list;
	std::string * lower_ip_array;
	pthread_t receiver_tid;
//	semaphore empty_nslots;
//	semaphore used_nslots;
//	semaphore *produced_list;// one per lower exchange.
	unsigned nexhausted_lowers;
	PartitionedBlockBuffer *buffer;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<Iterator>(*this) & state;
	}
private:
	//debug
	unsigned consumed_block[100];
	unsigned received_block[100];

};

#endif /* EXCHANGEITERATOREAGER_H_ */
