/*
 * Daemon.h
 *
 *  Created on: Feb 20, 2014
 *      Author: wangli
 */

#ifndef DAEMON_H_
#define DAEMON_H_
#include <pair>
#include <vector>
#include <string>
#include <list>
#include "../utility/lock.h"
struct remote_command{
	std::string cmd;
	int socket_fd;
};
class Daemon {
public:
	Daemon();
	virtual ~Daemon();

	static void worker(void*);
	std::list<remote_command> remote_command_queue_;
	semaphore semaphore_command_queue_;
};

#endif /* DAEMON_H_ */
