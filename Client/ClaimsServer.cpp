/*
 * ClaimsServer.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: volt
 */

#include "ClaimsServer.h"

#include <malloc.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>

#include "../Catalog/Catalog.h"

ClaimsServer::ClaimsServer(int maxClientNum) {

	m_num = 0;
	m_clientFds = new int[maxClientNum];
	m_fd = -1;
	m_port = -1;
}

ClaimsServer::~ClaimsServer() {
}

void ClaimsServer::startUp() {

	int &serverSockFd = m_fd;
	int clientSockFd;
	unsigned int nread;
	char *buf = new char[128];
	sockaddr_in serverSocket;
	sockaddr_in clientSocket;

	serverSocket.sin_family = AF_INET;
	serverSocket.sin_port = htons(8000);
	serverSocket.sin_addr.s_addr = INADDR_ANY;

	if ((serverSockFd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket error");
		return;
	}

	if (bind(serverSockFd, (sockaddr *) &serverSocket, sizeof(serverSocket))
			< 0) {
		perror("Server:: bind error!");
		return;
	}

	int backLog = 5;
	if (listen(serverSockFd, backLog) == -1) {
		printf("Server::listen error!\n");
		return;
	} else {
		printf("Server::listen ok!\n");
	}

	int nfds = serverSockFd + 1;
	unsigned int sockLen;
	fd_set watchFds;
	fd_set readFds;

	FD_ZERO(&watchFds);
	FD_SET(serverSockFd, &watchFds);

	while (1) {

		readFds = watchFds;
		switch (select(nfds, &readFds, NULL, NULL, (timeval *) 0)) {
		case -1:
			break;
		default:

			if (FD_ISSET(serverSockFd, &readFds)) {
				sockLen = sizeof(sockaddr_in);
				if ((clientSockFd = accept(serverSockFd,
						(sockaddr *) &clientSocket, &sockLen)) >= 0) {

					printf("open communication on ...\n");
					m_clientFds[m_num++] = clientSockFd;
					FD_SET(clientSockFd, &watchFds);
					nfds = (clientSockFd >= nfds ? clientSockFd + 1 : nfds);
				} else {

					printf("We should have something!\n");
				}
			}

			for (int i = 0; i < m_num; ++i) {

				if (FD_ISSET(m_clientFds[i], &readFds)) {

					ioctl(m_clientFds[i], FIONREAD, &nread);
					if (nread == 0)
						continue;
					read(m_clientFds[i], buf, nread);
					printf("Server: request from client: %s\n", buf);

					ClientResponse cliResponse;

					if( buf[0] == '0'){
						cliResponse.setError("no reason!");
						write(m_clientFds[i], cliResponse);
					}else if (buf[0] == '1'){
						cliResponse.setOk("yes ok!");
						write(m_clientFds[i], cliResponse);
					}else if (buf[0] == '2'){
						cliResponse.setSchema(Catalog::getInstance()->getTable(0)->getProjectoin(0)->getSchema());
						write(m_clientFds[i], cliResponse);
					}else{
						cliResponse.setOk("yes ok!");
						write(m_clientFds[i], cliResponse);
						cliResponse.setSchema(Catalog::getInstance()->getTable(0)->getProjectoin(0)->getSchema());
						write(m_clientFds[i], cliResponse);


						std::vector<Attribute> list = Catalog::getInstance()->getTable(0)->getProjectoin(0)->getAttributeList();
						ColumnHeader header;
						for(int i=0;i<list.size();++i){
							header.add_header(list[i].attrName);
						}
						cliResponse.setAttributeName(header);
						write(m_clientFds[i], cliResponse);
					}

				}
			}

			break;
		}
	}
}

int ClaimsServer::write(const int fd, const ClientResponse& res) const {

	int ret = 0;
	char *buffer;
	int length = res.serialize(buffer);
	ret = ::write(fd, buffer, length);
	printf("Server: %d bytes send!\n", ret);
	free(buffer);
	return ret;
}



