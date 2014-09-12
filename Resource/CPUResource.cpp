/*
 * CPUResource.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: wangli
 */

#include "CPUResource.h"
#include <numa.h>
#include <stdio.h>
#include <assert.h>
#include <sched.h>

CPUResource::CPUResource() {
	number_of_sockets_=numa_max_node()+1;
	int number_of_cpus=numa_num_configured_cpus();
	for(int i=0;i<number_of_sockets_;i++){
		socket sock;
		sock.busy_cores=0;
		sock.cores=number_of_cpus/number_of_sockets_;
		sockets_.push_back(sock);
	}
}

CPUResource::~CPUResource() {
	// TODO Auto-generated destructor stub
}

int CPUResource::socket::getNumberOfFreeCores() {
	return cores-busy_cores;
}

bool CPUResource::applyCore() {
	for(unsigned i=0;i<number_of_sockets_;i++){
		if(sockets_[i].applyCore()){
			return true;
		}
	}
	return false;
}

bool CPUResource::socket::applyCore() {
	if(getNumberOfFreeCores()==0)
		return false;
	busy_cores++;
	return true;
}

void CPUResource::freeCore(int socket_index) {
	assert(socket_index<number_of_sockets_);
	/* the caller does not specify the socket*/
	if(socket_index==-1){
		int cpu=sched_getcpu();
		socket_index=numa_node_of_cpu(cpu);
	}
	sockets_[socket_index].busy_cores-1;
}
