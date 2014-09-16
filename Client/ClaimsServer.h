/*
 * ClaimsServer.h
 *
 *  Created on: Feb 20, 2014
 *      Author: volt
 */

#ifndef CLAIMSSERVER_H_
#define CLAIMSSERVER_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "ClientResponse.h"

class ClaimsServer {
public:
	ClaimsServer(int port);
	virtual ~ClaimsServer();

	void configure();
//	void run();
	void shutdown();

private:

	int write(const int fd, const ClientResponse& res) const;
	int addClient(const int fd);
	int removeClient(const int fd);

	int receiveRequest(const int fd, const char *cmd);

	static void* receiveHandler(void *);
	static void* sendHandler(void *);

	bool isFull() const {
		return !(m_num < MAXCONN);
	}

	int m_port;
	int m_fd;	//listening fd
	int *m_clientFds;
	int m_num;
	const static int MAXCONN = 100;

};

#endif /* CLAIMSSERVER_H_ */
