/*
 * ExchangeIterator.h
 *
 *  Created on: Jun 24, 2013
 *      Author: wangli
 */

#ifndef EXCHANGEITERATOR_H_
#define EXCHANGEITERATOR_H_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


#include "../Schema/Schema.h"
#include "../iterator.hpp"
#include "../Block/Block.h"
#include "ExchangeIteratorLower.h"
#include "../Block/BlockReadable.h"
class ExchangeIterator:public Iterator {
public:

	struct State
	{
		Schema* schema;
		Iterator* child;
		unsigned block_size;
		State(Schema* schema, Iterator* child, unsigned block_size)
		:schema(schema),child(child),block_size(block_size)
		{}
		State(){}
	};
	ExchangeIterator(State state);
	virtual ~ExchangeIterator();
	bool open();
	bool next(void *);
	bool close();
private:
	ExchangeIteratorLower* GenerateLowerExchange();
	bool AskForNewBlock();
	bool PrepareTheSocket();
	bool SerializeAndSend();
	bool WaitForConnectionFromLowerExchange();
	bool WaitNewBlockFromLowerExchange();
	enum request{next_block,close_iterator};
	bool SendRequest(request req);
private:

	State state;
	BlockReadable* block;
	int sock_fd;
	int sock_fd_Lower;
	uint16_t port;
};

#endif /* EXCHANGEITERATOR_H_ */
