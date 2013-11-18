/*
 * BlockStreamPerformanceTest.h
 *
 *  Created on: 2013年10月19日
 *      Author: SCDONG
 */

#ifndef BLOCKSTREAMPERFORMANCETEST_H_
#define BLOCKSTREAMPERFORMANCETEST_H_
#include <pthread.h>
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../Schema/Schema.h"

class BlockStreamPerformanceTest:public BlockStreamIteratorBase {
public:
	struct State{
		State(Schema* schema,BlockStreamIteratorBase* child1, BlockStreamIteratorBase* child2, unsigned block_size,unsigned report_cycles)
		:schema_(schema),child1_(child1),child2_(child2),block_size_(block_size),report_cycles_(report_cycles){};
		Schema* schema_;
		BlockStreamIteratorBase* child1_, *child2_;
		unsigned block_size_;
		unsigned report_cycles_;//in milliseconds.

	};
	BlockStreamPerformanceTest(State state_);
	virtual ~BlockStreamPerformanceTest();
	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* block);
	bool close();
private:
	static void* report(void* arg);
private:
	State state_;
	BlockStreamBase* block_;
	unsigned long int tuplecount_;
	unsigned long int tuplecount1_;
	unsigned long int tuplecount2_;
	unsigned long long int start_;
	pthread_t report_tid_;
};

#endif /* BLOCKSTREAMPERFORMANCETEST_H_ */
