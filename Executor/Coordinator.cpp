/*
 * Coordinator.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: wangli
 */

#include "Coordinator.h"
#include <libconfig.h++>
#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <arpa/inet.h>
#include <stdio.h>
#include "../Debug.h"
#include "../Message.h"
#include "../Environment.h"
#include "../TimeOutReceiver.h"
#include "../Config.h"
Coordinator::Coordinator() {
	logging = new CoordinatorLogging();
	/** swap the order of SetupTheTheron and PreparetheSocket to provide more time
	 *  to Theron::EndPoint initialization before other EndPoints connect to it.
	 *
	 */

	if (SetupTheTheron() == false) {
		logging->elog("Error occurs during the Theron setup!");
		return;
	}
	if (PrepareTheSocket() == false) {
		logging->elog("Error occurs during the socket prepare!");
		return;
	}

	if (CreateListeningThread() == false) {
		logging->elog("Error occurs during creating listening thread!");
		return;
	}

}

Coordinator::~Coordinator() {
	pthread_cancel(prochaseId);

	framework->~Framework();
	endpoint->~EndPoint();
}
bool Coordinator::PrepareTheSocket() {
	libconfig::Config cfg;
	cfg.readFile(Config::config_file.c_str());
	std::string master_ip = (const char *) cfg.lookup("coordinator.ip");
	std::string master_port = (const char*) cfg.lookup("coordinator.port");

	struct hostent* host;
	struct sockaddr_in my_addr;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		logging->elog("socket create errors!\n");
		return false;
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(master_port.c_str()));
	my_addr.sin_addr.s_addr = INADDR_ANY;

	bzero(&(my_addr.sin_zero), 8);

	/* Enable address reuse */
	int on = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	if (bind(socket_fd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
			== -1) {
		logging->elog("bind errors!\n");
		return false;
	}

	if (listen(socket_fd, 100) == -1) {
		logging->elog("listen errors!\n");
		return false;
	}

	logging->log("Coordinator: PrepareTheSocket() is successful.");

	return true;
}
bool Coordinator::SetupTheTheron() {

	std::string ip_port;
	std::string ip = Environment::getInstance()->getIp();
	std::ostringstream port;
	port << PortManager::getInstance()->applyPort();
	EndPointPort = port.str();
	ip_port = std::string("tcp://") + ip + ":" + port.str();

	logging->log("[Coordinator]: Now is initializing the Theron EndPoint as %s",ip_port.c_str());

	endpoint = new Theron::EndPoint((ip + ":" + port.str()).c_str(),
			ip_port.c_str());

	framework = new Theron::Framework(*endpoint);
	framework->SetMaxThreads(1);

	logging->log("[Coordinator]: Now is initializing The CoordinatorActor");

	return true;

}
bool Coordinator::CreateListeningThread() {

	const int error = pthread_create(&prochaseId, NULL, ListeningNewNode, this);

	logging->log("[Coordinator]: The listening thread is created!");

	return error == 0;
}

