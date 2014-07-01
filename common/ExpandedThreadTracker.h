/*
 * ExpandedThreadTracker.h
 *
 *  Created on: May 25, 2014
 *      Author: wangli
 */

#ifndef EXPANDEDTHREADTRACKER_H_
#define EXPANDEDTHREADTRACKER_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../utility/rdtsc.h"
#include "../utility/lock.h"
#include "../Debug.h"




struct PerformanceInfo{
	PerformanceInfo();
	void processed_one_block();
	double report_instance_performance_in_millibytes();
	ticks current_time_slice_start_;
	unsigned long nblocks_in_current_slice_;
	SpineLock lock_;
};



class ExpandedThreadTracker {
public:
	ExpandedThreadTracker();
	virtual ~ExpandedThreadTracker();
	inline PerformanceInfo& getPerformanceInfo(){
		return perf_info_;
	}
private:
	PerformanceInfo perf_info_;
};

#endif /* EXPANDEDTHREADTRACKER_H_ */
