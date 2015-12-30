/*
 * data_type.h
 *
 *  Created on: May 12, 2013
 *      Author: wangli
 */

#ifndef COMMON_DATA_TYPE_H_
#define COMMON_DATA_TYPE_H_
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <limits.h>
#include <float.h>
#include <glog/logging.h>
#include <string>
#include <sstream>
#include "./error_define.h"
#include "./hash.h"
#include "../utility/string_process.h"
#include "./types/NValue.hpp"
#include "./types/decimal.h"
using boost::gregorian::date_duration;
using boost::gregorian::from_undelimited_string;
using boost::gregorian::from_string;
using boost::posix_time::duration_from_string;
using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;
using boost::posix_time::time_from_string;
using boost::posix_time::neg_infin;
using boost::hash_value;
using boost::hash_combine;
using decimal::NValue;
using claims::common::Decimal;
using decimal::ExportSerializeOutput;
using boost::lexical_cast;
using namespace decimal;
#define DATA_TYPE_NUMBER 20
enum data_type {
  t_smallInt,
  t_int,
  t_u_long,
  t_float,
  t_double,
  t_string,
  t_date,
  t_time,
  t_datetime,
  t_decimal,
  t_boolean,
  t_u_smallInt,
  t_date_day,
  t_date_week,
  t_date_month,
  t_date_year,
  t_date_quarter
};

// enum TransformRet { rSuccess = 0, kWarning, kError };
inline string GetPrecision(double d) {
  ostringstream ss;
  ss.precision(1000);
  ss << d;
  return ss.str();
}
typedef int RetCode;

const string kIntMin = GetPrecision(-INT_MAX);       //"-2147483648";
const string kIntMax = GetPrecision(INT_MAX);        //"2147483647";
const string kIntMax_1 = GetPrecision(INT_MAX - 1);  // "2147483646";
const string kFloatMin =
    GetPrecision(-FLT_MAX);  //"-340282346638528859811704183484516925440";
const string kFloatMax =
    GetPrecision(FLT_MAX);  //"340282346638528859811704183484516925440";
const string kFloatMax_1 =
    GetPrecision(FLT_MAX - 1e23);  // "340282346638528746474908594613031796736";
const string kDoubleMin = GetPrecision(-DBL_MAX);
const string kDoubleMax = GetPrecision(DBL_MAX);
const string kDoubleMax_1 = GetPrecision(DBL_MAX - 1e305);
const string kULongMax =
    GetPrecision(ULONG_LONG_MAX);  // lexical_cast<string>(ULONG_LONG_MAX);
const string kULongMax_1 = GetPrecision(
    ULONG_LONG_MAX - 1);  // lexical_cast<string>(ULONG_LONG_MAX - 1);
const string kSmallIntMin =
    GetPrecision(-SHRT_MAX);  // lexical_cast<string>(-SHRT_MAX);
const string kSmallIntMax = GetPrecision(SHRT_MAX);
const string kSmallIntMax_1 = GetPrecision(SHRT_MAX - 1);
const string kUSmallIntMax = GetPrecision(USHRT_MAX);
const string kUSmallIntMax_1 = GetPrecision(USHRT_MAX - 1);

typedef void (*fun)(void*, void*);

#define NULL_SMALL_INT SHRT_MAX
#define NULL_INT INT_MAX
#define NULL_U_LONG ULONG_LONG_MAX

#define NULL_FLOAT FLT_MAX   // const transfor to int 2139095039
#define NULL_DOUBLE DBL_MAX  // const transfor to int -1
#define NULL_STRING '7'
#define NULL_DATE neg_infin  // is_neg_infinity() return true
#define NULL_TIME neg_infin
#define NULL_DATETIME neg_infin
#define NULL_DECIMAL nvalue_null
#define NULL_U_SMALL_INT USHRT_MAX
#define NULL_BOOLEAN 2

static NValue nvalue_null = NValue::getDecimalValueFromString(
    "99999999999999999999999999.999999999999");
const int max_double_length = 1 + 308;
// static int count_open_for_data_column=0;

/**
 * the number of bytes that are aligned between any two adjacent data types
 */
#define allign_bytes 4
#define byte_align(size) (((size - 1) / allign_bytes + 1) * allign_bytes)

template <typename T>
inline void ADD_FUNC(void* target, void* increment) {
  *(T*)target += *(T*)increment;
}
template <typename T>
inline void MULTIPLE(void* target, void* increment) {
  (*(T*)target) = (*(T*)target) * (*(T*)increment);
}
template <>
inline void ADD_FUNC<char*>(void* target, void* increment) {}

