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
#include <glog/logging.h>
#include <atomic>
#include <functional>
#include <string>

#include "../../utility/lock.h"
#include "../error_define.h"

using std::function;
namespace claims {
namespace common {
using std::string;
using std::atomic;
class FileHandleImpFactory;
enum FileOpenFlag {
  kReadFile = 0,
  kCreateFile,
  kAppendFile,
};
static const char* file_open_flag_info[3] = {"kReadFile", "kCreateFile",
                                             "kAppendFile"};

static const char* file_status_info[4] = {"Reading", "Writing", "Appending",
                                          "Closed"};

class FileHandleImp {
  friend FileHandleImpFactory;

 public:
  enum FileStatus { kInReading = 0, kInOverWriting, kInAppending, kClosed };

 protected:
  class RefHolder {
   public:
    explicit RefHolder(atomic<int>& ref) : ref_(ref) {
      ++ref_;
      LOG(INFO) << "ref post:" << ref_ << std::endl;
    }
    ~RefHolder() {
      --ref_;
      LOG(INFO) << "ref wait:" << ref_;
    }

   private:
    atomic<int>& ref_;
  };

 public:
  explicit FileHandleImp(std::string file_name) : file_name_(file_name) {}
  virtual ~FileHandleImp() {}
  //  virtual RetCode Open(std::string file_name, FileOpenFlag open_flag) = 0;
  /**
   * @brief Method description: write buffer into file and make sure write
   *        length char
   * @param buffer: the data
   * @param length: the no. of bytes to write
   * @return rSuccess if wrote length bytes
   */
  virtual RetCode Append(const void* buffer, const size_t length) = 0;

  virtual RetCode AtomicAppend(const void* buffer, const size_t length,
                               function<void()> lock_func,
                               function<void()> unlock_func) = 0;

  virtual RetCode OverWrite(const void* buffer, const size_t length) = 0;

  virtual RetCode AtomicOverWrite(const void* buffer, const size_t length,
                                  function<void()> lock_func,
                                  function<void()> unlock_func) = 0;

  virtual RetCode Close() = 0;
  /**
   * @brief Method description: read total file into memory, update length to
   * the length of file
   * @param buffer: new memory buffer to hold length bytes read from file
   * @param length: hold the no. bytes of the all file
   * @return rSuccess if succeed
   */
  virtual RetCode ReadTotalFile(void*& buffer, size_t* length) = 0;  // NOLINT

  /**
   * @brief Method description: read length bytes from file into memory, usually
   *        called after SetPosition()
   * @param buffer: hold the data read from file
   * @param length: the no. of bytes to read
   * @return rSuccess if succeed
   */
  virtual RetCode Read(void* buffer, size_t length) = 0;
  RetCode PRead(void* buffer, size_t length, size_t start_pos);
  virtual bool CanAccess(std::string file_name) = 0;

  virtual RetCode DeleteFile() = 0;

  const string& get_file_name() { return file_name_; }

  virtual RetCode SwitchStatus(FileStatus status_to_be) = 0;

 protected:
  virtual RetCode SetPosition(size_t pos) = 0;

 protected:
  std::string file_name_;
  volatile FileStatus file_status_ = kClosed;
  //  Lock write_lock_;
  //  atomic<int> reference_count_;
  //  SpineLock i_win_to_close_;
  //  semaphore can_close_;
};

}  // namespace common
} /* namespace claims */

#endif  // COMMON_FILE_HANDLE_FILE_HANDLE_IMP_H_
