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
#include "../Daemon/Daemon.h"
ClientListener::ClientListener(int port) {

	m_num = 0;
	m_clientFds = new int[MAXCONN];
	for (int i = 0; i < MAXCONN; ++i) {
		m_clientFds[i] = -1;
	}
	m_fd = -1;
	m_port = port;

}

ClientListener::~ClientListener() {

	delete m_clientFds;
}

int ClientListener::addClient(const int fd) {

	if (m_num == MAXCONN) {

		perror("Server has the maximum connection");
		return -1;
	}

	for (int i = 0; i < MAXCONN; ++i) {
		if (m_clientFds[i] < 0) {
			m_clientFds[i] = fd;
			++m_num;
			break;
		}
	}
	return 0;
}

int ClientListener::removeClient(const int fd) {

	int ret = -1;
	for (int i = 0; i < MAXCONN; ++i) {
		if (m_clientFds[i] == fd) {
			m_clientFds[i] = -1;
			--m_num;
			ret = 0;
			break;
		}
	}
	return ret;
}

int ClientListener::receiveRequest(const int fd, const char *cmd) {

	remote_command rcmd;
	rcmd.socket_fd = fd;
	rcmd.cmd.append(cmd);

	Daemon::getInstance()->addRemoteCommand(rcmd);
	return 0;
}

/**
 * configure the socket parameter, bind to the specific ip/port
 * startup the send/receive threads
 */
void ClientListener::configure() {

	int &serverSockFd = m_fd;
	int clientSockFd;
	unsigned int nread;
	char *buf = new char[128];
	sockaddr_in serverSocket;
	sockaddr_in clientSocket;

	serverSocket.sin_family = AF_INET;
	serverSocket.sin_port = htons(m_port);
	serverSocket.sin_addr.s_addr = INADDR_ANY;

	if ((serverSockFd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket error");
		return;
	}

	/* Enable address reuse */
	int on=1;
	setsockopt(serverSockFd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));
	if (bind(serverSockFd, (sockaddr *) &serverSocket, sizeof(serverSocket))
			< 0) {
		perror("Server:: bind error!");
		return;
	}


	int backLog = MAXCONN - 1;
	if (listen(serverSockFd, backLog) == -1) {
		printf("Server::listen error!\n");
		return;
	} else {
		printf("Server::listen ok!\n");
	}

	pthread_t t_Receiver;
	pthread_t t_Sender;

	const int error1 = pthread_create(&t_Receiver, NULL, receiveHandler, this);
	if (error1 != 0) {
		std::cout << "cannot create receive thread!" << std::endl;
	}

	const int error2 = pthread_create(&t_Sender, NULL, sendHandler, this);
	if (error2 != 0) {
		std::cout << "cannot create send thread!" << std::endl;
	}

	printf("sender thread id=%x\n",t_Sender);

}

void* ClientListener::receiveHandler(void *para) {

	printf("-Init receive handler.\n");
	ClientListener *server = (ClientListener *) para;
	int &serverSockFd = server->m_fd;
	int clientSockFd;

	char *buf = new char[1024];
	memset(buf, 0, sizeof(buf));

	unsigned int sockLen;
	int nread;

	int nfds = serverSockFd + 1;
	fd_set watchFds;
	fd_set readFds;

	FD_ZERO(&watchFds);
	FD_SET(serverSockFd, &watchFds);

	bool stop = false;
	while (!stop) {
		usleep(1);
		readFds = watchFds;
		switch (select(nfds, &readFds, NULL, NULL, (timeval *) 0)) {
		case -1:
			break;
		case 0:
			break;
		default:

			if (FD_ISSET(serverSockFd, &readFds)) {
				//ADD Client
				sockaddr_in clientSocket;
				sockLen = sizeof(sockaddr_in);

				if (!server->isFull()) {
					if ((clientSockFd = accept(serverSockFd,
							(sockaddr *) &clientSocket, &sockLen)) >= 0) {

						printf("open communication with client, %d\n",
								clientSockFd);
						//						server->m_clientFds[server->m_num++] = clientSockFd;
						server->addClient(clientSockFd);
						FD_SET(clientSockFd, &watchFds);
						nfds = (clientSockFd >= nfds ? clientSockFd + 1 : nfds);
					} else {
						printf("Accept Error!\n");
					}
				}
			}
			for (int i = 0; i < MAXCONN; ++i) {

				if (server->m_clientFds[i] < 0)
					continue;

				if (FD_ISSET(server->m_clientFds[i], &readFds)) {

					ioctl(server->m_clientFds[i], FIONREAD, &nread);
					if (0 == nread) {
						//TODO does here means a client close the connection with server?
						printf("close connection on socket %d!\n", server->m_clientFds[i]);
						FD_CLR(server->m_clientFds[i], &watchFds);
						::close(server->m_clientFds[i]);
						server->removeClient(server->m_clientFds[i]);
						continue;
					}

//					cout<<"nread:"<<nread<<endl;
					memset(buf, 0, sizeof(buf));
					int read_count = read(server->m_clientFds[i], buf, nread);
					buf[read_count] = '\0';	// fix a bug
					cout<<"buf: "<<buf<<endl;

					int sql_type = buf[0]-48;	// '1' - 48 = 1
					ClientLogging::log("sql_type is %d", sql_type);

					generateSqlStmt(sql_type, buf);
//					strcpy(buf, "select row_id from trade limit 100;\0");

					int retCode = server->receiveRequest(server->m_clientFds[i], buf);
					if (0 == retCode) {
						printf("Successfully receive query %s from client %d.\n",
								buf,
								server->m_clientFds[i]);
					}
					//					else if (-1 == retCode) {
					//						server->shutdown();
					//						stop = true;
					//						printf("Shut Down!\n");
					//					} else {
					//						printf("Failed query from client %d.\n",
					//								server->m_clientFds[i]);
					//					}
				}
			}
			break;
		} /*end of switch*/
	}
	FD_ZERO(&watchFds);
	FD_ZERO(&readFds);
	delete buf;

	return NULL;
}

