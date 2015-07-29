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
 * ErrorNoTest.h
 *
 *  Created on: Jul 29, 2015
 *      Author: yukai
 *		 Email: yukai2014@gmail.com
 * 
 * Description:
 *
 */

#include <gtest/gtest.h>
#include <iostream>
#include "../../common/ErrorNo.h"
using std::cout;
using std::endl;
using namespace claims::common;

class ErrorNoTest : public ::testing::Test{
 protected:
   static void SetUpTestCase() {
     cout<<"start new test case"<<endl;
   }
   static void TearDownTestCase() {
     cout<<"finish new test case"<<endl;
   }
};

TEST_F(ErrorNoTest, A) {
  int errerno = C_TYPE_ERROR;

//  cout<<errerno<<" , "<<CStrError(errerno)<<endl;
  const char *res = CStrError(errerno);
  EXPECT_STREQ("Type error", res);
}

TEST_F(ErrorNoTest, B) {
  int errerno = -3;

//  cout<<errerno<<" , "<<CStrError(errerno)<<endl;
  const char *res = CStrError(errerno);
  EXPECT_STREQ("Unknown Error", res);
}

TEST_F(ErrorNoTest, C) {
  int errerno = C_SUCCESS;

//  cout<<errerno<<" , "<<CStrError(errerno)<<endl;
  const char *res = CStrError(errerno);
  EXPECT_STREQ("Success", res);
}
