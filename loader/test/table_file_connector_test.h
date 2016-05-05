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
#include "../../common/memory_handle.h"
#include "../file_connector.h"
#include "../single_file_connector.h"
#include "../table_file_connector.h"

#include "../../catalog/catalog.h"
#include "../../catalog/table.h"
#include "../../common/error_define.h"

#include "../../Daemon/Daemon.h"
#include "../../Daemon/Executing.h"
namespace claims {
namespace loader {
using std::vector;
using std::endl;
using std::string;
using claims::catalog::Catalog;

using namespace claims::common;  // NOLINT

class TableFileConnectorTest : public ::testing::Test {
 public:
  TableFileConnectorTest() {
    connector_ = NULL;
    eightfold_data_ =
        "fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  a.";
    sevenfold_data_ =
        "fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  "
        "a.fafasfffffffffffffffdfsfsffsfsfsfs  a.";
    double_data_ =
        "fafasfffffffffffffffdfsfsffsfsfsfs  a"
        ".fafasfffffffffffffffdfsfsffsfsfsfs  a.";
    data_length_ = 38;
    LOG(INFO) << "data_: " << data_ << std::endl;

    // creation of table and projection is done in elastic_iterator_model_test.h
    string create_table_stmt =
        "create table " + table_name + " (a int , b varchar(12));";
    string create_prj_stmt1 = "create projection on " + table_name +
                              " (a  , b ) number = 2 partitioned on a ;";
    string create_prj_stmt2 = "create projection on " + table_name +
                              " (a ) number = 3 partitioned on a ;";
    RetCode ret = rSuccess;
    EXEC_AND_ONLY_LOG_ERROR(ret, Catalog::getInstance()->restoreCatalog(),
                            "failed to restore catalog");
  }

  static void SetUpTestCase() {
    std::cout << "=============" << std::endl;
    LOG(INFO) << "=============" << std::endl;
  }
  static void TearDownTestCase() {}

  void WriteOrAppendFile(common::FilePlatform file_platform,
                         common::FileOpenFlag open_flag, char* expect,
                         int expect_length) {
    TableDescriptor* table = Catalog::getInstance()->getTable(table_name);
    assert(table != NULL);
    connector_ = new TableFileConnector(file_platform, table, open_flag);

    connector_->Open();
    for (int i = 0; i < table->getNumberOfProjection(); ++i) {
      for (int j = 0; j < (*table->GetProjectionList())[i]
                              ->getPartitioner()
                              ->getNumberOfPartitions();
           ++j) {
        if (rSuccess != connector_->AtomicFlush(i, j, data_, data_length_)) {
          LOG(ERROR) << "failed to flush (" << i << "," << j << ")" << endl;
          FAIL();
        }
      }
    }
    if (rSuccess != connector_->Close()) FAIL();
    DELETE_PTR(connector_);

    SingleFileConnector* reader = NULL;
    void* read_buffer = NULL;
    uint64_t length = 0;

    vector<vector<string>> write_path_name_ = table->GetAllPartitionsPath();
    for (auto projection_iter : write_path_name_) {
      for (auto partition_iter : projection_iter) {
        reader =
            new SingleFileConnector(file_platform, partition_iter, kReadFile);
        if (rSuccess != reader->LoadTotalFile(read_buffer, &length)) {
          FAIL();
        }
        EXPECT_EQ(expect_length, length);
        EXPECT_STREQ(expect, static_cast<char*>(read_buffer));
        reader->Close();
        DELETE_PTR(reader);
      }
    }
    SUCCEED();
  }

