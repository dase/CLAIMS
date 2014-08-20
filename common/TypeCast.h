/*
 * TypeCast.h
 *
 *  Created on: Feb 26, 2014
 *      Author: wangli
 */
//TODO change TypeCast warning the temporary variable
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
/***************int****************************/

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
inline void *int_to_boolean(void *value)
{
	bool ans=1;
	if(*(int *)value==0)
		ans=0;
	return &ans;
}
/***************int****************************/
/***************string****************************/

inline void * string_to_int(void *value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	int tvalue=atoi((char *)value);
	return &tvalue;
}
inline void * string_to_smallint(void *value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	short int tvalue=atoi((char *)value);//???
	return &tvalue;
}
inline void * string_to_ulong(void * value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	unsigned long tvalue=strtoul((char *)value,NULL,10);
	return &tvalue;
}
inline void *string_to_float(void * value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	float tvalue=atof((char *)value);
	return &tvalue;
}
inline void *string_to_double(void *value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	double tvalue=strtod((char *)value,NULL);
	return &tvalue;
}
inline void *string_to_string(void *value)
{
//	if(*(string *)value==NULL_STRING)
//		return NULL;
	return value;
}
inline void *string_to_decimal(void *value)
{
	NValue tvalue=NValue::getDecimalValueFromString(string((char *)value));
	return &tvalue;
}
inline void *string_to_boolean(void *value)
{
	bool ans=1;
	ans=value==NULL;
	return &ans;
}
/***************string****************************/
/***************unsigned long****************************/

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
inline void *ulong_to_boolean(void *value)
{
	bool ans=1;
	if(*(unsigned long *)value==0)
		ans=0;
	return &ans;
}

/***************unsigned long****************************/

/***************smallInt****************************/
inline void * smallInt_to_smallInt(void *value)
{
	return value;
}
inline void * smallInt_to_int(void *value)
{
	int tvalue =*(short int *)value;
	return &tvalue;
}
inline void *smallInt_to_ulong(void *value)
{
	unsigned long tvalue=*(short int *)value;
	return &tvalue;
}
inline void *smallInt_to_float(void *value)
{
	float tvalue=*(short int *)value;
	return &tvalue;
}
inline void *smallInt_to_double(void *value)
{
	double tvalue=*(short int *)value;
	return &tvalue;
}
inline void *smallInt_to_string(void *value)
{
	short int tvalue=*(short int *)value;
	stringstream va;
	va<<tvalue;
	return &va.str();
}
inline void *smallInt_to_boolean(void *value)
{
	bool ans=1;
	if(*(short int *)value==0)
		ans=0;
	return &ans;
}
inline void *smallInt_to_decimal(void *value)
{
	stringstream va;
	va<<*(short int *)value;
	NValue tvalue=NValue::getDecimalValueFromString(va.str());
	va.clear();
	return &tvalue;
}
/***************smallInt****************************/

/***************float****************************/
inline void *float_to_float(void *value)
{
	return value;
}
inline void *float_to_double(void *value)
{
	double tvalue=*(float *)value;
	return &tvalue;
}
inline void *float_to_string(void *value)
{
	float tvalue=*(float *)value;
	stringstream va;
	va<<tvalue;
	return &va.str();
}
inline void *float_to_boolean(void *value)
{
	bool ans=1;
	if(*(float *)value==0)
		ans=0;
	return &ans;
}
inline void *float_to_decimal(void *value)
{
	stringstream va;
	va<<*(float *)value;
	NValue tvalue=NValue::getDecimalValueFromString(va.str());
	va.clear();
	return &tvalue;
}
/***************float****************************/


/***************double****************************/
inline void *double_to_double(void *value)
{
	return value;
}
inline void *double_to_string(void *value)
{
	double tvalue=*(double *)value;
	stringstream va;
	va<<tvalue;
	return &va.str();
}
inline void *double_to_boolean(void *value)
{
	bool ans=1;
	if(*(double *)value==0)
		ans=0;
	return &ans;
}
inline void *double_to_decimal(void *value)
{
	stringstream va;
	va<<*(double *)value;
	NValue tvalue=NValue::getDecimalValueFromString(va.str());
	va.clear();
	return &tvalue;
}
/***************double****************************/

/***************boolean****************************/
inline void *boolean_to_smallInt(void *value)
{
	short int ans=1;
	if(*(bool *)value==0)
		ans=0;
	return &ans;
}
inline void *boolean_to_int(void *value)
{
	int ans=1;
	if(*(bool *)value==0)
		ans=0;
	return &ans;
}
inline void *boolean_to_float(void *value)
{
	float ans=1.0;
	if(*(bool *)value==0)
		ans=0.0;
	return &ans;
}
inline void *boolean_to_double(void *value)
{
	double ans=1.0;
	if(*(bool *)value==0)
		ans=0.0;
	return &ans;
}
inline void *boolean_to_boolean(void *value)
{
	return value;
}
inline void *boolean_to_ulong(void *value)
{
	unsigned long ans=1;
	if(*(bool *)value==0)
		ans=0;
	return &ans;
}
inline void *boolean_to_decimal(void *value)
{
	stringstream va;
	va<<*(bool *)value;
	NValue tvalue=NValue::getDecimalValueFromString(va.str());
	va.clear();
	return &tvalue;
}

