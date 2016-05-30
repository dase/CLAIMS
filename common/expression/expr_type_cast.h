/*
 * expr_type_cast.h
 *  Created on: May 29, 2015 3:57:55 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_EXPR_TYPE_CAST_H_
#define COMMON_EXPRESSION_EXPR_TYPE_CAST_H_
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "../data_type.h"
#include <boost/date_time/gregorian/greg_duration.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using boost::gregorian::months;
using boost::gregorian::weeks;
using boost::gregorian::years;
namespace claims {
namespace common {

typedef void *(*TypeCastFunc)(void *value, void *tovalue);
class ExprTypeCast {
 public:
  ExprTypeCast();
  virtual ~ExprTypeCast();
  static TypeCastFunc type_cast_func_[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];
};

/***************int****************************/

inline void *int_to_int(void *value, void *tovalue) {
  //  if (*(int *)value == NULL_INT)  // in order to judge the return result is
  //                                  // NULL,so void * is NULL will be simple
  //    return NULL;
  *(int *)tovalue = *(int *)value;
  return tovalue;
}
inline void *int_to_smallint(void *value, void *tovalue) {
  //  if (*(int *)value == NULL_INT) return NULL;
  *(short int *)tovalue = *(int *)value;
  return tovalue;
}
inline void *int_to_float(void *value, void *tovalue) {
  //  if (*(int *)value == NULL_INT) return NULL;
  *(float *)tovalue = *(int *)value;
  return tovalue;
}
inline void *int_to_double(void *value, void *tovalue) {
  //  if (*(int *)value == NULL_INT) return NULL;
  *(double *)tovalue = *(int *)value;
  return tovalue;
}
inline void *int_to_ulong(void *value, void *tovalue) {
  //  if (*(int *)value == NULL_INT) return NULL;
  *(unsigned long *)tovalue = *(int *)value;
  return tovalue;
}
inline void *int_to_decimal(void *value, void *tovalue) {
  //  if (*(int *)value == NULL_INT) return NULL;
  stringstream va;
  va << *(int *)value;
  *(Decimal *)tovalue = Decimal(DECIMAL_PSUBS, 0, va.str());
  va.clear();
  return tovalue;
}
inline void *int_to_boolean(void *value, void *tovalue) {
  *(bool *)tovalue = (*(int *)value == 0 ? 0 : 1);
  return tovalue;
}

/***************int****************************/
/***************string****************************/

inline void *string_to_int(void *value, void *tovalue) {
  //  if(*(string *)value==NULL_STRING)
  //      return NULL;
  *(int *)tovalue = atoi((char *)value);
  return tovalue;
}
inline void *string_to_smallint(void *value, void *tovalue) {
  //  if(*(string *)value==NULL_STRING)
  //      return NULL;
  *(short int *)tovalue = atoi((char *)value);  //???
  return tovalue;
}
inline void *string_to_ulong(void *value, void *tovalue) {
  //  if(*(string *)value==NULL_STRING)
  //      return NULL;
  *(unsigned long *)tovalue = strtoul((char *)value, NULL, 10);
  return tovalue;
}
inline void *string_to_float(void *value, void *tovalue) {
  //  if(*(string *)value==NULL_STRING)
  //      return NULL;
  *(float *)tovalue = atof((char *)value);
  return tovalue;
}
inline void *string_to_double(void *value, void *tovalue) {
  //  if(*(string *)value==NULL_STRING)
  //      return NULL;
  *(double *)tovalue = strtod((char *)value, NULL);
  return tovalue;
}
inline void *string_to_string(void *value, void *tovalue) {
  //  if(*(string *)value==NULL_STRING)
  //      return NULL;
  strcpy((char *)tovalue, (char *)value);
  return tovalue;
}
inline void *string_to_decimal(void *value, void *tovalue) {
  *(Decimal *)tovalue =
      Decimal(DECIMAL_MAXPRCISION, DECIMAL_MAXSCALE, (char *)value);
  return tovalue;
}
inline void *string_to_boolean(void *value, void *tovalue) {
  *(bool *)tovalue = ((char *)value == NULL ? 0 : 1);
  return tovalue;
}
inline void *string_to_date(void *value, void *tovalue) {
  *(date *)tovalue = from_string(string((char *)value));
  return tovalue;
}
inline void *string_to_time(void *value, void *tovalue) {
  *(time_duration *)tovalue = duration_from_string(string((char *)value));
  return tovalue;
}

