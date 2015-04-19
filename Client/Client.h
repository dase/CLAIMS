/*
 * Client.h
 *
 *  Created on: Feb 18, 2014
 *      Author: volt
 */
#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>

#include "../common/Block/ResultSet.h"
#ifdef DMALLOC
#include "dmalloc.h"
#endif
class ClientResponse;

class Client {
public:
	enum query_result{result,error,message};
public:
	Client();

	void connection(std::string host, int port);
	ClientResponse* submitQuery(std::string args);

	query_result submit(std::string command, std::string& message, ResultSet& rt);


	ClientResponse* receive();

	void shutdown();

	bool connected()const;

	virtual ~Client();

private:
	bool connected_;
	int m_clientFd;
};

#endif /* CLIENT_H_ */
