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
 * /Claims/utility/lock_guard.h
 *
 *  Created on: Oct 23, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef UTILITY_LOCK_GUARD_H_
#define UTILITY_LOCK_GUARD_H_
#include "../common/rename.h"
namespace claims {

namespace utility {

template <typename LockType>
class LockGuard {
 public:
  explicit LockGuard(LockType& lock) : lock_(lock) { lock_.acquire(); }
  ~LockGuard() { lock_.release(); }

  NO_COPY_AND_ASSIGN(LockGuard);

 public:
  LockType& lock_;
};

}  // namespace utility
}  // namespace claims

#endif  // UTILITY_LOCK_GUARD_H_
