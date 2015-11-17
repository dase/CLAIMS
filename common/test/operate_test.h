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
 * /CLAIMS/common/test/data_type_test.h
 *
 *  Created on: Nov 12, 2015
 *      Author: imdb
 *		   Email: 
 * 
 * Description:
 *
 */
#ifndef COMMON_TEST_OPERATE_TEST
#define COMMON_TEST_OPERATE_TEST

#include <gtest/gtest.h>
#include <iostream>
#include <limits.h>
#include <float.h>
#include "../data_type.h"
#include "../error_define.h"
using claims::common::rSuccess;
using claims::common::rTooSmallData;
using claims::common::rTooLargeData;
using claims::common::rTooLongData;
using claims::common::rInterruptedData;
using claims::common::rIncorrectData;
using claims::common::rInvalidNullData;
using std::cout;
using std::endl;
string big = "";
string _big = "";

class OperateTest : public ::testing::Test {
 public:
  static void SetUpTestCase() {
    for(auto i = 0;i<12;i++)
      big = big + "999999999999999999999999999999";
    _big = "-" + big;
    cout << "start operate test case" << endl;
  }
  static void TearDownTestCase() {
    cout << "finish operate test case" << endl;
  }
};

void test(string str, string exp1, RetCode exp2,Operate* op) {
  RetCode ret = op->CheckSet(str);
  EXPECT_EQ(exp1,str);
  EXPECT_EQ(exp2,ret);
}

TEST_F(OperateTest, Int) {
 OperateInt * opt = new OperateInt(true);
 OperateInt * opf = new OperateInt(false);


 test("-"+big.substr(0,40),kIntMin,rTooSmallData,opt);
 test(kIntMax,kIntMax_1,rTooLargeData,opt);
 test(kIntMax,kIntMax,rSuccess,opf);
 test(big,kIntMax_1,rTooLargeData,opt);
 test("123456.654321","123456.654321",rSuccess,opt);
 test("12a12","12",rInterruptedData,opt);
 test("a12","a12",rIncorrectData,opt);

 test("-12345","-12345",rSuccess,opt);
 test("12345","12345",rSuccess,opt);
 delete opt;
 delete opf;
}

TEST_F(OperateTest, Float) {
  OperateFloat * opt = new OperateFloat(true);
  OperateFloat * opf = new OperateFloat(false);

  test("-"+big.substr(0,40),kFloatMin,rTooSmallData,opt);
  test(kFloatMax,kFloatMax_1,rTooLargeData,opt);
  test(kFloatMax,kFloatMax,rSuccess,opf);
  test(big.substr(0,40),kFloatMax_1,rTooLargeData,opt);
  test("12a12.12","12",rInterruptedData,opt);
  test("a12.12","a12.12",rIncorrectData,opt);

  test("-1234.567","-1234.567",rSuccess,opt);
  test("1234.567","1234.567",rSuccess,opt);
  delete opt;
  delete opf;
}

TEST_F(OperateTest, Double) {
  OperateDouble * opt = new OperateDouble(true);
  OperateDouble * opf = new OperateDouble(false);

  test("-"+big,kDoubleMin,rTooSmallData,opt);
  test(kDoubleMax,kDoubleMax_1,rTooLargeData,opt);
  test(kDoubleMax,kDoubleMax,rSuccess,opf);
  test(big,kDoubleMax_1,rTooLargeData,opt);
  test("9."+big,"9."+big,rSuccess,opt);
  test("1234a56789.987654321","1234",rInterruptedData,opt);
  test("a123456789.987654321","a123456789.987654321",rIncorrectData,opt);

  test("-123456789.987654321","-123456789.987654321",rSuccess,opt);
  test("123456789.987654321","123456789.987654321",rSuccess,opt);
  delete opt;
  delete opf;
}

TEST_F(OperateTest, ULong) {
  OperateULong * opt = new OperateULong(true);
  OperateULong * opf = new OperateULong(false);

  test("-"+big,"0",rTooSmallData,opt);
  test(kULongMax, kULongMax_1,rTooLargeData,opt);
  test(kULongMax, kULongMax,rSuccess,opf);
  test(big,kULongMax_1,rTooLargeData, opt);
  test("123456789.987654321","123456789.987654321",rSuccess,opt);
  test("12345a","12345",rInterruptedData,opt);
  test("a123","a123",rIncorrectData,opt);

  test("12345678987654321","12345678987654321",rSuccess,opt);
  delete opt;
  delete opf;
}

