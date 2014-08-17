/*
 * TypeCast.h
 *
 *  Created on: Feb 26, 2014
 *      Author: wangli
 */
//TODO change TypeCast
#ifndef TYPECAST_H_
#define TYPECAST_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "ExpressionItem.h"

#include <sstream>
#include <stdlib.h>

#include "data_type.h"

typedef bool (*TypeCastFunction) (ExpressionItem& in);//

typedef void *(*TypeCastF)(void *value);
class TypeCast{
public:
	static TypeCastF type_cast_func[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];
	static TypeCastFunction type_cast_functions[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];//

};
///////////////////////////////////
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
////////////////////////////

inline void *int_to_int(void *value)
{
	if(*(int *)value==NULL_INT)//in order to judge the return result is NULL,so void * is NULL will be simple
		return NULL;
	return value;
}
inline void * int_to_smallint(void * value)
{
	if(*(int *)value==NULL_INT)
	return NULL;
	short int tvalue=*(int *)value;
	return &tvalue;
}
inline void *int_to_float(void *value)
{
	if(*(int *)value==NULL_INT)
		return NULL;
	float tvalue=*(int *)value;
	return &tvalue;

}
inline void *int_to_double(void *value)
{
	if(*(int *)value==NULL_INT)
		return NULL;
	double tvalue=*(int *)value;
	return &tvalue;
}
inline void *int_to_ulong(void *value)
{
	if(*(int *)value==NULL_INT)
		return NULL;
	unsigned long tvalue=*(int*)value;
	return &tvalue;
}
inline void *int_to_decimal(void *value)
{
	if(*(int *)value==NULL_INT)
		return NULL;
	stringstream va;
	va<<*(int *)value;
	NValue tvalue=NValue::getDecimalValueFromString(va.str());
	va.clear();
	return &tvalue;
}


inline void * string_to_int(void *value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	int tvalue=atoi((*(string *)value).c_str());
	return &tvalue;
}
inline void * string_to_smallint(void *value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	short int tvalue=atoi((*(string *)value).c_str());//???
	return &tvalue;
}
inline void * string_to_ulong(void * value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	unsigned long tvalue=strtoul((*(string *)value).c_str(),NULL,10);
	return &tvalue;
}
inline void *string_to_float(void * value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	float tvalue=atof((*(string *)value).c_str());
	return &tvalue;
}
inline void *string_to_double(void *value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	double tvalue=strtod((*(string *)value).c_str(),NULL);
	return &tvalue;
}
inline void *string_to_string(void *value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	return value;
}

inline void *ulong_to_ulong(void *value)
{
	return value;
}
inline void *ulong_to_float(void *value)
{
	float tvalue=*(unsigned long *)value;
	return &tvalue;
}
inline void *ulong_to_double(void *value)
{
	double tvalue=*(unsigned long *)value;
	return &tvalue;
}
inline void *ulong_to_decimal(void *value)
{
	stringstream va;
	va<<*(unsigned long *)value;
	NValue tvalue=NValue::getDecimalValueFromString(va.str());
	va.clear();
	return &tvalue;
}

