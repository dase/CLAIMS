/*
 * my_glog.h
 *  Created on: Jun 12, 2015 11:07:40 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_LOG_LOGGING_H_
#define COMMON_LOG_LOGGING_H_
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <gflags/gflags.h>
namespace claims {
namespace common {

#define LOG_INFO LOG(INFO)
#define LOG_WARNING LOG(WARNING)
#define LOG_ERROR LOG(ERROR)
#define LOG_FATAL LOG(FATAL)

void GlogSignalHandle(const char* data, int size);

class Logging {
 public:
  explicit Logging(char* program);
  ~Logging();
};
}  // namespace common
}  // namespace claims

#endif  // COMMON_LOG_LOGGING_H_
