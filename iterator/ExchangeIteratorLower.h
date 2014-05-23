/*
 * ExchangeIteratorLower.h
 *
 *  Created on: Jun 24, 2013
 *      Author: wangli
 */

#ifndef EXCHANGEITERATORLOWER_H_
#define EXCHANGEITERATORLOWER_H_

#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <netdb.h>

#include "../common/Schema/Schema.h"
#include "../iterator.hpp"
#include "../Block/BlockWritable.h"

class ExchangeIteratorLower:public Iterator {
public:
	struct State
	{
		Schema* schema;
		Iterator* child;
		std::string upper_ip;
		std::string upper_port;
		unsigned block_size;
		State(){};
		State(Schema *schema, Iterator* child, std::string upper_ip, std::string upper_port, unsigned block_size)
		:schema(schema),child(child),upper_ip(upper_ip),upper_port(upper_port),block_size(block_size)
		{}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			ar & schema & child & upper_ip & upper_port & block_size;
		}

	};
	ExchangeIteratorLower(State state);
	ExchangeIteratorLower():block(0),sock_fd(0){};
	virtual ~ExchangeIteratorLower();
	bool open();
	bool next(void*);
	bool close();
private:
	bool ConnectToUpperExchange(hostent* host, unsigned short port);
	bool PrepareForTheNewBlock();
	bool SendTheNewBlock();
	bool SendTheFinishNotification();
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Iterator>(*this) & state;
	}

private:
	State state;//should be serialized.
	BlockWritable* block;
	int sock_fd;
	void* tuple;
};

#endif /* EXCHANGEITERATORLOWER_H_ */
