/*
 * ExpandedThreadTracker.cpp
 *
 *  Created on: May 25, 2014
 *      Author: wangli
 */

#include <malloc.h>
#include "ExpandedThreadTracker.h"
#include "../utility/rdtsc.h"
#include "../Config.h"

//#define PRINT_PERFORMANCE_REALTIME

#define CPU_CYCLES_IN_us (CPU_FRE/1000000L)
#define TIME_SLICE  (1000*CPU_CYCLES_IN_us)
#define get_time_slice_start(total_cycles) (total_cycles/TIME_SLICE*TIME_SLICE)
#define NUMOFSLICES 10

#define MONITOR_FRECY (2000*CPU_CYCLES_IN_us)

/* the entry in scalability vector is considered as out of date
 * if the last update was conducted VALID_FRECY ago. */
#define VALID_FRECY (50000*CPU_CYCLES_IN_us)


PerformanceInfo::PerformanceInfo(ExpandabilityShrinkability* expand_shrink):nblocks_in_current_slice_(0),expand_shrink_(expand_shrink),last_update_(0){
	current_time_slice_start_=get_time_slice_start(curtick());
	slices=(unsigned*)calloc(sizeof(unsigned),NUMOFSLICES);
	scalability_vector_=std::vector<entry>(Config::getInstance()->max_degree_of_parallelism+1);
	/* the performance when Dop=0 is always 0, so we set last_update to be infinity,
	 * indicating that the value is always up to date*/
	scalability_vector_[0].performance=0;
	scalability_vector_[0].last_update=-1;
};
//PerformanceInfo::PerformanceInfo():nblocks_in_current_slice_(0),expand_shrink_(0),last_update_(0){
//	current_time_slice_start_=get_time_slice_start(curtick());
//	slices=(unsigned*)calloc(sizeof(unsigned),NUMOFSLICES);
//	scalability_vector_=std::vector<entry>(Config::getInstance()->max_degree_of_parallelism);
//
//	/* the performance when Dop=0 is always 0, so we set last_update to be infinity,
//	 * indicating that the value is always up to date*/
//	scalability_vector_[0].performance=0;
//	scalability_vector_[0].last_update=-1;
//};
void PerformanceInfo::processed_one_block(){
	const ticks cur_tick=curtick();
//	if(current_time_slice_start_!=get_time_slice_start(cur_tick)){
//		/* A new time slice comes*/
//		current_time_slice_start_=get_time_slice_start(cur_tick);
//		nblocks_in_current_slice_=1;
//	}
//	else{
//		/*
//		 * It's better use atomic add for accurate. But this is for now,
//		 * assuming that the conflict is rare.
//		 */
//		nblocks_in_current_slice_++;
//	}
	if(current_time_slice_start_!=get_time_slice_start(cur_tick)){
		/* A new time slice comes..
		/* so update the slices according to the current cycles*/
		lock_.acquire();
		updateTimeSlicesToCurrentTicks();
		hitCurrentSlice();
		lock_.release();
	}
	else{
		lock_.acquire();
		hitCurrentSlice();
		lock_.release();
	}
	/*
	 * if it has been long enough since last update to the scalability vector, we trigger
	 * the update.
	 */

	if(last_update_+MONITOR_FRECY<cur_tick){
		unsigned cur_dop=expand_shrink_->getDegreeOfParallelism();
//		unsigned cur_dop=1;
		last_update_=cur_tick;
		scalability_vector_[cur_dop].last_update=cur_tick;
		scalability_vector_[cur_dop].performance=report_instance_performance_in_millibytes();
#ifdef PRINT_PERFORMANCE_REALTIME
		int max_dop=Config::getInstance()->max_degree_of_parallelism+1;
		for(unsigned i=0;i<max_dop;i++){
			if(scalability_vector_[i].isUpdateToDate())
				printf("%4.0f\t",scalability_vector_[i].performance);
			else{
				printf("N/A\t");
			}
		}
		printf("\n");
#endif
	}

}
double PerformanceInfo::report_instance_performance_in_millibytes(){
//	const ticks cur_tick=curtick();
//	const ticks slice_start=get_time_slice_start(cur_tick);
//	if(current_time_slice_start_!=slice_start){
//		return 0;
//	}
//	else{
//		const double size=(nblocks_in_current_slice_)*BLOCK_SIZE/(double)1024/1024;
//		const double duration=getSecondDuratoin(slice_start,cur_tick);
//		printf("%d blocks!\n",nblocks_in_current_slice_);
//		return size/duration;
//	}
	const ticks cur_tick=curtick();
	const ticks slice_start=get_time_slice_start(cur_tick);
	if(current_time_slice_start_!=slice_start){
		lock_.acquire();
		updateTimeSlicesToCurrentTicks();
		lock_.release();
	}

	double duration=getSecondDuratoin(slice_start,cur_tick)+(NUMOFSLICES-1)*convertCyclesToSecond(TIME_SLICE);
	duration=duration<getSecond(init_ticks_)?duration:getSecond(init_ticks_);
	const unsigned long accumulate_blocks=getBlockSumInAllSlices();
//	printf("accu %d  duration: %f   %f   \n",accumulate_blocks,getSecondDuratoin(slice_start,cur_tick),(NUMOFSLICES-1)*convertCyclesToSecond(TIME_SLICE));
	return accumulate_blocks*BLOCK_SIZE/duration/(double)1024/1024;;
}


ExpandedThreadTracker::ExpandedThreadTracker(ExpandabilityShrinkability* expand_shrink):perf_info_(expand_shrink) {
	// TODO Auto-generated constructor stub

}

ExpandedThreadTracker::~ExpandedThreadTracker() {
	// TODO Auto-generated destructor stub
}

void PerformanceInfo::initialize() {
	current_time_slice_start_=curtick();
	nblocks_in_current_slice_=0;
	init_ticks_=curtick();
	last_update_=curtick();

}

void PerformanceInfo::eclipseTimeSlices(unsigned offset) {
	if(offset<=0)
		return;
	for(unsigned i = 0; i < NUMOFSLICES ; i++){
		if(i+offset<NUMOFSLICES)
			slices[i]=slices[i+offset];
		else
			slices[i]=0;
	}
}

void PerformanceInfo::hitCurrentSlice() {
	slices[NUMOFSLICES-1]++;
}

void PerformanceInfo::updateTimeSlicesToCurrentTicks() {
	const ticks cur_tick=curtick();
	const ticks cur_slice_start=get_time_slice_start(cur_tick);
	const ticks old_slice_start=current_time_slice_start_;
	unsigned passed_time_slices = (cur_slice_start-old_slice_start)/TIME_SLICE;

	current_time_slice_start_=cur_slice_start;
	/* update the slices according to the current cycles*/
	eclipseTimeSlices(passed_time_slices);
}

PerformanceInfo::~PerformanceInfo() {
//	free(slices);
}

unsigned long PerformanceInfo::getBlockSumInAllSlices() {
	lock_.acquire();
	unsigned long ret=0;
	for(unsigned i=0;i<NUMOFSLICES;i++){
		ret+=slices[i];
	}
	lock_.release();
	return ret;
}

bool PerformanceInfo::entry::isUpdateToDate() {
	return last_update>=curtick()-VALID_FRECY;
}
