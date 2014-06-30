/*
 * TypeCast.h
 *
 *  Created on: Feb 26, 2014
 *      Author: wangli
 */

#ifndef TYPECAST_H_
#define TYPECAST_H_

#include "ExpressionItem.h"

#include <sstream>
#include <stdlib.h>

#include "data_type.h"

typedef bool (*TypeCastFunction) (ExpressionItem& in);
class TypeCast{
public:
	static TypeCastFunction type_cast_functions[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];
};

inline bool int_to_int(ExpressionItem& in){
	assert(in.return_type==t_int);
	return true;
}

inline bool int_to_float(ExpressionItem& in){
	assert(in.return_type==t_int);
	float new_value=(float)in.content.data.value._int;
	in.return_type=t_float;
	in.content.data.value._float=new_value;
	return true;
}

inline bool int_to_double(ExpressionItem& in){
	assert(in.return_type==t_int);
	float new_value=(double)in.content.data.value._int;
	in.return_type=t_double;
	in.content.data.value._double=new_value;
	return true;
}

inline bool int_to_ulong(ExpressionItem& in){
	assert(in.return_type==t_int);
	const unsigned long new_value=(unsigned long)in.content.data.value._int;
	in.return_type=t_u_long;
	in.content.data.value._ulong=new_value;
	return true;
}

inline bool int_to_decimal(ExpressionItem& in){
	assert(in.return_type==t_int);
	stringstream va;
	va<<in.content.data.value._int;
	in.return_type=t_decimal;
	in._decimal=NValue::getDecimalValueFromString(va.str());
	return true;
}

inline bool string_to_int(ExpressionItem& in){
	assert(in.return_type==t_string);
	int rt=atoi(in._string.c_str());
	in.return_type=t_int;
	in.content.data.value._int=rt;
	return true;
}

inline void initialize_type_cast_functions(){
	TypeCast::type_cast_functions[t_int][t_int]=int_to_int;
	TypeCast::type_cast_functions[t_int][t_float]=int_to_float;
	TypeCast::type_cast_functions[t_int][t_double]=int_to_double;
	TypeCast::type_cast_functions[t_int][t_u_long]=int_to_ulong;
	TypeCast::type_cast_functions[t_int][t_decimal]=int_to_decimal;
}

#endif
