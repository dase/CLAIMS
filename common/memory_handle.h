/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * /Claims/common/memory_allocate.h
 *
 *  Created on: Oct 24, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef COMMON_MEMORY_H_
#define COMMON_MEMORY_H_
#include <glog/logging.h>
#include <malloc.h>
#include <stdlib.h>

namespace claims {

namespace common {

static inline void* Malloc(size_t length) {
  int repeat_time = 0;
  int max_times = 3;
  void* buffer = NULL;
  while (NULL == (buffer = malloc(length)) && ++repeat_time < max_times) {
    usleep(1);
  }  // newmalloc
  if (buffer == NULL) {
    LOG(ERROR) << "failed to allocate " << length << " length memory after "
               << max_times << " times tries" << std::endl;
  }
  memset(buffer, 0, length);
  return buffer;
}

#define DELETE_PTR(ptr) \
  do {                  \
    delete ptr;         \
    ptr = NULL;         \
  } while (false)

}  // namespace common

}  // namespace claims

#endif  // COMMON_MEMORY_H_
