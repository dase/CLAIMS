/*
 * ExpandedThreadTracker.h
 *
 *  Created on: May 25, 2014
 *      Author: wangli
 */

#ifndef EXPANDEDTHREADTRACKER_H_
#define EXPANDEDTHREADTRACKER_H_
#include "../utility/rdtsc.h"
#include "../utility/lock.h"
#include "../Debug.h"

//#define NUMBER_OF_TRACKED_PERFORMANCE_HISTORY 5
	struct PerformanceInfo{
		PerformanceInfo():start_tick_(curtick()),accumulated_blocks_(0){
			last_report_tick_=start_tick_;
			last_report_accumulated_blocks_=accumulated_blocks_;
		};
		void processed_one_block(){
//			lock_.lock();
			accumulated_blocks_++;
//			printf("repeated!  --%ld---\n",accumulated_blocks_);
//			lock_.unlock();
		}
		unsigned long get_accumulated_blocks_(){
			unsigned long ret;
			lock_.lock();
			ret=accumulated_blocks_;
//			lock_.unlock();
			return ret;

		}
		double report_instance_performance_in_millibytes(){
			const double size=(accumulated_blocks_-last_report_accumulated_blocks_)*BLOCK_SIZE/(double)1024/1024;
			const double duration=getSecond(last_report_tick_);

			/*update report status*/
			last_report_tick_=curtick();
			last_report_accumulated_blocks_=accumulated_blocks_;

			return size/duration;
		}
		unsigned long long int start_tick_;
		unsigned long accumulated_blocks_;
		unsigned long long int last_report_tick_;
		unsigned long last_report_accumulated_blocks_;
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
