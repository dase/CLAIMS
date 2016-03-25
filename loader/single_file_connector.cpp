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
 * /Claims/loader/SingleFileConnector.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./single_file_connector.h"
#include <string>
#include "../utility/lock_guard.h"

using claims::utility::LockGuard;

namespace claims {
namespace loader {

SingleFileConnector::SingleFileConnector(FilePlatform platform,
                                         string file_name,
                                         FileOpenFlag open_flag)
    : platform_(platform),
      file_name_(file_name),
      is_closed(true),
      open_flag_(open_flag) {
  imp_ = common::FileHandleImpFactory::Instance().CreateFileHandleImp(
      platform_, file_name_);
  if (common::FileOpenFlag::kCreateFile == open_flag_) {
    flush_function = std::bind(&FileHandleImp::OverWrite, imp_,
                               std::placeholders::_1, std::placeholders::_2);
  } else if (common::FileOpenFlag::kAppendFile == open_flag_) {
    flush_function = std::bind(&FileHandleImp::Append, imp_,
                               std::placeholders::_1, std::placeholders::_2);
  } else {
    flush_function = [](const void* source, const size_t length) -> RetCode {
      assert(false && "Can't flush a file opened with read mode");
      return common::rFailure;
    };
  }
}

RetCode SingleFileConnector::Open() {
  RetCode ret = rSuccess;
  if (0 != ref_) {
    ++ref_;
  } else {
    LockGuard<SpineLock> guard(open_close_lcok_);
    if (0 == ref_) {
      EXEC_AND_RETURN_ERROR(
          ret, imp_->SwitchStatus(
                   static_cast<FileHandleImp::FileStatus>(open_flag_)),
          "failed to open file:" << file_name_);
      ++ref_;
      is_closed = false;
    }
  }
  return ret;
}

RetCode SingleFileConnector::Close() {
  RetCode ret = rSuccess;
  if (!is_closed && 0 == (--ref_)) {
    LockGuard<SpineLock> guard(open_close_lcok_);
    if (0 == ref_ && !is_closed) {
      EXEC_AND_RETURN_ERROR(ret, imp_->Close(), "file name: " << file_name_);
      is_closed = true;
    }
  }
  return ret;
}

RetCode SingleFileConnector::Delete() {
  RetCode ret = rSuccess;
  if (0 != ref_) {
    ret = common::rFileInUsing;
    EXEC_AND_RETURN_ERROR(ret, ret, "file name: " << file_name_);
  }
  LockGuard<SpineLock> guard(open_close_lcok_);
  EXEC_AND_RETURN_ERROR(ret, imp_->DeleteFile(), "failed to delete file "
                                                     << file_name_);
  is_closed = true;
  return ret;
}

} /* namespace loader */
} /* namespace claims */
