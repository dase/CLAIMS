/*
 * TypePromotionMap.h
 *
 *  Created on: Feb 26, 2014
 *      Author: wangli
 */

#ifndef COMMON_EXPRESSION_TYPE_CONVERSION_MATRIX_H_
#define COMMON_EXPRESSION_TYPE_CONVERSION_MATRIX_H_
#include <memory.h>
#include "../common/data_type.h"
namespace claims {
namespace common {
class TypeConversionMatrix {
 public:
  static data_type type_conversion_matrix[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];
};

/**
 * Note: This function must be called before computing any Expression
 */
static void InitTypeConversionMatrix() {
  /*
   * all the element is set to DATA_TYPE_NUMBER such that the default value of
   * each element is DATA_TYPE_NUMBER,
   * which means that the promotion is not supported for now.
   */
  memset(TypeConversionMatrix::type_conversion_matrix, DATA_TYPE_NUMBER,
         DATA_TYPE_NUMBER * DATA_TYPE_NUMBER);

  // t_smallInt
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_smallInt] =
      t_smallInt;
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_int] = t_int;
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_u_long] = t_u_long;
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_float] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_double] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_string] = t_int;
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_decimal] =
      t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_smallInt][t_boolean] =
      t_smallInt;

  // t_int
  TypeConversionMatrix::type_conversion_matrix[t_int][t_smallInt] = t_int;
  TypeConversionMatrix::type_conversion_matrix[t_int][t_int] = t_int;
  TypeConversionMatrix::type_conversion_matrix[t_int][t_u_long] = t_u_long;
  TypeConversionMatrix::type_conversion_matrix[t_int][t_float] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_int][t_double] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_int][t_string] = t_int;
  TypeConversionMatrix::type_conversion_matrix[t_int][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_int][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_int][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_int][t_decimal] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_int][t_boolean] = t_int;

  // t_u_long
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_smallInt] = t_u_long;
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_int] = t_u_long;
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_u_long] = t_u_long;
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_float] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_double] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_string] = t_u_long;
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_decimal] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_u_long][t_boolean] = t_u_long;

  // t_float
  TypeConversionMatrix::type_conversion_matrix[t_float][t_smallInt] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_float][t_int] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_float][t_u_long] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_float][t_float] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_float][t_double] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_float][t_string] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_float][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_float][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_float][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_float][t_decimal] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_float][t_boolean] = t_float;

  // t_double
  TypeConversionMatrix::type_conversion_matrix[t_double][t_smallInt] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_double][t_int] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_double][t_u_long] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_double][t_float] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_double][t_double] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_double][t_string] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_double][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_double][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_double][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_double][t_decimal] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_double][t_boolean] = t_double;

  // t_string
  TypeConversionMatrix::type_conversion_matrix[t_string][t_smallInt] =
      t_smallInt;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_int] = t_int;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_u_long] = t_u_long;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_float] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_double] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_string] = t_string;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_date] = t_date;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_time] = t_time;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_datetime] =
      t_datetime;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_decimal] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_string][t_boolean];

  // t_string
  TypeConversionMatrix::type_conversion_matrix[t_date][t_smallInt];
  TypeConversionMatrix::type_conversion_matrix[t_date][t_int];
  TypeConversionMatrix::type_conversion_matrix[t_date][t_u_long];
  TypeConversionMatrix::type_conversion_matrix[t_date][t_float];
  TypeConversionMatrix::type_conversion_matrix[t_date][t_double];
  TypeConversionMatrix::type_conversion_matrix[t_date][t_string] = t_date;
  TypeConversionMatrix::type_conversion_matrix[t_date][t_date] = t_date;
  TypeConversionMatrix::type_conversion_matrix[t_date][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_date][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_date][t_decimal];
  TypeConversionMatrix::type_conversion_matrix[t_date][t_boolean];

  // t_time
  TypeConversionMatrix::type_conversion_matrix[t_time][t_smallInt];
  TypeConversionMatrix::type_conversion_matrix[t_time][t_int];
  TypeConversionMatrix::type_conversion_matrix[t_time][t_u_long];
  TypeConversionMatrix::type_conversion_matrix[t_time][t_float];
  TypeConversionMatrix::type_conversion_matrix[t_time][t_double];
  TypeConversionMatrix::type_conversion_matrix[t_time][t_string] = t_time;
  TypeConversionMatrix::type_conversion_matrix[t_time][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_time][t_time] = t_time;
  TypeConversionMatrix::type_conversion_matrix[t_time][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_time][t_decimal];
  TypeConversionMatrix::type_conversion_matrix[t_time][t_boolean];

  // t_datetime
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_smallInt];
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_int];
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_u_long];
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_float];
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_double];
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_string] =
      t_datetime;
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_datetime] =
      t_datetime;
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_decimal];
  TypeConversionMatrix::type_conversion_matrix[t_datetime][t_boolean];

  // t_decimal
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_smallInt] =
      t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_int] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_u_long] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_float] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_double] = t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_string];
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_decimal] =
      t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_decimal][t_boolean] =
      t_decimal;

  // t_boolean
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_smallInt] =
      t_smallInt;
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_int] = t_int;
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_u_long] = t_u_long;
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_float] = t_float;
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_double] = t_double;
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_string];
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_date];
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_time];
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_datetime];
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_decimal] =
      t_decimal;
  TypeConversionMatrix::type_conversion_matrix[t_boolean][t_boolean] =
      t_boolean;
}
}  // namespace common
}  // namespace claims
#endif  //  COMMON_EXPRESSION_TYPE_CONVERSION_MATRIX_H_
