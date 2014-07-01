/*
 * BlockStreamPerformanceMonitorTop.h
 *
 *  Created on: Aug 31, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMPERFORMANCEMONITORTOP_H_
#define BLOCKSTREAMPERFORMANCEMONITORTOP_H_
#include <pthread.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../common/Schema/Schema.h"
#include "../common/Logging.h"

class BlockStreamPerformanceMonitorTop:public BlockStreamIteratorBase {
public:
	struct State{
		State(){};
		State(Schema* schema,BlockStreamIteratorBase* child, unsigned block_size,unsigned report_cycles=1000)
		:schema_(schema),child_(child),block_size_(block_size),report_cycles_(report_cycles){};
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
		unsigned report_cycles_;//in milliseconds.
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ & block_size_ & report_cycles_;
		}

	};
	BlockStreamPerformanceMonitorTop(State state_);
	virtual ~BlockStreamPerformanceMonitorTop();
	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase* block);
	bool close();
	void print();
	unsigned long int getNumberOfTuples()const;
private:
	static void* report(void* arg);
private:
	State state_;
	BlockStreamBase* block_;
	unsigned long int tuplecount_;
	unsigned long long int start_;
	pthread_t report_tid_;
	Logging* logging_;
private:
	BlockStreamPerformanceMonitorTop();
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
            ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
    }
};

#endif /* BLOCKSTREAMPERFORMANCEMONITORTOP_H_ */
