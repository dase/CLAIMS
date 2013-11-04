/*
 * BlockStreamPerformanceMonitorTop.h
 *
 *  Created on: Aug 31, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMPERFORMANCEMONITORTOP_H_
#define BLOCKSTREAMPERFORMANCEMONITORTOP_H_
#include <pthread.h>
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../Schema/Schema.h"

class BlockStreamPerformanceMonitorTop:public BlockStreamIteratorBase {
public:
	struct State{
		State(Schema* schema,BlockStreamIteratorBase* child, unsigned block_size,unsigned report_cycles)
		:schema_(schema),child_(child),block_size_(block_size),report_cycles_(report_cycles){};
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
		unsigned report_cycles_;//in milliseconds.

	};
	BlockStreamPerformanceMonitorTop(State state_);
	virtual ~BlockStreamPerformanceMonitorTop();
	bool open();
	bool next(BlockStreamBase* block);
	bool close();
private:
	static void* report(void* arg);
private:
	State state_;
	BlockStreamBase* block_;
	unsigned long int tuplecount_;
	unsigned long long int start_;
	pthread_t report_tid_;
};

#endif /* BLOCKSTREAMPERFORMANCEMONITORTOP_H_ */
