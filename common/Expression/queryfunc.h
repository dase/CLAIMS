/*
 * queryfunc.h
 *
 *  Created on: Aug 6, 2014
 *      Author: imdb
 */

#ifndef QUERYFUNC_H_
#define QUERYFUNC_H_
#include"qnode.h"
#include "../../common/data_type.h"
#include "../../common/Logging.h"
#define OPER_TYPE_NUM 50
#define DATA_TYPE_NUM 20
#include<string.h>
class ExectorFunction
{
public:
	static ExecFunc operator_function[DATA_TYPE_NUM][OPER_TYPE_NUM];
};
inline void oper_not_support(FuncCallInfo fcinfo)
{
	SQLParse_elog("This oper_functions is not supported now!!!!!!!!!!!!!");
	assert(false);
}
/*******************int*************************/
inline void int_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]+(*(int *)fcinfo->args[1]));
}
inline void int_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]-(*(int *)fcinfo->args[1]));
}
inline void int_multi(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]*(*(int *)fcinfo->args[1]));
}
inline void int_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	int val=*(int *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("error:The int_divide divided =0!!!!!");
		assert(val!=0);
	}
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]/(*(int *)fcinfo->args[1]));
}
inline void int_mod(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]%(*(int *)fcinfo->args[1]));
}
inline void int_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])==(*(int *)fcinfo->args[1]);
}
inline void int_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])!=(*(int *)fcinfo->args[1]);
}
inline void int_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])>(*(int *)fcinfo->args[1]);
}
inline void int_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])>=(*(int *)fcinfo->args[1]);
}
inline void int_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])<(*(int *)fcinfo->args[1]);
}
inline void int_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])<=(*(int *)fcinfo->args[1]);
}
/*******************int*************************/


/*******************u_long*************************/

inline void u_long_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])+(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])-(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_multi(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])*(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	unsigned long val=*(unsigned long *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The u_long_divide divided = 0");
		assert(false);
	}
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])/(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_mod(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	unsigned long val=*(unsigned long *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The u_long_mod moded = 0");
		assert(false);
	}
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])%(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0]==*(unsigned long *)fcinfo->args[1]);
}
inline void u_long_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])!=(*(unsigned long *)fcinfo->args[1]);

}
inline void u_long_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])>(*(unsigned long *)fcinfo->args[1]);

}
inline void u_long_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])>=(*(unsigned long *)fcinfo->args[1]);
}
inline void u_long_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])<(*(unsigned long *)fcinfo->args[1]);
}
inline void u_long_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])<=(*(unsigned long *)fcinfo->args[1]);
}
/*******************u_long*************************/


/*******************float*************************/
inline void float_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])+(*(float *)fcinfo->args[1]));
}
inline void float_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])-(*(float *)fcinfo->args[1]));
}
inline void float_multi(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])*(*(float *)fcinfo->args[1]));
}
inline void float_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	float val=*(float *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The float_divide divided = 0");
		assert(false);
	}
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])/(*(float *)fcinfo->args[1]));
}

inline void float_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])==(*(float *)fcinfo->args[1]);
}
inline void float_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])!=(*(float *)fcinfo->args[1]);
}
inline void float_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])>(*(float *)fcinfo->args[1]);
}
inline void float_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])>=(*(float *)fcinfo->args[1]);
}
inline void float_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])<(*(float *)fcinfo->args[1]);
}
inline void float_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])<=(*(float *)fcinfo->args[1]);
}
/*******************float*************************/


/*******************double*************************/

inline void double_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])+(*(double *)fcinfo->args[1]));
}
inline void double_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])-(*(double *)fcinfo->args[1]));
}
inline void double_multi(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])*(*(double *)fcinfo->args[1]));
}
inline void double_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	double val=*(double *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The double_divide divided = 0");
		assert(false);
	}
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])/(*(double *)fcinfo->args[1]));
}

inline void double_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]==*(double *)fcinfo->args[1]);
}
inline void double_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]!=*(double *)fcinfo->args[1]);
}
inline void double_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]>*(double *)fcinfo->args[1]);
}
inline void double_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]>=*(double *)fcinfo->args[1]);
}
inline void double_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	bool ans=1;
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]<*(double *)fcinfo->args[1]);
}
inline void double_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]<=*(double *)fcinfo->args[1]);
}
/*******************double*************************/

