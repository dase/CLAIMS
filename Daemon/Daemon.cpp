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
#define WORK_THREAD_COUNT 1

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

/*
void* Daemon::worker(void* para) {
	Daemon* pthis = (Daemon*) para;
	while (true) {

		remote_command rc = Daemon::getInstance()->getRemoteCommand();

		//assume all commands are sql commands.
		executed_result result;
		std::string error_info;
		Node* node;
		ResultSet* result_set = Executing::run_sql(std::string(rc.cmd),error_info);

		if(result_set==0){
			printf("-Worker: add error result!\n");
			result.error_info = error_info;
			result.result = 0;
			result.status = EXECUTED_RESULT_STATUS_ERROR;
			result.fd = rc.socket_fd;
		}
		else{
			printf("-Worker: add ok result!\n");
			result.status = EXECUTED_RESULT_STATUS_OK;
			result.result = result_set;
			result.fd = rc.socket_fd;
		}


		printf("-Worker add result into the queue!\n");
		pthis->addExecutedResult(result);

		//		pthis->;
	}
}
*/

void* Daemon::worker(void* para) {
	Daemon* pthis = (Daemon*) para;
	while (true) {

		remote_command rc = Daemon::getInstance()->getRemoteCommand();

		//assume all commands are sql commands.
		executed_result result;
		result.fd = rc.socket_fd;
		result.status = true;

		// must be delete after loading
//		rc.cmd = "load table field from \"/home/imdb/data/stock/field\" with '\t','\n';\n\n"
//				"load table area from \"/home/imdb/data/stock/area\" with '\t','\n';\n\n"
//				"load table trade from "
//				"\"/home/imdb/data/poc/CJ/CJ20100901.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100902.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100903.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100906.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100907.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100908.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100909.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100910.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100913.txt\","
//				"\"/home/imdb/data/poc/CJ/CJ20100914.txt\" with '|','\n';\n\n"
				;

		// result is a pointer, which now is NULL and should be assigned in function.

		ClientListener::checkFdValid(result.fd);

		Executing::run_sql(rc.cmd, result.result, result.status, result.error_info, result.info, result.fd);
		ClientLogging::log("after running sql, the result is : status-%d, err-%s, info-%s",
				result.status, result.error_info.c_str(), result.info.c_str());
		ClientListener::checkFdValid(result.fd);
		printf("-Worker add result into the queue!\n");
		pthis->addExecutedResult(result);

	}
	return NULL;
}

void Daemon::addRemoteCommand(const remote_command& rc) {
	lock_.acquire();
	remote_command_queue_.push_back(rc);
	lock_.release();
	semaphore_command_queue_.post();
//	cout<<"post command queue semaphore"<<endl;
}
remote_command Daemon::getRemoteCommand() {
	semaphore_command_queue_.wait();
//	cout<<"minus command queue semaphore "<<endl;
	remote_command ret;
	lock_.acquire();
	ret = remote_command_queue_.front();
	remote_command_queue_.pop_front();
	lock_.release();
	return ret;
}

executed_result Daemon::getExecutedResult() {
	semaphore_result_queue_.wait();
	//	assert(false);
	printf("-Worker: acquire the get semphore!\n");
	executed_result ret;
	lock_.acquire();
	ret = executed_result_queue_.front();
	executed_result_queue_.pop_front();
	lock_.release();
	printf("------ has been fucked!\n");
	return ret;
}
void Daemon::addExecutedResult(const executed_result& item) {
	lock_.acquire();
	executed_result_queue_.push_back(item);
	lock_.release();
	semaphore_result_queue_.post();
	printf("-----------------------------------Added! current size:%d\n",executed_result_queue_.size());

}
