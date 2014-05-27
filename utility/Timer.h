/*
 * Timer.h
 *
 *  Created on: Jun 5, 2013
 *      Author: wangli
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int gettimeofday(struct timeval *tv, struct timezone *tz);
long get_current_ms()
{
	timeval t_start;
	gettimeofday(&t_start,NULL);
	return t_start.tv_sec*1000+t_start.tv_usec/1000;
}


#endif /* TIMER_H_ */
