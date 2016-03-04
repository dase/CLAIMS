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
 * /Claims/common/file_handle/file_handle_imp.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./file_handle_imp.h"

#include <glog/logging.h>
#include "../../common/error_define.h"

using std::endl;
namespace claims {
namespace common {
RetCode FileHandleImp::AtomicAppend(const void* buffer, const size_t length,
                                    function<void()> lock_func,
                                    function<void()> unlock_func) {
  lock_func();
  RetCode ret = Append(buffer, length);
  unlock_func();
  return ret;
}
RetCode FileHandleImp::AtomicOverWrite(const void* buffer, const size_t length,
                                       function<void()> lock_func,
                                       function<void()> unlock_func) {
  lock_func();
  RetCode ret = OverWrite(buffer, length);
  unlock_func();
  //  if (rSuccess != ret) {
  //    return ret;
  //  } else {
  //    ret = Close();
  //  }
  return ret;
}
RetCode FileHandleImp::PRead(void* buffer, size_t length, size_t start_pos) {
  int ret = rSuccess;
  EXEC_AND_RETURN_ERROR(ret, this->SwitchStatus(kInReading),
                        "failed to switch status");
  EXEC_AND_RETURN_ERROR(ret, this->SetPosition(start_pos),
                        "failed to set position to " << start_pos);
  return Read(buffer, length);
}
}  // namespace common
} /* namespace claims */