/*******************smallInt*************************/

inline void smallInt_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0])+(*(short int *)fcinfo->args[1]);
}
inline void smallInt_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0])-(*(short int *)fcinfo->args[1]);
}
inline void smallInt_multi(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0])*(*(short int *)fcinfo->args[1]);
}
inline void smallInt_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	short int val=*(short int *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The smallInt_divide divided = 0");
		assert(false);
	}
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0])/(*(short int *)fcinfo->args[1]);
}
inline void smallInt_mod(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	short int val=*(short int *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The smallInt_mod moded = 0");
		assert(false);
	}
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0]%(*(short int *)fcinfo->args[1]));
}
inline void smallInt_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]==*(short int *)fcinfo->args[1]);
}
inline void smallInt_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]!=*(short int *)fcinfo->args[1]);
}
inline void smallInt_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]>*(short int *)fcinfo->args[1]);
}
inline void smallInt_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]>=*(short int *)fcinfo->args[1]);
}
inline void smallInt_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]<*(short int *)fcinfo->args[1]);
}
inline void smallInt_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]<=*(short int *)fcinfo->args[1]);
}
/*******************smallInt*************************/

/*****************boolean********************/


inline void boolean_and(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=((*(bool *)fcinfo->args[0])&&(*(bool *)fcinfo->args[1]));
}
inline void boolean_or(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=((*(bool *)fcinfo->args[0])||(*(bool *)fcinfo->args[1]));
}
inline void boolean_xor(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=((*(bool *)fcinfo->args[0])^(*(bool *)fcinfo->args[1]));
}
inline void boolean_not(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==1);
	*(bool *)fcinfo->results=(!(*(bool *)fcinfo->args[0]));
}
inline void boolean_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]==*(bool *)fcinfo->args[1]);
}
inline void boolean_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]!=*(bool *)fcinfo->args[1]);
}
inline void boolean_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]>*(bool *)fcinfo->args[1]);
}
inline void boolean_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]>=*(bool *)fcinfo->args[1]);
}
inline void boolean_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]<*(bool *)fcinfo->args[1]);
}
inline void boolean_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]<=*(bool *)fcinfo->args[1]);
}
/*****************boolean********************/


/*****************decimal********************/
inline void decimal_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_add(*(NValue *)fcinfo->args[1]);
}
inline void decimal_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_subtract(*(NValue *)fcinfo->args[1]);
}
inline void decimal_multi(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_multiply(*(NValue *)fcinfo->args[1]);
}
inline void decimal_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_divide(*(NValue *)fcinfo->args[1]);
}
inline void decimal_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_equals(*(NValue *)fcinfo->args[1]);
}
inline void decimal_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_not_equals(*(NValue *)fcinfo->args[1]);
}
inline void decimal_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_great(*(NValue *)fcinfo->args[1]);
}
inline void decimal_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_great_equals(*(NValue *)fcinfo->args[1]);
}
inline void decimal_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_less(*(NValue *)fcinfo->args[1]);
}
inline void decimal_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_less_equals(*(NValue *)fcinfo->args[1]);
}
/*****************decimal********************/


/*****************string********************/
//the limit is the left should be column

inline void string_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])==0);
}
inline void string_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])!=0);
}
inline void string_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])>0);
}
inline void string_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])>=0);
}
inline void string_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])<0);
}
inline void string_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])<=0);
}
inline void string_both_trim(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	int begin=0,end=strlen((char *)fcinfo->args[1])-1;
	for(;((char *)fcinfo->args[1])[begin]==((char *)fcinfo->args[0])[0]&&((char *)fcinfo->args[1])[begin]!='\0';begin++);
	if(end>begin)
	for(;((char *)fcinfo->args[1])[end]==((char *)fcinfo->args[0])[0]&&end>=0;end--);
	strncpy((char *)fcinfo->results,(char *)fcinfo->args[1]+begin,end-begin+1);
}
inline void string_leading_trim(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	int begin=0,end=strlen((char *)fcinfo->args[1])-1;
	for(;((char *)fcinfo->args[1])[begin]==((char *)fcinfo->args[0])[0]&&((char *)fcinfo->args[1])[begin]!='\0';begin++);
	strncpy((char *)fcinfo->results,(char *)fcinfo->args[1]+begin,end-begin+1);
}
inline void string_trailing_trim(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	int begin=0,end=strlen((char *)fcinfo->args[1])-1;
	for(;((char *)fcinfo->args[1])[end]==((char *)fcinfo->args[0])[0]&&end>=0;end--);
	strncpy((char *)fcinfo->results,(char *)fcinfo->args[1]+begin,end-begin+1);
}
inline void string_like(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);

}
inline void string_not_like(FuncCallInfo fcinfo)
{
	string_like(fcinfo);

}
/*****************string********************/

