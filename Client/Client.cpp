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

Client::query_result Client::submit(std::string& command, std::string& message,
		ResultSet& rs) {
	if (m_clientFd < 0) {
		perror("Client does not connect to the server!\n");
		return Client::error;
	}
	ClientResponse *response = new ClientResponse();

	write(m_clientFd, command.c_str(), command.length() + 1);
	ClientLogging::log("Client: message from server!\n");
	const int maxBytes = 10000L;
	char *buf = new char[maxBytes];
	memset(buf, 0, sizeof(buf));

	int receivedBytesNum;
	//compute the length of ClientResponse object
	recv(m_clientFd, buf, sizeof(int) + sizeof(int), MSG_WAITALL );
	int len = *((int*) buf + 1);
	if ((receivedBytesNum = recv(m_clientFd, buf+sizeof(int) + sizeof(int), len, MSG_WAITALL)) < 0) {
		perror(
				"Client: submit query error, has problem with the communication!\n");
	}else {
		ClientLogging::log("receive %d bytes from server.\n", receivedBytesNum);
	}
	assert(maxBytes>=receivedBytesNum);
	response->deserialize(buf, receivedBytesNum);
	delete buf;

	switch(response->status){
		case ERROR:{
			message+="ERROR>"+response->content+"\n";
			return Client::error;
		}
		case OK:{
			while (response->status != END) {
				switch(response->status){
				case SCHEMA:
					rs.schema_=response->getSchema();
					break;
				case HEADER:
					rs.column_header_list_=response->getAttributeName().header_list;
					break;
				case DATA:
					assert(rs.schema_!=0);
					rs.appendNewBlock(response->getDataBlock(rs.schema_));
					break;
				}
				response = receive();
					ClientLogging::log("Message: %s\n", response->getMessage().c_str());
			}
			rs.query_time_=atof(response->content.c_str());
			return Client::result;
		}
		case CHANGE:{
			message=response->content+"\n";
			return Client::message;
		}
		default:{
			printf("Unexpected response from server!n");
			message="Unexpected response from server!\n";
			return Client::error;
		}
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
	recv(m_clientFd, buf, sizeof(int) + sizeof(int), MSG_WAITALL );
	int len = *((int*) buf + 1);
	if ((receivedBytesNum = recv(m_clientFd, buf+sizeof(int) + sizeof(int), len, MSG_WAITALL)) < 0) {
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
	recv(m_clientFd, buf, sizeof(int) + sizeof(int), MSG_WAITALL );
	int len = *((int*) buf + 1);


	if ((receivedBytesNum = recv(m_clientFd, buf+2*sizeof(int), len, MSG_WAITALL)) < 0) {
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
	close(m_clientFd);
	ClientLogging::log("-----for debug:	close fd %d", m_clientFd);
}