inline void *errormsg(void *value)
{
	puts("!!!!!!!!!!!!!this TypeCast is not supported now!!!!!!!!!!");
	assert(false);
	return NULL;
}
inline void initialize_type_cast_functions(){
/////////////////
	TypeCast::type_cast_functions[t_int][t_int]=int_to_int;
	TypeCast::type_cast_functions[t_int][t_float]=int_to_float;
	TypeCast::type_cast_functions[t_int][t_double]=int_to_double;
	TypeCast::type_cast_functions[t_int][t_u_long]=int_to_ulong;
	TypeCast::type_cast_functions[t_int][t_decimal]=int_to_decimal;
///////////////////////


	//t_smallInt
	TypeCast::type_cast_func[t_smallInt][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_int]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_float]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_double]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_string]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_date]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_time]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_decimal]=errormsg;

	//t_int
	TypeCast::type_cast_func[t_int][t_smallInt]=int_to_smallint;
	TypeCast::type_cast_func[t_int][t_int]=int_to_int;
	TypeCast::type_cast_func[t_int][t_u_long]=int_to_ulong;
	TypeCast::type_cast_func[t_int][t_float]=int_to_float;
	TypeCast::type_cast_func[t_int][t_double]=int_to_double;
	TypeCast::type_cast_func[t_int][t_string]=errormsg;
	TypeCast::type_cast_func[t_int][t_date]=errormsg;
	TypeCast::type_cast_func[t_int][t_time]=errormsg;
	TypeCast::type_cast_func[t_int][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_int][t_decimal]=int_to_decimal;

	//t_u_long
	TypeCast::type_cast_func[t_u_long][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_u_long][t_int]=errormsg;
	TypeCast::type_cast_func[t_u_long][t_u_long]=ulong_to_ulong;
	TypeCast::type_cast_func[t_u_long][t_float]=ulong_to_float;
	TypeCast::type_cast_func[t_u_long][t_double]=ulong_to_double;
	TypeCast::type_cast_func[t_u_long][t_string]=errormsg;
	TypeCast::type_cast_func[t_u_long][t_date]=errormsg;
	TypeCast::type_cast_func[t_u_long][t_time]=errormsg;
	TypeCast::type_cast_func[t_u_long][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_u_long][t_decimal]=ulong_to_decimal;

	//t_float
	TypeCast::type_cast_func[t_float][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_float][t_int]=errormsg;
	TypeCast::type_cast_func[t_float][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_float][t_float]=errormsg;
	TypeCast::type_cast_func[t_float][t_double]=errormsg;
	TypeCast::type_cast_func[t_float][t_string]=errormsg;
	TypeCast::type_cast_func[t_float][t_date]=errormsg;
	TypeCast::type_cast_func[t_float][t_time]=errormsg;
	TypeCast::type_cast_func[t_float][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_float][t_decimal]=errormsg;

	//t_double
	TypeCast::type_cast_func[t_double][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_double][t_int]=errormsg;
	TypeCast::type_cast_func[t_double][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_double][t_float]=errormsg;
	TypeCast::type_cast_func[t_double][t_double]=errormsg;
	TypeCast::type_cast_func[t_double][t_string]=errormsg;
	TypeCast::type_cast_func[t_double][t_date]=errormsg;
	TypeCast::type_cast_func[t_double][t_time]=errormsg;
	TypeCast::type_cast_func[t_double][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_double][t_decimal]=errormsg;

	//t_string
	TypeCast::type_cast_func[t_string][t_smallInt]=string_to_smallint;
	TypeCast::type_cast_func[t_string][t_int]=string_to_int;
	TypeCast::type_cast_func[t_string][t_u_long]=string_to_ulong;
	TypeCast::type_cast_func[t_string][t_float]=string_to_float;
	TypeCast::type_cast_func[t_string][t_double]=string_to_double;
	TypeCast::type_cast_func[t_string][t_string]=string_to_string;
	TypeCast::type_cast_func[t_string][t_date]=errormsg;
	TypeCast::type_cast_func[t_string][t_time]=errormsg;
	TypeCast::type_cast_func[t_string][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_string][t_decimal]=errormsg;
	//

	//t_date
	TypeCast::type_cast_func[t_date][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_date][t_int]=errormsg;
	TypeCast::type_cast_func[t_date][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_date][t_float]=errormsg;
	TypeCast::type_cast_func[t_date][t_double]=errormsg;
	TypeCast::type_cast_func[t_date][t_string]=errormsg;
	TypeCast::type_cast_func[t_date][t_date]=errormsg;
	TypeCast::type_cast_func[t_date][t_time]=errormsg;
	TypeCast::type_cast_func[t_date][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_date][t_decimal]=errormsg;
	//

	//t_time
	TypeCast::type_cast_func[t_time][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_time][t_int]=errormsg;
	TypeCast::type_cast_func[t_time][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_time][t_float]=errormsg;
	TypeCast::type_cast_func[t_time][t_double]=errormsg;
	TypeCast::type_cast_func[t_time][t_string]=errormsg;
	TypeCast::type_cast_func[t_time][t_date]=errormsg;
	TypeCast::type_cast_func[t_time][t_time]=errormsg;
	TypeCast::type_cast_func[t_time][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_time][t_decimal]=errormsg;
	//

	//t_datetime
	TypeCast::type_cast_func[t_datetime][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_int]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_float]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_double]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_string]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_date]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_time]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_datetime][t_decimal]=errormsg;

	//t_decimal
	TypeCast::type_cast_func[t_decimal][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_int]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_float]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_double]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_string]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_date]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_time]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_decimal][t_decimal]=errormsg;

}

#endif
