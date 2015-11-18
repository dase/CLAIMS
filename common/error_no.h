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
 * error_no.h
 *
 *  Created on: Aug 5, 2015
 *      Author: yukai
 *		 Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#pragma once
#ifndef COMMON_ERROR_NO_H_
#define COMMON_ERROR_NO_H_

namespace claims {
namespace common {

const int kErrorMaxNumber = 10000;
static const char* kErrorMessage[kErrorMaxNumber];

const char* CStrError(int errorno);

static struct ErrorInit {
  ErrorInit();

  static inline void DefineErrorAndMessage(int value, const char* messege) {
    kErrorMessage[-value] = messege;
  }
} local_init;

}  // end namespace common
}  // end namespace claims

#endif  // COMMON_ERROR_NO_H_
