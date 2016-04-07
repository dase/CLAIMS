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
 *      Author: lizhifang
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
using namespace claims::common;
int null_int_value = NULL_INT;
float null_float_value = NULL_FLOAT;
double null_double_value = NULL_DOUBLE;
unsigned long null_u_long_value = ULONG_LONG_MAX;
char null_string_value[] = {1};
date null_date_value(NULL_DATE);
ptime null_datetime_value(NULL_DATETIME);
short null_small_int_value = NULL_SMALL_INT;
unsigned short null_u_small_int_value = NULL_U_SMALL_INT;
Decimal null_decimal_value = Decimal::CreateNullDecimal();
int null_boolean_value = 2;
/**
 * if a string to input is warning, we modify it to a right value
 *     and return it's warning-code
 * if a string to input is error, we don't modify it, but return a error-code
 *
 * if there is no error or warning exist , we return success-code
 * @param str
 * @return warning/error code
 */
RetCode OperateInt::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  /*
   * Input a null value, and it is valid (warning)
   */
  if (str == "" && nullable) return rSuccess;
  /*
   * Input a null value, but it is inValid (error)
   */
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  /*
   * The input's format is error for number type like a12, c21, etc. (error)
   */
  if (!isdigit(str[0]) && str[0] != '-') {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  /*
   * The input need to be interrupted like 12a34, the string will be cut off to
   *   12. (warning)
   */
  for (auto i = 1; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      ret = rInterruptedData;
      WLOG(ret, str);
      str = str.substr(0, i);
      break;
    }
  long value = atol(str.c_str());
  /*
   * The input is less than int_min, we set it to int_min
   */
  if (value < INT_MIN) {
    ret = rTooSmallData;
    WLOG(ret, str);
    str = kIntMin;
  } else if (value > INT_MAX || (value == INT_MAX && nullable)) {
    /*
     * if input is int_max, we set it to int_max - 1, because int_max is
     *    null-value actually.
     * if input is larger than int_max, we set it to int_max-1
     */
    ret = rTooLargeData;
    WLOG(ret, str);
    str = kIntMax_1;
  }
  return ret;
}

RetCode OperateFloat::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str)
    return ret;
  }
  if (!isdigit(str[0]) && str[0] != '-' && str[0] != '.') {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      ret = rInterruptedData;
      WLOG(ret, str);
      str = str.substr(0, i);
      break;
    }
  double value = atof(str.c_str());
  if (value < -FLT_MAX) {
    ret = rTooSmallData;
    WLOG(ret, str);
    str = kFloatMin;
  } else if (value > FLT_MAX || (value == FLT_MAX && nullable)) {
    ret = rTooLargeData;
    WLOG(ret, str);
    str = kFloatMax_1;
  }
  return ret;
}
RetCode OperateDouble::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  if (str == kDoubleMax && !nullable) return rSuccess;
  if (!isdigit(str[0]) && str[0] != '-') {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      ret = rInterruptedData;
      WLOG(ret, str);
      str = str.substr(0, i);
      break;
    }
  /*
   *  integer part of a double number
   */
  auto len = 0;
  for (auto i = str.begin(); i != str.end(); i++, len++)
    if (*i == '.') break;
  /*
   * ToDo we could improve the size check for double type *_*
   */
  if (len >= 309) {
    if (str[0] == '-') {
      ret = rTooSmallData;
      WLOG(ret, str);
      str = kDoubleMin;
    } else {
      ret = rTooLargeData;
      WLOG(ret, str);
      str = kDoubleMax_1;
    }
  }
  return ret;
}

RetCode OperateULong::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  if (str == kULongMax && !nullable) return rSuccess;
  if (!isdigit(str[0]) && str[0] != '-') {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      ret = rInterruptedData;
      WLOG(ret, str);
      str = str.substr(0, i);
      break;
    }
  if (str[0] == '-') {
    ret = rTooSmallData;
    WLOG(ret, str);
    str = '0';
  } else {
    auto len = 0;
    for (auto i = str.begin(); i != str.end(); i++, len++)
      if (*i == '.') break;
    /*
     * ToDo we could improve the size check for unsigned long type *_*
     */
    if (len >= 20 && str[19] > '1') {
      ret = rTooLargeData;
      WLOG(ret, str)
      str = kULongMax_1;
    }
  }
  return ret;
}

RetCode OperateString::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str[0] == NULL_STRING && nullable) return rSuccess;
  if (str[0] == NULL_STRING && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  /*
   * The input string is too long to storage in a tuple
   */
  if (str.length() > size) {
    ret = rTooLongData;
    WLOG(ret, " length of str:" << str
                                << " is longer than expected length:" << size);
    str = str.substr(0, size);
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
        ret = rIncorrectData;
        ELOG(ret, str);
        return ret;
      }
    if (ret == rSuccess) {
      /*
       * Claims don't support date before 1400-01-01, but why?
       */
      if (str < "14000101") {
        ret = rTooSmallData;
        WLOG(ret, str);
        str = "14000101";
      }
      if (str > "99991231") {
        ret = rTooLargeData;
        WLOG(ret, str);
        str = "99991231";
      }
    }
  } else if (str.length() == 10) {
    for (auto i = 0; i < str.length(); i++)
      if (!isdigit(str[i]) && i != 4 && i != 7) {
        ret = rIncorrectData;
        WLOG(ret, str);
        return ret;
      }
    if (str < "1400-01-01") {
      ret = rTooSmallData;
      WLOG(ret, str);
      str = "1400-01-01";
    }
    if (str > "9999-12-31") {
      ret = rTooLargeData;
      WLOG(ret, str);
      str = "9999-12-31";
    }
  } else {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  return ret;
}

