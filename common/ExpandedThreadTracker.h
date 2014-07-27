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
#include "../utility/ExpandabilityShrinkability.h"
#include <vector>



typedef unsigned long long int timestamp;

struct PerformanceInfo{
	struct entry{
		entry():performance(0),last_update(0){};
		float performance;
		timestamp last_update;
	};
//	PerformanceInfo();
	PerformanceInfo(ExpandabilityShrinkability* expand_shrink);
	~PerformanceInfo();
	void processed_one_block();
	double report_instance_performance_in_millibytes();
	ticks current_time_slice_start_;
	unsigned long nblocks_in_current_slice_;
	void initialize();
	void eclipseTimeSlices(unsigned offset);
	void hitCurrentSlice();

	/* eclipse the slices that are out of the scope and initialize the new entries*/
	void updateTimeSlicesToCurrentTicks();

	unsigned long getBlockSumInAllSlices();
	SpineLock lock_;
	unsigned *slices;
	unsigned long long int init_ticks_;
	std::vector<entry> scalability_vector_;
	ExpandabilityShrinkability* expand_shrink_;
	/* the timestamp for the last update to the scalability vector */
	timestamp last_update_;

};



class ExpandedThreadTracker {
public:
	ExpandedThreadTracker(ExpandabilityShrinkability* expand_shrink);
	virtual ~ExpandedThreadTracker();
	inline PerformanceInfo& getPerformanceInfo(){
		return perf_info_;
	}
private:
	PerformanceInfo perf_info_;
};

#endif /* EXPANDEDTHREADTRACKER_H_ */
