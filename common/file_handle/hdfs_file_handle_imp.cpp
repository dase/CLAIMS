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
 * /Claims/common/file_handle/hdfs_file_handle_imp.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

//// this macro decides whether write DLOG message into log file.
//// Open means no DLOG message.
//// it will also prohibit assert()
// #define NDEBUG

#include "./hdfs_file_handle_imp.h"
#include <glog/logging.h>
#include <string>

#include "../../Config.h"
#include "../memory_handle.h"
#include "../../utility/lock_guard.h"

using std::endl;
using claims::utility::LockGuard;

namespace claims {
namespace common {

RetCode HdfsFileHandleImp::SwitchStatus(FileStatus status_to_be) {
  int old_file_status = file_status_;
  if (kInReading == status_to_be && kInReading != file_status_) {
    Close();
    file_ = hdfsOpenFile(fs_, file_name_.c_str(), O_RDONLY, 0, 0, 0);
  } else if (kInOverWriting == status_to_be) {
    Close();
    file_ = hdfsOpenFile(fs_, file_name_.c_str(), O_WRONLY, 0, 0, 0);
  } else if (kInAppending == status_to_be && kInAppending != file_status_) {
    Close();
    if (!CanAccess(file_name_)) {  // this file doesn't exist, create one
      file_ = hdfsOpenFile(fs_, file_name_.c_str(), O_WRONLY, 0, 0, 0);
      if (NULL == file_) {
        PLOG(ERROR) << "failed to create hdfs file :" << file_name_;
        return rOpenHdfsFileFail;
      } else {
        LOG(INFO) << "created hdfs file :" << file_name_ << endl;
        if (0 != hdfsCloseFile(fs_, file_)) {
          LOG(ERROR) << "failed to close hdfs file: " << file_name_ << endl;
          return rCloseHdfsFileFail;
        }
      }
    }
    file_ = hdfsOpenFile(fs_, file_name_.c_str(), O_WRONLY | O_APPEND, 0, 0, 0);
  } else {
    return rSuccess;
  }

  if (NULL == file_) {
    PLOG(ERROR) << "failed to reopen file:" << file_name_ << "("
                << file_status_info[file_status_] << ")  in mode "
                << file_status_info[status_to_be] << " .";
    return rOpenHdfsFileFail;
  } else {
    //    can_close_.set_value(1);
    file_status_ = status_to_be;
    LOG(INFO) << "HDFS file:" << file_name_ << "("
              << file_status_info[old_file_status] << ") is reopened for "
              << file_status_info[file_status_] << endl;
    return rSuccess;
  }
}

RetCode HdfsFileHandleImp::Write(const void* buffer, const size_t length) {
  assert(NULL != fs_ && "failed to connect hdfs");
  assert(NULL != file_ && "make sure file is opened");
  //  RefHolder holder(reference_count_);

  size_t total_write_num = 0;
  while (total_write_num < length) {
    int32_t write_num = hdfsWrite(
        fs_, file_, static_cast<const char*>(buffer) + total_write_num,
        length - total_write_num);
    if (-1 == write_num) {
      PLOG(ERROR) << "failed to write buffer(" << buffer
                  << ") to file: " << file_name_ << endl;
      return rWriteDiskFileFail;
    }
    total_write_num += write_num;
  }
  //  if (length > 100) {
  //    DLOG(INFO) << "write " << length << " length data from " << buffer
  //               << " into hdfs file:" << file_name_ << endl;
  //  } else {
  //    DLOG(INFO) << "write " << length
  //               << " length data :" << static_cast<const char*>(buffer)
  //               << " from " << buffer << " into  hdfs file:" << file_name_
  //               << endl;
  //  }
  return rSuccess;
}

RetCode HdfsFileHandleImp::Close() {
  if (NULL == file_) {
    LOG(INFO) << "hdfs file:" << file_name_ << " have been closed " << endl;
    return rSuccess;
  }
  assert(NULL != fs_ && "failed to connect hdfs");
  //  if (0 != reference_count_  // someone are still using this file descriptor
  //      || !i_win_to_close_.try_lock())  // someone win the lock to close
  //    return rSuccess;

  if (0 != hdfsCloseFile(fs_, file_)) {
    PLOG(ERROR) << "failed to close hdfs file: " << file_name_;
    return rCloseHdfsFileFail;
  }
  file_ = NULL;
  file_status_ = kClosed;
  LOG(INFO) << "hdfs file: " << file_name_ << " is closed " << endl;
  return rSuccess;
}

RetCode HdfsFileHandleImp::ReadTotalFile(void*& buffer, size_t* length) {
  assert(NULL != fs_ && "failed to connect hdfs");
  //  RefHolder holder(reference_count_);
  int ret = rSuccess;
  EXEC_AND_RETURN_ERROR(ret, SwitchStatus(kInReading),
                        "failed to switch status");

  assert(NULL != fs_ && "failed to connect hdfs");
  assert(NULL != file_ && "make sure file is opened");

  hdfsFileInfo* hdfsfile = hdfsGetPathInfo(fs_, file_name_.c_str());
  if (NULL == hdfsfile) {
    PLOG(ERROR) << "failed to open file :" << file_name_ << " in mode"
                << file_status_info[kInReading] << " .";
    return rOpenHdfsFileFail;
  }
  int file_length = hdfsfile->mSize;
  LOG(INFO) << "The length of file " << file_name_ << " is " << file_length
            << endl;
  buffer = Malloc(file_length + 1);
  // set position 0
  if (rSuccess != (ret = SetPosition(0))) {
    return ret;
  }
  *length = file_length;

  return Read(buffer, file_length);
}

RetCode HdfsFileHandleImp::Read(void* buffer, size_t length) {
  assert(NULL != fs_ && "failed to connect hdfs");
  //  RefHolder holder(reference_count_);

  int ret = rSuccess;
  EXEC_AND_RETURN_ERROR(ret, SwitchStatus(kInReading),
                        "failed to switch status");

  assert(NULL != fs_ && "failed to connect hdfs");
  assert(NULL != file_ && "make sure file is opened");
  int total_read_num = 0;
  while (total_read_num < length) {
    int read_num =
        hdfsRead(fs_, file_, static_cast<char*>(buffer) + total_read_num,
                 length - total_read_num);
    if (-1 == read_num) {
      LOG(ERROR) << "failed to read " << length << "from file:" << file_name_
                 << endl;
      return rReadHdfsFileFail;
    }
    total_read_num += read_num;
  }
  LOG(INFO) << "read total " << total_read_num << " from hdfs file "
            << file_name_ << endl;
  return rSuccess;
}

RetCode HdfsFileHandleImp::SetPosition(size_t pos) {
  assert(NULL != fs_ && "failed to connect hdfs");
  assert(NULL != file_ && "make sure file is opened");

  assert(kInReading == file_status_ &&
         "Seeking is only work for files opened in read-only mode");
  int ret = hdfsSeek(fs_, file_, pos);
  if (0 != ret) {
    PLOG(ERROR) << "failed to seek to " << pos << " in " << file_name_
                << " file" << endl;
    return rLSeekHdfsFileFail;
  }

  LOG(INFO) << "Seek to " << pos << "in " << file_name_ << " file" << endl;
  return rSuccess;
}

RetCode HdfsFileHandleImp::Append(const void* buffer, const size_t length) {
  //  RefHolder holder(reference_count_);

  int ret = rSuccess;
  EXEC_AND_RETURN_ERROR(ret, SwitchStatus(kInAppending),
                        "failed to switch status");

  return Write(buffer, length);
}

RetCode HdfsFileHandleImp::OverWrite(const void* buffer, const size_t length) {
  //  RefHolder holder(reference_count_);
  int ret = rSuccess;
  EXEC_AND_RETURN_ERROR(ret, SwitchStatus(kInOverWriting),
                        "failed to switch status");
  return Write(buffer, length);
}

RetCode HdfsFileHandleImp::DeleteFile() {
  assert(NULL != fs_ && "failed to connect hdfs");

  int ret = rSuccess;
  EXEC_AND_ONLY_LOG_ERROR(ret, Close(), "file name: " << file_name_);
  if (CanAccess(file_name_)) {
    if (0 != hdfsDelete(fs_, file_name_.c_str())) {
      LOG(ERROR) << "Failed to delete file : [" + file_name_ + "]."
                 << std::endl;
      return rFailure;
    }
  } else {
    file_ = NULL;
    file_status_ = kClosed;
    LOG(WARNING) << "The file " << file_name_ << "is not exits!\n" << std::endl;
  }
  return rSuccess;
}

}  // namespace common
} /* namespace claims */