template <>
inline void ADD_FUNC<NValue*>(void* target, void* increment) {
  *(NValue*)target = ((NValue*)target)->op_add(*(NValue*)increment);
}
template <>
inline void ADD_FUNC<date*>(void* target, void* increment) {
  *(date*)target = *(date*)target + *(date_duration*)increment;
}
template <>
inline void ADD_FUNC<ptime*>(void* target, void* increment) {
  *(ptime*)target = *(ptime*)target + *(time_duration*)increment;
}
template <>
inline void ADD_FUNC<time_duration*>(void* target, void* increment) {
  *(time_duration*)target =
      *(time_duration*)target + *(time_duration*)increment;
}

template <typename T>
inline void MIN(void* target, void* increment) {
  if (*(T*)target > *(T*)increment) *(T*)target = *(T*)increment;
}
template <>
inline void MIN<char*>(void* target, void* increment) {}
template <>
inline void MIN<NValue*>(void* target, void* increment) {
  *(NValue*)target = ((NValue*)target)->op_min(*(NValue*)increment);
}

template <typename T>
inline void MAX(void* target, void* increment) {
  if (*(T*)target < *(T*)increment) *(T*)target = *(T*)increment;
}
template <>
inline void MAX<char*>(void* target, void* increment) {}
template <>
inline void MAX<NValue*>(void* target, void* increment) {
  *(NValue*)target = ((NValue*)target)->op_max(*(NValue*)increment);
}

template <typename T>
inline void IncreaseByOne(void* target, void* increment) {
  (*(T*)target)++;
}
template <>
inline void IncreaseByOne<char*>(void* target, void* increment) {}

template <>
inline void IncreaseByOne<NValue*>(void* target, void* increment) {
  NValue nv1 = NValue::getDecimalValueFromString("1");
  *(NValue*)target = ((NValue*)target)->op_add(nv1);
}
template <typename T>  //暂时先实现这点
inline void ADD_IncreaseByOne(void* target, void* increment) {
  //	*(T*)target+=*(T*)increment;//add
  //	(*(T*)target)++;//increase
}
template <>
inline void ADD_IncreaseByOne<char*>(void* target, void* increment) {}

template <>
inline void ADD_IncreaseByOne<NValue*>(void* target, void* increment) {
  *(NValue*)target = ((NValue*)target)->op_add(*(NValue*)increment);  // add
  NValue nv1 = NValue::getDecimalValueFromString("1");
  *(NValue*)target = ((NValue*)target)->op_add(nv1);
}
// template<>
// inline void ADD_IncreaseByOne<date*>(void* target, void* increment)
//{
//	*(date*)target = *(date*)target + *(date_duration*)increment;
//	(*(date*)target)++;
//}
// template<>
// inline void ADD_IncreaseByOne<ptime*>(void* target, void* increment)
//{
//	*(ptime*)target = *(ptime*)target + *(date_duration*)increment;
//	(*(ptime*)target)++;
//}
// template<>
// inline void ADD_IncreaseByOne<time_duration*>(void* target, void* increment)
//{
//	*(time_duration*)target = *(time_duration*)target +
//*(time_duration*)increment;
//	(*(time_duration*)target)++;
//}

template <typename T>
inline void assigns(const void* const& src, void* const& desc) {
  *(T*)desc = *(T*)src;
}
template <>
inline void assigns<char*>(const void* const& src, void* const& desc) {
  strcpy((char*)desc, (char*)src);
}
class Operate {
 public:
  Operate(bool _nullable = true) : nullable(_nullable){};
  virtual ~Operate(){};
  inline void ass(void* src, void* desc) { *(int*)desc = *(int*)src; }
  inline virtual void assignment(const void* const& src,
                                 void* const& desc) const = 0;
  virtual unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const = 0;
  virtual unsigned getPartitionValue(const void* key,
                                     const unsigned long& mod) const = 0;
  virtual unsigned getPartitionValue(const void* key) const = 0;
  virtual std::string toString(void* value) = 0;
  virtual void toValue(void* target, const char* string) = 0;
  virtual bool equal(const void* const& a, const void* const& b) const = 0;
  virtual bool less(const void*& a, const void*& b) const = 0;
  virtual bool greate(const void*& a, const void*& b) const = 0;
  virtual void add(void* target, void* increment) = 0;
  virtual void multiple(void* target, void* increment) = 0;
  virtual int compare(const void* a, const void* b) const = 0;
  virtual fun GetADDFunction() = 0;
  virtual fun GetMINFunction() = 0;
  virtual fun GetMAXFunction() = 0;
  virtual fun GetIncreateByOneFunction() = 0;
  virtual fun GetAVGFunction() = 0;
  void (*assign)(const void* const& src, void* const& desc);
  virtual Operate* duplicateOperator() const = 0;

  inline virtual bool setNull(void* value) = 0;
  inline virtual bool isNull(void* value) const = 0;

  inline virtual RetCode CheckSet(string& str) const = 0;
  inline virtual void SetDefault(string& str) const = 0;