inline void *string_to_date_day(void *value, void *tovalue) {
  *(date_duration *)tovalue = date_duration(atof((char *)value));
  return tovalue;
}
inline void *string_to_date_week(void *value, void *tovalue) {
  *(weeks *)tovalue = weeks(atof((char *)value));
  return tovalue;
}
inline void *string_to_date_month(void *value, void *tovalue) {
  *(months *)tovalue = months(atof((char *)value));
  return tovalue;
}
inline void *string_to_date_year(void *value, void *tovalue) {
  *(years *)tovalue = years(atof((char *)value));
  return tovalue;
}
inline void *string_to_date_quarter(void *value, void *tovalue) {
  *(months *)tovalue = months(atof((char *)value) * 3);
  return tovalue;
}
inline void *string_to_datetime(void *value, void *tovalue) {
  *(ptime *)tovalue = time_from_string(string((char *)value));
  return tovalue;
}
/***************string****************************/
/***************unsigned long****************************/

inline void *ulong_to_ulong(void *value, void *tovalue) {
  *(unsigned long *)tovalue = *(unsigned long *)value;
  return tovalue;
}
inline void *ulong_to_float(void *value, void *tovalue) {
  *(float *)tovalue = *(unsigned long *)value;
  return tovalue;
}
inline void *ulong_to_double(void *value, void *tovalue) {
  *(double *)tovalue = *(unsigned long *)value;
  return tovalue;
}
inline void *ulong_to_decimal(void *value, void *tovalue) {
  stringstream va;
  va << *(unsigned long *)value;
  *(Decimal *)tovalue = Decimal(DECIMAL_PSUBS, 0, (char *)value);
  va.clear();
  return tovalue;
}
inline void *ulong_to_boolean(void *value, void *tovalue) {
  *(bool *)tovalue = (*(unsigned long *)value == 0 ? 0 : 1);
  return tovalue;
}

/***************unsigned long****************************/

/***************smallInt****************************/
inline void *smallInt_to_smallInt(void *value, void *tovalue) {
  *(short int *)tovalue = *(short int *)value;
  return tovalue;
}
inline void *smallInt_to_int(void *value, void *tovalue) {
  *(int *)tovalue = *(short int *)value;
  return tovalue;
}
inline void *smallInt_to_ulong(void *value, void *tovalue) {
  *(unsigned long *)tovalue = *(short int *)value;
  return tovalue;
}
inline void *smallInt_to_float(void *value, void *tovalue) {
  *(float *)tovalue = *(short int *)value;
  return tovalue;
}
inline void *smallInt_to_double(void *value, void *tovalue) {
  *(double *)tovalue = *(short int *)value;
  return tovalue;
}
inline void *smallInt_to_string(void *value, void *tovalue) {
  short int tvalue = *(short int *)value;
  stringstream va;
  va << tvalue;
  strcpy((char *)tovalue, va.str().c_str());
  va.clear();
  return tovalue;
}
inline void *smallInt_to_boolean(void *value, void *tovalue) {
  *(bool *)tovalue = (*(short int *)value == 0 ? 0 : 1);
  return tovalue;
}
inline void *smallInt_to_decimal(void *value, void *tovalue) {
  stringstream va;
  va << *(short int *)value;
  *(Decimal *)tovalue = Decimal(DECIMAL_PSUBS, 0, va.str());
  va.clear();
  return tovalue;
}
/***************smallInt****************************/

/***************float****************************/
inline void *float_to_float(void *value, void *tovalue) {
  *(float *)tovalue = *(float *)value;
  return tovalue;
}
inline void *float_to_double(void *value, void *tovalue) {
  *(double *)tovalue = *(float *)value;
  return tovalue;
}
inline void *float_to_string(void *value, void *tovalue) {
  float tvalue = *(float *)value;
  stringstream va;
  va << tvalue;
  strcpy((char *)tovalue, va.str().c_str());
  va.clear();
  return tovalue;
}
inline void *float_to_boolean(void *value, void *tovalue) {
  *(bool *)tovalue = (*(float *)value == 0 ? 0 : 1);
  return tovalue;
}
inline void *float_to_decimal(void *value, void *tovalue) {
  stringstream va;
  va << *(float *)value;
  *(Decimal *)tovalue =
      Decimal(DECIMAL_MAXPRCISION, DECIMAL_MAXSCALE, va.str());
  va.clear();
  return tovalue;
}
/***************float****************************/

