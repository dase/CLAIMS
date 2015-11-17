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
#include <fcntl.h>
#include <string>

#include "./file_handle_imp.h"
#include "../../common/rename.h"
#include "../memory_handle.h"
namespace claims {
namespace common {

using std::endl;
using std::string;

DiskFileHandleImp::~DiskFileHandleImp() {
  int ret = Close();
  if (ret != 0) LOG(ERROR) << "failed to close file fd. ret:" << ret << endl;
}

RetCode DiskFileHandleImp::Open(string file_name, FileOpenFlag open_flag) {
  file_name_ = file_name;
  int ret = rSuccess;
  if (false == CanAccess(file_name_)) {
    ret = rAccessDiskFileFail;
    ELOG(ret, "File name:" << file_name_);
    return ret;
  }
  if (kCreateFile == open_flag) {
    fd_ = FileOpen(file_name_.c_str(), O_RDWR | O_TRUNC | O_CREAT,
                   S_IWUSR | S_IRUSR);
  } else if (kAppendFile == open_flag) {
    fd_ = FileOpen(file_name_.c_str(), O_RDWR | O_CREAT | O_APPEND,
                   S_IWUSR | S_IRUSR);
  } else if (kReadFile == open_flag) {
    fd_ = FileOpen(file_name_.c_str(), O_RDONLY, S_IWUSR | S_IRUSR);
  } else {
    LOG(ERROR) << "parameter flag:" << open_flag << " is invalid" << endl;
    return rParamInvalid;
  }

  if (-1 == fd_) {
    PLOG(ERROR) << "failed to open file :" << file_name_ << ".";
    return rOpenDiskFileFail;
  } else {
    LOG(INFO) << "opened disk file:" << file_name_ << "with "
              << (kCreateFile == open_flag
                      ? "kCreateFile"
                      : kAppendFile == open_flag ? "kAppendFile" : "kReadFile")
              << endl;
    return rSuccess;
  }
}

RetCode DiskFileHandleImp::Write(const void* buffer, const size_t length) {
  assert(fd_ >= 3);
  size_t total_write_num = 0;
  while (total_write_num < length) {
    ssize_t write_num =
        write(fd_, static_cast<const char*>(buffer) + total_write_num,
              length - total_write_num);
    if (-1 == write_num) {
      PLOG(ERROR) << "failed to write buffer(" << buffer << ") to file(" << fd_
                  << "): " << file_name_ << endl;
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
  if (-1 == fd_) {
    return rSuccess;
  } else if (0 == FileClose(fd_)) {
    LOG(INFO) << "closed file: " << file_name_ << " whose fd is " << fd_
              << endl;
    fd_ = -1;
    return rSuccess;
  } else {
    return rCloseDiskFileFail;
  }
}

RetCode DiskFileHandleImp::ReadTotalFile(void*& buffer, size_t* length) {
  assert(fd_ >= 3);
  int ret = rSuccess;
  ssize_t file_length = lseek(fd_, 0, SEEK_END);
  if (-1 == file_length) {
    PLOG(ERROR) << "lseek called on fd to set pos to the end of file " << fd_
                << " failed : ";
    return rLSeekDiskFileFail;
  }
  LOG(INFO) << "The length of file " << file_name_ << "is " << file_length
            << endl;
  buffer = Malloc(file_length + 1);

  // reset pos to 0
  if (rSuccess != (ret = SetPosition(0))) {
    return ret;
  }

  ssize_t read_num = read(fd_, buffer, file_length);
  LOG(INFO) << "read " << read_num << " from disk file " << file_name_ << endl;

  if (read_num != file_length) {
    LOG(ERROR) << "read file [" << file_name_
               << "] from disk failed, expected read " << file_length
               << " , actually read " << read_num << endl;
    return rReadDiskFileFail;
  }
  *length = read_num;
  return rSuccess;
}

RetCode DiskFileHandleImp::Read(void* buffer, size_t length) {
  assert(fd_ >= 3);
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
  return rSuccess;
}

RetCode DiskFileHandleImp::SetPosition(size_t pos) {
  assert(fd_ >= 3);
  if (-1 == lseek(fd_, pos, SEEK_SET)) {
    PLOG(ERROR) << "failed to lseek at " << pos << " in file(fd:" << fd_ << ", "
                << file_name_ << ")";
    return rLSeekDiskFileFail;
  }
  return rSuccess;
}
}  // namespace common
} /* namespace claims */
