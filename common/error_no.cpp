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
 * error_no.cpp
 *
 *  Created on: Aug 5, 2015
 *      Author: yukai
 *		 Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include <string.h>
#include <iostream>
#include "./error_no.h"

namespace claims {
namespace common {

const char* CStrError(int errorno) {
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

  const char* res = "Unknown Error";
  if (likely(errorno <= 0 && errorno > -kErrorMaxNumber)) {
    res = kErrorMessage[-errorno];
    //    std::cout<<res<<"----"<<std::endl;
    if (unlikely(NULL == res)) {
      res = "Unknown Error";
    }
  }
  return res;
}

ErrorInit::ErrorInit() {
  memset(kErrorMessage, 0, sizeof(kErrorMessage));
  DefineErrorAndMessage(kSuccess, "Success");

  /* errorno for common  -1 ~ -1000 */
  DefineErrorAndMessage(kTypeError, "Type error");
  DefineErrorAndMessage(kNotInit, "Not initialize");

  /* errorno for SQL parser -1001 ~ -2000  */
  DefineErrorAndMessage(kNoTableFound, "No such table found");

  /* errorno for codegen -3001 ~ -4000 */
  DefineErrorAndMessage(kTestError, "test it is error ");

  /* errorno for logical_operator -4001~ -5000 */
  DefineErrorAndMessage(kUninitializedJoinPolicy,
                        "the dedicated join police is not initialized, e.g., "
                        "in corss join, equal join logical operator");
  DefineErrorAndMessage(kGeneratePlanContextFailed,
                        "generate the dataflow failed in the logical operator");

  /* errorno for physical query plan -5001 ~ -6000 */
  DefineErrorAndMessage(kGenerateSubPhyPlanFailed,
                        "generate the sub physical plan failed");
  DefineErrorAndMessage(kNoPartitionIdScan,
                        "The partition id does not existed.");
  DefineErrorAndMessage(kCodegenFailed, "codegen failed.");

  //  std::cout<<ERROR_MESSEGE[1]<<" , "<<ERROR_MESSEGE[2]<<std::endl;
}
}  // namespace common
}  // namespace claims
