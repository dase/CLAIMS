/*
 * Daemon.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: wangli
 */

#include "Daemon.h"
#include "../Parsetree/runparsetree.cpp"
#include "Executing.h"
#include <pthread.h>
#define WORK_THREAD_COUNT 5

Daemon* Daemon::instance_ = 0;

Daemon* Daemon::getInstance(){
	if( instance_ == 0 ){
		instance_ = new Daemon();
	}

	return instance_;
}

Daemon::Daemon() {
	// TODO Auto-generated constructor stub
	//TODO: create work threads
	for (unsigned i = 0; i < WORK_THREAD_COUNT; i++) {
		pthread_t tid;
		const int error = pthread_create(&tid, NULL, worker, this);
		if (error != 0) {
			std::cout << "cannot create thread!" << std::endl;
			return;
		}
	}
}

Daemon::~Daemon() {
	// TODO Auto-generated destructor stub
}

void* Daemon::worker(void* para) {
	Daemon* pthis = (Daemon*) para;
	while (true) {

		remote_command rc = Daemon::getInstance()->getRemoteCommand();

		//assume all commands are sql commands.
		executed_result result;
		Node* node;
		if ((node = getparsetreeroot(rc.cmd.c_str())) == 0) {
			result.error_info = "SQL Parser Error!";
			result.result = 0;
			result.status = EXECUTED_RESULT_STATUS_ERROR;
			result.fd = rc.socket_fd;
		} else {
			ResultSet* result_set = Executing::run_sql(std::string(rc.cmd));
			result.status = EXECUTED_RESULT_STATUS_OK;
			result.result = result_set;
			result.fd = rc.socket_fd;
		}
		pthis->addExecutedResult(result);

//		pthis->;
	}
}
void Daemon::addRemoteCommand(const remote_command& rc) {
	lock_.acquire();
	remote_command_queue_.push_back(rc);
	lock_.release();
	semaphore_command_queue_.post();
}
remote_command Daemon::getRemoteCommand() {
	semaphore_command_queue_.wait();
	remote_command ret;
	lock_.acquire();
	ret = remote_command_queue_.front();
	remote_command_queue_.pop_front();
	lock_.release();
	return ret;
}

executed_result Daemon::getExecutedResult() {
	semaphore_result_queue_.wait();
	executed_result ret;
	lock_.acquire();
	ret = executed_result_queue_.front();
	executed_result_queue_.pop_front();
	lock_.release();
	return ret;
}
void Daemon::addExecutedResult(const executed_result& item) {
	lock_.acquire();
	executed_result_queue_.push_back(item);
	lock_.release();
	semaphore_result_queue_.post();
}
