/*
 * TypePromotionMap.h
 *
 *  Created on: Feb 26, 2014
 *      Author: wangli
 */

#ifndef TYPEPROMOTIONMAP_H_
#define TYPEPROMOTIONMAP_H_
#include "../data_type.h"
class TypePromotion{
public:
	static data_type arith_type_promotion_map[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];

};
//static data_type TypePromotion::arith_type_promotion_map[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];
//enum data_type{t_smallInt,t_int,t_u_long,t_float,t_double,t_string, t_date, t_time, t_datetime, t_decimal};

/**
 *
 */
static void initialize_arithmetic_type_promotion_matrix(){

	//t_smallInt
	TypePromotion::arith_type_promotion_map[t_smallInt][t_smallInt]=t_smallInt;
	TypePromotion::arith_type_promotion_map[t_smallInt][t_int]=t_int;
	TypePromotion::arith_type_promotion_map[t_smallInt][t_u_long]=t_u_long;
	TypePromotion::arith_type_promotion_map[t_smallInt][t_float]=t_float;
	TypePromotion::arith_type_promotion_map[t_smallInt][t_double]=t_double;
	TypePromotion::arith_type_promotion_map[t_smallInt][t_string]=t_int;
	TypePromotion::arith_type_promotion_map[t_smallInt][t_date];
	TypePromotion::arith_type_promotion_map[t_smallInt][t_time];
	TypePromotion::arith_type_promotion_map[t_smallInt][t_datetime];
	TypePromotion::arith_type_promotion_map[t_smallInt][t_decimal];

	//t_int
	TypePromotion::arith_type_promotion_map[t_int][t_smallInt]=t_int;
	TypePromotion::arith_type_promotion_map[t_int][t_int]=t_int;
	TypePromotion::arith_type_promotion_map[t_int][t_u_long]=t_u_long;
	TypePromotion::arith_type_promotion_map[t_int][t_float]=t_float;
	TypePromotion::arith_type_promotion_map[t_int][t_double]=t_double;
	TypePromotion::arith_type_promotion_map[t_int][t_string]=t_int;
	TypePromotion::arith_type_promotion_map[t_int][t_date];
	TypePromotion::arith_type_promotion_map[t_int][t_time];
	TypePromotion::arith_type_promotion_map[t_int][t_datetime];
	TypePromotion::arith_type_promotion_map[t_int][t_decimal];

	//t_u_long
	TypePromotion::arith_type_promotion_map[t_u_long][t_smallInt]=t_u_long;
	TypePromotion::arith_type_promotion_map[t_u_long][t_int]=t_u_long;
	TypePromotion::arith_type_promotion_map[t_u_long][t_u_long]=t_u_long;
	TypePromotion::arith_type_promotion_map[t_u_long][t_float]=t_float;
	TypePromotion::arith_type_promotion_map[t_u_long][t_double]=t_double;
	TypePromotion::arith_type_promotion_map[t_u_long][t_string]=t_u_long;
	TypePromotion::arith_type_promotion_map[t_u_long][t_date];
	TypePromotion::arith_type_promotion_map[t_u_long][t_time];
	TypePromotion::arith_type_promotion_map[t_u_long][t_datetime];
	TypePromotion::arith_type_promotion_map[t_u_long][t_decimal];

	//t_float
	TypePromotion::arith_type_promotion_map[t_float][t_smallInt]=t_float;
	TypePromotion::arith_type_promotion_map[t_float][t_int]=t_float;
	TypePromotion::arith_type_promotion_map[t_float][t_u_long]=t_float;
	TypePromotion::arith_type_promotion_map[t_float][t_float]=t_float;
	TypePromotion::arith_type_promotion_map[t_float][t_double]=t_double;
	TypePromotion::arith_type_promotion_map[t_float][t_string]=t_float;
	TypePromotion::arith_type_promotion_map[t_float][t_date];
	TypePromotion::arith_type_promotion_map[t_float][t_time];
	TypePromotion::arith_type_promotion_map[t_float][t_datetime];
	TypePromotion::arith_type_promotion_map[t_float][t_decimal];

	//t_double
	TypePromotion::arith_type_promotion_map[t_double][t_smallInt]=t_double;
	TypePromotion::arith_type_promotion_map[t_double][t_int]=t_double;
	TypePromotion::arith_type_promotion_map[t_double][t_u_long]=t_double;
	TypePromotion::arith_type_promotion_map[t_double][t_float]=t_double;
	TypePromotion::arith_type_promotion_map[t_double][t_double]=t_double;
	TypePromotion::arith_type_promotion_map[t_double][t_string]=t_double;
	TypePromotion::arith_type_promotion_map[t_double][t_date];
	TypePromotion::arith_type_promotion_map[t_double][t_time];
	TypePromotion::arith_type_promotion_map[t_double][t_datetime];
	TypePromotion::arith_type_promotion_map[t_double][t_decimal];

	//t_string
	TypePromotion::arith_type_promotion_map[t_string][t_smallInt]=t_smallInt;
	TypePromotion::arith_type_promotion_map[t_string][t_int]=t_int;
	TypePromotion::arith_type_promotion_map[t_string][t_u_long]=t_u_long;
	TypePromotion::arith_type_promotion_map[t_string][t_float]=t_float;
	TypePromotion::arith_type_promotion_map[t_string][t_double]=t_double;
	TypePromotion::arith_type_promotion_map[t_string][t_string]=t_string;
	TypePromotion::arith_type_promotion_map[t_string][t_date];
	TypePromotion::arith_type_promotion_map[t_string][t_time];
	TypePromotion::arith_type_promotion_map[t_string][t_datetime];
	TypePromotion::arith_type_promotion_map[t_string][t_decimal];
	//

	//t_string
	TypePromotion::arith_type_promotion_map[t_date][t_smallInt];
	TypePromotion::arith_type_promotion_map[t_date][t_int];
	TypePromotion::arith_type_promotion_map[t_date][t_u_long];
	TypePromotion::arith_type_promotion_map[t_date][t_float];
	TypePromotion::arith_type_promotion_map[t_date][t_double];
	TypePromotion::arith_type_promotion_map[t_date][t_string];
	TypePromotion::arith_type_promotion_map[t_date][t_date]=t_date;
	TypePromotion::arith_type_promotion_map[t_date][t_time];
	TypePromotion::arith_type_promotion_map[t_date][t_datetime];
	TypePromotion::arith_type_promotion_map[t_date][t_decimal];
	//

	//t_time
	TypePromotion::arith_type_promotion_map[t_time][t_smallInt];
	TypePromotion::arith_type_promotion_map[t_time][t_int];
	TypePromotion::arith_type_promotion_map[t_time][t_u_long];
	TypePromotion::arith_type_promotion_map[t_time][t_float];
	TypePromotion::arith_type_promotion_map[t_time][t_double];
	TypePromotion::arith_type_promotion_map[t_time][t_string];
	TypePromotion::arith_type_promotion_map[t_time][t_date];
	TypePromotion::arith_type_promotion_map[t_time][t_time]=t_time;
	TypePromotion::arith_type_promotion_map[t_time][t_datetime];
	TypePromotion::arith_type_promotion_map[t_time][t_decimal];
	//

	//t_datetime
	TypePromotion::arith_type_promotion_map[t_datetime][t_smallInt];
	TypePromotion::arith_type_promotion_map[t_datetime][t_int];
	TypePromotion::arith_type_promotion_map[t_datetime][t_u_long];
	TypePromotion::arith_type_promotion_map[t_datetime][t_float];
	TypePromotion::arith_type_promotion_map[t_datetime][t_double];
	TypePromotion::arith_type_promotion_map[t_datetime][t_string];
	TypePromotion::arith_type_promotion_map[t_datetime][t_date];
	TypePromotion::arith_type_promotion_map[t_datetime][t_time];
	TypePromotion::arith_type_promotion_map[t_datetime][t_datetime]=t_datetime;
	TypePromotion::arith_type_promotion_map[t_datetime][t_decimal];

	//t_decimal
	TypePromotion::arith_type_promotion_map[t_decimal][t_smallInt];
	TypePromotion::arith_type_promotion_map[t_decimal][t_int];
	TypePromotion::arith_type_promotion_map[t_decimal][t_u_long];
	TypePromotion::arith_type_promotion_map[t_decimal][t_float];
	TypePromotion::arith_type_promotion_map[t_decimal][t_double];
	TypePromotion::arith_type_promotion_map[t_decimal][t_string];
	TypePromotion::arith_type_promotion_map[t_decimal][t_date];
	TypePromotion::arith_type_promotion_map[t_decimal][t_time];
	TypePromotion::arith_type_promotion_map[t_decimal][t_datetime];
	TypePromotion::arith_type_promotion_map[t_decimal][t_decimal]=t_decimal;
}

#endif /* TYPEPROMOTIONMAP_H_ */
