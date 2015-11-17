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
<<<<<<< HEAD
=======
 *
 * Description:
>>>>>>> origin/checkset-lzf1116
 *
 *  ATTENTION: have to add Error Message in error_no.h and error_no.cpp,
 *  if new error no is add
 */

#ifndef COMMON_ERROR_DEFINE_H_
#define COMMON_ERROR_DEFINE_H_

#include <stdlib.h>
#include "./error_no.h"

typedef int RetCode;  // means return code
using claims::common::CStrError;
/*
 * have to add Error Message in error_no.h and error_no.cpp, if new error no is
 * add
 */
namespace claims {
namespace common {

#define ELOG(ret, err_info)                                                \
  LOG(ERROR) << "[ " << ret << ", " << CStrError(ret) << " ] " << err_info \
             << std::endl;

#define EXEC_AND_ONLY_LOG_ERROR(ret, f, err_info) \
  do {                                            \
    if (rSuccess != (ret = f)) {                  \
      ELOG(ret, err_info)                         \
    }                                             \
  } while (0)

#define EXEC_AND_LOG(ret, f, info, err_info) \
  do {                                       \
    if (rSuccess == (ret = f)) {             \
      LOG(INFO) << info << std::endl;        \
    } else {                                 \
      ELOG(ret, err_info)                    \
    }                                        \
  } while (0)

#define EXEC_AND_PLOG(ret, f, info, err_info)                       \
  do {                                                              \
    if (rSuccess == (ret = f)) {                                    \
      LOG(INFO) << info << std::endl;                               \
    } else {                                                        \
      PLOG(ERROR) << "[ " << ret << ", " << CStrError(ret) << " ] " \
                  << err_info << std::endl;                         \
    }                                                               \
  } while (0)

const int rSuccess = EXIT_SUCCESS;
const int rFailure = EXIT_FAILURE;

/* errorno begin from here   */

/* errorno for common  -1 ~ -1000 */
const int rTypeError = -1;
const int rNotInit = -2;
const int rReadHdfsFileFail = -3;
const int rOpenHdfsFileFail = -4;
const int rReadDiskFileFail = -5;
const int rOpenDiskFileFail = -6;
const int rWriteDiskFileFail = -7;
const int rWriteHdfsFileFail = -8;
const int rLSeekDiskFileFail = -9;
const int rLSeekHdfsFileFail = -10;
const int rCloseDiskFileFail = -11;
const int rCloseHdfsFileFail = -12;
const int rFileEOF = -13;
const int rParamInvalid = -14;
const int rAccessDiskFileFail = -15;
const int rAccessHdfsFileFail = -16;

const int rNoMemory = -17;

/* errorno for SQL parser -1001 ~ -2000  */
const int rNoTableFound = -1001;

/* errorno for loader -2001 ~ -3000  */
const int rUnbindEntireProjectionFail = -2001;
const int rInvalidInsertData = -1002;
const int rTooLargeData = -2003;
const int rTooSmallData = -2004;
const int rTooLongData = -2005;
const int rInterruptedData = -2006;  // 123a45 => 123
const int rIncorrectData = -2007;    // a
const int rInvaildNullData = -2008;

/* errorno for codegen -3001 ~ -4000 */
const int rTestError = -3001;

/* errorno for logical_operator -4001 ~ -5000*/
const int rUninitializedJoinPolicy = -4001;

const int rGeneratePlanContextFailed = -4002;

const int rGenerateDataflowFailed = -4003;

/* errorno for physical_operator -5001 ~ -6000 */
const int rGenerateSubPhyPlanFailed = -5001;
const int rNoPartitionIdScan = -5002;
// the optimization of filter function failed;
const int rCodegenFailed = -5003;

/* errorno for physical_query_plan -6001 ~ -7000 */
const int rCatalogRestoreInvild = -6001;
const int rCatalogNotFound = -6002;

}  // end namespace common
}  // end namespace claims

#endif  // COMMON_ERROR_DEFINE_H_
