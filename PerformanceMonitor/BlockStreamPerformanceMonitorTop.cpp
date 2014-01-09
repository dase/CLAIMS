/*
 * BlockStreamPerformanceMonitorTop.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: wangli
 */

#include "BlockStreamPerformanceMonitorTop.h"
//#include "hash.h"
#include "../rdtsc.h"

BlockStreamPerformanceMonitorTop::BlockStreamPerformanceMonitorTop(State state)
:state_(state){
	// TODO Auto-generated constructor stub

}

BlockStreamPerformanceMonitorTop::~BlockStreamPerformanceMonitorTop() {
	// TODO Auto-generated destructor stub
}

bool BlockStreamPerformanceMonitorTop::open(const PartitionOffset& partition_offset){
	start_=curtick();
	state_.child_->open(partition_offset);
	block_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	tuplecount_=0;
	int error;
	error=pthread_create(&report_tid_,NULL,report,this);
	if(error!=0){
		std::cout<<"create threads error!"<<std::endl;
	}

	return true;
}

bool BlockStreamPerformanceMonitorTop::next(BlockStreamBase*){
//	PartitionFunction* hash=PartitionFunctionFactory::createBoostHashFunction(4);
//	const int partition_index=3;
	block_->setEmpty();
	if(state_.child_->next(block_)){
		BlockStreamBase::BlockStreamTraverseIterator* it=block_->createIterator();
		while(it->nextTuple()){
//			tuplecount_++;
//			if(rand()%10000<3){
//				printf("partition value:%d",state_.schema_->getcolumn(partition_index).operate->ge)
//			}
		}
		tuplecount_+=block_->getTuplesInBlock();
		return true;
	}
	return false;
}

bool BlockStreamPerformanceMonitorTop::close(){
	pthread_cancel(report_tid_);
	double eclipsed_seconds=getSecond(start_);
	double processed_data_in_bytes=tuplecount_*state_.schema_->getTupleMaxSize();

	printf("Total time: %5.5f seconds\n",getSecond(start_));
	printf("Total tuples: %d\n",tuplecount_);
	printf("Avg throughput: %5.3f M data/s, %5.3f M tuples/s\n",processed_data_in_bytes/eclipsed_seconds/1024/1024,(float)tuplecount_/2014/1024/eclipsed_seconds);
	block_->~BlockStreamBase();
	state_.child_->close();
	return true;

}

void* BlockStreamPerformanceMonitorTop::report(void* arg){
	BlockStreamPerformanceMonitorTop* Pthis=(BlockStreamPerformanceMonitorTop*)arg;

	while(true){
		const unsigned long last_tuple_count=Pthis->tuplecount_;
		usleep(Pthis->state_.report_cycles_*1000);

		double eclipsed_seconds=getSecond(Pthis->start_);
		double processed_data_in_bytes=Pthis->tuplecount_*Pthis->state_.schema_->getTupleMaxSize();
		double processed_data_in_bytes_during_last_cycle=(Pthis->tuplecount_-last_tuple_count)*Pthis->state_.schema_->getTupleMaxSize();
		printf("[%2.3f s] Real Time: %5.3f M/s\tAVG: %5.3f M/s.\t%5.2f M tuples received.\n",eclipsed_seconds,processed_data_in_bytes_during_last_cycle/(Pthis->state_.report_cycles_/1000)/1024/1024,processed_data_in_bytes/eclipsed_seconds/1024/1024,(float)Pthis->tuplecount_/1024/1024);
	}
}