void ClientListener::generateSqlStmt(int type, char *buf) {
	ClientLogging::log("in generateSqlStmt function:type argument is %d, buf argument is %s", type, buf);
	// the first byte of buf is type, other are argument
	string arg(buf+1);
	cout<<"arg is :"<<arg<<endl;
	switch(type) {
	case 1: {
		string sql = "select avg(Trade_Price), substr(Trade_Date,0,7) as months from trade where Sec_Code = \'";
		sql += arg;
		sql += "\' group by substr(Trade_Date,0,7)  order by months; ";
		ClientLogging::log("the whole sql string is: %s",sql.c_str());
		memset(buf, 0, sizeof(buf));
		strcpy(buf, sql.c_str());
		ClientLogging::log("sql buf is %s\n", buf);
	}
	break;
	default: {
		ClientLogging::elog("No supported");
	}
	}
}


void *ClientListener::sendHandler(void *para) {

	printf("-Init send handler!\n");
	ClientListener *server = (ClientListener*) para;

	ClientResponse cliRes;
	while (true) {
		usleep(1);
		printf("-SendHandler: wait for result!\n");
		executed_result result = Daemon::getInstance()->getExecutedResult();
		printf("-SendHandler: get executed_result for %d\n", result.fd);

		if (result.status == true) {
			//OK
			if (result.result == NULL) {
				// DDL return true
				cliRes.setChange(result.info);
				ClientLogging::log("to send change response-- status:%d  length:%d  content:%s",
								cliRes.status, cliRes.length, cliRes.content.c_str());
				server->write(result.fd, cliRes);
			}
			else {
				sendJsonPacket(cliRes, result);
				server->write(result.fd, cliRes);
				/*
				// query return true
				cliRes.setOk("Yes Ok");
				ClientLogging::log("to send ok response-- status:%d  length:%d  content:%s",
								cliRes.status, cliRes.length, cliRes.content.c_str());
				server->write(result.fd, cliRes);

				cliRes.setSchema(result.result->schema_);
				ClientLogging::log("to send schema response-- status:%d  length:%d  content:%s",
								cliRes.status, cliRes.length, cliRes.content.c_str());
				server->write(result.fd, cliRes);

				std::vector<std::string> list = result.result->column_header_list_;
				ColumnHeader header;
				for (int i = 0; i < list.size(); ++i) {
					header.add_header(list[i]);
				}
				cliRes.setAttributeName(header);
				ClientLogging::log("to send attr response-- status:%d  length:%d  content:%s",
								cliRes.status, cliRes.length, cliRes.content.c_str());
				server->write(result.fd, cliRes);


				ResultSet::Iterator it = result.result->createIterator();
				BlockStreamBase* block;

				Block serialzed_block(64*1024);

				while (block = (BlockStreamBase*) it.atomicNextBlock()) {
					block->serialize(serialzed_block);
					cliRes.setDataBlock(serialzed_block);
					ClientLogging::log("to send data response-- status:%d  length:%d  content:%s",
									cliRes.status, cliRes.length, cliRes.content.c_str());
					server->write(result.fd, cliRes);
				}

				cliRes.setEnd(result.result->query_time_);
				ClientLogging::log("to send end response-- status:%d  length:%d  content:%s",
								cliRes.status, cliRes.length, cliRes.content.c_str());
				server->write(result.fd, cliRes);
			*/
			}
		} else {
			//ERROR
			cliRes.setError(result.error_info);
			ClientLogging::log("to send err response-- status:%d  length:%d  content:%s",
							cliRes.status, cliRes.length, cliRes.content.c_str());
			server->write(result.fd, cliRes);
		}
	}
	return NULL;
}

