/*
 * ExchangeIteratorEagerLower.h
 *
 *  Created on: Aug 15, 2013
 *      Author: wangli
 */

#ifndef EXCHANGEITERATOREAGERLOWER_H_
#define EXCHANGEITERATOREAGERLOWER_H_
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
#include "../iterator.hpp"
#include "../Block/PartitionedBlockBuffer.h"
#include "../Block/BlockWritable.h"
#include "../common/rename.h"
#include "../common/Schema/Schema.h"


class ExchangeIteratorEagerLower:public Iterator {
public:
	struct State{
		Schema* schema;
		Iterator* child;
		unsigned long long int exchange_id;
		//Currently, support give the ip vector.
		//TODO: support ip vector provided by scheduler
		std::vector<std::string> upper_ip_list;
		unsigned block_size;
		State(Schema *schema, Iterator* child, std::vector<std::string> upper_ip_list, unsigned block_size,
						unsigned long long int exchange_id=0)
		:schema(schema),child(child),upper_ip_list(upper_ip_list),block_size(block_size),exchange_id(exchange_id)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema & child & exchange_id & upper_ip_list &block_size;
		}
	};
	ExchangeIteratorEagerLower(State state);
	ExchangeIteratorEagerLower(){};
	virtual ~ExchangeIteratorEagerLower();
	bool open();
	bool next(void*);
	bool close();
private:
	static void* sender(void* arg);
	static void* debug(void* arg);
	unsigned hash(void*);
	bool ConnectToUpperExchangeWithMulti(int &sock_fd,struct hostent* host,int port);
	void WaitingForNotification(int target_socket_fd);
	void WaitingForCloseNotification();
private:
	State state;
	unsigned nuppers;
	void* tuple;
	int* socket_fd_upper_list;
	PartitionedBlockBuffer* buffer;

	/* one BlockWritable for each uppers, the tuples from the child
	 * iterator are fed to the cur_block_list according to their
	 * partition key.
	 */
	BlockWritable** cur_block_list;
	BlockWritable* block_for_sending;
	pthread_t sender_tid;
	pthread_t debug_tid;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<Iterator>(*this) & state;
	}
private:
	//debug

	unsigned generated_blocks;
	unsigned sendedblocks;
	unsigned readsendedblocks;
};

#endif /* EXCHANGEITERATOREAGERLOWER_H_ */
