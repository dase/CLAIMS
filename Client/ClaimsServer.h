/*
 * ClaimsServer.h
 *
 *  Created on: Feb 20, 2014
 *      Author: volt
 */

#ifndef CLAIMSSERVER_H_
#define CLAIMSSERVER_H_
#include "ClientResponse.h"

class ClaimsServer {
public:
	ClaimsServer(int maxClientNum);
	virtual ~ClaimsServer();

	void startUp();


private:

	int write(const int fd, const ClientResponse& res) const;

	int m_port;

	int m_fd;

	int *m_clientFds;
	int m_num;

};

#endif /* CLAIMSSERVER_H_ */
