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
 * /Claims/loader/single_file_connector.h
 *
 *  Created on: Oct 21, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef LOADER_SINGLE_FILE_CONNECTOR_H_
#define LOADER_SINGLE_FILE_CONNECTOR_H_
#include <string>
#include "./file_connector.h"
#include "../common/file_handle/file_handle_imp_factory.h"
#include "../common/file_handle/file_handle_imp.h"
#include "../common/memory_handle.h"

namespace claims {
namespace loader {

using std::string;
using claims::common::FileHandleImp;
using claims::common::FilePlatform;

class SingleFileConnector : public FileConnector {
 public:
  SingleFileConnector(FilePlatform platform, string file_name)
      : FileConnector(platform), file_name_(file_name) {
    imp_ =
        common::FileHandleImpFactory::Instance().CreateFileHandleImp(platform_);
  }
  ~SingleFileConnector() { DELETE_PTR(imp_); }

  virtual RetCode Open(common::FileOpenFlag oepn_flag) {
    return imp_->Open(file_name_, oepn_flag);
  }
  virtual RetCode Close() { return imp_->Close(); }
  virtual RetCode Flush(const void* source, unsigned length) {
    return imp_->Write(source, length);
  }
  virtual RetCode AtomicFlush(const void* source, unsigned length) {
    return imp_->AtomicWrite(source, length);
  }
  virtual RetCode Flush(unsigned projection_offset, unsigned partition_offset,
                        const void* source, unsigned length) {
    assert(false && "not implemented");
    return common::rFailure;
  }
  virtual RetCode AtomicFlush(unsigned projection_offset,
                              unsigned partition_offset, const void* source,
                              unsigned length) {
    assert(false && "not implemented");
    return common::rFailure;
  }

  virtual bool CanAccess() { return imp_->CanAccess(file_name_); }
  /**
   * @brief Method description: load total file into memory
   * @param buffer: set buffer point to a new memory allocated by this method,
   * @param length: set to the length of file, also the length of new memory
   * @return  rSuccess if succeed.
   * @details   (additional) this method will modify buffer, set to a new memory
   */
  virtual RetCode LoadTotalFile(void*& buffer, uint64_t* length) {
    return imp_->ReadTotalFile(buffer, length);
  }

  /**
   * @brief Method description: load a part of file into memory pointed by
   * buffer
   * @param buffer: point to the memory where file is load
   * @param start: the position from which to read
   * @param length: the length of data need to read
   * @return  rSuccess if OK
   * @details   (additional)
   */
  virtual RetCode LoadFile(void* buffer, int64_t start, uint64_t length) {
    int ret = imp_->SetPosition(start);
    if (ret != common::rSuccess) {
      LOG(ERROR) << "failed to set postion at " << start << ". ret:" << ret;
      return ret;
    }
    return imp_->Read(buffer, length);
  }

 private:
  string file_name_;
};

} /* namespace loader */
} /* namespace claims */

#endif  // LOADER_SINGLE_FILE_CONNECTOR_H_
