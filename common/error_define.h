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
 * Description:
 *
 */

#ifndef COMMON_ERROR_DEFINE_H_
#define COMMON_ERROR_DEFINE_H_

#include <stdlib.h>

/*
 * have to add Error Message in error_no.h and error_no.cpp, if new error no is
 * add
 */
namespace claims {
namespace common {

const int kErrorMaxNumber = 100000;
static const char* kErrorMessage[kErrorMaxNumber];

const int rSuccess = EXIT_SUCCESS;
const int rFailure = EXIT_FAILURE;

/* errorno begin from here   */

/* errorno for common  -1 ~ -1000 */
const int kTypeError = -1;
const int kNotInit = -2;
const int EMemoryAllocationFailed = -3;

/* errorno for SQL parser -1001 ~ -2000  */
const int kNoTableFound = -1001;

/* errorno for Loader -2001 ~ -3000  */

/* errorno for codegen -3001 ~ -4000 */
const int kTestError = -3001;

/* errorno for logical_operator -4001 ~ -5000*/
const int kUninitializedJoinPolicy = -4001;

const int kGeneratePlanContextFailed = -4002;

const int kGenerateDataflowFailed = -4003;

/* errorno for physical_operator -5001 ~ -6000 */
const int kGenerateSubPhyPlanFailed = -5001;
const int kNoPartitionIdScan = -5002;
// the optimization of filter function failed;
const int kCodegenFailed = -5003;

/* errorno for stmt_handler -14001 ~ -15000*/
const int kStmtHandlerOk = -14001;
const int kStmtHandlerTableExistDuringCreate = -14002;
const int kStmtHandlerTypeNotSupport = -14003;
const int kStmtHandlerCreateTableSuccess = -14004;
const int kStmtHandlerTableNotExistDuringLoad = -14005;
const int kStmtHandlerLoadDataSuccess = -14006;
const int kStmtHandlerTableNotExistDuringInsert = -14007;
const int kStmtHandlerInsertDataSuccess = -14008;
const int kStmtHandlerInsertNoValue = -14009;

const int rTableNotExist = -14100;
const int rTableillegal = -14101;
const int rTableAliasEqualExistedTable = -14102;
const int rTableAliasEqualLowerAlias = -14103;
const int rColumnNotExist = -14104;
const int rColumnIsAmbiguous = -14105;
const int rTabelHaveNotColumn = -14106;
const int rColumnAliasIsAmbiguous = -14107;
const int rColumnIsAmbiguousAfterAlias = -14108;
const int rColumnIsAmbiguousToExistedColumn = -14109;
const int rUnaryArgNotExist = -14110;
const int rFromClauseIsNULL = -14111;
const int rSelectClauseIsNULL = -14112;
const int rGroupbyListIsNULL = -14113;
const int rGroupByNotSupportColumn = -141014;
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
}  // end namespace common
}  // end namespace claims

#endif  // COMMON_ERROR_DEFINE_H_
