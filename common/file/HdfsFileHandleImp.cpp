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
 * /Claims/common/file/HdfsFileHandleImp.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./HdfsFileHandleImp.h"

#include <glog/logging.h>
using std::endl;
namespace claims {
namespace common {
HdfsFileHandleImp::HdfsFileHandleImp(string file_name) : file_name_(file_name) {
  fs_ = hdfsConnect(Config::hdfs_master_ip.c_str(), Config::hdfs_master_port);
  if (NULL == fs_) {
    LOG(ERROR) << "failed to connect to HDFS(ip:" << Config::hdfs_master_ip
               << ", port:" << Config::hdfs_master_port << ")" << endl;
  }
  LOG(INFO) << "connected to HDFS(ip:" << Config::hdfs_master_ip
            << ", port:" << Config::hdfs_master_port << ")" << endl;
}

HdfsFileHandleImp::~HdfsFileHandleImp() {
  int ret = Close() || hdfsDisconnect(fs_);
  if (ret != 0) LOG(ERROR) << "failed to disconnect to hdfs" << endl;
}

RetCode HdfsFileHandleImp::Open(FileOpenFlag flag) {
  assert(fs_ != NULL && "failed to connect hdfs");

  if (kCreateFile == flag) {
    file_ = hdfsOpenFile(fs_, file_name_.c_str(), O_WRONLY, 0, 0, 0);
  } else if (kAppendFile == flag) {
    file_ = hdfsOpenFile(fs_, file_name_.c_str(), O_WRONLY | O_APPEND, 0, 0, 0);
  } else {
    LOG(ERROR) << "parameter flag:" << flag << " is invalid" << endl;
    return EParamInvalid;
  }
  if (NULL == file_) {
    PLOG(ERROR) << "failed to open hdfs file :" << file_name_;
    return EOpenHdfsFileFail;
  } else {
    LOG(INFO) << "opened hdfs file:" << file_name_ << endl;
    return kSuccess;
  }
}

RetCode HdfsFileHandleImp::Write(const void* buffer, const size_t length) {
  assert(fs_ != NULL && "failed to connect hdfs");
  size_t total_write_num = 0;
  while (total_write_num < length) {
    int32_t write_num =
        hdfsWrite(fs_, file_, static_cast<char*>(buffer) + total_write_num,
                  length - total_write_num);
    if (-1 == write_num) {
      PLOG(ERROR) << "failed to write to file: " << file_name_ << endl;
      return EWriteDiskFileFail;
    }
    total_write_num += write_num;
  }
  LOG(INFO) << "write " << length << " length data from " << buffer << " into "
            << file_name_ << endl;
  return kSuccess;
}

RetCode HdfsFileHandleImp::Close() {
  assert(fs_ != NULL && "failed to connect hdfs");
  if (0 != hdfsCloseFile(fs_, file_)) {
    LOG(ERROR) << "failed to close hdfs file " << file_name_ << endl;
    return ECloseHdfsFileFail;
  }
  return kSuccess;
}

RetCode HdfsFileHandleImp::ReadTotalFile(void*& buffer, size_t* length) {
  assert(fs_ != NULL && "failed to connect hdfs");
  hdfsFileInfo* hdfsfile = hdfsGetPathInfo(fs_, file_name_.c_str());
  int file_length = hdfsfile->mSize;
  LOG(INFO) << "The length of file " << file_name_ << "is " << file_length
            << endl;

  buffer = new char[file_length];
  int read_num = hdfsRead(fs_, file_, buffer, file_length);
  LOG(INFO) << "read " << read_num << " data from hdfs file " << file_name_
            << endl;

  if (read_num != file_length) {
    LOG(ERROR) << "failed to read file [" << file_name_
               << "] from hdfs , expected read " << file_length
               << " , actually read " << read_num << endl;
    return EReadHdfsFileFail;
  }
  *length = read_num;
  return kSuccess;
}

RetCode HdfsFileHandleImp::ReadFile(void* buffer, size_t length) {
  assert(fs_ != NULL && "failed to connect hdfs");
  int total_read_num = 0;
  while (total_read_num < length) {
    int read_num =
        hdfsRead(fs_, file_, static_cast<char*>(buffer) + total_read_num,
                 length - total_read_num);
    if (-1 == read_num) {
      LOG(ERROR) << "failed to read " << length << "from file:" << file_name_
                 << endl;
      return EReadHdfsFileFail;
    }
    total_read_num += read_num;
  }
  LOG(INFO) << "read total " << total_read_num << " from hdfs file "
            << file_name_ << endl;
  return kSuccess;
}

RetCode HdfsFileHandleImp::SetPostion(size_t pos) {
  assert(fs_ != NULL && "failed to connect hdfs");
  int ret = hdfsSeek(fs_, file_, pos);
  if (0 != ret) {
    LOG(ERROR) << "failed to seek to " << pos << "in " << file_name_ << " file"
               << endl;
    return ELSeekHdfsFileFail;
  }

  LOG(INFO) << "Seek to " << pos << "in " << file_name_ << " file" << endl;
  return kSuccess;
}

}  // namespace common
} /* namespace claims */
