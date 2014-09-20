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
#include <malloc.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include "../Catalog/Catalog.h"
#include "../Daemon/Daemon.h"
#include "ClientResponse.h"
#include "json/json.h"

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

	static void sendJsonPacket(ClientResponse &cr, executed_result &res);

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
