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
    imp_ = FileHandleImpFactory::Instance().CreateFileHandleImp(kDisk);
    std::cout << "=============" << std::endl;
  }
  static void TearDownTestCase() { DELETE_PTR(imp_); }

 public:
  static FileHandleImp* imp_;

  static string file_name_;
};
string DiskFileHandleImpTest::file_name_ = "DiskFileHandleImpTest";
FileHandleImp* DiskFileHandleImpTest::imp_ = NULL;

TEST_F(DiskFileHandleImpTest, TestAccess1) {
  bool ret = imp_->CanAccess(file_name_);
  EXPECT_FALSE(ret);
}

TEST_F(DiskFileHandleImpTest, TestAccess2) {
  imp_->Open(file_name_, kCreateFile);
  bool ret = imp_->CanAccess(file_name_);
  EXPECT_TRUE(ret);
  int res = imp_->Close();
  EXPECT_EQ(rSuccess, res);
}

TEST_F(DiskFileHandleImpTest, Write) {
  imp_->Open(file_name_, kCreateFile);
  char* buffer = "abc";
  char* data = static_cast<char*>(Malloc(4));
  int ret = rSuccess;
  if (rSuccess != (imp_->Write(buffer, 3)) ||
      rSuccess != (imp_->SetPosition(0)) ||
      rSuccess != (ret = imp_->Read(data, 3)) ||
      rSuccess != (ret = imp_->Close())) {
    FAIL();
  }
  EXPECT_STREQ("abc", data);
}

TEST_F(DiskFileHandleImpTest, Append) {
  imp_->Open(file_name_, kAppendFile);
  char* buffer = "abc";
  char* data = static_cast<char*>(Malloc(7));
  int ret = rSuccess;
  if (rSuccess != (imp_->Write(buffer, 3)) || rSuccess != (imp_->Close()) ||
      rSuccess != (imp_->Open(file_name_, kReadFile)) ||
      rSuccess != (imp_->SetPosition(0)) ||
      rSuccess != (ret = imp_->Read(data, 6)) ||
      rSuccess != (ret = imp_->Close())) {
    FAIL();
  }
  EXPECT_STREQ("abcabc", data);
}

TEST_F(DiskFileHandleImpTest, Read) {
  imp_->Open(file_name_, kReadFile);
  char* data = static_cast<char*>(Malloc(7));
  int ret = rSuccess;
  if (rSuccess != (ret = imp_->Read(data, 6)) ||
      rSuccess != (ret = imp_->Close())) {
    FAIL();
  }
  EXPECT_STREQ("abcabc", data);
}

TEST_F(DiskFileHandleImpTest, ReadTotalFile) {
  imp_->Open(file_name_, kAppendFile);
  char* buffer = "abc";
  void* data = NULL;
  size_t a = 0;
  int ret = rSuccess;
  if (rSuccess != (imp_->Write(buffer, 3)) || rSuccess != (imp_->Close()) ||
      rSuccess != (imp_->Open(file_name_, kReadFile)) ||
      rSuccess != (ret = imp_->ReadTotalFile(data, &a)) ||
      rSuccess != (ret = imp_->Close())) {
    FAIL();
  }
  EXPECT_STREQ("abcabcabc", static_cast<char*>(data));
}

TEST_F(DiskFileHandleImpTest, PositionalRead) {
  imp_->Open(file_name_, kReadFile);
  char* buffer = "abc";
  char* data = static_cast<char*>(Malloc(4));
  size_t a = 0;
  int ret = rSuccess;
  if (rSuccess != (ret = imp_->SetPosition(5)) ||
      rSuccess != (ret = imp_->Read(data, 3)) ||
      rSuccess != (ret = imp_->Close())) {
    FAIL();
  }
  EXPECT_STREQ("cab", data);
}

TEST_F(DiskFileHandleImpTest, OverWrite) {
  imp_->Open(file_name_, kCreateFile);
  char* buffer = "abc";
  void* data = NULL;
  size_t a = 0;
  int ret = rSuccess;
  if (rSuccess != (imp_->Write(buffer, 3)) || rSuccess != (imp_->Close()) ||
      rSuccess != (imp_->Open(file_name_, kReadFile)) ||
      rSuccess != (ret = imp_->ReadTotalFile(data, &a)) ||
      rSuccess != (ret = imp_->Close())) {
    FAIL();
  }
  EXPECT_STREQ("abc", static_cast<char*>(data));
}

}  // namespace common

}  // namespace claims

#endif  // COMMON_FILE_HANDLE_TEST_DISK_FILE_HANDLE_IMP_TEST_H_
