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
 * /Claims/common/file_handle/file_handle_imp.h
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef COMMON_FILE_HANDLE_HDFS_FILE_HANDLE_IMP_H_
#define COMMON_FILE_HANDLE_HDFS_FILE_HANDLE_IMP_H_
#include <assert.h>
#include <hdfs.h>
#include <string>

#include "./file_handle_imp.h"
#include "../../common/rename.h"

namespace claims {
namespace common {

class FileHandleImpFactory;

class HdfsFileHandleImp : public FileHandleImp {
  friend FileHandleImpFactory;

 private:
  HdfsFileHandleImp();

  NO_COPY_AND_ASSIGN(HdfsFileHandleImp);

 public:
  /**
   * @brief Method description: call Close() and disconnect HDFS
   */
  virtual ~HdfsFileHandleImp();
  virtual RetCode Open(std::string file_name, FileOpenFlag open_flag);
  // see more in FileHandleImp class
  virtual RetCode Write(const void* buffer, const size_t length);
  virtual RetCode AtomicWrite(const void* buffer, const size_t length);
  virtual RetCode Close();
  // see more in FileHandleImp class
  virtual RetCode ReadTotalFile(void*& buffer, size_t* length);
  // see more in FileHandleImp class
  virtual RetCode Read(void* buffer, size_t length);
  virtual bool CanAccess(std::string file_name) {
    assert(fs_ != NULL && "failed to connect hdfs");
    return 0 == hdfsExists(fs_, file_name.c_str());
  }
  virtual RetCode SetPosition(size_t pos);

  virtual RetCode DeleteFile();

 private:
  hdfsFS fs_ = NULL;
  hdfsFile file_ = NULL;
  int64_t read_start_pos_;
  FileOpenFlag open_flag_ = -1;
};
}  // namespace common
} /* namespace claims */

#endif  // COMMON_FILE_HANDLE_HDFS_FILE_HANDLE_IMP_H_
