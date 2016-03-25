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
using namespace claims::common;  // NOLINT

namespace claims {

namespace loader {

// void SingleFileConnectorTest::WriteOrAppendFile(FilePlatform file_platform,
//                                                FileOpenFlag open_flag,
//                                                char* expect,
//                                                int expect_length) {
//  connector_ = new SingleFileConnector(file_platform, path_);
//  if (open_flag == kCreateFile) {
//    if (rSuccess != connector_->AtomicFlush(data_, data_length_)) {
//      LOG(ERROR) << "failed to flush (" << path_ << ")" << std::endl;
//      FAIL();
//    }
//  }
//  if (rSuccess != connector_->Close()) FAIL();
//  DELETE_PTR(connector_);
//
//  void* read_buffer = NULL;
//  uint64_t length = 0;
//  SingleFileConnector* reader = NULL;
//  reader = new SingleFileConnector(file_platform, path_);
//  if (rSuccess != reader->LoadTotalFile(read_buffer, &length)) {
//    FAIL();
//  }
//  EXPECT_EQ(expect_length, length);
//  EXPECT_STREQ(expect, static_cast<char*>(read_buffer));
//  reader->Close();
//  DELETE_PTR(reader);
//  SUCCEED();
//}

/*
void SingleFileConnectorTest::WriteOrAppendFile(FilePlatform file_platform,
                                                FileOpenFlag open_flag,
                                                char* expect,
                                                int expect_length) {
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
  EXPECT_EQ(expect_length, length);
  EXPECT_STREQ(expect, static_cast<char*>(read_buffer));
  reader->Close();
  DELETE_PTR(reader);
  SUCCEED();
}

void SingleFileConnectorTest::TryDeleteInUsingFile(FilePlatform file_platform,
                                                   FileOpenFlag open_flag,
                                                   char* expect,
                                                   int expect_length) {
  SingleFileConnector* con =
      new SingleFileConnector(file_platform, path_, open_flag);
  EXPECT_EQ(rSuccess, con->Delete());
  std::thread t1(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 1);
  std::thread t2(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 2);
  std::thread t3(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 3);
  std::thread t4(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 4);
  std::thread t5([con] { assert(rFileInUsing == con->Delete()); });
  std::thread t6(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 6);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  DELETE_PTR(con);

  SingleFileConnector* reader =
      new SingleFileConnector(kDisk, path_, kReadFile);
  reader->Open();
  void* buffer;
  uint64_t length = 0;
  EXPECT_EQ(rSuccess, reader->LoadTotalFile(buffer, &length));
  EXPECT_EQ(rFileInUsing, con->Delete());  // can't delete
  EXPECT_EQ(rSuccess, con->Close());
  EXPECT_EQ(rSuccess, con->Delete());
  DELETE_PTR(reader);

  EXPECT_EQ(expect_length, length);
  EXPECT_STREQ(expect, static_cast<char*>(buffer));
}

void SingleFileConnectorTest::MultiThreadWrite(FilePlatform file_platform,
                                               FileOpenFlag open_flag,
                                               char* expect,
                                               int expect_length) {
  SingleFileConnector* con =
      new SingleFileConnector(file_platform, path_, open_flag);
  EXPECT_EQ(rSuccess, con->Delete());
  std::thread t1(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 1);
  std::thread t2(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 2);
  std::thread t3(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 3);
  std::thread t4(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 4);
  std::thread t5(&SingleFileConnectorTest::MultiThreadWriteOrAppend, this, con,
                 5);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  DELETE_PTR(con);

  SingleFileConnector* reader =
      new SingleFileConnector(kDisk, path_, kReadFile);
  reader->Open();
  void* buffer;
  uint64_t length = 0;
  EXPECT_EQ(rSuccess, reader->LoadTotalFile(buffer, &length));
  EXPECT_EQ(rSuccess, reader->Close());
  DELETE_PTR(reader);

  EXPECT_STREQ(expect, static_cast<char*>(buffer));
  EXPECT_EQ(expect_length, length);
}
*/

}  // namespace loader

}  // namespace claims
