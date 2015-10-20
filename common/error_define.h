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
 * error_define.h
 *
 *  Created on: Aug 5, 2015
 *      Author: yukai
 *		 Email: yukai2014@gmail.com
 *
 *  ATTENTION: have to add Error Message in error_no.h and error_no.cpp,
 *  if new error no is add
 */

#ifndef COMMON_ERROR_DEFINE_H_
#define COMMON_ERROR_DEFINE_H_

#include <stdlib.h>

typedef int RetCode;  // means return code

namespace claims {
namespace common {

const int kErrorMaxNumber = 10000;
static const char* kErrorMessage[kErrorMaxNumber];

const int kSuccess = EXIT_SUCCESS;
const int kFailure = EXIT_FAILURE;

/* errorno begin from here   */

/* errorno for common  -1 ~ -1000 */
const int kTypeError = -1;
const int kNotInit = -2;
const int EReadHdfsFileFail = -3;
const int EOpenHdfsFileFail = -4;
const int EReadDiskFileFail = -5;
const int EOpenDiskFileFail = -6;
const int EWriteDiskFileFail = -7;
const int EWriteHdfsFileFail = -8;
const int ELSeekDiskFileFail = -9;
const int ELSeekHdfsFileFail = -10;
const int ECloseDiskFileFail = -11;
const int ECloseHdfsFileFail = -12;
const int EParamInvalid = -13;

/* errorno for SQL parser -1001 ~ -2000  */
const int kNoTableFound = -1001;

/* errorno for Loader -2001 ~ -3000  */

/* errorno for codegen -3001 ~ -4000 */
const int kTestError = -3001;

/* errorno for logical_query_plan -4001 ~ -5000*/
const int kUninitializedJoinPolicy = -4001;
const int kGeneratePlanContextFailed = -4002;

/* errorno for physical_query_plan -5001 ~ -6000 */
const int kGenerateSubPhyPlanFailed = -5001;

/* errorno for physical_query_plan -6001 ~ -7000 */
const int ECatalogRestoreInvild = -6001;
const int ECatalogNotFound = -6002;

}  // end namespace common
}  // end namespace claims

#endif  // COMMON_ERROR_DEFINE_H_
