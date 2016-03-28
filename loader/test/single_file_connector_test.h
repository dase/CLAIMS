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
#include <unistd.h>
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
    LOG(INFO) << "data_: " << data_ << std::endl
              << " double_data_: " << double_data_ << std::endl;
  }

  void MultiThreadWriteOrAppend(SingleFileConnector* connector, int length) {
    usleep(length);  // stagger excuting of all threads
    ASSERT_EQ(rSuccess, connector->Open());
    ASSERT_EQ(rSuccess, connector->AtomicFlush(
                            (length % 2 ? data_ : double_data_),
                            (length % 2 ? data_length_ : data_length_ * 2)));
    ASSERT_EQ(rSuccess, connector->Close());
  }

  /*
    void WriteOrAppendFile(FilePlatform file_platform, FileOpenFlag open_flag,
                           char* expect, int expect_length);

    void TryDeleteInUsingFile(FilePlatform file_platform, FileOpenFlag
    open_flag,
                              char* expect, int expect_length);
    void MultiThreadWrite(FilePlatform file_platform, FileOpenFlag open_flag,
                          char* expect, int expect_length);
  */

  void Delete(FilePlatform file_platform) {
    SingleFileConnector* con =
        new SingleFileConnector(file_platform, path_, kAppendFile);
    con->Open();
    con->AtomicFlush(data_, data_length_);
    con->Close();
    DELETE_PTR(con);

    void* buffer;
    uint64_t length = 0;
    con = new SingleFileConnector(file_platform, path_, kReadFile);
    con->Open();
    ASSERT_EQ(rSuccess, con->LoadTotalFile(buffer, &length));
    ASSERT_EQ(rFileInUsing, con->Delete());  // can't delete
    ASSERT_EQ(rSuccess, con->Close());
    ASSERT_EQ(rSuccess, con->Delete());
    DELETE_PTR(con);

    con = new SingleFileConnector(file_platform, path_, kCreateFile);
    con->Open();
    ASSERT_EQ(rSuccess, con->AtomicFlush(data_, data_length_));
    ASSERT_EQ(rFileInUsing, con->Delete());  // can't delete
    ASSERT_EQ(rSuccess, con->Close());
    ASSERT_EQ(rSuccess, con->Delete());
    DELETE_PTR(con);

    con = new SingleFileConnector(file_platform, path_, kAppendFile);
    con->Open();
    ASSERT_EQ(rSuccess, con->AtomicFlush(data_, data_length_));
    ASSERT_EQ(rFileInUsing, con->Delete());  // can't delete
    ASSERT_EQ(rSuccess, con->Close());
    ASSERT_EQ(rSuccess, con->Delete());
    DELETE_PTR(con);
  }

  void WriteOrAppendFile(FilePlatform file_platform, FileOpenFlag open_flag,
                         char* expect, int expect_length) {
    connector_ = new SingleFileConnector(file_platform, path_, open_flag);
    connector_->Open();
    if (rSuccess != connector_->AtomicFlush(data_, data_length_)) {
      LOG(ERROR) << "failed to flush (" << path_ << ")" << std::endl;
      FAIL();
    }
    if (rSuccess != connector_->Close()) FAIL();
    DELETE_PTR(connector_);

    void* read_buffer = NULL;
    uint64_t length = 0;
    SingleFileConnector* reader = NULL;
    reader = new SingleFileConnector(file_platform, path_, kReadFile);
    reader->Open();
    if (rSuccess != reader->LoadTotalFile(read_buffer, &length)) {
      FAIL();
    }
    ASSERT_EQ(expect_length, length);
    ASSERT_STREQ(expect, static_cast<char*>(read_buffer));
    reader->Close();
    DELETE_PTR(reader);
    SUCCEED();
  }

  void TryDeleteInUsingFile(FilePlatform file_platform, FileOpenFlag open_flag,
                            char* expect, int expect_length) {
    SingleFileConnector* con =
        new SingleFileConnector(file_platform, path_, open_flag);
    ASSERT_EQ(rSuccess, con->Delete());
    std::thread t1(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 1);
    std::thread t2(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 2);
    std::thread t3(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 3);
    std::thread t4(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 4);
    std::thread t5([con] { ASSERT_EQ(rFileInUsing, con->Delete()); });
    std::thread t6(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 6);
    DLOG(INFO) << "waiting for all thread finished";
    usleep(100);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    DELETE_PTR(con);

    DLOG(INFO) << "start to read file ";
    SingleFileConnector* reader =
        new SingleFileConnector(file_platform, path_, kReadFile);
    reader->Open();
    void* buffer;
    uint64_t length = 0;
    ASSERT_EQ(rSuccess, reader->LoadTotalFile(buffer, &length));
    ASSERT_EQ(rFileInUsing, reader->Delete());  // can't delete
    ASSERT_EQ(rSuccess, reader->Close());
    ASSERT_EQ(rSuccess, reader->Delete());
    DELETE_PTR(reader);

    ASSERT_EQ(expect_length, length);
    ASSERT_STREQ(expect, static_cast<char*>(buffer));
  }

  void MultiThreadWrite(FilePlatform file_platform, FileOpenFlag open_flag,
                        char* expect, int expect_length) {
    SingleFileConnector* con =
        new SingleFileConnector(file_platform, path_, open_flag);
    ASSERT_EQ(rSuccess, con->Delete());
    std::thread t1(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 1);
    std::thread t2(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 2);
    std::thread t3(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 3);
    std::thread t4(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 4);
    std::thread t5(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this,
                   con, 5);
    DLOG(INFO) << "waiting for all thread finished";
    usleep(100);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    DELETE_PTR(con);

    DLOG(INFO) << "start to read file ";
    SingleFileConnector* reader =
        new SingleFileConnector(file_platform, path_, kReadFile);
    reader->Open();
    void* buffer;
    uint64_t length = 0;
    ASSERT_EQ(rSuccess, reader->LoadTotalFile(buffer, &length));
    ASSERT_EQ(rSuccess, reader->Close());
    DELETE_PTR(reader);

    std::cout << "data:" << data_ << std::endl << " length:" << length
              << std::endl;
    //    EXPECT_TRUE((0 == strcmp(data_, static_cast<char*>(buffer)) &&
    //                 data_length_ == length) ||
    //                (0 == strcmp(double_data_, static_cast<char*>(buffer)) &&
    //                 data_length_ * 2 == length));
    ASSERT_STREQ(expect, static_cast<char*>(buffer));
    ASSERT_EQ(expect_length, length);
  }

  static void SetUpTestCase() {
    std::cout << "=============" << std::endl;
    LOG(INFO) << "=============" << std::endl;
  }

  static void TearDownTestCase() {
    LOG(INFO) << "----------=============--------------" << std::endl;
  }

  virtual void SetUp() { LOG(INFO) << "-----------------------" << std::endl; }
  virtual void TearDown() { LOG(INFO) << "---------------------" << std::endl; }

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
  WriteOrAppendFile(kDisk, kAppendFile, double_data_, data_length_ * 2);
}
TEST_F(SingleFileConnectorTest, DiskOverWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}

TEST_F(SingleFileConnectorTest, SingleThreadDiskDelete) { Delete(kDisk); }

TEST_F(SingleFileConnectorTest, HdfsWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, HdfsAppend) {
  WriteOrAppendFile(kHdfs, kAppendFile, double_data_, data_length_ * 2);
}
TEST_F(SingleFileConnectorTest, HdfsOverWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}
TEST_F(SingleFileConnectorTest, SingleThreadHdfsDelete) { Delete(kHdfs); }

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
