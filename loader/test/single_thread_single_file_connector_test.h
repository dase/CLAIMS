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
 * /Claims/loader/test/single_thread_single_file_connector_test.h
 *
 *  Created on: Mar 23, 2016
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef LOADER_TEST_SINGLE_THREAD_SINGLE_FILE_CONNECTOR_TEST_H_
#define LOADER_TEST_SINGLE_THREAD_SINGLE_FILE_CONNECTOR_TEST_H_

#include <gtest/gtest.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <ostream>

#include "../../common/memory_handle.h"
#include "../single_thread_single_file_connector.h"

namespace claims {
namespace loader {
using namespace claims::common;  // NOLINT

class SingleThreadSingleFileConnectorTest : public ::testing::Test {
 public:
  SingleThreadSingleFileConnectorTest()
      : connector_(NULL), path_("SingleFileConnectorTest") {
    double_data_ =
        "fafasfffffffffffffffdfsfsffsfsfsfs  a"
        ".fafasfffffffffffffffdfsfsffsfsfsfs  a.";
    data_length_ = 38;
    //  snprintf(data_, 38, "fafasfffffffffffffffdfsfsffsfsfsfs  a.");
    LOG(INFO) << "data_: " << data_ << std::endl;
  }

  void WriteOrAppendFile(FilePlatform file_platform, FileOpenFlag open_flag,
                         char* expect, int expect_length) {
    connector_ = new SingleThreadSingleFileConnector(file_platform, path_);
    if (open_flag == kCreateFile) {
      if (rSuccess != connector_->Overwrite(data_, data_length_)) {
        LOG(ERROR) << "failed to flush (" << path_ << ")" << std::endl;
        FAIL();
      }
    } else if (kAppendFile == open_flag) {
      if (rSuccess != connector_->Append(data_, data_length_)) {
        LOG(ERROR) << "failed to append (" << path_ << ")" << std::endl;
        FAIL();
      }
    }
    if (rSuccess != connector_->Close()) FAIL();
    DELETE_PTR(connector_);

    void* read_buffer = NULL;
    uint64_t length = 0;
    SingleThreadSingleFileConnector* reader =
        new SingleThreadSingleFileConnector(file_platform, path_);
    if (rSuccess != reader->LoadTotalFile(read_buffer, &length)) {
      FAIL();
    }
    EXPECT_EQ(expect_length, length);
    EXPECT_STREQ(expect, static_cast<char*>(read_buffer));
    reader->Close();
    DELETE_PTR(reader);

    SUCCEED();
  }

  void EasyTranslate(FilePlatform platform) {
    void* read_buffer = NULL;
    uint64_t length = 0;
    connector_ = new SingleThreadSingleFileConnector(platform, path_);
    EXPECT_EQ(rSuccess, connector_->Delete());
    // test access
    EXPECT_EQ(false, connector_->CanAccess());

    // overwrite
    EXPECT_EQ(rSuccess, connector_->Overwrite(data_, data_length_));
    EXPECT_EQ(rSuccess, connector_->LoadTotalFile(read_buffer, &length));
    EXPECT_STREQ(data_, static_cast<char*>(read_buffer));
    EXPECT_EQ(data_length_, length);

    // test access
    EXPECT_EQ(true, connector_->CanAccess());

    // append
    EXPECT_EQ(rSuccess, connector_->Append(data_, data_length_));
    EXPECT_EQ(rSuccess, connector_->LoadTotalFile(read_buffer, &length));
    EXPECT_STREQ(double_data_, static_cast<char*>(read_buffer));
    EXPECT_EQ(data_length_ * 2, length);

    // close
    EXPECT_EQ(rSuccess, connector_->Close());
    // duplicate close
    EXPECT_EQ(rSuccess, connector_->Close());

    // overwrite after closed
    EXPECT_EQ(rSuccess, connector_->Overwrite(data_, data_length_));
    EXPECT_EQ(rSuccess, connector_->LoadTotalFile(read_buffer, &length));
    EXPECT_STREQ(data_, static_cast<char*>(read_buffer));
    EXPECT_EQ(data_length_, length);

    // overwrite again
    EXPECT_EQ(rSuccess, connector_->Overwrite(data_, data_length_));
    EXPECT_EQ(rSuccess, connector_->LoadTotalFile(read_buffer, &length));
    EXPECT_STREQ(data_, static_cast<char*>(read_buffer));
    EXPECT_EQ(data_length_, length);

    // close
    EXPECT_EQ(rSuccess, connector_->Close());

    // delete file
    EXPECT_EQ(rSuccess, connector_->Delete());

    // test access
    EXPECT_EQ(false, connector_->CanAccess());

    // double delete file
    EXPECT_EQ(rSuccess, connector_->Delete());
  }

  static void SetUpTestCase() {
    std::cout << "=============" << std::endl;
    LOG(INFO) << "=============" << std::endl;
  }

  static void TearDownTestCase() {}
  virtual void SetUp() { LOG(INFO) << "-----------------------" << std::endl; }
  virtual void TearDown() { LOG(INFO) << "---------------------" << std::endl; }

 public:
  SingleThreadSingleFileConnector* connector_ = NULL;
  std::string path_;
  char* data_ = "fafasfffffffffffffffdfsfsffsfsfsfs  a.";
  char* double_data_;
  int data_length_ = 38;
};
TEST_F(SingleThreadSingleFileConnectorTest, DiskWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(SingleThreadSingleFileConnectorTest, DiskAppend) {
  WriteOrAppendFile(kDisk, kAppendFile, double_data_, data_length_ * 2);
}
TEST_F(SingleThreadSingleFileConnectorTest, DiskOverWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(SingleThreadSingleFileConnectorTest, HdfsWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}
TEST_F(SingleThreadSingleFileConnectorTest, HdfsAppend) {
  WriteOrAppendFile(kHdfs, kAppendFile, double_data_, data_length_ * 2);
}
TEST_F(SingleThreadSingleFileConnectorTest, HdfsOverWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}

TEST_F(SingleThreadSingleFileConnectorTest, DiskEeayTranslate) {
  EasyTranslate(kDisk);
}

TEST_F(SingleThreadSingleFileConnectorTest, HdfsEeayTranslate) {
  EasyTranslate(kHdfs);
}

}  // namespace loader
}  // namespace claims
#endif  // LOADER_TEST_SINGLE_THREAD_SINGLE_FILE_CONNECTOR_TEST_H_
