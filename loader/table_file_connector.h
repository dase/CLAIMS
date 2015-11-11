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
 * /Claims/loader/table_file_connector.h
 *
 *  Created on: Oct 21, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef LOADER_TABLE_FILE_CONNECTOR_H_
#define LOADER_TABLE_FILE_CONNECTOR_H_

#include <vector>
#include <string>

#include "../common/error_define.h"
#include "../common/file_handle/file_handle_imp.h"
#include "../common/file_handle/file_handle_imp_factory.h"
#include "./file_connector.h"

namespace claims {
namespace loader {

class TableFileConnector : public FileConnector {
 public:
  TableFileConnector(common::FilePlatform platform,
                     std::vector<std::vector<std::string>> writepath);
  virtual ~TableFileConnector();
  virtual RetCode Open(common::FileOpenFlag open_flag_);
  virtual RetCode Close();

  /**
   * @brief Method description: flush length bytes data from source into file
   *        whose projection id is projection_offset and partition id is
   *        partition_offset
   */
  virtual RetCode Flush(unsigned projection_offset, unsigned partition_offset,
                        const void* source, unsigned length);
  virtual RetCode Flush(const void* source, unsigned length) {
    assert(false && "not implemented");
    return common::kFailure;
  }
  virtual RetCode LoadTotalFile(void*& buffer, uint64_t* length) {
    assert(false);
    return common::kFailure;
  }
  virtual RetCode LoadFile(void* buffer, int64_t start, uint64_t length) {
    assert(false);
    return common::kFailure;
  }

 private:
  std::vector<std::vector<common::FileHandleImp*>> file_handles_;
  std::vector<std::vector<std::string>> write_path_name_;
  common::FileHandleImp* imp_;
};

} /* namespace loader */
} /* namespace claims */

#endif  // LOADER_TABLE_FILE_CONNECTOR_H_
