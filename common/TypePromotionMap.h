/*
 * TypePromotionMap.h
 *
 *  Created on: Feb 26, 2014
 *      Author: wangli
 */

#ifndef TYPEPROMOTIONMAP_H_
#define TYPEPROMOTIONMAP_H_
#include <memory.h>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/data_type.h"
class TypePromotionMatrix{
public:
	static data_type type_conversion_matrix[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];

};
//static data_type TypePromotion::arith_type_promotion_map[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];
//enum data_type{t_smallInt,t_int,t_u_long,t_float,t_double,t_string, t_date, t_time, t_datetime, t_decimal};

/**
 * Note: This function must be called before computing any Expression
 */
static void initialize_arithmetic_type_promotion_matrix(){

	/*
	 * all the element is set to DATA_TYPE_NUMBER such that the default value of each element is DATA_TYPE_NUMBER,
	 * which means that the promotion is not supported for now.
	 */
	memset(TypePromotionMatrix::type_conversion_matrix,DATA_TYPE_NUMBER,DATA_TYPE_NUMBER*DATA_TYPE_NUMBER);

	//t_smallInt
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_smallInt]=t_smallInt;
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_int]=t_int;
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_u_long]=t_u_long;
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_float]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_double]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_string]=t_int;
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_date];
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_time];
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_datetime];
	TypePromotionMatrix::type_conversion_matrix[t_smallInt][t_decimal]=t_decimal;

	//t_int
	TypePromotionMatrix::type_conversion_matrix[t_int][t_smallInt]=t_int;
	TypePromotionMatrix::type_conversion_matrix[t_int][t_int]=t_int;
	TypePromotionMatrix::type_conversion_matrix[t_int][t_u_long]=t_u_long;
	TypePromotionMatrix::type_conversion_matrix[t_int][t_float]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_int][t_double]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_int][t_string]=t_int;
	TypePromotionMatrix::type_conversion_matrix[t_int][t_date];
	TypePromotionMatrix::type_conversion_matrix[t_int][t_time];
	TypePromotionMatrix::type_conversion_matrix[t_int][t_datetime];
	TypePromotionMatrix::type_conversion_matrix[t_int][t_decimal]=t_decimal;

	//t_u_long
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_smallInt]=t_u_long;
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_int]=t_u_long;
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_u_long]=t_u_long;
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_float]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_double]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_string]=t_u_long;
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_date];
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_time];
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_datetime];
	TypePromotionMatrix::type_conversion_matrix[t_u_long][t_decimal]=t_decimal;

	//t_float
	TypePromotionMatrix::type_conversion_matrix[t_float][t_smallInt]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_float][t_int]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_float][t_u_long]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_float][t_float]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_float][t_double]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_float][t_string]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_float][t_date];
	TypePromotionMatrix::type_conversion_matrix[t_float][t_time];
	TypePromotionMatrix::type_conversion_matrix[t_float][t_datetime];
	TypePromotionMatrix::type_conversion_matrix[t_float][t_decimal]=t_decimal;

	//t_double
	TypePromotionMatrix::type_conversion_matrix[t_double][t_smallInt]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_double][t_int]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_double][t_u_long]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_double][t_float]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_double][t_double]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_double][t_string]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_double][t_date];
	TypePromotionMatrix::type_conversion_matrix[t_double][t_time];
	TypePromotionMatrix::type_conversion_matrix[t_double][t_datetime];
	TypePromotionMatrix::type_conversion_matrix[t_double][t_decimal]=t_decimal;

	//t_string
	TypePromotionMatrix::type_conversion_matrix[t_string][t_smallInt]=t_smallInt;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_int]=t_int;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_u_long]=t_u_long;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_float]=t_float;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_double]=t_double;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_string]=t_string;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_date]=t_date;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_time]=t_time;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_datetime]=t_datetime;
	TypePromotionMatrix::type_conversion_matrix[t_string][t_decimal]=t_decimal;
	//

	//t_string
	TypePromotionMatrix::type_conversion_matrix[t_date][t_smallInt];
	TypePromotionMatrix::type_conversion_matrix[t_date][t_int];
	TypePromotionMatrix::type_conversion_matrix[t_date][t_u_long];
	TypePromotionMatrix::type_conversion_matrix[t_date][t_float];
	TypePromotionMatrix::type_conversion_matrix[t_date][t_double];
	TypePromotionMatrix::type_conversion_matrix[t_date][t_string]=t_date;
	TypePromotionMatrix::type_conversion_matrix[t_date][t_date]=t_date;
	TypePromotionMatrix::type_conversion_matrix[t_date][t_time];
	TypePromotionMatrix::type_conversion_matrix[t_date][t_datetime];
	TypePromotionMatrix::type_conversion_matrix[t_date][t_decimal];
	//

	//t_time
	TypePromotionMatrix::type_conversion_matrix[t_time][t_smallInt];
	TypePromotionMatrix::type_conversion_matrix[t_time][t_int];
	TypePromotionMatrix::type_conversion_matrix[t_time][t_u_long];
	TypePromotionMatrix::type_conversion_matrix[t_time][t_float];
	TypePromotionMatrix::type_conversion_matrix[t_time][t_double];
	TypePromotionMatrix::type_conversion_matrix[t_time][t_string]=t_time;
	TypePromotionMatrix::type_conversion_matrix[t_time][t_date];
	TypePromotionMatrix::type_conversion_matrix[t_time][t_time]=t_time;
	TypePromotionMatrix::type_conversion_matrix[t_time][t_datetime];
	TypePromotionMatrix::type_conversion_matrix[t_time][t_decimal];
	//

	//t_datetime
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_smallInt];
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_int];
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_u_long];
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_float];
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_double];
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_string]=t_datetime;
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_date];
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_time];
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_datetime]=t_datetime;
	TypePromotionMatrix::type_conversion_matrix[t_datetime][t_decimal];

	//t_decimal
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_smallInt]=t_decimal;
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_int]=t_decimal;
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_u_long]=t_decimal;
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_float]=t_decimal;
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_double]=t_decimal;
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_string];
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_date];
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_time];
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_datetime];
	TypePromotionMatrix::type_conversion_matrix[t_decimal][t_decimal]=t_decimal;
}

#endif /* TYPEPROMOTIONMAP_H_ */