void* Coordinator::ListeningNewNode(void *arg) {

	Coordinator* Cthis = (Coordinator*) arg;
	while (true) {
		socklen_t sin_size = sizeof(struct sockaddr_in);
		struct sockaddr_in remote_addr;
		int socket_fd_new;

		Cthis->logging->log(
				"[Coordinator]: I'm waiting for the new node's connection!");

		if ((socket_fd_new = accept(Cthis->socket_fd,
				(struct sockaddr*) &remote_addr, &sin_size)) == -1) {
			perror("accept errors!\n");
			return false;
		}

		Cthis->logging->log(
				"[Coordinator]: New Node is connected! Waiting for the port infor..");

		/*Set the timeout value, which is essential to guarantee the correctness when
		 * there are more nodes trying to connect the coordinate at the same time.  */
		struct timeval timeout = { 1, 0 };
		setsockopt(socket_fd_new, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout,
				sizeof(struct timeval));

		int recvbytes;
		int port;

		if ((recvbytes = recv(socket_fd_new, &port, sizeof(port), MSG_WAITALL))
				== -1) {
			std::cout << "New node " << inet_ntoa(remote_addr.sin_addr)
					<< " has connected, but the receiving the information times out!"
					<< std::endl;
			close(socket_fd_new);
			continue;
			//return false;
		}
		if (recvbytes != sizeof(int)) {
			std::cout << "Information received, but the length is not right!"
					<< std::endl;
			close(socket_fd_new);
			continue;
		}

		Cthis->logging->log(
				"[Coordinator]: The EndPoint of the new node is %s:%d",
				inet_ntoa(remote_addr.sin_addr), port);

		if (!Cthis->SendCoordinatorEndPointPort(socket_fd_new)) {
			continue;
		}

		std::ostringstream ost;
		ost << port;
		std::string new_node_ip = inet_ntoa(remote_addr.sin_addr);
		std::string new_node_port = ost.str();

		if (!Cthis->endpoint->Connect(("tcp://" + new_node_ip + ":"
				+ new_node_port).c_str())) {
			Cthis->logging->elog(
					"Error occurs when the Coordinator EndPoint is connecting to the EndPoint of the new node: "
					"tcp://%s:%s", new_node_ip.c_str(), new_node_port.c_str());
			Cthis->logging->log(" tcp://%s:%s", new_node_ip.c_str(),
					new_node_port.c_str());
		}

		else
		{
		Cthis->logging->log(
				"[Coordinator]: The Coordinator EndPoint has successfully connected to the EndPoint of the new node!");
		}

		TimeOutReceiver *receiver = new TimeOutReceiver(Cthis->endpoint);
		Theron::Catcher<int> resultCatcher;
		receiver->RegisterHandler(&resultCatcher, &Theron::Catcher<int>::Push);
		const int TimeOut = 1000;//ms

		/**
		 * TODO: In the current version, the Coordinator will repeatedly send
		 * message to each NodeConnectionActor until the feedback is received
		 * which means the target node has conducted new connection based on
		 * message received.
		 * However, if the target node is dead, the message will be sent repeatedly
		 * and infinitely. Additional code is needed to handle the dead node.
		 */
		for (unsigned i = 0; i < Cthis->PeersIpPort.size(); i++) {
			NodeConnectionMessage new_NCM(new_node_ip, new_node_port);
			NodeConnectionMessage old_NCM(Cthis->PeersIpPort[i].first,
					Cthis->PeersIpPort[i].second);
			receiver->Reset();
			Cthis->framework->Send(NodeConnectionMessage::serialize(new_NCM),
					receiver->GetAddress(), Theron::Address(
							("ConnectionActor://" + old_NCM.ip + ":"
									+ old_NCM.port).c_str()));
			while (receiver->TimeOutWait(1, TimeOut) != 1) {
				Cthis->framework->Send(
						NodeConnectionMessage::serialize(new_NCM),
						receiver->GetAddress(), Theron::Address(
								("ConnectionActor://" + old_NCM.ip + ":"
										+ old_NCM.port).c_str()));
			}

			receiver->Reset();

			Cthis->framework->Send(NodeConnectionMessage::serialize(old_NCM),
					receiver->GetAddress(), Theron::Address(
							("ConnectionActor://" + new_NCM.ip + ":"
									+ new_NCM.port).c_str()));
			while (receiver->TimeOutWait(1, TimeOut) != 1) {
				Cthis->framework->Send(
						NodeConnectionMessage::serialize(old_NCM),
						receiver->GetAddress(), Theron::Address(
								("ConnectionActor://" + new_NCM.ip + ":"
										+ new_NCM.port).c_str()));
			}
		}

		Cthis->PeersIpPort.push_back(std::pair<std::string, std::string>(
				new_node_ip, new_node_port));

		Cthis->logging->log(
				"[Coordinator]: New node %s;%s is successfully added to the Theron communication network!",
				new_node_ip.c_str(), new_node_port.c_str());

		Cthis->SendReadyNotificationToNewNode(socket_fd_new);

		close(socket_fd_new);
		receiver->~TimeOutReceiver();
	}
}

bool Coordinator::SendReadyNotificationToNewNode(int socket_new_node) {
	char signal = 'R';
	if (!send(socket_new_node, &signal, sizeof(char), 0)) {
		logging->log(
				"Error occurs when sending the Coordinate EndPoint port to the new node!");
	}

	logging->log("[Coordinator]: The 'Ready' signal is sent to the new node!");

	return true;
}
bool Coordinator::SendCoordinatorEndPointPort(int socket_new_node) {
	int port = atoi(EndPointPort.c_str());

	if (!send(socket_new_node, &port, sizeof(int), 0)) {
		logging->elog(
				"Error occurs when sending the Coordinate EndPoint port to the new node!");
	}

	logging->log(
			"[Coordinator]: The Coordinator EndPoint port [%d] is sent to the new node!",
			port);

	return true;
}
Coordinator::CoordinateActor::CoordinateActor(Theron::Framework *framework,
		const char* Name) :
	Actor(*framework, Name) {

}

