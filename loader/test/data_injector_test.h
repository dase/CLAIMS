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
 * /Claims/loader/test/data_injector_test.h
 *
 *  Created on: Oct 30, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef LOADER_TEST_DATA_INJECTOR_TEST_H_
#define LOADER_TEST_DATA_INJECTOR_TEST_H_

#include <gtest/gtest.h>
#include <iosfwd>
#include <string>
#include "../../loader/data_injector.h"
#include "../../catalog/catalog.h"

using std::ofstream;
using claims::catalog::Catalog;

namespace claims {
namespace loader {

class DataInjectorTest : public ::testing::Test {
 public:
  DataInjectorTest() {}
  virtual ~DataInjectorTest() {}

 public:
  //  Client client_;
  std::string ip_;
  int port_;
};

TEST_F(DataInjectorTest, GetTuple1) {
  char* data[2] = {"sf|sfs||sfssfs|\n", "sfsf\fw|fsfegbf,s,nn|fwnelweh\n"};

  ofstream ofs("test_for_DI_GetTuple");
  for (int i = 0; i < 2; ++i) ofs << data[i];

  ifstream ifs("test_for_DI_GetTuple");
  string res;
  string ter("\n");
  int i = 0;
  while (DataInjector::GetTupleTerminatedBy(ifs, res, ter)) {
    EXPECT_STREQ(data[i++], res.c_str());
    res.clear();
  }
}

TEST_F(DataInjectorTest, GetTuple2) {
  char* data[2] = {"sf|sfs||sfssfs|\n", "sfsf\fw|fsfegbf,s,nn|fwnelweh\n|\n"};

  ofstream ofs("test_for_DI_GetTuple");
  for (int i = 0; i < 2; ++i) ofs << data[i];

  ifstream ifs("test_for_DI_GetTuple");
  string res;
  string ter = "|\n";
  int i = 0;
  while (DataInjector::GetTupleTerminatedBy(ifs, res, ter)) {
    EXPECT_STREQ(data[i++], res.c_str());
    res.clear();
  }
}
TEST_F(DataInjectorTest, GetTuple3) {
  char* data[3] = {"sf|sfs||sfssfs|\n", "sfsf\fw|fsfegbf,s,nn|fwnelweh\n|\n",
                   "|\n"};

  ofstream ofs("test_for_DI_GetTuple");
  for (int i = 0; i < 3; ++i) ofs << data[i];

  ifstream ifs("test_for_DI_GetTuple");
  string res;
  string ter = "|\n";
  int i = 0;
  while (DataInjector::GetTupleTerminatedBy(ifs, res, ter)) {
    EXPECT_STREQ(data[i++], res.c_str());
    res.clear();
  }
}

TEST_F(DataInjectorTest, GetTuple4) {
  char* data[4] = {"sf|sfs||sfssfs||||",
                   "sfsf\fw|fsfe|||gbf,s,nn|fwnelweh\n||||", "|\n||||", "sfsf"};

  ofstream ofs("test_for_DI_GetTuple");
  for (int i = 0; i < 4; ++i) ofs << data[i];

  ifstream ifs("test_for_DI_GetTuple");
  string res;
  string ter = "||||";
  int i = 0;
  while (DataInjector::GetTupleTerminatedBy(ifs, res, ter)) {
    EXPECT_STREQ(data[i++], res.c_str());
    res.clear();
  }
}
} /* namespace loader */
} /* namespace claims */

#endif  // LOADER_TEST_DATA_INJECTOR_TEST_H_