 public:
  bool nullable;
  unsigned size;
};

class OperateInt : public Operate {
 public:
  OperateInt(bool _nullable = true) {
    this->nullable = _nullable;
    assign = assigns<int>;
  }
  ~OperateInt() {}
  inline void assignment(const void* const& src, void* const& desc) const {
    *(int*)desc = *(int*)src;
  }
  inline std::string toString(void* value) {
    std::string ret;
    if (this->nullable == true && (*(int*)value) == NULL_INT) {
      ret = "NULL";
    } else {
      std::ostringstream ss;
      ss << *(int*)value;
      ret = ss.str();
    }
    return ret;
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      *(int*)target = NULL_INT;
    else
      *(int*)target = atoi(string);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(int*)a == *(int*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(int*)a < *(int*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(int*)a > *(int*)b;
  }
  int compare(const void* a, const void* b) const {
    return *(int*)a - *(int*)b;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<int>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    MULTIPLE<int>(target, increment);
  }
  inline fun GetADDFunction() { return ADD_FUNC<int>; }
  inline fun GetMINFunction() { return MIN<int>; }
  inline fun GetMAXFunction() { return MAX<int>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<int>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<int>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    return partition_function->get_partition_value(*(int*)key);
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value(*(int*)key);
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value(*(int*)key) % mod;
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(int*)value = NULL_INT;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(int*)value) == NULL_INT) return true;
    return false;
  }
  Operate* duplicateOperator() const { return new OperateInt(this->nullable); }

  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = string("0"); }
};

class OperateFloat : public Operate {
 public:
  OperateFloat(bool _nullable = true) { this->nullable = _nullable; };
  ~OperateFloat(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    *(float*)desc = *(float*)src;
  };
  inline std::string toString(void* value) {
    std::string ret;
    if (this->nullable == true && (*(float*)value) == NULL_FLOAT)
      ret = "NULL";
    else {
      std::ostringstream ss;
      ss << *(float*)value;
      ret = ss.str();
    }
    return ret;
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      *(float*)target = NULL_FLOAT;
    else
      *(float*)target = atof(string);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(float*)a == *(float*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(float*)a < *(float*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(float*)a > *(float*)b;
  }
  int compare(const void* a, const void* b) const {
    return *(float*)a - *(float*)b;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<float>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    MULTIPLE<float>(target, increment);
  }
  inline fun GetADDFunction() { return ADD_FUNC<float>; }
  inline fun GetMINFunction() { return MIN<float>; }
  inline fun GetMAXFunction() { return MAX<float>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<float>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<float>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    return partition_function->get_partition_value(*(float*)key);
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value(*(float*)key);
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value(*(float*)key) % mod;
  }
  Operate* duplicateOperator() const {
    return new OperateFloat(this->nullable);
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(float*)value = NULL_FLOAT;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(int*)value) == (int)NULL_FLOAT)
      return true;
    return false;
  }
  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = string("0"); }
};

class OperateDouble : public Operate {
 public:
  OperateDouble(bool _nullable = true) { this->nullable = _nullable; };
  ~OperateDouble(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    *(double*)desc = *(double*)src;
  }
  inline std::string toString(void* value) {
    std::string ret;
    if (this->nullable == true && (*(double*)value) == NULL_DOUBLE)
      ret = "NULL";
    else {
      std::ostringstream ss;
      ss << *(double*)value;
      ret = ss.str();
    }
    return ret;
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      *(double*)target = NULL_DOUBLE;
    else
      *(double*)target = atof(string);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(double*)a == *(double*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(double*)a < *(double*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(double*)a > *(double*)b;
  }
  int compare(const void* a, const void* b) const {
    return *(double*)a - *(double*)b;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<double>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    MULTIPLE<double>(target, increment);
  }
  inline fun GetADDFunction() { return ADD_FUNC<double>; }
  inline fun GetMINFunction() { return MIN<double>; }
  inline fun GetMAXFunction() { return MAX<double>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<double>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<double>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    return partition_function->get_partition_value(*(double*)key);
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value(*(double*)key);
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value(*(double*)key) % mod;
  }
  Operate* duplicateOperator() const {
    return new OperateDouble(this->nullable);
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(double*)value = NULL_DOUBLE;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(int*)value) == (int)NULL_DOUBLE)
      return true;
    return false;
  }
  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = string("0"); }
};