RetCode OperateTime::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  if (str.length() != 15 && str.length() != 8) {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  if (str.length() == 15) {
    for (auto i = 0; i < str.length(); i++)
      if (!isdigit(str[i]) && i != 2 && i != 5 && i != 8) {
        ret = rIncorrectData;
        ELOG(ret, str);
        return ret;
      }
    if (str < "00:00:00.000000") {
      ret = rTooSmallData;
      ELOG(ret, str);
      str = "00:00:00.000000";
    } else if (str > "23:59:59.999999") {
      ret = rTooLargeData;
      ELOG(ret, str)
      str = "23:59:59.999999";
    }
  }
  if (str.length() == 8) {
    for (auto i = 0; i < str.length(); i++)
      if (!isdigit(str[i]) && i != 2 && i != 5) {
        ret = rIncorrectData;
        ELOG(ret, str);
        return ret;
      }
    if (str < "00:00:00") {
      ret = rTooSmallData;
      ELOG(ret, str);
      str = "00:00:00";
    } else if (str > "23:59:59") {
      ret = rTooLargeData;
      ELOG(ret, str)
      str = "23:59:59";
    }
  }

  return ret;
}

RetCode OperateDatetime::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  if (str.length() != 26 && str.length() != 19) {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  if (str.length() == 26) {
    for (auto i = 0; i < str.length(); i++)
      if (!isdigit(str[i]) && i != 4 && i != 7 && i != 10 & i != 13 &&
          i != 16 & i != 19) {
        ret = rIncorrectData;
        ELOG(ret, str);
        return ret;
      }
    if (str < "1400-01-01 00:00:00.000000") {
      ret = rTooSmallData;
      ELOG(ret, str);
      str = "1400-01-01 00:00:00.000000";
      return ret;
    } else if (str > "9999-12-31 23:59:59.999999") {
      ret = rTooLargeData;
      str = "9999-12-31 23:59:59.999999";
      return ret;
    }
  }
  if (str.length() == 19) {
    for (auto i = 0; i < str.length(); i++)
      if (!isdigit(str[i]) && i != 4 && i != 7 && i != 10 & i != 13 &&
          i != 16) {
        ret = rIncorrectData;
        ELOG(ret, str);
        return ret;
      }
    if (str < "1400-01-01 00:00:00") {
      ret = rTooSmallData;
      ELOG(ret, str);
      str = "1400-01-01 00:00:00";
      return ret;
    } else if (str > "9999-12-31 23:59:59") {
      ret = rTooLargeData;
      str = "9999-12-31 23:59:59";
      return ret;
    }
  }

  return ret;
}

RetCode OperateSmallInt::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  if (!isdigit(str[0]) && str[0] != '-') {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      ret = rInterruptedData;
      WLOG(ret, str);
      str = str.substr(0, i);
      break;
    }
  long value = atol(str.c_str());
  if (value < SHRT_MIN) {
    ret = rTooSmallData;
    WLOG(ret, str);
    str = kSmallIntMin;
  } else if (value > SHRT_MAX || (value == SHRT_MAX && nullable)) {
    ret = rTooLargeData;
    WLOG(ret, str);
    str = kSmallIntMax_1;
  }
  return ret;
}

RetCode OperateUSmallInt::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  if (!isdigit(str[0]) && str[0] != '-') {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  for (auto i = 0; i < str.length(); i++)
    if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
      ret = rInterruptedData;
      WLOG(ret, str);
      str = str.substr(0, i);
      break;
    }
  long value = atol(str.c_str());
  if (value < 0) {
    ret = rTooSmallData;
    WLOG(ret, str);
    str = "0";
  } else if (value > USHRT_MAX || (value == USHRT_MAX && nullable)) {
    ret = rTooLargeData;
    WLOG(ret, str);
    str = kUSmallIntMax_1;
  }
  return ret;
}
/*
 * ToDo There is still some work for decimal type
 */
RetCode OperateDecimal::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if ((str == "" || str == "NULL") && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  if (Decimal::StringToDecimal(this->precision_, this->scale_, str)) {
    ret = rSuccess;
  } else {
    ret = rInvalidInsertData;
    ELOG(ret, str);
  }
  return ret;
}

RetCode OperateBool::CheckSet(string& str) const {
  RetCode ret = rSuccess;
  if (str == "" && nullable) return rSuccess;
  if (str == "" && !nullable) {
    ret = rInvalidNullData;
    ELOG(ret, str);
    return ret;
  }
  if (str != "false" && str != "true") {
    ret = rIncorrectData;
    ELOG(ret, str);
    return ret;
  }
  return ret;
}
