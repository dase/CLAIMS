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
 * /Claims/common/file_handle/file_handle_imp_factory.h
 *
 *  Created on: Oct 21, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description: Factory class for generating FileHandleImp, using meyers
 *  singleton pattern
 *
 */

#ifndef COMMON_FILE_HANDLE_FILE_HANDLE_IMP_FACTORY_H_
#define COMMON_FILE_HANDLE_FILE_HANDLE_IMP_FACTORY_H_
#include <stdlib.h>

#include "../../common/rename.h"
#include "./file_handle_imp.h"
#include "./disk_file_handle_imp.h"
#include "./hdfs_file_handle_imp.h"

namespace claims {
namespace common {

enum FilePlatform { kDisk, kHdfs };
class FileHandleImpFactory {
 public:
  static FileHandleImpFactory& Instance() {
    static FileHandleImpFactory factory;
    return factory;
  }
  FileHandleImp* CreateFileHandleImp(FilePlatform platform) {
    if (kHdfs == platform)
      return new HdfsFileHandleImp();
    else if (kDisk == platform)
      return new DiskFileHandleImp();
    assert(false && "FilePlatform flag is invalid");
    return NULL;
  }

 private:
  FileHandleImpFactory() {}
  NO_COPY_AND_ASSIGN(FileHandleImpFactory);
};

}  // namespace common

}  // namespace claims

#endif  // COMMON_FILE_HANDLE_FILE_HANDLE_IMP_FACTORY_H_