class OperateULong : public Operate {
 public:
  OperateULong(bool _nullable = true) { this->nullable = _nullable; };
  ~OperateULong(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    *(unsigned long*)desc = *(unsigned long*)src;
  }
  inline std::string toString(void* value) {
    std::string ret;
    if (this->nullable == true && (*(unsigned long*)value) == NULL_U_LONG)
      ret = "NULL";
    else {
      std::ostringstream ss;
      ss << *(unsigned long*)value;
      ret = ss.str();
    }
    return ret;
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      *(unsigned long*)target = NULL_U_LONG;
    else
      *(unsigned long*)target = strtoul(string, 0, 10);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(unsigned long*)a == *(unsigned long*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(unsigned long*)a < *(unsigned long*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(unsigned long*)a > *(unsigned long*)b;
  }
  int compare(const void* a, const void* b) const {
    return *(unsigned long*)a - *(unsigned long*)b;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<unsigned long>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    MULTIPLE<unsigned long>(target, increment);
  }
  inline fun GetADDFunction() { return ADD_FUNC<unsigned long>; }
  inline fun GetMINFunction() { return MIN<unsigned long>; }
  inline fun GetMAXFunction() { return MAX<unsigned long>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<unsigned long>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<unsigned long>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    return partition_function->get_partition_value(*(unsigned long*)key);
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value(*(unsigned long*)key);
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value(*(unsigned long*)key) % mod;
  }
  Operate* duplicateOperator() const {
    return new OperateULong(this->nullable);
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(unsigned long*)value = NULL_U_LONG;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(unsigned long*)value) == NULL_U_LONG)
      return true;
    return false;
  }
  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = string("0"); }
};

class OperateString : public Operate {
 public:
  OperateString(unsigned size, bool nullable = true) {
    this->size = size;
    this->nullable = nullable;
  }
  OperateString(const OperateString& right) {
    this->size = right.size;
    this->nullable = right.nullable;
  }
  ~OperateString() {}
  inline void assignment(const void* const& src, void* const& desc) const {
    assert(desc != 0 && src != 0);
    strcpy((char*)desc, (char*)src);
  }
  inline std::string toString(void* value) {
    if (this->nullable == true && (*(char*)value) == NULL_STRING)
      return "NULL";
    else
      return trimSpecialCharactor(std::string((char*)value));
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      *(char*)target = NULL_STRING;
    else
      strcpy((char*)target, string);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return strcmp((char*)a, (char*)b) == 0;
  }

  /**
   * The following function may return a wrong value
   */
  bool less(const void*& a, const void*& b) const {
    return strcmp((char*)a, (char*)b) < 0;
  }
  bool greate(const void*& a, const void*& b) const {
    return strcmp((char*)a, (char*)b) > 0;
  }
  int compare(const void* a, const void* b) const {
    return strcmp((char*)a, (char*)b);
  }
  inline void add(void* target, void* increment) {
    // TODO: throw exception or implement the add for string.
    printf("The sum for String is not current supported!\n");
  }
  inline void multiple(void* target, void* increment) {
    // TODO: throw exception or implement the add for string.
    printf("The sum for String is not current supported!\n");
  }
  inline fun GetADDFunction() { return ADD_FUNC<char*>; }
  inline fun GetMINFunction() { return MIN<char*>; }
  inline fun GetMAXFunction() { return MAX<char*>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<char*>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<char*>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    printf("The hash function for char[] type is not implemented yet!\n");
    assert(false);

    return 0;
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value((std::string)((char*)key));
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value((std::string)((char*)key)) % mod;
  }
  Operate* duplicateOperator() const {
    return new OperateString(this->size, this->nullable);
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(char*)value = NULL_STRING;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(char*)value) == NULL_STRING) return true;
    return false;
  }
  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = ""; }
};

class OperateDate : public Operate {
 public:
  OperateDate(bool _nullable = true) { this->nullable = _nullable; };
  //	~OperateDate(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    assert(desc != 0 && src != 0);
    *(date*)desc = *(date*)src;
  }
  inline std::string toString(void* value) {
    if (this->nullable == true && (*(date*)value).is_neg_infinity() == true)
      return "NULL";
    else
      return to_iso_extended_string(*(date*)value);
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      setNull(target);
    else {
      std::string s(string);
      bool all_digit = false;
      if (s.length() == 8) {
        all_digit = true;
        for (int i = 0; i < 8; i++) {
          if (isdigit(s[i]) == 0) {
            all_digit = false;
            break;
          }
        }
      }
      if (all_digit == true)
        *(date*)target = from_undelimited_string(s);
      else
        *(date*)target = from_string(s);
    }
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(date*)a == *(date*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(date*)a < *(date*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(date*)a > *(date*)b;
  }
  int compare(const void* a, const void* b) const {
    if (*(date*)a < *(date*)b)
      return -1;
    else if (*(date*)a > *(date*)b)
      return 1;
    return 0;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<date*>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    // TODO: throw exception or implement the add for string.
    printf("The sum for String is not current supported!\n");
  }
  inline fun GetADDFunction() { return ADD_FUNC<date*>; }
  inline fun GetMINFunction() { return MIN<date*>; }
  inline fun GetMAXFunction() { return MAX<date*>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<date*>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<date*>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    printf("The hash function for date type is not implemented yet!\n");
    assert(false);

    return 0;
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value((*(boost::gregorian::date*)(key)).julian_day());
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value((*(boost::gregorian::date*)(key)).julian_day()) %
           mod;
  }
  Operate* duplicateOperator() const { return new OperateDate(this->nullable); }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    date d(NULL_DATE);
    *(date*)value = d;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(date*)value).is_neg_infinity() == true)
      return true;
    return false;
  }
  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = "1400-01-01"; }
};

