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
 * /Claims/common/file_handle/disk_handle_imp.h
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description: implementation class of handling disk file
 *
 */

#ifndef COMMON_FILE_HANDLE_DISK_FILE_HANDLE_IMP_H_
#define COMMON_FILE_HANDLE_DISK_FILE_HANDLE_IMP_H_
#include <unistd.h>
#include <string>
#include "../common/error_define.h"
#include "./file_handle_imp.h"

namespace claims {
namespace common {

class FileHandleImpFactory;

class DiskFileHandleImp : public FileHandleImp {
  friend FileHandleImpFactory;

 private:
  explicit DiskFileHandleImp(std::string file_name)
      : fd_(-1), FileHandleImp(file_name) {}

 public:
  virtual ~DiskFileHandleImp();
  // see more in FileHandleImp class
  virtual RetCode Append(const void* buffer, const size_t length);

  //  virtual RetCode AtomicAppend(const void* buffer, const size_t length,
  //                               function<void()> lock_func,
  //                               function<void()> unlock_func);

  virtual RetCode OverWrite(const void* buffer, const size_t length);

  //  virtual RetCode AtomicOverWrite(const void* buffer, const size_t length,
  //                                  function<void()> lock_func,
  //                                  function<void()> unlock_func);

  virtual RetCode Close();
  // see more in FileHandleImp class
  virtual RetCode ReadTotalFile(void*& buffer, size_t* length);
  // see more in FileHandleImp class
  virtual RetCode Read(void* buffer, size_t length);
  virtual bool CanAccess(std::string file_name) {
    return 0 == access(file_name.c_str(), 0);
  }
  virtual RetCode SetPosition(size_t pos);

  virtual RetCode DeleteFile();

 private:
  RetCode Write(const void* buffer, const size_t length);

 private:
  int fd_;
};

}  // namespace common
} /* namespace claims */

#endif  // COMMON_FILE_HANDLE_DISK_FILE_HANDLE_IMP_H_
