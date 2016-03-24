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
#include <thread>  //NOLINT
#include "../../common/file_handle/file_handle_imp_factory.h"
#include "../../common/memory_handle.h"
#include "../file_connector.h"
#include "../single_file_connector.h"

namespace claims {

namespace loader {
using std::thread;
using namespace claims::common;  // NOLINT

class SingleFileConnector;

class SingleFileConnectorTest : public ::testing::Test {
 public:
  SingleFileConnectorTest()
      : connector_(NULL), path_("SingleFileConnectorTest") {
    double_data_ =
        "fafasfffffffffffffffdfsfsffsfsfsfs  a"
        ".fafasfffffffffffffffdfsfsffsfsfsfs  a.";
    data_length_ = 38;
    //  snprintf(data_, 38, "fafasfffffffffffffffdfsfsffsfsfsfs  a.");
    LOG(INFO) << "data_: " << data_ << std::endl;
  }

  void WriteOrAppendFile(FilePlatform file_platform, FileOpenFlag open_flag,
                         char* expect, int expect_length);

  void MultiThreadWriteOrAppend(SingleFileConnector* connector, int length) {
    EXPECT_EQ(rSuccess, connector->Open());
    EXPECT_EQ(rSuccess, connector->AtomicFlush(
                            (length % 2 ? data_ : double_data_),
                            (length % 2 ? data_length_ : data_length_ * 2)));
    EXPECT_EQ(rSuccess, connector->Close());
  }

  void TryDeleteInUsingFile(FilePlatform file_platform, FileOpenFlag open_flag,
                            char* expect, int expect_length);
  void MultiThreadWrite(FilePlatform file_platform, FileOpenFlag open_flag,
                        char* expect, int expect_length);

  static void SetUpTestCase() {
    std::cout << "=============" << std::endl;
    LOG(INFO) << "=============" << std::endl;
  }

  static void TearDownTestCase() {}

 public:
  SingleFileConnector* connector_ = NULL;
  std::string path_;
  char* data_ = "fafasfffffffffffffffdfsfsffsfsfsfs  a.";
  char* double_data_;
  int data_length_ = 38;
};
TEST_F(SingleFileConnectorTest, DiskWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, DiskAppend) {
  WriteOrAppendFile(kDisk, kAppendFile, double_data_, data_length_ * 2 - 1);
}
TEST_F(SingleFileConnectorTest, DiskOverWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, HdfsWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, HdfsAppend) {
  WriteOrAppendFile(kHdfs, kAppendFile, double_data_, data_length_ * 2 - 1);
}
TEST_F(SingleFileConnectorTest, HdfsOverWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}

TEST_F(SingleFileConnectorTest, DiskMultiThreadWrite) {
  MultiThreadWrite(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, HdfsMultiThreadWrite) {
  MultiThreadWrite(kHdfs, kCreateFile, data_, data_length_);
}

TEST_F(SingleFileConnectorTest, DiskTryDeleteInUsingFile) {
  TryDeleteInUsingFile(kDisk, kCreateFile, double_data_, data_length_ * 2);
}
TEST_F(SingleFileConnectorTest, HdfsTryDeleteInUsingFile) {
  TryDeleteInUsingFile(kHdfs, kCreateFile, double_data_, data_length_ * 2);
}

}  // namespace loader

}  // namespace claims

#endif  // LOADER_TEST_SINGLE_FILE_CONNECTOR_TEST_H_