  void Delete(FilePlatform file_platform) {
    TableFileConnector* con = new TableFileConnector(
        file_platform, Catalog::getInstance()->getTable(table_name),
        kAppendFile);
    con->Open();
    con->AtomicFlush(0, 0, data_, data_length_);
    con->Close();
    DELETE_PTR(con);

    void* buffer;
    uint64_t length = 0;
    con = new TableFileConnector(
        file_platform, Catalog::getInstance()->getTable(table_name), kReadFile);
    con->Open();
    //    ASSERT_EQ(rSuccess, con->LoadTotalFile(buffer, &length));
    ASSERT_EQ(rFileInUsing, con->DeleteAllTableFiles());  // can't delete
    ASSERT_EQ(rSuccess, con->Close());
    ASSERT_EQ(rSuccess, con->DeleteAllTableFiles());
    DELETE_PTR(con);

    con = new TableFileConnector(file_platform,
                                 Catalog::getInstance()->getTable(table_name),
                                 kCreateFile);
    con->Open();
    ASSERT_EQ(rSuccess, con->AtomicFlush(0, 0, data_, data_length_));
    ASSERT_EQ(rFileInUsing, con->DeleteAllTableFiles());  // can't delete
    ASSERT_EQ(rSuccess, con->Close());
    ASSERT_EQ(rSuccess, con->DeleteAllTableFiles());
    DELETE_PTR(con);

    con = new TableFileConnector(file_platform,
                                 Catalog::getInstance()->getTable(table_name),
                                 kAppendFile);
    con->Open();
    ASSERT_EQ(rSuccess, con->AtomicFlush(1, 0, data_, data_length_));
    ASSERT_EQ(rFileInUsing, con->DeleteAllTableFiles());  // can't delete
    ASSERT_EQ(rSuccess, con->Close());
    ASSERT_EQ(rSuccess, con->DeleteAllTableFiles());
    DELETE_PTR(con);
  }

  void MultiThreadWriteOrAppend(TableFileConnector* connector, int length,
                                int i, int j) {
    usleep(length * 2);  // stagger executing of all threads
    ASSERT_EQ(rSuccess, connector->Open());
    ASSERT_EQ(rSuccess, connector->AtomicFlush(
                            i, j, (length % 2 ? data_ : double_data_),
                            (length % 2 ? data_length_ : data_length_ * 2)));
    ASSERT_EQ(rSuccess, connector->Close());
  }

  void MultiThreadWrite(FilePlatform file_platform, FileOpenFlag open_flag,
                        char* expect, int expect_length) {
    TableFileConnector* con = new TableFileConnector(
        file_platform, Catalog::getInstance()->getTable(table_name),
        kAppendFile);
    ASSERT_EQ(rSuccess, con->DeleteAllTableFiles());
  }

  virtual void SetUp() { LOG(INFO) << "-----------------------" << std::endl; }
  virtual void TearDown() { LOG(INFO) << "---------------------" << std::endl; }

 public:
  TableFileConnector* connector_;
  vector<vector<string>> paths_;

  string table_name = "sfdfsf";
  char* data_;
  char* double_data_;
  char* sevenfold_data_;
  char* eightfold_data_;
  int data_length_ = 38;
};

TEST_F(TableFileConnectorTest, DiskWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(TableFileConnectorTest, DiskAppend) {
  char double_data[] =
      "fafasfffffffffffffffdfsfsffsfsfsfs  "
      "a.fafasfffffffffffffffdfsfsffsfsfsfs  a.";
  WriteOrAppendFile(kDisk, kAppendFile, double_data, sizeof(double_data) - 1);
}
TEST_F(TableFileConnectorTest, DiskOverWrite) {
  WriteOrAppendFile(kDisk, kCreateFile, data_, data_length_);
}
TEST_F(TableFileConnectorTest, HdfsWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}
TEST_F(TableFileConnectorTest, HdfsAppend) {
  char double_data[] =
      "fafasfffffffffffffffdfsfsffsfsfsfs  "
      "a.fafasfffffffffffffffdfsfsffsfsfsfs  a.";
  WriteOrAppendFile(kHdfs, kAppendFile, double_data, sizeof(double_data) - 1);
}
TEST_F(TableFileConnectorTest, HdfsOverWrite) {
  WriteOrAppendFile(kHdfs, kCreateFile, data_, data_length_);
}

}  // namespace loader

}  // namespace claims

#endif  // LOADER_TEST_TABLE_FILE_CONNECTOR_TEST_H_
