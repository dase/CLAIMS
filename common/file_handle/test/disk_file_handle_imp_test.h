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
 * /Claims/common/file_handle/test/disk_file_handle_imp_test.h
 *
 *  Created on: Oct 27, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef COMMON_FILE_HANDLE_TEST_DISK_FILE_HANDLE_IMP_TEST_H_
#define COMMON_FILE_HANDLE_TEST_DISK_FILE_HANDLE_IMP_TEST_H_

#include <stdio.h>
#include <gtest/gtest.h>
#include <string>

#include "../file_handle_imp.h"
#include "../disk_file_handle_imp.h"
#include "../file_handle_imp_factory.h"

#include "../../memory_handle.h"
using std::string;
namespace claims {

namespace common {
class DiskFileHandleImpTest : public ::testing::Test {
 public:
  static void SetUpTestCase() {
    //    file_name_ = "DiskFileHandleImpTest";
    imp_ =
        FileHandleImpFactory::Instance().CreateFileHandleImp(kDisk, file_name_);
    std::cout << "=============" << std::endl;
  }
  static void TearDownTestCase() { DELETE_PTR(imp_); }

 public:
  static FileHandleImp* imp_;

  static string file_name_;
  char* buffer = "abc";
};
string DiskFileHandleImpTest::file_name_ =
    "/home/imdb/data/yk/DiskFileHandleImpTest";
FileHandleImp* DiskFileHandleImpTest::imp_ = NULL;
TEST_F(DiskFileHandleImpTest, TestAccess1) {
  int ret = rSuccess;
  EXEC_AND_LOG(ret, imp_->DeleteFile(), "deleted file ",
               "failed to delete file");
  ret = imp_->CanAccess(file_name_);
  EXPECT_FALSE(ret);
}

TEST_F(DiskFileHandleImpTest, TestAccess2) {
  imp_->Append(buffer, 3);
  bool ret = imp_->CanAccess(file_name_);
  EXPECT_TRUE(ret);
}

TEST_F(DiskFileHandleImpTest, Delete) {
  imp_->OverWrite(buffer, 3);
  bool ret = imp_->CanAccess(file_name_);
  EXPECT_TRUE(ret);
  EXPECT_EQ(rSuccess, imp_->DeleteFile());
  EXPECT_FALSE(imp_->CanAccess(file_name_));
}

TEST_F(DiskFileHandleImpTest, DeleteNonExistFile) {
  EXPECT_EQ(rSuccess, imp_->DeleteFile());
}

TEST_F(DiskFileHandleImpTest, ReadNonExistFile) {
  void* data = NULL;
  size_t length = 0;
  EXPECT_EQ(rSuccess, imp_->DeleteFile());
  EXPECT_EQ(rOpenDiskFileFail, imp_->ReadTotalFile(data, &length));
}

TEST_F(DiskFileHandleImpTest, Write) {
  char* data = static_cast<char*>(Malloc(4));
  int ret = rSuccess;
  EXPECT_EQ(rSuccess, imp_->OverWrite(buffer, 3));
  EXPECT_EQ(rSuccess, imp_->PRead(data, 3, 0));
  EXPECT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abc", data);
}

TEST_F(DiskFileHandleImpTest, Append) {
  void* data = static_cast<char*>(Malloc(7));
  uint64_t length;
  int ret = rSuccess;
  EXPECT_EQ(rSuccess, imp_->Append(buffer, 3));
  EXPECT_EQ(rSuccess, imp_->ReadTotalFile(data, &length));
  EXPECT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abcabc", (char*)data);
}

TEST_F(DiskFileHandleImpTest, Read) {
  char* data = static_cast<char*>(Malloc(7));
  int ret = rSuccess;
  EXPECT_EQ(rSuccess, imp_->Read(data, 6));
  EXPECT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abcabc", data);
}

TEST_F(DiskFileHandleImpTest, ReadTotalFile) {
  void* data = NULL;
  size_t a = 0;
  int ret = rSuccess;
  EXPECT_EQ(rSuccess, imp_->Append(buffer, 3));
  EXPECT_EQ(rSuccess, imp_->ReadTotalFile(data, &a));
  EXPECT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abcabcabc", static_cast<char*>(data));
}

TEST_F(DiskFileHandleImpTest, PositionalRead) {
  char* data = static_cast<char*>(Malloc(4));
  size_t a = 0;
  int ret = rSuccess;
  EXPECT_EQ(rSuccess, imp_->PRead(data, 3, 5));
  EXPECT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("cab", data);
}

TEST_F(DiskFileHandleImpTest, OverWrite) {
  void* data = NULL;
  size_t a = 0;
  int ret = rSuccess;
  EXPECT_EQ(rSuccess, imp_->OverWrite(buffer, 3));
  EXPECT_EQ(rSuccess, imp_->ReadTotalFile(data, &a));
  EXPECT_EQ(rSuccess, imp_->Close());
  EXPECT_STREQ("abc", static_cast<char*>(data));
}

TEST_F(DiskFileHandleImpTest, AppendIntoNonExistFile) {
  void* data = NULL;
  size_t a = 0;
  int ret = rSuccess;
  EXPECT_EQ(rSuccess, imp_->DeleteFile());
  EXPECT_EQ(rSuccess, imp_->Append(buffer, 3));
  EXPECT_EQ(rSuccess, imp_->ReadTotalFile(data, &a));
  EXPECT_STREQ("abc", static_cast<char*>(data));
}

TEST_F(DiskFileHandleImpTest, GetName) {
  EXPECT_STREQ(file_name_.c_str(), imp_->get_file_name().c_str());
}
TEST_F(DiskFileHandleImpTest, AtomicAppend) {}

}  // namespace common
}  // namespace claims

#endif  // COMMON_FILE_HANDLE_TEST_DISK_FILE_HANDLE_IMP_TEST_H_
