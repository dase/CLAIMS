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
 * /CLAIMS/common/data_type.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: imdb
 *		   Email:
 *
 * Description:
 *
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <glog/logging.h>
#include <string>
#include <sstream>
#include "./data_type.h"
#include "./error_define.h"
using claims::common::rSuccess;
using claims::common::rTooSmallData;
using claims::common::rTooLargeData;
using claims::common::rTooLongData;
using claims::common::rInterruptedData;
using claims::common::rIncorrectData;
using claims::common::rInvalidNullData;
using claims::common::kErrorMessage;

RetCode OperateInt::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (!isdigit(str[0]) && str[0] != '-') {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  for (auto i = 1; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rInterruptedData]
                   << "] for " << str << endl;
      str = str.substr(0, i);
      ret = rInterruptedData;
      break;
    }
  long value = atol(str.c_str());
  if (value < INT_MIN) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData] << "] for "
                 << str << endl;
    str = kIntMin;
    ret = rTooSmallData;
  } else if (value > INT_MAX || (value == INT_MAX && nullable)) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLargeData] << "] for "
                 << str << endl;
    str = kIntMax_1;
    ret = rTooLargeData;
  }
  return ret;
}

RetCode OperateFloat::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (!isdigit(str[0]) && str[0] != '-') {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rInterruptedData]
                   << "] for " << str << endl;
      str = str.substr(0, i);
      ret = rInterruptedData;
      break;
    }
  double value = atof(str.c_str());
  if (value < -FLT_MAX) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData] << "] for "
                 << str << endl;
    str = kFloatMin;
    ret = rTooSmallData;
  } else if (value > FLT_MAX || (value == FLT_MAX && nullable)) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLargeData] << "] for "
                 << str << endl;
    str = kFloatMax_1;
    ret = rTooLargeData;
  }
  return ret;
}
RetCode OperateDouble::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (str == kDoubleMax && !nullable) return rSuccess;
  if (!isdigit(str[0]) && str[0] != '-') {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rInterruptedData]
                   << "] for " << str << endl;
      str = str.substr(0, i);
      ret = rInterruptedData;
      break;
    }
  /*
   * @brief integer part of a double number
   */
  auto len = 0;
  for (auto i = str.begin(); i != str.end(); i++, len++)
    if (*i == '.') break;
  if (len >= 309) {
    if (str[0] == '-') {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData]
                   << "] for " << str << endl;
      str = kDoubleMin;
      ret = rTooSmallData;
    } else {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLargeData]
                   << "] for " << str << endl;
      str = kDoubleMax_1;
      ret = rTooLargeData;
    }
  }
  return ret;
}

RetCode OperateULong::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (str == kULongMax && !nullable) return rSuccess;
  if (!isdigit(str[0]) && str[0] != '-') {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInterruptedData]
                 << "] for " << str << endl;
      str = str.substr(0, i);
      ret = rInterruptedData;
      break;
    }
  if (str[0] == '-') {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData] << "] for "
                 << str << endl;
    str = '0';
    ret = rTooSmallData;
  } else {
    auto len = 0;
    for (auto i = str.begin(); i != str.end(); i++, len++)
      if (*i == '.') break;
    if (len >= 20 && str[19] > '1') {
      LOG(WARNING) << "[checkSet]: [" << kErrorMessage[rTooLargeData]
                   << "] for " << str << endl;
      str = kULongMax_1;
      ret = rTooLargeData;
    }
  }
  return ret;
}

RetCode OperateString::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str[0] == NULL_STRING && nullable) return rSuccess;
  if (str[0] == NULL_STRING && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (str.length() > size) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLongData] << "] for "
                 << str << endl;
    str = str.substr(0, size);
    ret = rTooLongData;
  }
  return ret;
}

