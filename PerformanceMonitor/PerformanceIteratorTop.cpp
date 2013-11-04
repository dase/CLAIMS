/*
 * PerformanceIteratorTop.cpp
 *
 *  Created on: Aug 14, 2013
 *      Author: wangli
 */

#include "PerformanceIteratorTop.h"
#include <malloc.h>

#include <iostream>
#include "../configure.h"
#include "../rdtsc.h"
PerformanceIteratorTop::PerformanceIteratorTop(PerformanceIteratorTop::State state)
:state(state),tuplecount(0){
	// TODO Auto-generated constructor stub

}

PerformanceIteratorTop::~PerformanceIteratorTop() {
	// TODO Auto-generated destructor stub
}

bool PerformanceIteratorTop::open(){

	state.child->open();
	tuple=memalign(cacheline_size,state.schema->getTupleMaxSize());
	tuplecount=0;
	int error;
	error=pthread_create(&report_tid,NULL,report,this);
	if(error!=0){
		std::cout<<"create threads error!"<<std::endl;
	}
	start=curtick();
	return true;
}

bool PerformanceIteratorTop::close(){
	printf("Total time: %5.5f seconds\n",getSecond(start));
	pthread_cancel(report_tid);
	free(tuple);
	state.child->close();
	return true;
}
void* PerformanceIteratorTop::report(void* arg){
	PerformanceIteratorTop* Pthis=(PerformanceIteratorTop*)arg;
	while(true){
		usleep(Pthis->state.report_cycles*1000);

		double eclipsed_seconds=getSecond(Pthis->start);
		double processed_data_in_bytes=Pthis->tuplecount*Pthis->state.schema->getTupleMaxSize();
		printf("[Performace reporting:] %8.3f M/s.   %5.2f M tuples received.\n",processed_data_in_bytes/eclipsed_seconds/1024/1024,(float)Pthis->tuplecount/2014/1024);
	}
}

