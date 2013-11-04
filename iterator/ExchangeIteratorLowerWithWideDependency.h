/*
 * ExchangeIteratorLowerWithWideDependency.h
 *
 *  Created on: 2013-6-30
 *      Author: zhanglei
 */

#ifndef EXCHANGEITERATORLOWERWITHWIDEDEPENDENCY_H_
#define EXCHANGEITERATORLOWERWITHWIDEDEPENDENCY_H_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

#include "../Schema/Schema.h"
#include "../iterator.hpp"
#include "../Block/BlockWritableFixBuffer.h"
#include "../Block/synch.h"
#include "../rename.h"

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
using namespace std;

//Currently, we restrict the event number is 100
//TODO: support the define the event number because the 100 maybe small
//can be configured to be configuration property
//#define EVENTNUM 1

class ExchangeIteratorLowerWithWideDependency:public Iterator{
public:
	struct State{
		//Currently, only one one-child Iterator is supported.
		//TODO: support multiple children vector<Iterator>
		Schema* schema;
		Iterator* child;
		unsigned long long int exchange_id;
		//Currently, support give the ip vector.
		//TODO: support ip vector provided by scheduler
		std::vector<std::string > upper_ip_port_array;
		unsigned block_size;
		explicit State(Schema *schema, Iterator* child, std::vector<std::string > upper_ip_array, unsigned block_size,
						unsigned long long int exchange_id=0)
		:schema(schema),child(child),upper_ip_port_array(upper_ip_array),block_size(block_size),exchange_id(exchange_id)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema & child & upper_ip_port_array & block_size & exchange_id;
		}
	};
	ExchangeIteratorLowerWithWideDependency(State state);
	ExchangeIteratorLowerWithWideDependency(){};
	virtual ~ExchangeIteratorLowerWithWideDependency();
	bool open();
	bool next(void *);
	bool close();
private:
	bool ConnectToUpperExchangeWithMulti(int &sock_fd,hostent* host, int port);
	int PrepareForTheNewBlock();
	bool SendBusySignal(int sockfd);
	bool SendTheNewBlock(int sockfd,int socket_fd);
	bool SendTheFinishNotification(int socket_fd);
	static void *prochaseLower(void *arg);
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Iterator>(*this) & state;
	}
private:
	State state;//should be serialized.
	unsigned upper_count;
	//over: 0
	//not over: 1
	int over;//over or not?
	BlockWritableFixBuffer *bwfb;
	void* tuple;
	//will be deleted
	int sendcount;
	semaphore *empty_sem;
	semaphore *full_sem;
	Lock *lo;
	int *eventQueue;
	int *sock_fd;
};

#endif /* EXCHANGEITERATORLOWERWITHWIDEDEPENDENCY_H_ */
