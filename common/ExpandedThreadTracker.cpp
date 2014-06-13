/*
 * ExpandedThreadTracker.cpp
 *
 *  Created on: May 25, 2014
 *      Author: wangli
 */

#include "ExpandedThreadTracker.h"

#define CPU_CYCLES_IN_us (CPU_FRE/1000000L)
#define TIME_SLICE  (10000*CPU_CYCLES_IN_us)
#define get_time_slice_start(total_cycles) (total_cycles/TIME_SLICE*TIME_SLICE)

PerformanceInfo::PerformanceInfo():nblocks_in_current_slice_(0){
	current_time_slice_start_=get_time_slice_start(curtick());
};
void PerformanceInfo::processed_one_block(){
	const ticks cur_tick=curtick();
	if(current_time_slice_start_!=get_time_slice_start(cur_tick)){
		/* A new time slice comes*/
		current_time_slice_start_=get_time_slice_start(cur_tick);
		nblocks_in_current_slice_=1;
	}
	else{
		/*
		 * It's better use atomic add for accurate. But this is for now,
		 * assuming that the conflict is rare.
		 */
		nblocks_in_current_slice_++;
	}
}
double PerformanceInfo::report_instance_performance_in_millibytes(){
	const ticks cur_tick=curtick();
	const ticks slice_start=get_time_slice_start(cur_tick);
	if(current_time_slice_start_!=slice_start){
		return 0;
	}
	else{
		const double size=(nblocks_in_current_slice_)*BLOCK_SIZE/(double)1024/1024;
		const double duration=getSecondDuratoin(slice_start,cur_tick);
		return size/duration;
	}
}


ExpandedThreadTracker::ExpandedThreadTracker() {
	// TODO Auto-generated constructor stub

}

ExpandedThreadTracker::~ExpandedThreadTracker() {
	// TODO Auto-generated destructor stub
}

