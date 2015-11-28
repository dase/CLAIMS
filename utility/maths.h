/*
 * math.h
 *
 *  Created on: Mar 30, 2015
 *      Author: wangli
 */

#ifndef MATH_H_
#define MATH_H_

#include <vector>
#include <cmath>

#include "../utility/Timer.h"
static double get_stddev(std::vector<int> input) {
  double sum = 0, squre_sum = 0;
  for (int i = 0; i < input.size(); i++) {
    sum += (double)input[i];
    squre_sum += (double)input[i] * (double)input[i];
  }
  double mean = sum / input.size();
  double var = 0;
  for (int i = 0; i < input.size(); i++) {
    var += (input[i] - mean) * (input[i] - mean);
  }
  var = var / input.size();
  return std::sqrt((double)var);
  //	return 1;
}

static inline int GetRandomInt(int max) {
  unsigned int seed = GetCurrentMs();
  return rand_r(&seed) % max;
}

static inline double GetRandomDecimal() {
  unsigned int seed = GetCurrentMs();
  return rand_r(&seed) * 1.0 / RAND_MAX;
}

#endif /* MATH_H_ */