void ClientListener::sendJsonPacket(ClientResponse &cr, executed_result &res) {
//	ClientResponse cr;
	if (res.status) {
		if (res.result != NULL) {
			ResultSet *rs = res.result;
			vector<string> &col_name_list = rs->column_header_list_;
			Json::Value root;
			Json::Value jv_temp;

			DynamicBlockBuffer::Iterator it=rs->createIterator();
			BlockStreamBase* block;
			while(block=it.nextBlock()){
				BlockStreamBase::BlockStreamTraverseIterator* block_it=block->createIterator();
				void* tuple;
				while(tuple=block_it->nextTuple()){
//					rs->schema_->displayTuple(tuple,"\t");
					for (int i = 0; i < rs->schema_->getncolumns(); ++i) {
						jv_temp[col_name_list[i]] = Json::Value(rs->schema_->getColumnValue(tuple, i));
					}
					root.append(jv_temp);
				}
			}

//			Json::StyledWriter sw;
//			cout<<sw.write(root)<<endl;

			Json::FastWriter fw;
			string buf_to_send = fw.write(root);
			cout<<buf_to_send<<endl;

			cr.setData(buf_to_send);
		}
	}
}

/**
 * Thread shut down the server when it receives "shut down" command from a client.
 */
//void ClaimsServer::run() {
//
//	int &serverSockFd = m_fd;
//	int clientSockFd;
//	unsigned int nread;
//	char *buf = new char[128];
//	sockaddr_in serverSocket;
//	sockaddr_in clientSocket;
//
//	serverSocket.sin_family = AF_INET;
//	serverSocket.sin_port = htons(8000);
//	serverSocket.sin_addr.s_addr = INADDR_ANY;
//
//	if ((serverSockFd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//		printf("socket error");
//		return;
//	}
//
//	if (bind(serverSockFd, (sockaddr *) &serverSocket, sizeof(serverSocket))
//			< 0) {
//		perror("Server:: bind error!");
//		return;
//	}
//
//	int backLog = MAXCONN - 1;
//	if (listen(serverSockFd, backLog) == -1) {
//		printf("Server::listen error!\n");
//		return;
//	} else {
//		printf("Server::listen ok!\n");
//	}
//
//	int nfds = serverSockFd + 1;
//	unsigned int sockLen;
//	fd_set watchFds;
//	fd_set readFds;
//
//	FD_ZERO(&watchFds);
//	FD_SET(serverSockFd, &watchFds);
//
//	bool stop = false;
//	while (!stop) {
//
//		readFds = watchFds;
//		switch (select(nfds, &readFds, NULL, NULL, (timeval *) 0)) {
//		case -1:
//			break;
//		case 0:
//			break;
//		default:
//
//			if (FD_ISSET(serverSockFd, &readFds)) {
//				//ADD Client
//				sockLen = sizeof(sockaddr_in);
//
//				if (!isFull()) {
//					if ((clientSockFd = accept(serverSockFd,
//							(sockaddr *) &clientSocket, &sockLen)) >= 0) {
//
//						printf("open communication with client, %d\n",
//								clientSockFd);
//						m_clientFds[m_num++] = clientSockFd;
//						FD_SET(clientSockFd, &watchFds);
//						nfds = (clientSockFd >= nfds ? clientSockFd + 1 : nfds);
//					} else {
//						printf("Accept Error!\n");
//					}
//				}
//			}
//			for (int i = 0; i < MAXCONN; ++i) {
//
//				if (m_clientFds[i] < 0)
//					continue;
//
//				if (FD_ISSET(m_clientFds[i], &readFds)) {
//
//					ioctl(m_clientFds[i], FIONREAD, &nread);
//					if (0 == nread) {
//						//TODO does here means a client close the connection with server?
//						printf("retained!!!\n");
//						FD_CLR(m_clientFds[i], &watchFds);
////						::shutdown(m_clientFds[i], 2);
//						::close(m_clientFds[i]);
//
//						m_clientFds[i] = -1;
//						continue;
//					}
//					read(m_clientFds[i], buf, nread);
//
//					int retCode = answerRequest(m_clientFds[i], buf);
//					if (1 == retCode) {
//						printf("Successfully answer query from client %d.\n",
//								m_clientFds[i]);
//					} else if (-1 == retCode) {
//						shutdown();
//						stop = true;
//						printf("Shut Down!\n");
//					} else {
//						printf("Failed query from client %d.\n",
//								m_clientFds[i]);
//					}
//
//				}
//			}
//			break;
//		} /*end of switch*/
//	}
//	FD_ZERO(&watchFds);
//	FD_ZERO(&readFds);
//	delete buf;
//}

/**
 * shut down the server
 */
void ClientListener::shutdown() {

	for (int i = 0; i < MAXCONN; ++i) {
		if (m_clientFds[i] > 0)
			close(m_clientFds[i]);
	}
	close(m_fd);
}

int ClientListener::write(const int fd, const ClientResponse& res) const {

	int ret = 0;
	char *buffer;
	int length = res.serialize(buffer);
	//	ret = ::write(fd, buffer, length);
	ret = send(fd,buffer,length,MSG_WAITALL);
	printf("Server: %d bytes:%d\t%d\t%s is send!\n", ret, res.status, res.length, res.content.c_str());
	free(buffer);
	return ret;
}

