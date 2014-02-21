/*
 * Daemon.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: wangli
 */

#include "Daemon.h"
#include "../Parsetree/runparsetree.cpp"
Daemon::Daemon() {
	// TODO Auto-generated constructor stub

}

Daemon::~Daemon() {
	// TODO Auto-generated destructor stub
}

void Daemon::worker(void* para){
	Daemon* pthis=(Daemon*)para;
	while(true){
		pthis->semaphore_command_queue_.wait();
		remote_command rc=pthis->remote_command_queue_.front();
		pthis->remote_command_queue_.pop_front();
		Node* node=getparsetreeroot(rc.cmd.c_str());


//		pthis->;
	}
}
