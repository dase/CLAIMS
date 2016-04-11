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
#include <hdfs.h>
#include <vector>
#include <string>

#include "./file_connector.h"
#include "../catalog/table.h"
#include "../common/error_define.h"
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
                                       TableDescriptor* table,
                                       FileOpenFlag open_flag)
    : platform_(platform),
      table_(table),
      write_path_name_(table->GetAllPartitionsPath()),
      ref_(0),
      open_flag_(open_flag) {
  for (auto projection_iter : write_path_name_) {
    vector<FileHandleImp*> projection_files;
    projection_files.clear();
    vector<Lock> projection_locks;
    projection_locks.clear();
    for (auto partition_iter : projection_iter) {
      FileHandleImp* file =
          FileHandleImpFactory::Instance().CreateFileHandleImp(platform_,
                                                               partition_iter);
      projection_files.push_back(file);
      projection_locks.push_back(Lock());
      LOG(INFO)
          << "push file handler which handles " << partition_iter
          << " into projection_files. Now the size of projection_files is "
          << projection_files.size() << std::endl;
    }
    file_handles_.push_back(projection_files);
    write_locks_.push_back(projection_locks);
  }
  LOG(INFO) << "open all " << file_handles_.size() << " file successfully"
            << std::endl;
}

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
  RetCode ret = rSuccess;
  if (0 != ref_) {
    ++ref_;
  } else {
    LockGuard<Lock> guard(open_close_lock_);
    if (0 == ref_) {  // double-check
      for (auto partitions_imp : file_handles_) {
        for (auto imp : partitions_imp) {
          RetCode subret = rSuccess;
          EXEC_AND_ONLY_LOG_ERROR(
              subret, imp->SwitchStatus(
                          static_cast<FileHandleImp::FileStatus>(open_flag_)),
              "failed to open file:" << imp->get_file_name());
          if (rSuccess != subret) ret = subret;  // one failed, all failed
        }
      }
      if (rSuccess == ret) {
        table_->update_lock_.acquire();  // lock to avoid updating table
        ++ref_;
        is_closed = false;
      }
    } else {
      ++ref_;
    }
  }
  return ret;
}

/*RetCode TableFileConnector::Open() {
  for (auto projection_iter : write_path_name_) {
    vector<FileHandleImp*> projection_files;
    projection_files.clear();
    for (auto partition_iter : projection_iter) {
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
}*/
/*

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
*/
/*

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
*/

RetCode TableFileConnector::AtomicFlush(unsigned projection_offset,
                                        unsigned partition_offset,
                                        const void* source, unsigned length) {
  assert(file_handles_.size() != 0 && "make sure file handles is not empty");
  int ret = rSuccess;
  if (FileOpenFlag::kCreateFile == open_flag_) {
    LockGuard<Lock> guard(write_locks_[projection_offset][partition_offset]);
    EXEC_AND_ONLY_LOG_ERROR(
        ret, file_handles_[projection_offset][partition_offset]->OverWrite(
                 source, length),
        "failed to overwrite file.");
  } else if (FileOpenFlag::kAppendFile == open_flag_) {
    LockGuard<Lock> guard(write_locks_[projection_offset][partition_offset]);
    EXEC_AND_ONLY_LOG_ERROR(
        ret, file_handles_[projection_offset][partition_offset]->Append(source,
                                                                        length),
        "failed to append file.");
  } else {
    assert(false && "Can't flush a file opened with read mode");
    return common::rFailure;
  }
  return ret;
}

RetCode TableFileConnector::Close() {
  assert(file_handles_.size() != 0 && "make sure file handles is not empty");

  int ret = rSuccess;
  if (!is_closed && 0 == (--ref_)) {
    LockGuard<Lock> guard(open_close_lock_);
    if (0 == ref_ && !is_closed) {
      for (int i = 0; i < file_handles_.size(); ++i) {
        for (int j = 0; j < file_handles_[i].size(); ++j) {
          RetCode subret = rSuccess;
          EXEC_AND_ONLY_LOG_ERROR(subret, file_handles_[i][j]->Close(),
                                  "failed to close " << write_path_name_[i][j]);
          if (rSuccess != subret) ret = subret;
        }
      }
      if (rSuccess == ret) {
        is_closed = true;
        table_->update_lock_.release();  // now table can update its catalog.
        LOG(INFO) << "closed all file handles" << std::endl;
      }
    }
  }
  assert(ref_ >= 0);
  return ret;
}

RetCode TableFileConnector::DeleteAllTableFiles() {
  /*
   * this method may be called after creating table and before creating
   * projection, so file_handles_ may be empty
   */
  //  assert(!(0 == table_->row_number_ && file_handles_.size() != 0) &&
  //         "make sure file handles is not empty");

  RetCode ret = rSuccess;
  if (0 != ref_) {
    EXEC_AND_RETURN_ERROR(ret, common::rFileInUsing, "now reference is "
                                                         << ref_);
  }
  LockGuard<Lock> guard(open_close_lock_);
  // must double-check in case of deleting a file in using
  if (0 == ref_ && is_closed) {
    for (auto prj_files : file_handles_) {
      for (auto file : prj_files) {
        RetCode subret = rSuccess;
        EXEC_AND_ONLY_LOG_ERROR(subret, file->DeleteFile(),
                                "failed to delete file "
                                    << file->get_file_name());
        if (rSuccess != subret) ret = subret;
      }
    }
    if (rSuccess == ret) {
      LOG(INFO) << "deleted all files" << std::endl;
    }
  } else {
    ret = common::rFileInUsing;
    EXEC_AND_RETURN_ERROR(ret, ret, "now reference is " << ref_);
  }
  return ret;
}

RetCode TableFileConnector::UpdateWithNewProj() {
  int proj_index = table_->projection_list_.size() - 1;
  vector<string> prj_write_path;
  vector<Lock> prj_locks;
  vector<FileHandleImp*> prj_imps;
  for (int j = 0; j < table_->projection_list_[proj_index]
                          ->getPartitioner()
                          ->getNumberOfPartitions();
       ++j) {
    string path =
        PartitionID(table_->getProjectoin(proj_index)->getProjectionID(), j)
            .getPathAndName();

    prj_write_path.push_back(path);
    prj_locks.push_back(Lock());
    prj_imps.push_back(
        FileHandleImpFactory::Instance().CreateFileHandleImp(platform_, path));
  }
  write_path_name_.push_back(prj_write_path);
  write_locks_.push_back(prj_locks);
  file_handles_.push_back(prj_imps);
  return rSuccess;
}

} /* namespace loader */
} /* namespace claims */
