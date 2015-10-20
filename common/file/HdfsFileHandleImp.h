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
 * /Claims/common/file/HdfsFileHandleImp.h
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef COMMON_FILE_HDFSFILEHANDLEIMP_H_
#define COMMON_FILE_HDFSFILEHANDLEIMP_H_
#include <assert.h>
#include <hdfs.h>

#include "./FileHandleImp.h"

using claims::common::FileHandleImp;

namespace claims {
namespace common {
class HdfsFileHandleImp : public FileHandleImp {
 public:
  HdfsFileHandleImp(string file_name);
  virtual ~HdfsFileHandleImp();
  virtual RetCode Open(FileOpenFlag flag);
  virtual RetCode Write(const void* buffer, const size_t length);
  virtual RetCode Close();
  virtual RetCode ReadTotalFile(void*& buffer, size_t* length);
  virtual RetCode ReadFile(void* buffer, size_t length);
  virtual bool CanAccess() {
    assert(fs_ != NULL && "failed to connect hdfs");
    return 0 == hdfsExists(fs_, file_name_.c_str());
  }
  virtual RetCode SetPostion(size_t pos);

 private:
  hdfsFS fs_;
  hdfsFile file_;
};
}  // namespace common
} /* namespace claims */

#endif  // COMMON_FILE_HDFSFILEHANDLEIMP_H_
