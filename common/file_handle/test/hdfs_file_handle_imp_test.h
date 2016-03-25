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
 * /Claims/common/file_handle/test/hdfs_file_handle_imp_test.h
 *
 *  Created on: Oct 27, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef COMMON_FILE_HANDLE_TEST_HDFS_FILE_HANDLE_IMP_TEST_H_
#define COMMON_FILE_HANDLE_TEST_HDFS_FILE_HANDLE_IMP_TEST_H_
#include <stdio.h>
#include <gtest/gtest.h>
#include <string>

#include "../file_handle_imp.h"
#include "../hdfs_file_handle_imp.h"
#include "../file_handle_imp_factory.h"

#include "../../memory_handle.h"
#include "../../../Config.h"
namespace claims {

namespace common {

class HdfsFileHandleImpTest : public ::testing::Test {
 public:
  static void SetUpTestCase() {
    Config::getInstance();
    imp_ =
        FileHandleImpFactory::Instance().CreateFileHandleImp(kHdfs, file_name_);
    std::cout << "=============" << std::endl;
  }
  static void TearDownTestCase() { DELETE_PTR(imp_); }

 public:
  static FileHandleImp* imp_;
  static string file_name_;
  char* buffer = "abc";
};

string HdfsFileHandleImpTest::file_name_ =  // NOLINT
    "/home/imdb/data/yk/HdfsFileHandleImpTest";
FileHandleImp* HdfsFileHandleImpTest::imp_ = NULL;
TEST_F(HdfsFileHandleImpTest, TestAccess1) {
  ASSERT_EQ(rSuccess, imp_->DeleteFile());
  ASSERT_FALSE(imp_->CanAccess(file_name_));
}

TEST_F(HdfsFileHandleImpTest, TestAccess2) {
  assert(rSuccess == imp_->Append(buffer, 3));
  ASSERT_TRUE(imp_->CanAccess(file_name_));
}

TEST_F(HdfsFileHandleImpTest, Delete) {
  ASSERT_EQ(rSuccess, imp_->OverWrite(buffer, 3));
  ASSERT_TRUE(imp_->CanAccess(file_name_));
  ASSERT_EQ(rSuccess, imp_->DeleteFile());
  ASSERT_FALSE(imp_->CanAccess(file_name_));
}

TEST_F(HdfsFileHandleImpTest, DeleteNonExistFile) {
  ASSERT_EQ(rSuccess, imp_->DeleteFile());
}

TEST_F(HdfsFileHandleImpTest, ReadNonExistFile) {
  void* data = NULL;
  size_t length = 0;
  ASSERT_EQ(rSuccess, imp_->DeleteFile());
  ASSERT_EQ(rOpenHdfsFileFail, imp_->ReadTotalFile(data, &length));
}

TEST_F(HdfsFileHandleImpTest, Write) {
  char* data = static_cast<char*>(Malloc(4));
  int ret = rSuccess;
  ASSERT_EQ(rSuccess, imp_->OverWrite(buffer, 3));
  ASSERT_EQ(rSuccess, imp_->PRead(data, 3, 0));
  ASSERT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abc", data);
}

TEST_F(HdfsFileHandleImpTest, Append) {
  void* data = static_cast<char*>(Malloc(7));
  uint64_t length;
  int ret = rSuccess;
  ASSERT_EQ(rSuccess, imp_->Append(buffer, 3));
  ASSERT_EQ(rSuccess, imp_->ReadTotalFile(data, &length));
  ASSERT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abcabc", (char*)data);
}

TEST_F(HdfsFileHandleImpTest, Read) {
  char* data = static_cast<char*>(Malloc(7));
  int ret = rSuccess;
  ASSERT_EQ(rSuccess, imp_->Read(data, 6));
  ASSERT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abcabc", data);
}

TEST_F(HdfsFileHandleImpTest, ReadTotalFile) {
  void* data = NULL;
  size_t a = 0;
  int ret = rSuccess;
  ASSERT_EQ(rSuccess, imp_->Append(buffer, 3));
  ASSERT_EQ(rSuccess, imp_->ReadTotalFile(data, &a));
  ASSERT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abcabcabc", static_cast<char*>(data));
}

TEST_F(HdfsFileHandleImpTest, PositionalRead) {
  char* data = static_cast<char*>(Malloc(4));
  size_t a = 0;
  int ret = rSuccess;
  ASSERT_EQ(rSuccess, imp_->PRead(data, 3, 5));
  ASSERT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("cab", data);
}

TEST_F(HdfsFileHandleImpTest, OverWrite) {
  void* data = NULL;
  size_t a = 0;
  int ret = rSuccess;
  ASSERT_EQ(rSuccess, imp_->OverWrite(buffer, 3));
  ASSERT_EQ(rSuccess, imp_->ReadTotalFile(data, &a));
  ASSERT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abc", static_cast<char*>(data));
}

TEST_F(HdfsFileHandleImpTest, AppendIntoNonExistFile) {
  void* data = NULL;
  size_t a = 0;
  int ret = rSuccess;
  ASSERT_EQ(rSuccess, imp_->DeleteFile());
  ASSERT_EQ(rSuccess, imp_->Append(buffer, 3));
  ASSERT_EQ(rSuccess, imp_->ReadTotalFile(data, &a));
  EXPECT_STREQ("abc", static_cast<char*>(data));
}

TEST_F(HdfsFileHandleImpTest, GetName) {
  EXPECT_STREQ(file_name_.c_str(), imp_->get_file_name().c_str());
}

TEST_F(HdfsFileHandleImpTest, AtomicAppend) {}

}  // namespace common
}  // namespace claims

#endif  // COMMON_FILE_HANDLE_TEST_HDFS_FILE_HANDLE_IMP_TEST_H_
