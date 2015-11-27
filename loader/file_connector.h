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
 * /Claims/loader/file_connector.h
 *
 *  Created on: Oct 22, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef LOADER_FILE_CONNECTOR_H_
#define LOADER_FILE_CONNECTOR_H_
#include <glog/logging.h>
#include <string>

#include "../common/file_handle/file_handle_imp.h"
#include "../common/file_handle/file_handle_imp_factory.h"
#include "../common/error_define.h"

namespace claims {
namespace loader {

class FileConnector {
 public:
  explicit FileConnector(common::FilePlatform platform) : platform_(platform) {}
  virtual ~FileConnector() {}
  virtual RetCode Open(common::FileOpenFlag open_flag) = 0;
  virtual RetCode Close() = 0;
  virtual RetCode Flush(unsigned projection_offset, unsigned partition_offset,
                        const void* source, unsigned length) = 0;
  virtual RetCode Flush(const void* source, unsigned length) = 0;
  virtual RetCode LoadTotalFile(void*& buffer, uint64_t* length) = 0;
  virtual RetCode LoadFile(void* buffer, int64_t start, uint64_t length) = 0;

 protected:
  common::FilePlatform platform_;
  common::FileHandleImp* imp_;
};

}  // namespace loader
}  // namespace claims

#endif  // LOADER_FILE_CONNECTOR_H_
