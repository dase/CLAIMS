/*
 * Timer.h
 *
 *  Created on: Jun 5, 2013
 *      Author: wangli,yukai
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int gettimeofday(struct timeval *tv, struct timezone *tz);
static inline double GetCurrentMs() {
  timeval t_start;
  gettimeofday(&t_start, NULL);
  return t_start.tv_sec * 1000 + 1.0 * t_start.tv_usec / 1000;
}

/*
 * @param start: the start time
 * @return : the elapsed time(ms) from start, accurate to us
 */
static inline double GetElapsedTime(double start) {
  timeval t_end;
  gettimeofday(&t_end, NULL);
  return (t_end.tv_sec * 1000 + 1.0 * t_end.tv_usec / 1000) - start;
}

#define GETCURRENTTIME(t) \
  timeval t;              \
  gettimeofday(&t, NULL);

/*
 * @param start: the timeval of start time
 * @return : the elapsed time(ms) from start, accurate to us!
 */
static inline double GetElapsedTime(struct timeval start) {
  timeval end;
  gettimeofday(&end, NULL);
  return (end.tv_usec - start.tv_usec) / 1000.0 +
         (end.tv_sec - start.tv_sec) * 1000.0;
}

static inline uint64_t GetElapsedTimeInUs(struct timeval start) {
  timeval end;
  gettimeofday(&end, NULL);
  return end.tv_usec - start.tv_usec + (end.tv_sec - start.tv_sec) * 1000000;
}

#endif /* TIMER_H_ */
