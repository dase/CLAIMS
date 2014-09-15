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
CPUResourceManager* CPUResourceManager::instance_=0;
CPUResourceManager::CPUResourceManager() {
	number_of_sockets_=numa_max_node()+1;

	for(unsigned i=0;i<number_of_sockets_;i++){
		sockets_.push_back(socket());
	}

	int number_of_cores=numa_num_configured_cpus();
//	number_of_cores=8;
	printf("cpus: %d\n",number_of_cores);
	for(int i=0;i<number_of_cores;i++){
		int socket_index=numa_node_of_cpu(i);
		sockets_[socket_index].addCore(i);
		sem_available_cores_.post();
	}
}

CPUResourceManager::~CPUResourceManager() {
	// TODO Auto-generated destructor stub
}

int CPUResourceManager::socket::getNumberOfFreeCores() {
	return sem_in_socket_available_cores.get_value();
//	return cores-busy_cores;
}

int CPUResourceManager::tryApplyCore() {
	if(sem_available_cores_.try_wait()){
		for(unsigned i=0;i<number_of_sockets_;i++){
			int core_id;
			core_id=sockets_[i].tryApplyCore();
			if(core_id>=0)
				return core_id;
		}
		/* code should not arrive here. */
		assert(false);
	}
	return -1;
}

int CPUResourceManager::applyCore() {
	sem_available_cores_.wait();
	for(unsigned i=0;i<number_of_sockets_;i++){
		int core_id;
		core_id=sockets_[i].tryApplyCore();
		if(core_id>=0)
			return core_id;
	}
	/* code should not arrive here. */
	assert(false);
	return -1;

}

CPUResourceManager::socket::socket(int number_of_cores):number_of_cores(number_of_cores) {
	sem_in_socket_available_cores.set_value(number_of_cores);
}

int CPUResourceManager::socket::tryApplyCore() {
	if(sem_in_socket_available_cores.try_wait()){
		for(int i=0;i<number_of_cores;i++){
			if(cores[i].available==true){
				cores[i].available=false;
				return cores[i].id;
			}
		}
		/* code should not arrive here */
		assert(false);
		return -1;
	}
	else
		return -1;
}

int CPUResourceManager::socket::applyCore() {
	sem_in_socket_available_cores.wait();
	for(int i=0;i<number_of_cores;i++){
		if(cores[i].available==true){
			cores[i].available=false;
			return cores[i].id;
		}
	}
	/* code should not arrive here */
	assert(false);
	return -1;
}


void CPUResourceManager::freeCore(int core_id) {
	if(core_id==-1){
	/* the caller does not specify the core id, which means that the
	 * current thread is bound to the core that we want to free.*/
		core_id=sched_getcpu();
	}
	int socket_index=numa_node_of_cpu(core_id);
	sockets_[socket_index].freeCore(core_id);
	sem_available_cores_.post();
}


CPUResourceManager::socket::socket():number_of_cores(0) {

}

void CPUResourceManager::socket::addCore(int core_id) {
	core core;
	core.available=true;
	core.id=core_id;
	core.offset=cores.size();
	cores.push_back(core);
	number_of_cores++;
	sem_in_socket_available_cores.post();
}

bool CPUResourceManager::socket::freeCore(int core_id) {
	for(int i=0;i<number_of_cores;i++){
		if(cores[i].id==core_id){
			cores[i].available=true;
			sem_in_socket_available_cores.post();
			return true;
		}
	}
	return false;

}

CPUResourceManager* CPUResourceManager::getInstance() {
	if(instance_==0){
		instance_=new CPUResourceManager();
	}
	return instance_;
}

void CPUResourceManager::print() {
	for(unsigned i=0;i<number_of_sockets_;i++){
		sockets_[i].print();
		printf("\n");
	}
}

void CPUResourceManager::socket::print() {
	for(unsigned i=0;i<number_of_cores;i++){
		if(cores[i].available)
			printf("1\t");
		else
			printf("0\t");
	}
}
