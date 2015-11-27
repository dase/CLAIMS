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
 * /Claims/loader/test/single_file_connector_test.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */
#include "./single_file_connector_test.h"

#include <glog/logging.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <string.h>
#include <ostream>
#include <string>

#include "../../common/memory_handle.h"
#include "../../common/file_handle/file_handle_imp_factory.h"
#include "../file_connector.h"
#include "../single_file_connector.h"

using claims::common::FilePlatform;
using namespace claims::common;

namespace claims {

namespace loader {

SingleFileConnectorTest::SingleFileConnectorTest() {
  connector_ = NULL;
  path_ = "SingleFileConnectorTest";
  data_ = "fafasfffffffffffffffdfsfsffsfsfsfs  a.";
  data_length_ = 38;
  //  snprintf(data_, 38, "fafasfffffffffffffffdfsfsffsfsfsfs  a.");
  LOG(INFO) << "data_: " << data_ << std::endl;
}

void SingleFileConnectorTest::SetUpTestCase() {
  std::cout << "=============" << std::endl;
  LOG(INFO) << "=============" << std::endl;
}

void SingleFileConnectorTest::TearDownTestCase() {}

void SingleFileConnectorTest::WriteOrAppendFile(FilePlatform file_platform,
                                                FileOpenFlag open_flag,
                                                char* expect,
                                                int expect_length) {
  connector_ = new SingleFileConnector(file_platform, path_);
  if (rSuccess != connector_->Open(open_flag)) FAIL();
  if (rSuccess != connector_->Flush(data_, data_length_)) {
    LOG(ERROR) << "failed to flush (" << path_ << ")" << std::endl;
    FAIL();
  }
  if (rSuccess != connector_->Close()) FAIL();
  DELETE_PTR(connector_);

  void* read_buffer = NULL;
  uint64_t length = 0;
  FileConnector* reader = NULL;
  reader = new SingleFileConnector(file_platform, path_);
  reader->Open(kReadFile);
  if (rSuccess != reader->LoadTotalFile(read_buffer, &length)) {
    FAIL();
  }
  EXPECT_EQ(expect_length, length);
  EXPECT_STREQ(expect, static_cast<char*>(read_buffer));
  reader->Close();
  DELETE_PTR(reader);
  SUCCEED();
}
TEST_F(SingleFileConnectorTest, DiskWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, DiskAppend) {
  char double_data[] =
      "fafasfffffffffffffffdfsfsffsfsfsfs  a"
      ".fafasfffffffffffffffdfsfsffsfsfsfs  a.";
  WriteOrAppendFile(kDisk, kAppendFile, double_data, sizeof(double_data) - 1);
}
TEST_F(SingleFileConnectorTest, DiskOverWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, HdfsWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, HdfsAppend) {
  char double_data[] =
      "fafasfffffffffffffffdfsfsffsfsfsfs  a"
      ".fafasfffffffffffffffdfsfsffsfsfsfs  a.";
  WriteOrAppendFile(kHdfs, kAppendFile, double_data, sizeof(double_data) - 1);
}
TEST_F(SingleFileConnectorTest, HdfsOverWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}
}  // namespace loader

}  // namespace claims
