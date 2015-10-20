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
 * /Claims/Loader/FileHandleImp.h
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef COMMON_FILE_FILEHANDLEIMP_H_
#define COMMON_FILE_FILEHANDLEIMP_H_
#include <string>
using std::string;
namespace claims {
namespace common {

enum FileOpenFlag { kCreateFile, kAppendFile };

class FileHandleImp {
 public:
  explicit FileHandleImp(string file_name) : file_name_(file_name) {}
  virtual ~FileHandleImp();
  virtual RetCode Open(FileOpenFlag flag) = 0;
  virtual RetCode Write(const void* buffer, const size_t length) = 0;
  virtual RetCode Close() = 0;
  virtual RetCode ReadTotalFile(void*& buffer, size_t* length) = 0;
  virtual RetCode ReadFile(void* buffer, size_t length) = 0;
  virtual bool CanAccess() = 0;
  virtual RetCode SetPostion(size_t pos) = 0;

 protected:
  string file_name_;
};

}  // namespace common
} /* namespace claims */

#endif  // COMMON_FILE_FILEHANDLEIMP_H_
