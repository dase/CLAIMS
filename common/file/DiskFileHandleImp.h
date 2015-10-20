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
 * /Claims/Loader/DiskHandleImp.h
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description: implementation class of handling disk file
 *
 */

#ifndef COMMON_FILE_DISKHANDLEIMP_H_
#define COMMON_FILE_DISKHANDLEIMP_H_
#include <string>
#include "../common/error_define.h"
#include "./FileHandleImp.h"

namespace claims {
namespace common {

class DiskFileHandleImp : public FileHandleImp {
 public:
  DiskFileHandleImp(string file_name) : FileHandleImp(file_name), fd_(-1) {}
  virtual ~DiskFileHandleImp() {}
  virtual RetCode Open(FileOpenFlag flag);
  virtual RetCode Write(const void* buffer, const size_t length);
  virtual RetCode Close();
  virtual RetCode ReadTotalFile(void*& buffer, size_t* length);
  virtual RetCode ReadFile(void* buffer, size_t length);
  virtual bool CanAccess() { return 0 == access(file_name_.c_str(), 0); }
  virtual RetCode SetPostion(size_t pos);

 private:
  int fd_;
};

}  // namespace common
} /* namespace claims */

#endif  // COMMON_FILE_DISKHANDLEIMP_H_
