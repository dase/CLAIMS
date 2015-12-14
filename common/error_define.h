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
#include "./error_no.h"
using claims::common::CStrError;

typedef int RetCode;  // means return code
#define ELOG(ret, err_info)                                              \
  LOG(ERROR) << "[" << ret << ", " << CStrError(ret) << "] " << err_info \
             << std::endl;

#define WLOG(ret, warning_info)                                                \
  LOG(WARNING) << "[" << ret << ", " << CStrError(ret) << "] " << warning_info \
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

namespace claims {
namespace common {

const int rSuccess = EXIT_SUCCESS;
const int rFailure = EXIT_FAILURE;

/* errorno begin from here   */
/******************************************************************************
 * ATTENTION: have to add Error Message in error_no.h and error_no.cpp, if new
 * error no is add
 *****************************************************************************/
/* errorno for common  -1 ~ -1000 */
const int rTypeError = -1;
const int rNotInit = -2;
const int rInvalidArgument = -3;
const int rArrayOutOfRange = -4;
const int rServerListenError = -5;
const int rInitTwice = -6;
const int rNotSupport = -7;
const int rIterEnd = -8;
const int rIOError = -9;
const int rErrorFuncVersion = -10;
const int rPacEetNotSent = -11;
const int rResponseTimeOut = -12;
const int rAllocateMemoryFailed = -13;
const int rMemOverflow = -14;
const int rErrSys = -15;
const int rErrUnexpected = -16;
const int rSizeOverflow = -17;
const int rConflictValue = -18;
const int rItemNotSetted = -19;
const int rTryAgain = -20;
const int rBufNotEnough = -21;
const int rPartialFailed = -22;
const int rReadNothing = -23;
const int rFileNotExist = -24;
const int rDiscontinuousLog = -25;
const int rShemaError = -26;
const int rDataNotServe = -27;
const int rUnEnownObj = -28;
const int rNoMonitorData = -29;
const int rSerializeError = -30;
const int rDeserializeError = -31;
const int rAIOTimeout = -32;
const int rToEenExpired = -33;
const int rEncryptFailed = -34;
const int rDecryptFailed = -35;
const int rUserNotExist = -36;
const int rWrongPassword = -37;
const int rSEeyVersionError = -38;
const int rNotToEen = -39;
const int rNoPermission = -40;
const int rCondChecEFail = -41;
const int rNotRegistered = -42;
const int rAlreadyRegistered = -43;
const int rNoTableCreated = -44;
const int rInvalidValue = -45;
const int rTCPConnError = -46;
const int rDecimalOverflow = -47;
const int rDecimalError = -48;
const int rDecimalPrecisionError = -49;
const int rDividByZero = -50;
const int rDividError = -51;
const int rEmptyRange = -52;
const int rLogNotSync = -53;
const int rDirNotExist = -54;
const int rLogInvalid = -55;
const int rInvalidData = -56;
const int rAlreadyDone = -57;
const int rCanceled = -58;
const int rLogSrcChanged = -59;
const int rLogMissing = -60;
const int rValueOutOfRange = -61;
const int rExceedMemLimit = -62;
const int rResultUnEnown = -63;
const int rErrDataFormat = -64;
const int rEnqueueError = -65;
const int rQueueOverflow = -66;
const int rDeadLocE = -67;
const int rPartialLog = -68;
const int rChecEsumError = -69;
const int rInitError = -70;
const int rFatalState = -71;
const int rStopState = -72;
const int rFileAlreadyExist = -73;
const int rUnEnownPacEet = -74;
const int rLogTooLarge = -75;
const int rRPCSendError = -76;
const int rRPCpostError = -77;
const int rConnError = -78;
const int rInitSQLContextError = -79;
const int rSysConfigTableError = -80;
const int rInvalidConfig = -81;
const int rReadConfigError = -82;
const int rBadAddress = -83;
const int rMemoryAllocationFailed = -85;
const int rReadHdfsFileFail = -86;
const int rOpenHdfsFileFail = -87;
const int rReadDiskFileFail = -88;
const int rOpenDiskFileFail = -89;
const int rWriteDiskFileFail = -90;
const int rWriteHdfsFileFail = -91;
const int rLSeekDiskFileFail = -92;
const int rLSeekHdfsFileFail = -93;
const int rCloseDiskFileFail = -94;
const int rCloseHdfsFileFail = -95;
const int rFileEOF = -96;
const int rParamInvalid = -97;
const int rAccessDiskFileFail = -98;
const int rAccessHdfsFileFail = -99;
const int rNoMemory = -100;
const int rDataPathError = -101;

// schema associated
const int rEmptyAttributeName = -501;
const int rEmptyTableAlias = -502;

/* errorno for SQL parser -1001 ~ -2000  */
const int rInitSQLParserErr = -1001;
const int rSQLParserErr = -1002;
const int rUnEnownSysFunc = -1003;
const int rErrParserMalloc = -1004;
const int rErrParserSyntax = -1005;
const int rErrColumnSize = -1006;
const int rDuplicateColumn = -1007;
const int rUnEownColumn = -1008;
const int rUnEownOperator = -1009;
const int rIllegalID = -1010;
const int rErrPosition = -1011;
const int rIllegalValue = -1012;
const int rColumnAmbiguous = -1013;
const int rSchemaUnset = -1014;
const int rIllegalName = -1015;
const int rDuplicatedTable = -1016;
const int rUnEnownExpr = -1017;
const int rIllegalType = -1018;
const int rPrimaryEeyDuplicated = -1019;
const int rIllegalIndex = -1020;
const int rInvalidSchema = -1021;
const int rColumnNotFound = -1022;
const int rUserEmpty = -1023;
const int rUserIsLocEed = -1025;
const int rInvalidColumnNum = -1026;
const int rVariableUnEown = -1027;
const int rUserExist = -1028;
const int rPasswordEmpty = -1029;
const int rErrParamSize = -1030;
const int rDuplicatedParam = -1031;
const int rNoTableFound = -1032;

/* errorno for loader -2001 ~ -3000  */
const int rUnbindEntireProjectionFail = -2001;
const int rInvalidInsertData = -2002;
const int rTooLargeData = -2003;
const int rTooSmallData = -2004;
const int rTooLongData = -2005;
const int rInterruptedData = -2006;  // 123a45 => 123
const int rIncorrectData = -2007;    // a
const int rInvalidNullData = -2008;
const int rTooFewColumn = -2009;
const int rTooManyColumn = -2010;

/* errorno for codegen -3001 ~ -4000 */
const int rTestError = -3001;

/* errorno for logical_operator -4001 ~ -5000*/
const int rUninitializedJoinPolicy = -4001;
const int rGeneratePlanContextFailed = -4002;
const int rGenerateDataflowFailed = -4003;

/* errorno for physical_operator -5001 ~ -6000 */
const int rGenerateSubPhyPlanFailed = -5001;
const int rNoPartitionIdScan = -5002;
const int rCodegenFailed =
    -5003;  // the optimization of filter function failed;
const int rCatalogRestoreInvild = -5004;
const int rCatalogNotFound = -5005;

/* errorno for stmt_handler -14001 ~ -15000*/
const int rStmtHandlerCreateTableExisted = -14002;
const int rStmtHandlerTypeNotSupport = -14003;
const int rStmtHandlerCreateTableSuccess = -14004;
const int rStmtHandlerTableNotExistDuringLoad = -14005;
const int rStmtHandlerLoadDataSuccess = -14006;
const int rStmtHandlerTableNotExistDuringInsert = -14007;
const int rStmtHandlerInsertDataSuccess = -14008;
const int rStmtHandlerInsertNoValue = -14009;
const int rStmtHandlerCreateTableFailed = -14010;
const int rStmtHandlerCreateProjectionWithEmptyColumn = -14011;

const int rTableNotExisted = -14100;
const int rTableillegal = -14101;
const int rTableAliasEqualExistedTable = -14102;
const int rTableAliasEqualLowerAlias = -14103;
const int rColumnNotExist = -14104;
const int rColumnIsAmbiguous = -14105;
const int rTabelHaveNotColumn = -14106;
const int rColumnIsAmbiguousAfterAlias = -14108;
const int rColumnIsAmbiguousToExistedColumn = -14109;
const int rUnaryArgNotExist = -14110;
const int rFromClauseIsNULL = -14111;
const int rSelectClauseIsNULL = -14112;
const int rGroupbyListIsNULL = -14113;
const int rGroupByNotSupportColumn = -14114;
const int rAggNodeExprStrIsNULL = -14115;
const int rGroupbyNodeExprStrIsNULL = -14116;
const int rSelectNodeExprStrIsNULL = -14117;
const int rAliasConfictInSelectNode = -14118;
const int rAggHaveAgg = -14119;
const int rAggCouldNotInWhereClause = -14120;
const int rAggCouldNotInGroupByClause = -14121;
const int rAggSelectExprHaveOtherColumn = -14122;
const int rHavingNotAgg = -14123;
const int rTableNotExistInTableColumnALL = -14124;
const int rColumnAllShouldNotInOtherClause = -14125;
const int rMoreColumnsInSelectHaveALLALL = -14126;
const int rRightTableIsNULLInJoin = -14127;
const int rLeftTableIsNULLInJoin = -14128;
const int rNoDataTypeInConst = -14129;
const int rEqualJoinCondiInATable = -14130;
const int rEqualJoinCondiNotMatch = -14131;
const int rTableAlreadyExist = -14132;
const int rNoTalbeFound = -14133;
const int rNoProjection = -14134;
const int rLimitNotStandardized = -14135;
const int rLimitZero = -14136;
const int rLimitParaCouldnotLessZero = -14137;
const int rLimitParaShouldNaturalNumber = -14138;
}  // end namespace common
}  // end namespace claims

#endif  // COMMON_ERROR_DEFINE_H_
