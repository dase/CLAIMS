/*
 * ExpandedThreadTracker_test.cpp
 *
 *  Created on: May 25, 2014
 *      Author: wangli
 */

#include "ExpandedThreadTracker.h"

static void test_ExpandedThreadTracker(){
	ExpandedThreadTracker ett;
	printf("P:%lf\n",ett.getPerformanceInfo().report_instance_performance_in_millibytes());
	ett.getPerformanceInfo().processed_one_block();

	printf("P:%lf\n",ett.getPerformanceInfo().report_instance_performance_in_millibytes());
	printf("P:%lf\n",ett.getPerformanceInfo().report_instance_performance_in_millibytes());
}
