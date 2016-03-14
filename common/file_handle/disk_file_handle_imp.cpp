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
 * /Claims/common/file_handle/disk_handle_imp.cpp
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

#include "./disk_file_handle_imp.h"

#include <glog/logging.h>
#include <stdio.h>
#include <fcntl.h>
#include <string>

#include "./file_handle_imp.h"
#include "../../common/rename.h"
#include "../memory_handle.h"
#include "../../utility/lock_guard.h"
using std::endl;
using std::string;
using claims::utility::LockGuard;

namespace claims {
namespace common {
DiskFileHandleImp::~DiskFileHandleImp() {
  int ret = rSuccess;
  EXEC_AND_ONLY_LOG_ERROR(ret, Close(), "failed to close ");
}

RetCode DiskFileHandleImp::SwitchStatus(FileStatus status_to_be) {
  int old_file_status = file_status_;
  if (kInReading == status_to_be && kInReading != file_status_) {
    Close();
    fd_ = FileOpen(file_name_.c_str(), O_RDONLY, S_IWUSR | S_IRUSR);
  } else if (kInOverWriting == status_to_be) {
    Close();
    fd_ = FileOpen(file_name_.c_str(), O_RDWR | O_TRUNC | O_CREAT,
                   S_IWUSR | S_IRUSR);
  } else if (kInAppending == status_to_be && kInAppending != file_status_) {
    Close();
    fd_ = FileOpen(file_name_.c_str(), O_RDWR | O_CREAT | O_APPEND,
                   S_IWUSR | S_IRUSR);
  } else {
    return rSuccess;
  }
  if (-1 == fd_) {
    PLOG(ERROR) << "failed to reopen file:" << file_name_ << "("
                << file_status_info[old_file_status] << ")  in mode "
                << file_status_info[status_to_be] << " .";
    return rOpenDiskFileFail;
  } else {
    //    can_close_.set_value(1);
    file_status_ = status_to_be;
    LOG(INFO) << "disk file:" << file_name_ << "("
              << file_status_info[old_file_status] << ") is reopened for "
              << file_status_info[file_status_] << endl;
    return rSuccess;
  }
}

RetCode DiskFileHandleImp::Write(const void* buffer, const size_t length) {
  assert(fd_ >= 3);
  assert((kInOverWriting == file_status_ || kInAppending == file_status_) &&
         " files is not opened in writing mode");
  //  RefHolder holder(reference_count_);

  size_t total_write_num = 0;
  while (total_write_num < length) {
    ssize_t write_num =
        write(fd_, static_cast<const char*>(buffer) + total_write_num,
              length - total_write_num);
    if (-1 == write_num) {
      PLOG(ERROR) << "failed to write buffer(" << buffer << ") to file(" << fd_
                  << "): " << file_name_;
      return rWriteDiskFileFail;
    }
    total_write_num += write_num;
  }
  //  if (length > 100) {
  //    DLOG(INFO) << "write " << length << " length data from " << buffer
  //               << " into disk file:" << file_name_ << endl;
  //  } else {
  //    DLOG(INFO) << "write " << length
  //               << " length data :" << static_cast<const char*>(buffer)
  //               << " from " << buffer << " into  disk file:" << file_name_
  //               << endl;
  //  }
  return rSuccess;
}

RetCode DiskFileHandleImp::Close() {
  //  LOG(INFO) << "ref: " << can_close_.get_value();
  //  if (-1 == fd_ || 0 != reference_count_.load()  // someone are still using
  //  this
  //                                                 // file descriptor
  //      || !i_win_to_close_.try_lock()) {  // someone win the lock to close
  if (-1 == fd_) {
    return rSuccess;
  } else if (0 == FileClose(fd_)) {
    LOG(INFO) << "closed file: " << file_name_ << " whose fd is " << fd_
              << endl;
    //    i_win_to_close_.release();
    fd_ = -1;
    file_status_ = kClosed;
    return rSuccess;
  } else {
    //    i_win_to_close_.release();
    int ret = rCloseDiskFileFail;
    EXEC_AND_PLOG(ret, ret, "", "failed to close file:" << file_name_);
    return ret;
  }
}

RetCode DiskFileHandleImp::ReadTotalFile(void*& buffer, size_t* length) {
  int ret = rSuccess;
  //  RefHolder holder(reference_count_);

  EXEC_AND_RETURN_ERROR(ret, SwitchStatus(kInReading),
                        "failed to switch status");

  assert(fd_ >= 3);
  assert(kInReading == file_status_ && " files is not opened in reading mode");
  ssize_t file_length = lseek(fd_, 0, SEEK_END);
  if (-1 == file_length) {
    PLOG(ERROR) << "failed to set pos at the end of (fd: " << fd_
                << ", name: " << file_name_ << ").";
    return rLSeekDiskFileFail;
  }
  LOG(INFO) << "The length of file " << file_name_ << "is " << file_length
            << endl;
  buffer = Malloc(file_length + 1);

  // reset pos to 0
  if (rSuccess != (ret = SetPosition(0))) {
    return ret;
  }
  *length = file_length;

  return Read(buffer, file_length);
}

RetCode DiskFileHandleImp::Read(void* buffer, size_t length) {
  int ret = rSuccess;
  //  RefHolder holder(reference_count_);

  EXEC_AND_RETURN_ERROR(ret, SwitchStatus(kInReading),
                        "failed to switch status");

  assert(fd_ >= 3);
  assert(kInReading == file_status_ && " files is not opened in reading mode");
  ssize_t total_read_num = 0;
  while (total_read_num < length) {
    ssize_t read_num = read(fd_, static_cast<char*>(buffer) + total_read_num,
                            length - total_read_num);
    if (-1 == read_num) {
      LOG(ERROR) << "read file [" << file_name_
                 << "] from disk failed, expected read " << length
                 << " , actually read " << total_read_num << endl;
      return rReadDiskFileFail;
    } else if (0 == read_num) {
      LOG(INFO) << "the position has reached the end of file" << endl;
      return rFileEOF;
    }
    total_read_num += read_num;
  }
  LOG(INFO) << "read total " << total_read_num << " from disk file "
            << file_name_ << endl;
  return ret;
}

RetCode DiskFileHandleImp::SetPosition(size_t pos) {
  assert(fd_ >= 3);
  assert(kInReading == file_status_ &&
         "Seeking is only work for files opened in read-only mode");
  if (-1 == lseek(fd_, pos, SEEK_SET)) {
    PLOG(ERROR) << "failed to lseek at " << pos << " in file(fd:" << fd_ << ", "
                << file_name_ << ")";
    return rLSeekDiskFileFail;
  }
  return rSuccess;
}

RetCode DiskFileHandleImp::Append(const void* buffer, const size_t length) {
  int ret = rSuccess;
  //  RefHolder holder(reference_count_);

  EXEC_AND_RETURN_ERROR(ret, SwitchStatus(kInAppending),
                        "failed to switch status");
  assert(fd_ >= 3);
  assert(kInAppending == file_status_ &&
         " files is not opened in appending mode");
  return Write(buffer, length);
}
RetCode DiskFileHandleImp::AtomicAppend(const void* buffer, const size_t length,
                                        function<void()> lock_func,
                                        function<void()> unlock_func) {
  lock_func();
  RetCode ret = Append(buffer, length);
  unlock_func();
  return ret;
}

RetCode DiskFileHandleImp::OverWrite(const void* buffer, const size_t length) {
  int ret = rSuccess;
  //  RefHolder holder(reference_count_);

  EXEC_AND_RETURN_ERROR(ret, SwitchStatus(kInOverWriting),
                        "failed to switch status");
  assert(fd_ >= 3);
  assert(kInOverWriting == file_status_ &&
         " files is not opened in overwriting mode");

  return Write(buffer, length);
}

RetCode DiskFileHandleImp::AtomicOverWrite(const void* buffer,
                                           const size_t length,
                                           function<void()> lock_func,
                                           function<void()> unlock_func) {
  lock_func();
  RetCode ret = OverWrite(buffer, length);
  unlock_func();
  return ret;
}

RetCode DiskFileHandleImp::DeleteFile() {
  int ret = rSuccess;
  EXEC_AND_LOG(ret, Close(), "closed file name: " << file_name_,
               "failed to close file:" << file_name_);
  if (CanAccess(file_name_)) {
    if (0 != remove(file_name_.c_str())) {
      LOG(ERROR) << "Cannot delete disk file : [" + file_name_ +
                        "] ! Reason: " + strerror(errno) << std::endl;
      return rFailure;
    } else {
      fd_ = -1;
      file_status_ = kClosed;
      LOG(WARNING) << "The file " << file_name_ << "is not exits!\n"
                   << std::endl;
    }
  }
  return rSuccess;
}

}  // namespace common
} /* namespace claims */
