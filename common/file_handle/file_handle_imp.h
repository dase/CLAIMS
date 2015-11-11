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

#ifndef COMMON_FILE_HANDLE_FILE_HANDLE_IMP_H_
#define COMMON_FILE_HANDLE_FILE_HANDLE_IMP_H_
#include <string>
#include "../error_define.h"
namespace claims {
namespace common {

class FileHandleImpFactory;
enum FileOpenFlag { kCreateFile, kAppendFile, kReadFile };

class FileHandleImp {
  friend FileHandleImpFactory;

 public:
  FileHandleImp() {}
  virtual ~FileHandleImp() {}
  virtual RetCode Open(std::string file_name, FileOpenFlag open_flag) = 0;
  /**
   * @brief Method description: write buffer into file and make sure write
   *        length char
   * @param buffer: the data
   * @param length: the no. of bytes to write
   * @return kSuccess if wrote length bytes
   */
  virtual RetCode Write(const void* buffer, const size_t length) = 0;
  virtual RetCode Close() = 0;
  /**
   * @brief Method description: read total file into memory, update length to
   * the length of file
   * @param buffer: new memory buffer to hold length bytes read from file
   * @param length: hold the no. bytes of the all file
   * @return kSuccess if succeed
   */
  virtual RetCode ReadTotalFile(void*& buffer, size_t* length) = 0;
  /**
   * @brief Method description: read length bytes from file into memory, usually
   *        called after SetPosition()
   * @param buffer: hold the data read from file
   * @param length: the no. of bytes to read
   * @return kSuccess if succeed
   */
  virtual RetCode Read(void* buffer, size_t length) = 0;
  virtual bool CanAccess(std::string file_name) = 0;
  virtual RetCode SetPosition(size_t pos) = 0;

 protected:
  std::string file_name_;
};

}  // namespace common
} /* namespace claims */

#endif  // COMMON_FILE_HANDLE_FILE_HANDLE_IMP_H_
