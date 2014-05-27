/*
 * BlockStreamPerformanceTest.cpp
 *
 *  Created on: 2013年10月19日
 *      Author: SCDONG
 */

#include "BlockStreamPerformanceTest.h"
#include "../utility/rdtsc.h"

#include <fstream>
using namespace std;

BlockStreamPerformanceTest::BlockStreamPerformanceTest(State state)
:state_(state){
	// TODO Auto-generated constructor stub

}

BlockStreamPerformanceTest::~BlockStreamPerformanceTest() {
	// TODO Auto-generated destructor stub
}

bool BlockStreamPerformanceTest::open(const PartitionOffset& partition_offset){
	state_.child1_->open(partition_offset);
	state_.child2_->open(partition_offset);
	block_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	tuplecount_=0;
	tuplecount1_ = 0;
	tuplecount2_ = 0;
	int error;
	error=pthread_create(&report_tid_,NULL,report,this);
	if(error!=0){
		std::cout<<"create threads error!"<<std::endl;
	}
	start_=curtick();
	return true;
}

bool BlockStreamPerformanceTest::next(BlockStreamBase*){

	block_->setEmpty();
	if(state_.child1_->next(block_)){
		BlockStreamBase::BlockStreamTraverseIterator* it=block_->createIterator();
		void* data;
//		printf("before while!<<<<</n");
//		int count=0;
		while(data=it->nextTuple()){
			tuplecount1_++;
			tuplecount_++;
			if(*(int*)data==-1){
				tuplecount1_++;
			}
//			printf("%d\n",count++);
		}
		return true;
	}
	else if(state_.child2_->next(block_)) {
		BlockStreamBase::BlockStreamTraverseIterator* it = block_->createIterator();
		while(it->nextTuple()) {
			tuplecount2_++;
			tuplecount_++;
		}
		return true;
	}
	cout << "performance test return false!\n";
	return false;
}

bool BlockStreamPerformanceTest::close(){
	pthread_cancel(report_tid_);
	double eclipsed_seconds=getSecond(start_);
	double processed_data_in_bytes=tuplecount_*state_.schema_->getTupleMaxSize();

	printf("Total time: %5.5f seconds\n",getSecond(start_));
	printf("Total tuples: %d\n",tuplecount_);
	printf("Avg throughput: %5.3f M data/s, %5.3f M tuples/s\n",processed_data_in_bytes/eclipsed_seconds/1024/1024,(float)tuplecount_/1024/1024/eclipsed_seconds);

	printf("Scan tuples: %d\t Random tuples: %d\n", tuplecount1_, tuplecount2_);

	ofstream outFile("/home/imdb/mount/dsc/disk_result.dat", ios::app);
	if (!outFile)
	{
		cout << "Can not open file!\n";
	}
	else
	{
		outFile << "Total time: " << getSecond(start_) << endl;
		outFile << "Total tuples: " << tuplecount_ << endl;
		outFile << "Avg throught: " << processed_data_in_bytes/eclipsed_seconds/1024/1024 << " M data/s, " << (float)tuplecount_/1024/1024/eclipsed_seconds << " M tuples/s\n";

		outFile << "columns_num: " << (unsigned long int)1024*1024*1024/tuplecount_ << "\t random tuples: " << tuplecount2_ << endl << getSecond(start_) << endl << processed_data_in_bytes/eclipsed_seconds/1024/1024 << endl << (float)tuplecount_/1024/1024/eclipsed_seconds*1024*1.024 << endl << endl << endl;
	}

	block_->~BlockStreamBase();
	state_.child1_->close();
	state_.child2_->close();
	return true;

}

void* BlockStreamPerformanceTest::report(void* arg){
	BlockStreamPerformanceTest* Pthis=(BlockStreamPerformanceTest*)arg;

	while(true){
		const unsigned long last_tuple_count=Pthis->tuplecount_;
		usleep(Pthis->state_.report_cycles_*1000);

		double eclipsed_seconds=getSecond(Pthis->start_);
		double processed_data_in_bytes=Pthis->tuplecount_*Pthis->state_.schema_->getTupleMaxSize();
		double processed_data_in_bytes_during_last_cycle=(Pthis->tuplecount_-last_tuple_count)*Pthis->state_.schema_->getTupleMaxSize();
		printf("[%2.3f s] Real Time: %5.3f M/s\tAVG: %5.3f M/s.\t%5.3f M tuples received.\n",eclipsed_seconds,processed_data_in_bytes_during_last_cycle/(Pthis->state_.report_cycles_/1000)/1024/1024,processed_data_in_bytes/eclipsed_seconds/1024/1024,(float)Pthis->tuplecount_/1024/1024);
	}
}
