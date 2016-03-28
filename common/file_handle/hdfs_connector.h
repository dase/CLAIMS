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
 * /Claims/common/file_handle/HdfsConnector.h
 *
 *  Created on: Feb 29, 2016
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef COMMON_FILE_HANDLE_HDFS_CONNECTOR_H_
#define COMMON_FILE_HANDLE_HDFS_CONNECTOR_H_

#include <assert.h>
#include <glog/logging.h>
#include <hdfs.h>
#include <iostream>

#include "../../common/rename.h"
#include "../../Config.h"

using std::endl;
namespace claims {
namespace common {

class HdfsConnector {
 public:
  static hdfsFS Instance() {
    if (NULL == fs_) {
      Config::getInstance();
      fs_ =
          hdfsConnect(Config::hdfs_master_ip.c_str(), Config::hdfs_master_port);
      if (NULL == fs_) {
        LOG(ERROR) << "failed to connect to HDFS(ip:" << Config::hdfs_master_ip
                   << ", port:" << Config::hdfs_master_port << ")" << std::endl;
        assert(false);
      }
      LOG(INFO) << "connected to HDFS(ip:" << Config::hdfs_master_ip
                << ", port:" << Config::hdfs_master_port << ")" << std::endl;
    }
    return fs_;
  }

  NO_COPY_AND_ASSIGN(HdfsConnector);

  ~HdfsConnector() {
    hdfsDisconnect(fs_);
    fs_ = NULL;
    std::cerr << "disconnected hdfs and fs_ was set to NULL" << std::endl;
  }

 private:
  HdfsConnector() {}
  static hdfsFS fs_;
};

} /* namespace common */
} /* namespace claims */

#endif  // COMMON_FILE_HANDLE_HDFS_CONNECTOR_H_
