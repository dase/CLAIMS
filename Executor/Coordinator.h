/*
 * Coordinator.h
 * The Coordinator is the center for Theron connections.
 *  Created on: Aug 8, 2013
 *      Author: wangli
 */

#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include <Theron/Theron.h>
#include <string>
#include <vector>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/Logging.h"

class Coordinator {
public:
	Coordinator();
	virtual ~Coordinator();
private:
	bool PrepareTheSocket();
	bool SetupTheTheron();
	bool CreateListeningThread();
	bool SendReadyNotificationToNewNode(int socket_new_node);

	bool SendCoordinatorEndPointPort(int socket_new_node);
	static void* ListeningNewNode(void * arg);
private:
	int socket_fd;
	Theron::EndPoint* endpoint;
	Theron::Framework* framework;
	std::string EndPointPort;
	std::vector<std::pair<std::string,std::string> > PeersIpPort;
	Logging* logging;
	pthread_t prochaseId;
/**
 * Actor
 */
///////////////////////////////////////////////////////////////////////
public:
	class CoordinateActor:public Theron::Actor{
	public:
		CoordinateActor(Theron::Framework *framework, const char* Name);
	};
};

#endif /* COORDINATOR_H_ */