TEST_F(OperateTest, SmallInt) {
  OperateSmallInt * opt = new OperateSmallInt(true);
  OperateSmallInt * opf = new OperateSmallInt(false);

  test("-123456789",kSmallIntMin,rTooSmallData,opt);
  test(kSmallIntMax,kSmallIntMax_1,rTooLargeData,opt);
  test(kSmallIntMax,kSmallIntMax,rSuccess,opf);
  test("123456789",kSmallIntMax_1,rTooLargeData,opt);
  test("12345.54321","12345.54321",rSuccess,opt);
  test("123a45","123",rInterruptedData,opt);
  test("a123","a123",rIncorrectData,opt);


  test("-1234","-1234",rSuccess,opt);
  test("1234","1234",rSuccess,opt);
  delete opt;
  delete opf;
}

TEST_F(OperateTest, USmallInt) {
  OperateUSmallInt * opt = new OperateUSmallInt(true);
  OperateUSmallInt * opf = new OperateUSmallInt(false);

  test("-123456789","0",rTooSmallData,opt);
  test(kUSmallIntMax,kUSmallIntMax_1,rTooLargeData,opt);
  test(kUSmallIntMax,kUSmallIntMax,rSuccess,opf);
  test("123456789",kUSmallIntMax_1,rTooLargeData,opt);
  test("1234.4321","1234.4321",rSuccess,opt);
  test("a123","a123",rIncorrectData,opt);
  test("123a12","123",rInterruptedData,opt);

  test("1234","1234",rSuccess,opt);
  delete opt;
  delete opf;
}

TEST_F(OperateTest,Bool) {
  OperateBool * opt = new OperateBool(true);
  OperateBool * opf = new OperateBool(false);

  test("abc","abc",rIncorrectData,opt);

  test("false","false",rSuccess,opt);
  test("true","true",rSuccess,opt);
}

TEST_F(OperateTest, String) {
  OperateString * opt = new OperateString(10, true);
  OperateString * opf = new OperateString(10, false);
  string null_str = "abc";
  null_str[0] = NULL_STRING;
  test(null_str,null_str,rSuccess,opt);
  test(null_str,null_str,rInvalidNullData,opf);
  test("abc","abc",rSuccess,opt);
  string long_str = "123456789987654321";
  test(long_str,long_str.substr(0,10),rTooLongData,opt);
  delete opt;
  delete opf;
}

TEST_F(OperateTest, Decimal) {
  OperateDecimal * opt = new OperateDecimal(6, true);
  OperateDecimal * opf = new OperateDecimal(6, false);

  test("","",rSuccess,opt);
  test("","",rInvalidNullData,opf);
  delete opt;
  delete opf;
}

TEST_F(OperateTest, Date) {
  OperateDate * opt = new OperateDate(true);
  OperateDate * opf = new OperateDate(false);

  test("2015-01-01","2015-01-01",rSuccess,opt);
  test("1000-01-01","1400-01-01",rTooSmallData,opt);
  test("9999-15-01","9999-12-31",rTooLargeData,opt);
  test("99-12-01","99-12-01",rIncorrectData,opt);
  test("","",rSuccess,opt);
  test("","",rInvalidNullData,opf);
  delete opt;
  delete opf;
}

TEST_F(OperateTest, Time) {
  OperateTime * opt = new OperateTime(true);
  OperateTime * opf = new OperateTime(false);

  test("13:02:46.123456","13:02:46.123456",rSuccess,opt);
  test("a3:02:46.123456","a3:02:46.123456",rIncorrectData,opt);
  test("34:23:34.123000","23:59:59.999999",rTooLargeData,opt);
  test("","",rSuccess,opt);
  test("","",rInvalidNullData,opf);
  delete opt;
  delete opf;
}

TEST_F(OperateTest, DateTime) {
  OperateDatetime * opt = new OperateDatetime(true);
  OperateDatetime * opf = new OperateDatetime(false);

  test("1000-01-01 00:00:00.000000","1400-01-01 00:00:00.000000",rTooSmallData,opt);
  test("9999-31-31 23:59:59.999999","9999-12-31 23:59:59.999999",rTooLargeData,opt);
  test("","",rSuccess, opt);
  test("","",rInvalidNullData,opf);
  delete opt;
  delete opf;
}

#endif