class OperateTime : public Operate {
 public:
  OperateTime(bool _nullable = true) { this->nullable = _nullable; };
  //	~OperateTime(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    assert(desc != 0 && src != 0);
    *(time_duration*)desc = *(time_duration*)src;
  }
  inline std::string toString(void* value) {
    if (this->nullable == true &&
        (*(time_duration*)value).is_neg_infinity() == true)
      return "NULL";
    else
      return to_simple_string(*(time_duration*)value);
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      setNull(target);
    else
      *(time_duration*)target = duration_from_string(string);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(time_duration*)a == *(time_duration*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(time_duration*)a < *(time_duration*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(time_duration*)a > *(time_duration*)b;
  }
  int compare(const void* a, const void* b) const {
    if (*(time_duration*)a < *(time_duration*)b)
      return -1;
    else if (*(time_duration*)a > *(time_duration*)b)
      return 1;
    return 0;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<time_duration*>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    // TODO: throw exception or implement the add for string.
    printf("The sum for String is not current supported!\n");
  }
  inline fun GetADDFunction() { return ADD_FUNC<time_duration*>; }
  inline fun GetMINFunction() { return MIN<time_duration*>; }
  inline fun GetMAXFunction() { return MAX<time_duration*>; }
  inline fun GetIncreateByOneFunction() {
    return IncreaseByOne<time_duration*>;
  }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<time_duration*>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    printf("The hash function for time type is not implemented yet!\n");
    assert(false);

    return 0;
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value((*(time_duration*)(key)).total_nanoseconds());
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value((*(time_duration*)(key)).total_nanoseconds()) %
           mod;
  }
  Operate* duplicateOperator() const { return new OperateTime(this->nullable); }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    time_duration d(NULL_TIME);
    *(time_duration*)value = d;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true &&
        (*(time_duration*)value).is_neg_infinity() == true)
      return true;
    return false;
  }

  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = "00:00:00.000000"; }
};

class OperateDatetime : public Operate {
 public:
  OperateDatetime(bool _nullable = true) { this->nullable = _nullable; };
  //	~OperateDatetime(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    assert(desc != 0 && src != 0);
    *(ptime*)desc = *(ptime*)src;
  }
  inline std::string toString(void* value) {
    if (this->nullable == true && (*(ptime*)value).is_neg_infinity() == true)
      return "NULL";
    else
      return to_iso_extended_string(*(ptime*)value);
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      setNull(target);
    else
      *(ptime*)target = time_from_string(string);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(ptime*)a == *(ptime*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(ptime*)a < *(ptime*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(ptime*)a > *(ptime*)b;
  }
  int compare(const void* a, const void* b) const {
    if (*(ptime*)a < *(ptime*)b)
      return -1;
    else if (*(ptime*)a > *(ptime*)b)
      return 1;
    return 0;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<ptime*>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    // TODO: throw exception or implement the add for string.
    printf("The sum for String is not current supported!\n");
  }
  inline fun GetADDFunction() { return ADD_FUNC<ptime*>; }
  inline fun GetMINFunction() { return MIN<ptime*>; }
  inline fun GetMAXFunction() { return MAX<ptime*>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<ptime*>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<ptime*>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    printf("The hash function for datetime type is not implemented yet!\n");
    assert(false);

    return 0;
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value(to_simple_string(*(ptime*)(key))) % mod;
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value(to_simple_string(*(ptime*)(key)));
    // TODO: maybe there is a more efficient way.
  }
  Operate* duplicateOperator() const {
    return new OperateDatetime(this->nullable);
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    ptime d(NULL_DATETIME);
    *(ptime*)value = d;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(ptime*)value).is_neg_infinity() == true)
      return true;
    return false;
  }

  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = "1400-01-01 00:00:00.000000"; }
};