/***************boolean****************************/
inline void *decimal_to_decimal(void *value)
{
	return value;
}
inline void *decimal_to_boolean(void *value)
{
	NValue tvalue=*(NValue *)value;
	bool ans=tvalue.op_equals(NValue::getDecimalValueFromString("0"));
	return &ans;
}
/***************decimal****************************/




/***************decimal****************************/


/***************smallInt****************************/

/***************smallInt****************************/

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
	TypeCast::type_cast_func[t_smallInt][t_smallInt]=smallInt_to_smallInt;
	TypeCast::type_cast_func[t_smallInt][t_int]=smallInt_to_int;
	TypeCast::type_cast_func[t_smallInt][t_u_long]=smallInt_to_ulong;
	TypeCast::type_cast_func[t_smallInt][t_float]=smallInt_to_float;
	TypeCast::type_cast_func[t_smallInt][t_double]=smallInt_to_double;
	TypeCast::type_cast_func[t_smallInt][t_string]=smallInt_to_string;
	TypeCast::type_cast_func[t_smallInt][t_date]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_time]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_smallInt][t_decimal]=smallInt_to_decimal;
	TypeCast::type_cast_func[t_smallInt][t_boolean]=smallInt_to_boolean;

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
	TypeCast::type_cast_func[t_int][t_boolean]=int_to_boolean;

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
	TypeCast::type_cast_func[t_u_long][t_boolean]=ulong_to_boolean;

	//t_float
	TypeCast::type_cast_func[t_float][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_float][t_int]=errormsg;
	TypeCast::type_cast_func[t_float][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_float][t_float]=float_to_float;
	TypeCast::type_cast_func[t_float][t_double]=float_to_double;
	TypeCast::type_cast_func[t_float][t_string]=float_to_string;
	TypeCast::type_cast_func[t_float][t_date]=errormsg;
	TypeCast::type_cast_func[t_float][t_time]=errormsg;
	TypeCast::type_cast_func[t_float][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_float][t_decimal]=float_to_decimal;
	TypeCast::type_cast_func[t_float][t_boolean]=float_to_boolean;

	//t_double
	TypeCast::type_cast_func[t_double][t_smallInt]=errormsg;
	TypeCast::type_cast_func[t_double][t_int]=errormsg;
	TypeCast::type_cast_func[t_double][t_u_long]=errormsg;
	TypeCast::type_cast_func[t_double][t_float]=errormsg;
	TypeCast::type_cast_func[t_double][t_double]=double_to_double;
	TypeCast::type_cast_func[t_double][t_string]=double_to_string;
	TypeCast::type_cast_func[t_double][t_date]=errormsg;
	TypeCast::type_cast_func[t_double][t_time]=errormsg;
	TypeCast::type_cast_func[t_double][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_double][t_decimal]=double_to_decimal;
	TypeCast::type_cast_func[t_double][t_boolean]=double_to_boolean;

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
	TypeCast::type_cast_func[t_string][t_decimal]=string_to_decimal;
	TypeCast::type_cast_func[t_string][t_boolean]=string_to_boolean;

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
	TypeCast::type_cast_func[t_date][t_boolean]=errormsg;

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
	TypeCast::type_cast_func[t_time][t_boolean]=errormsg;

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
	TypeCast::type_cast_func[t_datetime][t_boolean]=errormsg;

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
	TypeCast::type_cast_func[t_decimal][t_decimal]=decimal_to_decimal;
	TypeCast::type_cast_func[t_decimal][t_boolean]=decimal_to_boolean;

	//t_boolean
	TypeCast::type_cast_func[t_boolean][t_smallInt]=boolean_to_smallInt;
	TypeCast::type_cast_func[t_boolean][t_int]=boolean_to_int;
	TypeCast::type_cast_func[t_boolean][t_u_long]=boolean_to_ulong;
	TypeCast::type_cast_func[t_boolean][t_float]=boolean_to_float;
	TypeCast::type_cast_func[t_boolean][t_double]=boolean_to_double;
	TypeCast::type_cast_func[t_boolean][t_string]=errormsg;
	TypeCast::type_cast_func[t_boolean][t_date]=errormsg;
	TypeCast::type_cast_func[t_boolean][t_time]=errormsg;
	TypeCast::type_cast_func[t_boolean][t_datetime]=errormsg;
	TypeCast::type_cast_func[t_boolean][t_decimal]=boolean_to_decimal;
	TypeCast::type_cast_func[t_boolean][t_boolean]=boolean_to_boolean;
}

#endif