/***************double****************************/
inline void *double_to_double(void *value, void *tovalue) {
  *(double *)tovalue = *(double *)value;
  return tovalue;
}
inline void *double_to_string(void *value, void *tovalue) {
  double tvalue = *(double *)value;
  stringstream va;
  va << tvalue;
  strcpy((char *)tovalue, va.str().c_str());
  va.clear();
  return tovalue;
}
inline void *double_to_boolean(void *value, void *tovalue) {
  *(bool *)tovalue = (*(double *)value == 0 ? 0 : 1);
  return tovalue;
}
inline void *double_to_decimal(void *value, void *tovalue) {
  stringstream va;
  va.precision(30);
  va << *(double *)value;
  *(Decimal *)tovalue =
      Decimal(DECIMAL_MAXPRCISION, DECIMAL_MAXSCALE, va.str());
  va.clear();
  return tovalue;
}
/***************double****************************/

/***************boolean****************************/
inline void *boolean_to_smallInt(void *value, void *tovalue) {
  *(short int *)tovalue = (*(bool *)value);
  return tovalue;
}
inline void *boolean_to_int(void *value, void *tovalue) {
  *(int *)tovalue = (*(bool *)value);
  return tovalue;
}
inline void *boolean_to_float(void *value, void *tovalue) {
  *(float *)tovalue = (*(bool *)value);
  return tovalue;
}
inline void *boolean_to_double(void *value, void *tovalue) {
  *(double *)tovalue = (*(bool *)value);
  return tovalue;
}
inline void *boolean_to_boolean(void *value, void *tovalue) {
  *(bool *)tovalue = *(bool *)value;
  return tovalue;
}
inline void *boolean_to_ulong(void *value, void *tovalue) {
  *(unsigned long *)tovalue = (*(bool *)value);
  return tovalue;
}
inline void *boolean_to_decimal(void *value, void *tovalue) {
  stringstream va;
  va << *(bool *)value;
  *(Decimal *)tovalue = Decimal(DECIMAL_PSUBS, 0, va.str());
  va.clear();
  return tovalue;
}

/***************boolean****************************/

/***************decimal****************************/
inline void *decimal_to_decimal(void *value, void *tovalue) {
  *(Decimal *)tovalue = *(Decimal *)value;
  return tovalue;
}
static Decimal zero(1, 0, "0");
inline void *decimal_to_boolean(void *value, void *tovalue) {
  Decimal tvalue = *(Decimal *)value;
  *(bool *)tovalue = tvalue.op_equals(zero);
  return tovalue;
}
/***************decimal****************************/

/***************date****************************/
inline void *date_to_date(void *value, void *tovalue) {
  *(date *)tovalue = *(date *)value;
  return tovalue;
}
inline void *date_to_boolean(void *value,
                             void *tovalue)  // now return true everytime
{
  *(bool *)tovalue = true;
  return tovalue;
}
inline void *date_to_string(void *value, void *tovalue) {
  strcpy((char *)tovalue, to_iso_extended_string(*(date *)value).c_str());
  return tovalue;
}
/***************date****************************/

/***************time****************************/

inline void *time_to_time(void *value, void *tovalue) {
  *(time_duration *)tovalue = *(time_duration *)value;
  return tovalue;
}
inline void *time_to_string(void *value, void *tovalue) {
  strcpy((char *)tovalue, to_simple_string(*(time_duration *)value).c_str());
  return tovalue;
}
/***************time****************************/

/***************datetime****************************/
inline void *datetime_to_datetime(void *value, void *tovalue) {
  *(ptime *)tovalue = *(ptime *)value;
  return tovalue;
}
inline void *datetime_to_string(void *value, void *tovalue) {
  strcpy((char *)tovalue, to_iso_extended_string(*(ptime *)value).c_str());
  return tovalue;
}

/***************datetime****************************/