RetCode OperateDate::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (str.length() == 8) {
    for (auto i = str.begin(); i != str.end(); i++)
      if (!isdigit(*i)) {
        LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData]
                   << "] for " << str << endl;
        return rIncorrectData;
      }
    if (ret == rSuccess) {
      if (str < "14000101") {
        LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData]
                     << "] for " << str << endl;
        str = "14000101";
        ret = rTooSmallData;
      }
      if (str > "99991231") {
        LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLargeData]
                     << "] for " << str << endl;
        str = "99991231";
        ret = rTooLargeData;
      }
    }
  } else if (str.length() == 10) {
    for (auto i = 0; i < str.length(); i++)
      if (!isdigit(str[i]) && i != 4 && i != 7) {
        LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData]
                   << "] for " << str << endl;
        ret = rIncorrectData;
        break;
      }
    if (str < "1400-01-01") {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData]
                   << "] for " << str << endl;
      str = "1400-01-01";
      ret = rTooSmallData;
    }
    if (str > "9999-12-31") {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLargeData]
                   << "] for " << str << endl;
      str = "9999-12-31";
      ret = rTooLargeData;
    }
  } else {
    LOG(ERROR) << "[CheckSet]: " << kErrorMessage[rIncorrectData] << "] for"
               << str << endl;
    return rIncorrectData;
  }
  return ret;
}

RetCode OperateTime::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (str.length() != 15) {
    LOG(ERROR) << "[CheckSet]:" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && i != 2 && i != 5 && i != 8) {
      LOG(ERROR) << "[CheckSet]: " << kErrorMessage[rIncorrectData] << "] for "
                 << str << endl;
      return rIncorrectData;
    }
  if (str < "00:00:00.000000") {
    LOG(WARNING) << "[CheckSet]: " << kErrorMessage[rTooSmallData] << "] for "
                 << str << endl;
    str = "00:00:00.000000";
    ret = rTooSmallData;
  }
  if (str > "23:59:59.999999") {
    LOG(WARNING) << "[CheckSet]: " << kErrorMessage[rTooLargeData] << "] for "
                 << str << endl;
    str = "23:59:59.999999";
    ret = rTooLargeData;
  }
  return ret;
}

RetCode OperateDatetime::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (str.length() != 26) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && i != 4 && i != 7 && i != 10 & i != 13 &&
        i != 16 & i != 19) {
      LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
                 << str << endl;
      return rIncorrectData;
    }
  if (str < "1400-01-01 00:00:00.000000") {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData] << "]  for "
                 << str << endl;
    str = "1400-01-01 00:00:00.000000";
    return rTooSmallData;
  } else if (str > "9999-12-31 23:59:59.999999") {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLargeData] << "] for "
                 << str << endl;
    str = "9999-12-31 23:59:59.999999";
    return rTooLargeData;
  }
  return ret;
}

RetCode OperateSmallInt::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (!isdigit(str[0]) && str[0] != '-') {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rInterruptedData]
                   << "] for " << str << endl;
      str = str.substr(0, i);
      ret = rInterruptedData;
      break;
    }
  long value = atol(str.c_str());
  if (value < SHRT_MIN) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData] << "] for "
                 << str << endl;
    str = kSmallIntMin;
    ret = rTooSmallData;
  } else if (value > SHRT_MAX || (value == SHRT_MAX && nullable)) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLargeData] << "] for "
                 << str << endl;
    str = kSmallIntMax_1;
    ret = rTooLargeData;
  }
  return ret;
}

RetCode OperateUSmallInt::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (!isdigit(str[0]) && str[0] != '-') {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rInterruptedData]
                   << "] for " << str << endl;
      str = str.substr(0, i);
      ret = rInterruptedData;
      break;
    }
  long value = atol(str.c_str());
  if (value < 0) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooSmallData] << "] for "
                 << str << endl;
    str = "0";
    ret = rTooSmallData;
  } else if (value > USHRT_MAX || (value == USHRT_MAX && nullable)) {
    LOG(WARNING) << "[CheckSet]: [" << kErrorMessage[rTooLargeData] << "] for "
                 << str << endl;
    str = kUSmallIntMax_1;
    ret = rTooLargeData;
  }
  return ret;
}

RetCode OperateDecimal::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  return ret;
}

RetCode OperateBool::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rInvalidNullData] << "] for "
               << str << endl;
    return rInvalidNullData;
  }
  if (str != "false" && str != "true") {
    LOG(ERROR) << "[CheckSet]: [" << kErrorMessage[rIncorrectData] << "] for "
               << str << endl;
    return rIncorrectData;
  }
  return ret;
}
