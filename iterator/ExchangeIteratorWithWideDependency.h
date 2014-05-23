/*
 * ExchangeIteratorWithWideDependency.h
 *
 *  Created on: 2013-7-1
 *      Author: zhanglei
 */

#ifndef EXCHANGEITERATORWITHWIDEDEPENDENCY_H_
#define EXCHANGEITERATORWITHWIDEDEPENDENCY_H_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

#include "ExchangeIteratorLowerWithWideDependency.h"
#include "../common/Schema/Schema.h"
#include "../iterator.hpp"
#include "../Block/Block.h"
#include "../Block/BlockReadableFixBuffer.h"
#include "../Executor/IteratorExecutorMaster.h"

#include <string>
#include <pthread.h>

//Currently, we define the BLOCKNUM here, define to 3
//TODO: this is must be determined by number of lower nodes
#define BLOCKNUM 2

class ExchangeIteratorWithWideDependency:public Iterator {
public:
	struct State{
		Schema *schema;
		Iterator* child;
		unsigned block_size;
		unsigned long long int exchange_id;
		std::vector<std::string > lower_ip_port_array;
		std::vector<std::string > upper_ip_port_array;
		explicit State(Schema* schema,Iterator* child,unsigned block_size,std::vector<std::string > lower_ip_port_array,
		std::vector<std::string> upper_ip_port_array,unsigned long long int exchange_id=0):schema(schema),child(child),block_size(block_size),
		lower_ip_port_array(lower_ip_port_array),upper_ip_port_array(upper_ip_port_array),exchange_id(exchange_id)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema & child & block_size & lower_ip_port_array & upper_ip_port_array & exchange_id;
		}
	};
	ExchangeIteratorWithWideDependency(State state);
	ExchangeIteratorWithWideDependency();
	virtual ~ExchangeIteratorWithWideDependency();
	bool open();
	bool next(void *);
	bool close();
private:
	bool PrepareTheSocket();
	bool WaitForConnectionFromLowerExchangeAllReached(int LowerNum);
	bool SerializeAndSendToMulti();
	bool AskForNewBlock(int cur);
	bool RegisterExchange();
	enum request{next_block,close_iterator};
	bool SendRequest(request req,int block);
	bool WaitNewBlockFromLowerExchange(int block);
	bool isMaster();
	static void *producerUpper(void *arg);
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		//the original code doesn't serialize the port.
		ar & boost::serialization::base_object<Iterator>(*this) & state &port;
	}
private:
	State state;
	BlockReadableFixBuffer *brfb;
	int sock_fd,in,out;
	//if a lower node finish his block, so collect++, until collect is reach
	//the number of lower nodes, destroy semaphore, kill threads and exit next()
	int collect;
	int sock_fd_lower_array[BLOCKNUM];
	std::string lower_ip_array[BLOCKNUM];
	//Currently, give a port to listen when new a instance
	//TODO: support the port is provided by the scheduler
	int port;
	//will be deleted
	int recvcount,printcount,spy1;
	semaphore *empty_sem,*full_sem;
	Lock *lo;
	int eventQueue[BLOCKNUM];
	int nullQueue[BLOCKNUM];
};

#endif /* EXCHANGEITERATORWITHWIDEDEPENDENCY_H_ */
