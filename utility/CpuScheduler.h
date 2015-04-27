/*
 * CpuScheduler.h
 *
 *  Created on: Apr 27, 2015
 *      Author: wangli
 */

#ifndef UTILITY_CPUSCHEDULER_H_
#define UTILITY_CPUSCHEDULER_H_
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <numa.h>

/* get number of sockets on this machine */
static int getNumberOfSockets(){
	return numa_max_node();
}

/* get the total number of cores */
static int getNumberOfCpus(){
	return  sysconf(_SC_NPROCESSORS_CONF);
}

/* get the id of the cpu running the current thread */
static int getCurrentCpuAffility(){
	cpu_set_t mask;
	if(sched_getaffinity(pthread_self(),sizeof(mask),&mask)<0)
		return -1;
	for(int i=0;i<getNumberOfCpus();i++){
		 if (CPU_ISSET(i, &mask)) {
			 return i;
		}
	}
	return -1;
}

/* bind the current thread on the specified cpu*/
static bool setCpuAffility(int cpu){
	cpu_set_t mask;
	CPU_SET(cpu,&mask);
	if(sched_setaffinity(pthread_self(),sizeof(mask),&mask)<0)
		return false;
	return true;
}

/* get the NUMA socket index of the specified thread */
static int getSocketAffility(int cpu){
	return numa_node_of_cpu(cpu);
}

/* get the NUMA socket index of the current thread */
static int getCurrentSocketAffility(){
	int current_cpu=getCurrentCpuAffility();
	return getSocketAffility(current_cpu);
}

#endif /* UTILITY_CPUSCHEDULER_H_ */
