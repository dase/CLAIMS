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
#include <string>
#include "./error_no.h"
#include "./error_define.h"
namespace claims {
namespace common {

static const char* CStrError(int errorno) {
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

  const char* res = "Unknown Error";
  if (likely(errorno <= 0 && errorno > -kErrorMaxNumber &&
             NULL != kErrorMessage[-errorno])) {
    res = kErrorMessage[-errorno];
  }
  return res;
}

ErrorInit::ErrorInit() {
  memset(kErrorMessage, 0, sizeof(kErrorMessage));
  DefineErrorAndMessage(rSuccess, "Success");

  /* errorno for common  -1 ~ -1000 */
  DefineErrorAndMessage(rTypeError, "Type error");
  DefineErrorAndMessage(rNotInit, "Not initialize");
  DefineErrorAndMessage(rReadHdfsFileFail, "Read data from hdfs failed");
  DefineErrorAndMessage(rOpenHdfsFileFail, "Open hdfs file failed");
  DefineErrorAndMessage(rReadDiskFileFail, "Read data from disk failed");
  DefineErrorAndMessage(rOpenDiskFileFail, "Open disk file failed");
  DefineErrorAndMessage(rWriteDiskFileFail,
                        "failed to write data into disk file");
  DefineErrorAndMessage(rWriteHdfsFileFail,
                        "failed to write data into hdfs file");
  DefineErrorAndMessage(rLSeekDiskFileFail, "lseek a disk file failed");
  DefineErrorAndMessage(rLSeekHdfsFileFail, "failed to seek hdfs file");
  DefineErrorAndMessage(rParamInvalid, "parameter of function is invalid");
  DefineErrorAndMessage(rCloseDiskFileFail, "failed to close disk file");
  DefineErrorAndMessage(rCloseHdfsFileFail, "failed to close hdfs file");
  DefineErrorAndMessage(rFileEOF, "reached the end of file");
  DefineErrorAndMessage(rAccessDiskFileFail, "failed to access disk file");
  DefineErrorAndMessage(rAccessHdfsFileFail, "failed to access hdfs file");

  DefineErrorAndMessage(rNoMemory, "failed to allocate memory from system");

  /* errorno for SQL parser -1001 ~ -2000  */
  DefineErrorAndMessage(rNoTableFound, "No such table found");
  DefineErrorAndMessage(rInvalidInsertData, "data to insert is invalid");

  /* errorno for loader -2001 ~ -3000  */
  DefineErrorAndMessage(rUnbindEntireProjectionFail,
                        "failed to unbind entire projection");
  DefineErrorAndMessage(rTooLargeData, "Load Too Large Data");
  DefineErrorAndMessage(rTooSmallData, "Load Too Small Data");
  DefineErrorAndMessage(rTooLongData, "Load Too Long Data");
  DefineErrorAndMessage(rInterruptedData, "The Load data is interrupted");
  DefineErrorAndMessage(rIncorrectData, "The Load data format is incorrect");
  DefineErrorAndMessage(rInvalidNullData, "The null value is invalid");
  DefineErrorAndMessage(rTooFewColumn, "too few column data");
  DefineErrorAndMessage(rTooManyColumn, "too many column data");

  /* errorno for codegen -3001 ~ -4000 */
  DefineErrorAndMessage(rTestError, "test it is error ");

  /* errorno for logical_operator -4001~ -5000 */
  DefineErrorAndMessage(rUninitializedJoinPolicy,
                        "the dedicated join police is not initialized, e.g., "
                        "in corss join, equal join logical operator");
  DefineErrorAndMessage(rGeneratePlanContextFailed,
                        "generate the dataflow failed in the logical operator");

  /* errorno for physical query plan -5001 ~ -6000 */
  DefineErrorAndMessage(rGenerateSubPhyPlanFailed,
                        "generate the sub physical plan failed");
  DefineErrorAndMessage(rNoPartitionIdScan,
                        "The partition id does not existed.");
  DefineErrorAndMessage(rCodegenFailed, "codegen failed.");

  DefineErrorAndMessage(rCatalogRestoreInvild, "restore catalog failed");
  DefineErrorAndMessage(rCatalogNotFound, "catalog file is not existed");

  //  std::cout<<ERROR_MESSEGE[1]<<" , "<<ERROR_MESSEGE[2]<<std::endl;
}
}  // namespace common
}  // namespace claims