inline void *errormsg(void *value, void *tovalue) {
  puts("!!!!!!!!!!!!!this TypeCast is not supported now!!!!!!!!!!");
  assert(false);
  return NULL;
}
inline void InitTypeCastFunc() {
  // t_smallInt
  ExprTypeCast::type_cast_func_[t_smallInt][t_smallInt] = smallInt_to_smallInt;
  ExprTypeCast::type_cast_func_[t_smallInt][t_int] = smallInt_to_int;
  ExprTypeCast::type_cast_func_[t_smallInt][t_u_long] = smallInt_to_ulong;
  ExprTypeCast::type_cast_func_[t_smallInt][t_float] = smallInt_to_float;
  ExprTypeCast::type_cast_func_[t_smallInt][t_double] = smallInt_to_double;
  ExprTypeCast::type_cast_func_[t_smallInt][t_string] = smallInt_to_string;
  ExprTypeCast::type_cast_func_[t_smallInt][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_smallInt][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_smallInt][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_smallInt][t_decimal] = smallInt_to_decimal;
  ExprTypeCast::type_cast_func_[t_smallInt][t_boolean] = smallInt_to_boolean;

  // t_int
  ExprTypeCast::type_cast_func_[t_int][t_smallInt] = int_to_smallint;
  ExprTypeCast::type_cast_func_[t_int][t_int] = int_to_int;
  ExprTypeCast::type_cast_func_[t_int][t_u_long] = int_to_ulong;
  ExprTypeCast::type_cast_func_[t_int][t_float] = int_to_float;
  ExprTypeCast::type_cast_func_[t_int][t_double] = int_to_double;
  ExprTypeCast::type_cast_func_[t_int][t_string] = errormsg;
  ExprTypeCast::type_cast_func_[t_int][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_int][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_int][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_int][t_decimal] = int_to_decimal;
  ExprTypeCast::type_cast_func_[t_int][t_boolean] = int_to_boolean;

  // t_u_long
  ExprTypeCast::type_cast_func_[t_u_long][t_smallInt] = errormsg;
  ExprTypeCast::type_cast_func_[t_u_long][t_int] = errormsg;
  ExprTypeCast::type_cast_func_[t_u_long][t_u_long] = ulong_to_ulong;
  ExprTypeCast::type_cast_func_[t_u_long][t_float] = ulong_to_float;
  ExprTypeCast::type_cast_func_[t_u_long][t_double] = ulong_to_double;
  ExprTypeCast::type_cast_func_[t_u_long][t_string] = errormsg;
  ExprTypeCast::type_cast_func_[t_u_long][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_u_long][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_u_long][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_u_long][t_decimal] = ulong_to_decimal;
  ExprTypeCast::type_cast_func_[t_u_long][t_boolean] = ulong_to_boolean;

  // t_float
  ExprTypeCast::type_cast_func_[t_float][t_smallInt] = errormsg;
  ExprTypeCast::type_cast_func_[t_float][t_int] = errormsg;
  ExprTypeCast::type_cast_func_[t_float][t_u_long] = errormsg;
  ExprTypeCast::type_cast_func_[t_float][t_float] = float_to_float;
  ExprTypeCast::type_cast_func_[t_float][t_double] = float_to_double;
  ExprTypeCast::type_cast_func_[t_float][t_string] = float_to_string;
  ExprTypeCast::type_cast_func_[t_float][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_float][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_float][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_float][t_decimal] = float_to_decimal;
  ExprTypeCast::type_cast_func_[t_float][t_boolean] = float_to_boolean;

  // t_double
  ExprTypeCast::type_cast_func_[t_double][t_smallInt] = errormsg;
  ExprTypeCast::type_cast_func_[t_double][t_int] = errormsg;
  ExprTypeCast::type_cast_func_[t_double][t_u_long] = errormsg;
  ExprTypeCast::type_cast_func_[t_double][t_float] = errormsg;
  ExprTypeCast::type_cast_func_[t_double][t_double] = double_to_double;
  ExprTypeCast::type_cast_func_[t_double][t_string] = double_to_string;
  ExprTypeCast::type_cast_func_[t_double][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_double][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_double][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_double][t_decimal] = double_to_decimal;
  ExprTypeCast::type_cast_func_[t_double][t_boolean] = double_to_boolean;

  // t_string
  ExprTypeCast::type_cast_func_[t_string][t_smallInt] = string_to_smallint;
  ExprTypeCast::type_cast_func_[t_string][t_int] = string_to_int;
  ExprTypeCast::type_cast_func_[t_string][t_u_long] = string_to_ulong;
  ExprTypeCast::type_cast_func_[t_string][t_float] = string_to_float;
  ExprTypeCast::type_cast_func_[t_string][t_double] = string_to_double;
  ExprTypeCast::type_cast_func_[t_string][t_string] = string_to_string;
  ExprTypeCast::type_cast_func_[t_string][t_date] = string_to_date;
  ExprTypeCast::type_cast_func_[t_string][t_time] = string_to_time;
  ExprTypeCast::type_cast_func_[t_string][t_datetime] = string_to_datetime;
  ExprTypeCast::type_cast_func_[t_string][t_decimal] = string_to_decimal;
  ExprTypeCast::type_cast_func_[t_string][t_boolean] = string_to_boolean;

  ExprTypeCast::type_cast_func_[t_string][t_date_day] = string_to_date_day;
  ExprTypeCast::type_cast_func_[t_string][t_date_week] = string_to_date_week;
  ExprTypeCast::type_cast_func_[t_string][t_date_month] = string_to_date_month;
  ExprTypeCast::type_cast_func_[t_string][t_date_year] = string_to_date_year;
  ExprTypeCast::type_cast_func_[t_string][t_date_quarter] =
      string_to_date_quarter;
  //

  // t_date
  ExprTypeCast::type_cast_func_[t_date][t_smallInt] = errormsg;
  ExprTypeCast::type_cast_func_[t_date][t_int] = errormsg;
  ExprTypeCast::type_cast_func_[t_date][t_u_long] = errormsg;
  ExprTypeCast::type_cast_func_[t_date][t_float] = errormsg;
  ExprTypeCast::type_cast_func_[t_date][t_double] = errormsg;
  ExprTypeCast::type_cast_func_[t_date][t_string] = date_to_string;
  ExprTypeCast::type_cast_func_[t_date][t_date] = date_to_date;
  ExprTypeCast::type_cast_func_[t_date][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_date][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_date][t_decimal] = errormsg;
  ExprTypeCast::type_cast_func_[t_date][t_boolean] = date_to_boolean;

  //

  // t_time
  ExprTypeCast::type_cast_func_[t_time][t_smallInt] = errormsg;
  ExprTypeCast::type_cast_func_[t_time][t_int] = errormsg;
  ExprTypeCast::type_cast_func_[t_time][t_u_long] = errormsg;
  ExprTypeCast::type_cast_func_[t_time][t_float] = errormsg;
  ExprTypeCast::type_cast_func_[t_time][t_double] = errormsg;
  ExprTypeCast::type_cast_func_[t_time][t_string] = time_to_string;
  ExprTypeCast::type_cast_func_[t_time][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_time][t_time] = time_to_time;
  ExprTypeCast::type_cast_func_[t_time][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_time][t_decimal] = errormsg;
  ExprTypeCast::type_cast_func_[t_time][t_boolean] = errormsg;

  //

  // t_datetime
  ExprTypeCast::type_cast_func_[t_datetime][t_smallInt] = errormsg;
  ExprTypeCast::type_cast_func_[t_datetime][t_int] = errormsg;
  ExprTypeCast::type_cast_func_[t_datetime][t_u_long] = errormsg;
  ExprTypeCast::type_cast_func_[t_datetime][t_float] = errormsg;
  ExprTypeCast::type_cast_func_[t_datetime][t_double] = errormsg;
  ExprTypeCast::type_cast_func_[t_datetime][t_string] = datetime_to_string;
  ExprTypeCast::type_cast_func_[t_datetime][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_datetime][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_datetime][t_datetime] = datetime_to_datetime;
  ExprTypeCast::type_cast_func_[t_datetime][t_decimal] = errormsg;
  ExprTypeCast::type_cast_func_[t_datetime][t_boolean] = errormsg;

  // t_decimal
  ExprTypeCast::type_cast_func_[t_decimal][t_smallInt] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_int] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_u_long] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_float] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_double] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_string] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_decimal][t_decimal] = decimal_to_decimal;
  ExprTypeCast::type_cast_func_[t_decimal][t_boolean] = decimal_to_boolean;

  // t_boolean
  ExprTypeCast::type_cast_func_[t_boolean][t_smallInt] = boolean_to_smallInt;
  ExprTypeCast::type_cast_func_[t_boolean][t_int] = boolean_to_int;
  ExprTypeCast::type_cast_func_[t_boolean][t_u_long] = boolean_to_ulong;
  ExprTypeCast::type_cast_func_[t_boolean][t_float] = boolean_to_float;
  ExprTypeCast::type_cast_func_[t_boolean][t_double] = boolean_to_double;
  ExprTypeCast::type_cast_func_[t_boolean][t_string] = errormsg;
  ExprTypeCast::type_cast_func_[t_boolean][t_date] = errormsg;
  ExprTypeCast::type_cast_func_[t_boolean][t_time] = errormsg;
  ExprTypeCast::type_cast_func_[t_boolean][t_datetime] = errormsg;
  ExprTypeCast::type_cast_func_[t_boolean][t_decimal] = boolean_to_decimal;
  ExprTypeCast::type_cast_func_[t_boolean][t_boolean] = boolean_to_boolean;
}

}  // namespace common
}  // namespace claims
#endif  // COMMON_EXPRESSION_EXPR_TYPE_CAST_H_