class OperateSmallInt : public Operate {
 public:
  OperateSmallInt(bool _nullable = true) {
    this->nullable = _nullable;
    assign = assigns<short>;
  }
  //	~OperateSmallInt(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    *(short*)desc = *(short*)src;
  }
  inline std::string toString(void* value) {
    if (this->nullable == true && (*(short*)value) == NULL_SMALL_INT)
      return "NULL";
    else {
      std::ostringstream ss;
      ss << *(short*)value;
      std::string ret = ss.str();
      return ret;
    }
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) &&
        this->nullable == true)  // modified by Li Wang in Sep.10th
      //		if(string==0 && this->nullable ==true)
      *(short*)target = NULL_SMALL_INT;
    else
      *(short*)target = (short)atoi(string);
  };
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(short*)a == *(short*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(short*)a < *(short*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(short*)a > *(short*)b;
  }
  int compare(const void* a, const void* b) const {
    return *(short*)a - *(short*)b;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<short>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    MULTIPLE<short>(target, increment);
  }
  inline fun GetADDFunction() { return ADD_FUNC<short>; }
  inline fun GetMINFunction() { return MIN<short>; }
  inline fun GetMAXFunction() { return MAX<short>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<short>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<short>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    return partition_function->get_partition_value(*(short*)key);
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value(*(short*)key);
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value(*(short*)key) % mod;
  }
  Operate* duplicateOperator() const {
    return new OperateSmallInt(this->nullable);
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(short*)value = NULL_SMALL_INT;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(short*)value) == NULL_SMALL_INT)
      return true;
    return false;
  }

  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = "0"; }
};

class OperateUSmallInt : public Operate {
 public:
  OperateUSmallInt(bool _nullable = true) {
    this->nullable = _nullable;
    assign = assigns<unsigned short>;
  }
  //  ~OperateSmallInt(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    *(unsigned short*)desc = *(unsigned short*)src;
  }
  inline std::string toString(void* value) {
    if (this->nullable == true && (*(unsigned short*)value) == NULL_U_SMALL_INT)
      return "NULL";
    std::ostringstream ss;
    ss << *(unsigned short*)value;
    std::string ret = ss.str();
    return ret;
  }
  void toValue(void* target, const char* string) {
    if ((strcmp(string, "") == 0) && this->nullable == true)
      *(unsigned short*)target = NULL_U_SMALL_INT;
    else
      *(unsigned short*)target = (unsigned short)atoi(string);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(unsigned short*)a == *(unsigned short*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(unsigned short*)a < *(unsigned short*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(unsigned short*)a > *(unsigned short*)b;
  }
  int compare(const void* a, const void* b) const {
    return *(unsigned short*)a - *(unsigned short*)b;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<unsigned short>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    MULTIPLE<unsigned short>(target, increment);
  }
  inline fun GetADDFunction() { return ADD_FUNC<unsigned short>; }
  inline fun GetMINFunction() { return MIN<unsigned short>; }
  inline fun GetMAXFunction() { return MAX<unsigned short>; }
  inline fun GetIncreateByOneFunction() {
    return IncreaseByOne<unsigned short>;
  }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<unsigned short>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    return partition_function->get_partition_value(*(unsigned short*)key);
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value(*(unsigned short*)key);
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value(*(unsigned short*)key) % mod;
  }
  Operate* duplicateOperator() const {
    return new OperateUSmallInt(this->nullable);
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(unsigned short*)value = NULL_U_SMALL_INT;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(unsigned short*)value) == NULL_U_SMALL_INT)
      return true;
    return false;
  }
  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = "0"; }
};

