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
 * errno_test.h
 *
 *  Created on: Aug 5, 2015
 *      Author: yukai
 *		 Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef TEST_COMMON_ERRNO_TEST_H_
#define TEST_COMMON_ERRNO_TEST_H_

#include <gtest/gtest.h>
#include <iostream>

#include "../../common/error_define.h"
#include "../../utility/Timer.h"
#include "../../common/error_no.h"
using std::cout;
using std::endl;
using namespace claims::common;  // NOLINT

class ErrorNoTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() { cout << "start new test case" << endl; }
  static void TearDownTestCase() { cout << "finish new test case" << endl; }
  static RetCode TestReturnErrorCode() { return rSuccess; }
};

TEST_F(ErrorNoTest, A) {
  int errerno = rTypeError;

  //  cout<<errerno<<" , "<<CStrError(errerno)<<endl;
  const char *res = CStrError(errerno);
  EXPECT_STREQ("Type error", res);
}

TEST_F(ErrorNoTest, B) {
  int errerno = -300000;

  //  cout<<errerno<<" , "<<CStrError(errerno)<<endl;
  const char *res = CStrError(errerno);
  EXPECT_STREQ("Unknown Error", res);
}

TEST_F(ErrorNoTest, C) {
  int errerno = claims::common::rSuccess;

  //  cout<<errerno<<" , "<<CStrError(errerno)<<endl;
  const char *res = CStrError(errerno);
  EXPECT_STREQ("Success", res);
}

TEST_F(ErrorNoTest, D) { EXPECT_EQ(TestReturnErrorCode(), rSuccess); }

TEST_F(ErrorNoTest, Performance) {
  int e[3] = {rParamInvalid, rOpenHdfsFileFail, rUninitializedJoinPolicy};
  EXPECT_STREQ("parameter of function is invalid", CStrError(e[0]));
  EXPECT_STREQ("Open hdfs file failed", CStrError(e[1]));
  EXPECT_STREQ(
      "the dedicated join police is not initialized, e.g., "
      "in corss join, equal join logical operator",
      CStrError(e[2]));
  GETCURRENTTIME(start);
  for (int i = 0; i < 10000000; ++i) {
    int error_no = i % 3;
    const char *res = CStrError(error_no);
  }
  cout << "10,000,000 times calls use :" << GetElapsedTime(start) << " ms"
       << endl;
}

#endif  // TEST_COMMON_ERRNO_TEST_H_
