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
#define likely(x)     __builtin_expect(!!(x), 1)
#define unlikely(x)   __builtin_expect(!!(x), 0)

  const char* res = "Unknown Error";
  if (likely(errorno <= 0 && errorno > -ERROR_MAX_NUMBER)) {
    res = ERROR_MESSEGE[-errorno];
//    std::cout<<res<<"----"<<std::endl;
    if (unlikely(NULL == res)) {
      res = "Unknown Error";
    }
  }
  return res;
}

ErrorInit::ErrorInit() {
  memset(ERROR_MESSEGE, 0, sizeof(ERROR_MESSEGE));
  DefineErrorAndMessage(C_SUCCESS, "Success");

  /* errorno for common  -1 ~ -1000 */
  DefineErrorAndMessage(C_TYPE_ERROR, "Type error");
  DefineErrorAndMessage(C_NOT_INIT, "Not initialize");

  /* errorno for SQL parser -1001 ~ -2000  */
  DefineErrorAndMessage(C_NO_TABLE_FOUND, "No such table found");

  /* errorno for codegen -3001 ~ -4000 */
  DefineErrorAndMessage(C_TEST_ERROR, "test it is error ");

//  std::cout<<ERROR_MESSEGE[1]<<" , "<<ERROR_MESSEGE[2]<<std::endl;
}
}  // namespace common
}  // namespace claims