class OperateDecimal : public Operate {
 public:
  OperateDecimal( int size = 0 , bool nullable = true) {
    assign = assigns<int>;
    this->size = size;
    this->nullable = nullable;
	this->precision_ = size / 1000;
	this->scale_ = size % 1000;
	cout << this->precision_ << ":" << this->scale_ <<endl;
  }
  //	~OperateDecimal(){};
  inline void assignment(const void* const& src, void* const& desc) const {
    *(Decimal*)desc = *(Decimal*)src;
  }
  inline std::string toString(void* value) {
    if (this->nullable == true && ((Decimal*)value)->isNull())
      return "NULL";
	/*
    char buf[43] = {"\0"};
    ExportSerializeOutput out(buf, 43);
    ((NValue*)value)->serializeToExport(out, &size);
    return std::string(buf + 4);
    */
    return ((Decimal*)value)->ToString(this->scale_);
  };
  static std::string toString(const NValue v, unsigned n_o_d_d = 12) {
    //		if (this->nullable == true && compare(v, (void*)(&NULL_DECIMAL))
    //==
    // 0)
    //			return "NULL";
    cout << "here toString:::" << endl;
	return "test by csw";
    char buf[43] = {"\0"};
    ExportSerializeOutput out(buf, 43);
    (v).serializeToExport(out, &n_o_d_d);
    return std::string(buf + 4);
  }
  void toValue(void* target, const char* str) {
    if ((strcmp(str, "") == 0) && this->nullable == true)
      *(Decimal*)target = Decimal::CreateNullDecimal();
    else
      *(Decimal*)target = Decimal(precision_, scale_, str);
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return ((Decimal*)a)->op_equals(*(Decimal*)b);
  }
  bool less(const void*& a, const void*& b) const {
    if (((Decimal*)a)->op_equals(*(Decimal*)b)) return false;
    Decimal tmp = ((Decimal*)a)->op_min(*(Decimal*)b);
    if (tmp.op_equals(*(Decimal*)a)) return true;
    return false;
  }
  bool greate(const void*& a, const void*& b) const {
    if (((Decimal*)a)->op_equals(*(Decimal*)b)) return false;
    Decimal tmp = ((Decimal*)a)->op_min(*(Decimal*)b);
    if (tmp.op_equals(*(Decimal*)a)) return false;
    return true;
  }
  int compare(const void* a, const void* b) const {
    if ((*(Decimal*)a).op_equals(*(Decimal*)b))
      return 0;
    else if (less(a, b))
      return -1;
    return 1;
  }
  inline void add(void* target, void* increment) {
      *(Decimal*)target = ((Decimal*)target)->op_add(*(Decimal*)increment);
  }
  inline void multiple(void* target, void* increment) {
    (*(Decimal*)target) = ((Decimal*)target)->op_multiply(*(Decimal*)increment);
  }
  inline fun GetADDFunction() { return ADD_FUNC<NValue*>; }
  inline fun GetMINFunction() { return MIN<NValue*>; }
  inline fun GetMAXFunction() { return MAX<NValue*>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<NValue*>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<NValue*>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {	
    const void * pttint = (&((*(Decimal*)key).GetTTInt()));
    unsigned long ul1 = *reinterpret_cast<const unsigned long*>(pttint);
    unsigned long ul2 = *reinterpret_cast<const unsigned long*>(pttint + 8);
    unsigned long ul3 = *reinterpret_cast<const unsigned long*>(pttint + 16);
    unsigned long ul4 = *reinterpret_cast<const unsigned long*>(pttint + 24);
    return partition_function->get_partition_value(ul1 + ul2 + ul3 + ul4);
  }
  unsigned getPartitionValue(const void* key) const {
	  const void * pttint = (&((*(Decimal*)key).GetTTInt()));
	  unsigned long ul1 = *reinterpret_cast<const unsigned long*>(pttint);
	  unsigned long ul2 = *reinterpret_cast<const unsigned long*>(pttint + 8);
	  unsigned long ul3 = *reinterpret_cast<const unsigned long*>(pttint + 16);
	  unsigned long ul4 = *reinterpret_cast<const unsigned long*>(pttint + 24);

    boost::hash_combine(ul1, ul2);	
	boost::hash_combine(ul1, ul3);
	boost::hash_combine(ul1, ul4);
    return ul1;
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
	  const void * pttint = (&((*(Decimal*)key).GetTTInt()));
	  unsigned long ul1 = *reinterpret_cast<const unsigned long*>(pttint);
	  unsigned long ul2 = *reinterpret_cast<const unsigned long*>(pttint + 8);
	  unsigned long ul3 = *reinterpret_cast<const unsigned long*>(pttint + 16);
	  unsigned long ul4 = *reinterpret_cast<const unsigned long*>(pttint + 24);

    boost::hash_combine(ul1, ul2);
	boost::hash_combine(ul1, ul3);
	boost::hash_combine(ul1, ul4);
    return ul1 % mod;
  }
  Operate* duplicateOperator() const {
    //    return new OperateDecimal(number_of_decimal_digits_, this->nullable);

    return new OperateDecimal(size, nullable);
  }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(Decimal*)value = Decimal::CreateNullDecimal();
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && ((Decimal*)value)->isNull())
      return true;
    return false;
  }

//  unsigned number_of_decimal_digits_;

  /**
   * @TODO min and max check is not implemented yet ! *-_-*
   */
  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = string("0"); }

  private:
  	int precision_;
	int scale_;
  	
};

