/*
 * Client.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: volt
 */

#include "Client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

#include "ClientResponse.h"
#include "../common/Logging.h"

Client::Client() {
	m_clientFd = -1;
}

Client::~Client() {
}

void Client::connection(std::string host, int port) {

	sockaddr_in clientSockAddr, serverSockAddr;
	int result;

	m_clientFd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serverSockAddr, 0, sizeof(serverSockAddr));
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_addr.s_addr = inet_addr(host.c_str());
	serverSockAddr.sin_port = htons(port);

	result = connect(m_clientFd, (struct sockaddr *) &serverSockAddr,
			sizeof(serverSockAddr));
	if (result < 0) {
		perror("Client::error on connecting \n");
	} else {
		printf("Client::succeed in connecting with server\n");
	}
}

ClientResponse* Client::submitQuery(std::string args) {

	if (m_clientFd < 0) {
		perror("Client does not connect to the server!\n");
		return NULL;
	}

	ClientResponse *ret = new ClientResponse();

	write(m_clientFd, args.c_str(), args.length() + 1);
	ClientLogging::log("Client: message from server!\n");

	const int maxBytes = 10000L;
	char *buf = new char[maxBytes];
	memset(buf, 0, sizeof(buf));

	int receivedBytesNum;

	//compute the length of ClientResponse object
	recv(m_clientFd, buf, sizeof(int) + sizeof(int), MSG_WAITALL | MSG_PEEK);
	int len = *((int*) buf + 1);

	len += sizeof(int) + sizeof(int);

	if ((receivedBytesNum = recv(m_clientFd, buf, len, MSG_WAITALL)) < 0) {
		perror(
				"Client: submit query error, has problem with the communication!\n");
	}else {
		ClientLogging::log("receive %d bytes from server.\n", receivedBytesNum);
	}
	assert(maxBytes>=receivedBytesNum);
	ret->deserialize(buf, receivedBytesNum);
	delete buf;
	return ret;
}

ClientResponse* Client::receive() {

	if (m_clientFd < 0) {
		perror("Client does not connect to the server!\n");
		return NULL;
	}

	ClientResponse* ret = new ClientResponse();

	const int maxBytes = 75536+sizeof(int)*2;
	char *buf = new char[maxBytes];
	memset(buf, 0, sizeof(buf));

	int receivedBytesNum;

	//compute the length of the ClientResponse object
	recv(m_clientFd, buf, sizeof(int) + sizeof(int), MSG_WAITALL | MSG_PEEK);
	int len = *((int*) buf + 1);

	len += sizeof(int) + sizeof(int);

	if ((receivedBytesNum = recv(m_clientFd, buf, len, MSG_WAITALL)) < 0) {
		perror(
				"Client: receive result error, has problem with the communication! \n");
		printf("ERROR: %s",strerror(errno));
	}else {
		ClientLogging::log("receive %d bytes from server.\n", receivedBytesNum);
	}
	assert(maxBytes>=receivedBytesNum);
	ret->deserialize(buf, receivedBytesNum);
	delete buf;
	return ret;
}

void Client::shutdown() {
//	::shutdown(m_clientFd, 2);
	close(m_clientFd);
}