/*****************decimal********************/
/*****************decimal********************/

inline void initialize_operator_function()
{
	/*****************int********************/
	ExectorFunction::operator_function[t_int][oper_add]=int_add;
	ExectorFunction::operator_function[t_int][oper_minus]=int_minus;
	ExectorFunction::operator_function[t_int][oper_multi]=int_multi;
	ExectorFunction::operator_function[t_int][oper_divide]=int_divide;
	ExectorFunction::operator_function[t_int][oper_mod]=int_mod;
	ExectorFunction::operator_function[t_int][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_int][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_int][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_int][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_int][oper_equal]=int_equal;
	ExectorFunction::operator_function[t_int][oper_not_equal]=int_not_equal;
	ExectorFunction::operator_function[t_int][oper_great]=int_great;
	ExectorFunction::operator_function[t_int][oper_great_equal]=int_great_equal;
	ExectorFunction::operator_function[t_int][oper_less]=int_less;
	ExectorFunction::operator_function[t_int][oper_less_equal]=int_less_equal;
	/*****************int********************/

	/*****************ulong********************/
	ExectorFunction::operator_function[t_u_long][oper_add]=u_long_add;
	ExectorFunction::operator_function[t_u_long][oper_minus]=u_long_minus;
	ExectorFunction::operator_function[t_u_long][oper_multi]=u_long_multi;
	ExectorFunction::operator_function[t_u_long][oper_divide]=u_long_divide;
	ExectorFunction::operator_function[t_u_long][oper_mod]=u_long_mod;
	ExectorFunction::operator_function[t_u_long][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_u_long][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_u_long][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_u_long][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_u_long][oper_equal]=u_long_equal;
	ExectorFunction::operator_function[t_u_long][oper_not_equal]=u_long_not_equal;
	ExectorFunction::operator_function[t_u_long][oper_great]=u_long_great;
	ExectorFunction::operator_function[t_u_long][oper_great_equal]=u_long_great_equal;
	ExectorFunction::operator_function[t_u_long][oper_less]=u_long_less;
	ExectorFunction::operator_function[t_u_long][oper_less_equal]=u_long_less_equal;
	/*****************ulong********************/

	/*****************float********************/
	ExectorFunction::operator_function[t_float][oper_add]=float_add;
	ExectorFunction::operator_function[t_float][oper_minus]=float_minus;
	ExectorFunction::operator_function[t_float][oper_multi]=float_multi;
	ExectorFunction::operator_function[t_float][oper_divide]=float_divide;
	ExectorFunction::operator_function[t_float][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_equal]=float_equal;
	ExectorFunction::operator_function[t_float][oper_not_equal]=float_not_equal;
	ExectorFunction::operator_function[t_float][oper_great]=float_great;
	ExectorFunction::operator_function[t_float][oper_great_equal]=float_great_equal;
	ExectorFunction::operator_function[t_float][oper_less]=float_less;
	ExectorFunction::operator_function[t_float][oper_less_equal]=float_less_equal;
	/*****************float********************/

	/*****************double********************/

	ExectorFunction::operator_function[t_double][oper_add]=double_add;
	ExectorFunction::operator_function[t_double][oper_minus]=double_minus;
	ExectorFunction::operator_function[t_double][oper_multi]=double_multi;
	ExectorFunction::operator_function[t_double][oper_divide]=double_divide;
	ExectorFunction::operator_function[t_double][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_equal]=double_equal;
	ExectorFunction::operator_function[t_double][oper_not_equal]=double_not_equal;
	ExectorFunction::operator_function[t_double][oper_great]=double_great;
	ExectorFunction::operator_function[t_double][oper_great_equal]=double_great_equal;
	ExectorFunction::operator_function[t_double][oper_less]=double_less;
	ExectorFunction::operator_function[t_double][oper_less_equal]=double_less_equal;
	/*****************double********************/

	/*****************smallInt********************/
	ExectorFunction::operator_function[t_smallInt][oper_add]=smallInt_add;
	ExectorFunction::operator_function[t_smallInt][oper_minus]=smallInt_minus;
	ExectorFunction::operator_function[t_smallInt][oper_multi]=smallInt_multi;
	ExectorFunction::operator_function[t_smallInt][oper_divide]=smallInt_divide;
	ExectorFunction::operator_function[t_smallInt][oper_mod]=smallInt_mod;
	ExectorFunction::operator_function[t_smallInt][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_smallInt][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_smallInt][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_smallInt][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_smallInt][oper_equal]=smallInt_equal;
	ExectorFunction::operator_function[t_smallInt][oper_not_equal]=smallInt_not_equal;
	ExectorFunction::operator_function[t_smallInt][oper_great]=smallInt_great;
	ExectorFunction::operator_function[t_smallInt][oper_great_equal]=smallInt_great_equal;
	ExectorFunction::operator_function[t_smallInt][oper_less]=smallInt_less;
	ExectorFunction::operator_function[t_smallInt][oper_less_equal]=smallInt_less_equal;

	/*****************smallInt********************/

	/*****************boolean********************/
	ExectorFunction::operator_function[t_boolean][oper_add]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_minus]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_multi]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_divide]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_and]=boolean_and;
	ExectorFunction::operator_function[t_boolean][oper_or]=boolean_or;
	ExectorFunction::operator_function[t_boolean][oper_xor]=boolean_xor;
	ExectorFunction::operator_function[t_boolean][oper_not]=boolean_not;
	ExectorFunction::operator_function[t_boolean][oper_equal]=boolean_equal;
	ExectorFunction::operator_function[t_boolean][oper_not_equal]=boolean_not_equal;
	ExectorFunction::operator_function[t_boolean][oper_great]=boolean_great;
	ExectorFunction::operator_function[t_boolean][oper_great_equal]=boolean_great_equal;
	ExectorFunction::operator_function[t_boolean][oper_less]=boolean_less;
	ExectorFunction::operator_function[t_boolean][oper_less_equal]=boolean_less_equal;
	/*****************boolean********************/


	/*****************decimal********************/

	ExectorFunction::operator_function[t_decimal][oper_add]=decimal_add;
	ExectorFunction::operator_function[t_decimal][oper_minus]=decimal_minus;
	ExectorFunction::operator_function[t_decimal][oper_multi]=decimal_multi;
	ExectorFunction::operator_function[t_decimal][oper_divide]=decimal_divide;
	ExectorFunction::operator_function[t_decimal][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_equal]=decimal_equal;
	ExectorFunction::operator_function[t_decimal][oper_not_equal]=decimal_not_equal;
	ExectorFunction::operator_function[t_decimal][oper_great]=decimal_great;
	ExectorFunction::operator_function[t_decimal][oper_great_equal]=decimal_great_equal;
	ExectorFunction::operator_function[t_decimal][oper_less]=decimal_less;
	ExectorFunction::operator_function[t_decimal][oper_less_equal]=decimal_less_equal;
	/*****************decimal********************/


	/*****************string********************/
	ExectorFunction::operator_function[t_string][oper_add]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_minus]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_multi]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_divide]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_equal]=string_equal;
	ExectorFunction::operator_function[t_string][oper_not_equal]=string_not_equal;
	ExectorFunction::operator_function[t_string][oper_great]=string_great;
	ExectorFunction::operator_function[t_string][oper_great_equal]=string_great_equal;
	ExectorFunction::operator_function[t_string][oper_less]=string_less;
	ExectorFunction::operator_function[t_string][oper_less_equal]=string_less_equal;

	ExectorFunction::operator_function[t_string][oper_both_trim]=string_both_trim;
	ExectorFunction::operator_function[t_string][oper_leading_trim]=string_leading_trim;
	ExectorFunction::operator_function[t_string][oper_trailing_trim]=string_trailing_trim;


	/*****************string********************/
	/*****************double********************/
	/*****************double********************/
	/*****************double********************/
	/*****************double********************/




}
#endif /* QUERYFUNC_H_ */
