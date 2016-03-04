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
 * /Claims/loader/test/table_file_connector_test.h
 *
 *  Created on: Oct 28, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef LOADER_TEST_TABLE_FILE_CONNECTOR_TEST_H_
#define LOADER_TEST_TABLE_FILE_CONNECTOR_TEST_H_
#include <gtest/gtest.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <ostream>

#include "../../common/file_handle/file_handle_imp_factory.h"
namespace claims {
namespace loader {
using std::vector;
using std::endl;
using std::string;
class TableFileConnector;

class TableFileConnectorTest : public ::testing::Test {
 public:
  TableFileConnectorTest();

  static void SetUpTestCase();
  static void TearDownTestCase();

  void WriteOrAppendFile(common::FilePlatform file_platform,
                         common::FileOpenFlag open_flag, char* expect,
                         int expect_length);

 public:
  TableFileConnector* connector_;
  vector<vector<string>> paths_;

  string table_name = "sfdfsf";
  char* data_;
  int data_length_;
};

}  // namespace loader

}  // namespace claims

#endif  // LOADER_TEST_TABLE_FILE_CONNECTOR_TEST_H_
