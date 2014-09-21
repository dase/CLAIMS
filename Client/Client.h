/*
 * Client.h
 *
 *  Created on: Feb 18, 2014
 *      Author: volt
 */
#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
class ClientResponse;

class Client {
public:
	Client();

	void connection(std::string host, int port);
	ClientResponse* submitQuery(std::string args);

	ClientResponse* receive();

	void shutdown();

	virtual ~Client();

private:

	int m_clientFd;
};

#endif /* CLIENT_H_ */
