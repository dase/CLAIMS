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
 * /Claims/loader/table_file_connector.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./table_file_connector.h"
#include <vector>
#include <string>
#include <hdfs.h>

#include "./file_connector.h"
#include "../catalog/table.h"
#include "../common/file_handle/file_handle_imp.h"
#include "../common/file_handle/file_handle_imp_factory.h"
#include "../common/memory_handle.h"

using claims::common::FilePlatform;
using claims::common::FileHandleImpFactory;
using claims::common::FileOpenFlag;
using claims::common::rSuccess;
using claims::common::FileHandleImp;
using claims::common::FilePlatform;
using std::vector;
using std::string;

namespace claims {
namespace loader {

TableFileConnector::TableFileConnector(FilePlatform platform,
                                       TableDescriptor* table)
    : FileConnector(platform),
      table_(table),
      write_path_name_(table->GetAllPartitionsPath()) {}

// TableFileConnector::TableFileConnector(FilePlatform platform,
//                                       TableDescriptor* table)
//    : TableFileConnector(platform, table->GetAllPartitionsPath()) {}

// TableFileConnector::TableFileConnector(FilePlatform platform,
//                                       vector<vector<string>> writepath)
//    : FileConnector(platform), write_path_name_(writepath) {
//  imp_ = FileHandleImpFactory::Instance().CreateFileHandleImp(platform_);
//}

TableFileConnector::~TableFileConnector() {
  Close();
  for (auto proj_iter : file_handles_) {
    for (auto part_iter : proj_iter) {
      DELETE_PTR(part_iter);
    }
  }
  LOG(INFO) << "closed all hdfs file of table" << std::endl;
  //  DELETE_PTR(imp_);
}

RetCode TableFileConnector::Open() {
  for (auto projection_iter : write_path_name_) {
    vector<FileHandleImp*> projection_files;
    projection_files.clear();
    for (auto partition_iter : projection_iter) {
      //      if (FileOpenFlag::kCreateFile == open_flag_ &&
      //          rSuccess != imp_->CanAccess(partition_iter)) {
      //        LOG(WARNING) << "The  file " << partition_iter
      //                     << " is already exits! It will be override!\n";
      //      }
      FileHandleImp* file =
          FileHandleImpFactory::Instance().CreateFileHandleImp(platform_,
                                                               partition_iter);
      projection_files.push_back(file);
      LOG(INFO)
          << "push file handler which handles " << partition_iter
          << " into projection_files. Now the size of projection_files is "
          << projection_files.size() << std::endl;
    }
    file_handles_.push_back(projection_files);
  }
  LOG(INFO) << "open all  file successfully" << std::endl;
  return rSuccess;
}

RetCode TableFileConnector::Flush(unsigned projection_offset,
                                  unsigned partition_offset, const void* source,
                                  unsigned length, bool overwrite = false) {
  assert(file_handles_.size() != 0 && "make sure file handles is not empty");
  int ret = rSuccess;
  if (overwrite)
    EXEC_AND_ONLY_LOG_ERROR(
        ret, file_handles_[projection_offset][partition_offset]->OverWrite(
                 source, length),
        "failed to overwrite file.");
  else
    EXEC_AND_ONLY_LOG_ERROR(
        ret, file_handles_[projection_offset][partition_offset]->Append(source,
                                                                        length),
        "failed to append file.");
  return ret;
}

RetCode TableFileConnector::AtomicFlush(unsigned projection_offset,
                                        unsigned partition_offset,
                                        const void* source, unsigned length,
                                        function<void()> lock_func,
                                        function<void()> unlock_func,
                                        bool overwrite = false) {
  assert(file_handles_.size() != 0 && "make sure file handles is not empty");
  int ret = rSuccess;
  if (overwrite)
    EXEC_AND_ONLY_LOG_ERROR(
        ret,
        file_handles_[projection_offset][partition_offset]->AtomicOverWrite(
            source, length, lock_func, unlock_func),
        "failed to overwrite file.");
  else
    EXEC_AND_ONLY_LOG_ERROR(
        ret, file_handles_[projection_offset][partition_offset]->AtomicAppend(
                 source, length, lock_func, unlock_func),
        "failed to append file.");
  return ret;
}

RetCode TableFileConnector::Close() {
  int ret = rSuccess;
  for (int i = 0; i < file_handles_.size(); ++i)
    for (int j = 0; j < file_handles_[i].size(); ++j)
      EXEC_AND_ONLY_LOG_ERROR(ret, file_handles_[i][j]->Close(),
                              "failed to close " << write_path_name_[i][j]
                                                 << ". ");
  if (rSuccess == ret) LOG(INFO) << "closed all file handles" << std::endl;
  return ret;
}

RetCode TableFileConnector::DeleteAllTableFiles() {
  RetCode ret = rSuccess;
  for (auto prj_files : file_handles_)
    for (auto file : prj_files)
      EXEC_AND_ONLY_LOG_ERROR(ret, file->DeleteFile(),
                              "failed to delete file "
                                  << file->get_file_name());

  return ret;
}

} /* namespace loader */
} /* namespace claims */