class OperateBool : public Operate {
 public:
  OperateBool(bool _nullable = true) {
    this->nullable = _nullable;
    assign = assigns<int>;
  }
  inline void assignment(const void* const& src, void* const& desc) const {
    *(int*)desc = *(int*)src;
  }
  inline std::string toString(void* value) {
    if (this->nullable == true && (*(int*)value) == NULL_BOOLEAN)
      return "NULL";
    else {
      std::ostringstream ss;
      if (*(int*)value == 0)
        return "FALSE";
      else
        return "TRUE";
    }
  }
  inline void toValue(void* target, const char* string) {
    std::string f = "FALSE";
    std::string t = "TRUE";
    std::string n = "NULL";
    if ((strcmp(string, n.c_str()) == 0) && this->nullable == true) {
      *(int*)target = NULL_BOOLEAN;
    } else if (strcmp(f.c_str(), string) == 0) {
      *(int*)target = 0;
    } else {
      *(int*)target = 1;
    }
  }
  inline bool equal(const void* const& a, const void* const& b) const {
    return *(int*)a == *(int*)b;
  }
  bool less(const void*& a, const void*& b) const {
    return *(int*)a < *(int*)b;
  }
  bool greate(const void*& a, const void*& b) const {
    return *(int*)a > *(int*)b;
  }
  int compare(const void* a, const void* b) const {
    return *(int*)a - *(int*)b;
  }
  inline void add(void* target, void* increment) {
    ADD_FUNC<int>(target, increment);
  }
  inline void multiple(void* target, void* increment) {
    MULTIPLE<int>(target, increment);
  }
  inline fun GetADDFunction() { return ADD_FUNC<int>; }
  inline fun GetMINFunction() { return MIN<int>; }
  inline fun GetMAXFunction() { return MAX<int>; }
  inline fun GetIncreateByOneFunction() { return IncreaseByOne<int>; }
  inline fun GetAVGFunction() { return ADD_IncreaseByOne<int>; }
  unsigned getPartitionValue(
      const void* key, const PartitionFunction* partition_function) const {
    return partition_function->get_partition_value(*(int*)key);
  }
  unsigned getPartitionValue(const void* key) const {
    return boost::hash_value(*(int*)key);
  }
  unsigned getPartitionValue(const void* key, const unsigned long& mod) const {
    return boost::hash_value(*(int*)key) % mod;
  }
  Operate* duplicateOperator() const { return new OperateBool(this->nullable); }

  inline bool setNull(void* value) {
    if (this->nullable == false) return false;
    *(int*)value = NULL_BOOLEAN;
    return true;
  }

  inline bool isNull(void* value) const {
    if (this->nullable == true && (*(int*)value) == NULL_SMALL_INT) return true;
    return false;
  }

  RetCode CheckSet(string& str) const;
  void SetDefault(string& str) const { str = "false"; }
};

class column_type {
 public:
  column_type(data_type type, unsigned _size = 0, bool _nullable = true)
      : type(type), size(_size), nullable(_nullable) {
    initialize();
  }
  column_type(const column_type& r) {
    this->type = r.type;
    this->size = r.size;
    this->nullable = r.nullable;
    this->operate = r.operate->duplicateOperator();
  }
  column_type& operator=(const column_type& r) {
    this->type = r.type;
    this->size = r.size;
    this->nullable = r.nullable;
    this->operate = r.operate->duplicateOperator();
  }
  column_type() : operate(0){};
  ~column_type() {
    delete operate;
    operate = 0;
  }
  inline unsigned get_length() const {
    switch (type) {
      case t_int:
        return sizeof(int);
      case t_float:
        return sizeof(float);
      case t_double:
        return sizeof(double);
      case t_u_long:
        return sizeof(unsigned long);
      case t_string:
        return byte_align(size);
      case t_date:
        return sizeof(date);
      case t_time:
        return sizeof(time_duration);
      case t_datetime:
        return sizeof(ptime);
      case t_decimal:
	  	/* here the 1000 is the same as the  */
        return sizeof(Decimal);
      case t_smallInt:
        return sizeof(short);
      case t_u_smallInt:
        return sizeof(unsigned short);
      case t_boolean:
        return sizeof(int);
      default:
        return 0;
    }
  }
  bool operator==(const class column_type& c) const {
    return this->type == c.type;
  }
  bool operator<(const class column_type& c) const {
    return this->type < c.type;
  }

 public:
  Operate* operate;
  //这个data_type是什么type
  data_type type;
  bool nullable;
  unsigned size;

 private:
  //且这个data_type的size是多少
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& type& size& nullable;
    if (operate == 0) {
      initialize();
    }
  }
  /**
   * called after deserialization to construct the right operator.
   */
  void initialize() {
    switch (type) {
      case t_int:
        operate = new OperateInt(nullable);
        break;
      case t_float:
        operate = new OperateFloat(nullable);
        break;
      case t_double:
        operate = new OperateDouble(nullable);
        break;
      case t_string:
        operate = new OperateString(size, nullable);
        break;
      case t_u_long:
        operate = new OperateULong(nullable);
        break;
      case t_date:
        operate = new OperateDate(nullable);
        break;
      case t_time:
        operate = new OperateTime(nullable);
        break;
      case t_datetime:
        operate = new OperateDatetime(nullable);
        break;
      case t_decimal:
        operate = new OperateDecimal(size, nullable);
        break;
      case t_smallInt:
        operate = new OperateSmallInt(nullable);
        break;
      case t_u_smallInt:
        operate = new OperateUSmallInt(nullable);
        break;
      case t_boolean:
        operate = new OperateBool(nullable);
        break;
      default:
        operate = 0;
        break;
    }
  }
};

#endif /* DATA_TYPE_H_ */
