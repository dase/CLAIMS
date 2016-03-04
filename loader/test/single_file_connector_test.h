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
 * /Claims/loader/test/single_file_connector_test.h
 *
 *  Created on: Oct 28, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef LOADER_TEST_SINGLE_FILE_CONNECTOR_TEST_H_
#define LOADER_TEST_SINGLE_FILE_CONNECTOR_TEST_H_

#include <gtest/gtest.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <ostream>

#include "../../common/file_handle/file_handle_imp_factory.h"
#include "../../common/memory_handle.h"

namespace claims {

namespace loader {

using claims::common::FilePlatform;
using claims::common::FileOpenFlag;

class SingleFileConnector;

class SingleFileConnectorTest : public ::testing::Test {
 public:
  SingleFileConnectorTest()
      : connector_(NULL), path_("SingleFileConnectorTest") {
    double_data =
        "fafasfffffffffffffffdfsfsffsfsfsfs  a"
        ".fafasfffffffffffffffdfsfsffsfsfsfs  a.";
    data_length_ = 38;
    //  snprintf(data_, 38, "fafasfffffffffffffffdfsfsffsfsfsfs  a.");
    LOG(INFO) << "data_: " << data_ << std::endl;
  }

  static void SetUpTestCase();
  static void TearDownTestCase();

  void WriteOrAppendFile(FilePlatform file_platform, FileOpenFlag open_flag,
                         char* expect, int expect_length);

 public:
  SingleFileConnector* connector_ = NULL;
  std::string path_;
  char* data_ = "fafasfffffffffffffffdfsfsffsfsfsfs  a.";
  char* double_data;
  int data_length_ = 38;
};

}  // namespace loader

}  // namespace claims

#endif  // LOADER_TEST_SINGLE_FILE_CONNECTOR_TEST_H_
