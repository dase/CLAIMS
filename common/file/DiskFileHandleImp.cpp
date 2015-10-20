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
 * /Claims/Loader/DiskHandleImp.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./DiskFileHandleImp.h"

#include <glog/logging.h>
#include <fcntl.h>
#include <string>
using std::endl;
namespace claims {
namespace common {

RetCode DiskFileHandleImp::Open(FileOpenFlag flag) {
  if (kCreateFile == flag) {
    fd_ = FileOpen(file_name_.c_str(), O_RDWR | O_TRUNC | O_CREAT,
                   S_IWUSR | S_IRUSR);
  } else if (kAppendFile == flag) {
    fd_ = FileOpen(file_name_.c_str(), O_RDWR | O_CREAT | O_APPEND,
                   S_IWUSR | S_IRUSR);
  } else {
    LOG(ERROR) << "parameter flag:" << flag << " is invalid" << endl;
    return EParamInvalid;
  }

  if (-1 == fd_) {
    PLOG(ERROR) << "failed to open file :" << file_name_;
    return EOpenDiskFileFail;
  } else {
    LOG(INFO) << "opened disk file:" << file_name_ << endl;
    return kSuccess;
  }
}

RetCode DiskFileHandleImp::Write(const void* buffer, const size_t length) {
  size_t total_write_num = 0;
  while (total_write_num < length) {
    ssize_t write_num = write(fd_, static_cast<char*>(buffer) + total_write_num,
                              length - total_write_num);
    if (-1 == write_num) {
      PLOG(ERROR) << "failed to write to file(" << fd_ << "): " << file_name_
                  << endl;
      return EWriteDiskFileFail;
    }
    total_write_num += write_num;
  }
  LOG(INFO) << "write " << length << " length data from " << buffer << " into "
            << file_name_ << endl;
  return kSuccess;
}

RetCode DiskFileHandleImp::Close() {
  if (-1 != fd_ && 0 == FileClose(fd_))
    return kSuccess;
  else
    return ECloseDiskFileFail;
}

RetCode DiskFileHandleImp::ReadTotalFile(void*& buffer, size_t* length) {
  ssize_t file_length = lseek(fd_, 0, SEEK_END);
  if (-1 == file_length) {
    PLOG(ERROR) << "lseek called on fd to set pos to the end of file " << fd_
                << " failed : ";
    return ELSeekDiskFileFail;
  }
  LOG(INFO) << "The length of file " << file_name_ << "is " << file_length
            << endl;
  buffer = new char[file_length];

  // reset pos to 0
  if (0 == lseek(fd_, 0, SEEK_SET)) {
    PLOG(ERROR) << "lseek called on fd to reset pos to the start of file "
                << fd_ << " failed : ";
    return ELSeekDiskFileFail;
  }
  ssize_t read_num = read(fd_, buffer, file_length);
  LOG(INFO) << "read " << read_num << " from disk file " << file_name_ << endl;

  if (read_num != file_length) {
    LOG(ERROR) << "read file [" << file_name_
               << "] from disk failed, expected read " << file_length
               << " , actually read " << read_num << endl;
    return EReadDiskFileFail;
  }
  *length = read_num;
  return kSuccess;
}

RetCode DiskFileHandleImp::ReadFile(void* buffer, size_t length) {
  ssize_t total_read_num = 0;
  while (total_read_num < length) {
    ssize_t read_num = read(fd_, static_cast<char*>(buffer) + total_read_num,
                            length - total_read_num);

    if (-1 == read_num) {
      LOG(ERROR) << "read file [" << file_name_
                 << "] from disk failed, expected read " << length
                 << " , actually read " << total_read_num << endl;
      return EReadDiskFileFail;
    }
    total_read_num += read_num;
  }
  LOG(INFO) << "read total " << total_read_num << " from disk file "
            << file_name_ << endl;
  return kSuccess;
}

RetCode DiskFileHandleImp::SetPostion(size_t pos) {
  if (0 == lseek(fd_, pos, SEEK_SET)) {
    PLOG(ERROR) << "lseek called on fd to reset pos to the start of file "
                << fd_ << " failed : ";
    return ELSeekDiskFileFail;
  }
  return kSuccess;
}
}  // namespace common
} /* namespace claims */
