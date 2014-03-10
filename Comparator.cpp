/*
 * Comparator.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wangli
 */

#include "Comparator.h"
#include <stdio.h>
#include <stdlib.h>
#include "types/NValue.hpp"
// =
template<typename X,typename Y>
bool equal(const void *x,const void *y){
	return (*(X*)x)==(*(Y*)y);
}
template<>
bool equal<NValue*, NValue*>(const void *x,const void *y){
	return ((NValue*)x)->op_equals(*(NValue*)y);
}

///////////////////////less
template<typename X, typename Y>
bool LESS(const void* x,const  void* y)
{
	return *(X*)x<*(Y*)y;
}

template<>
bool LESS<int,char*>(const void* x,const  void* y)
{
	return *(int*)x<atoi((char*)y);
}
template<>
bool LESS<float,char*>(const void* x,const  void* y)
{
	return *(float*)x<atof((char*)y);
}
template<>
bool LESS<char*,char*>(const void* x,const  void* y)
{
	return strcmp((char*)x,(char*)y);
}

template<>
bool LESS<char*,int>(const void* x,const  void* y)
{
	return atoi((char*)y)<*(int*)x;
}
template<>
bool LESS<char*,float>(const void* x,const  void* y)
{
	return atof((char*)y)<*(float*)x;
}
template<>
bool LESS<NValue*, NValue*>(const void *x,const void *y){
	if (((NValue*)x)->op_equals(*(NValue*)y))
		return false;
	NValue tmp = ((NValue*)x)->op_min(*(NValue*)y);
	if (tmp.op_equals(*(NValue*)x))
		return true;
	return false;
}
////////////////////////////////////////////

////////////////////////////////////////////
/*
 * greatEqual
 */
template<typename X,typename Y>
inline bool greatEqual(const void* x,const void* y)
{
	return !LESS<X,Y>(x,y);
}

////////////////////
std::map<Comparator::Pair,comFun> Comparator::funs_L;
std::map<Comparator::Pair,comFun> Comparator::funs_GEQ;
std::map<Comparator::Pair,comFun> Comparator::funs_EQ;
void Comparator::initialize_L()
{
	funs_L[Comparator::Pair(t_int,t_int)]=LESS<int,int>;
	funs_L[Comparator::Pair(t_int,t_float)]=LESS<int,float>;
	funs_L[Comparator::Pair(t_int,t_string)]=LESS<int,char*>;

	funs_L[Comparator::Pair(t_float,t_int)]=LESS<float,int>;
	funs_L[Comparator::Pair(t_float,t_float)]=LESS<float,float>;
	funs_L[Comparator::Pair(t_float,t_string)]=LESS<float,char*>;

	funs_L[Comparator::Pair(t_string,t_int)]=LESS<char*,int>;
	funs_L[Comparator::Pair(t_string,t_float)]=LESS<char*,float>;
	funs_L[Comparator::Pair(t_string,t_string)]=LESS<char*,char*>;

	funs_L[Comparator::Pair(t_date,t_date)]=LESS<date,date>;
	funs_L[Comparator::Pair(t_time,t_time)]=LESS<time_duration,time_duration>;
	funs_L[Comparator::Pair(t_datetime,t_datetime)]=LESS<ptime,ptime>;
	funs_L[Comparator::Pair(t_datetime,t_decimal)]=LESS<NValue*,NValue*>;

	funs_L[Comparator::Pair(t_u_long,t_u_long)]=LESS<unsigned long,unsigned long>;
}
void Comparator::initialize_GEQ()
{
	funs_GEQ[Comparator::Pair(t_int,t_int)]=greatEqual<int,int>;
	funs_GEQ[Comparator::Pair(t_int,t_float)]=greatEqual<int,float>;
	funs_GEQ[Comparator::Pair(t_int,t_string)]=greatEqual<int,char*>;

	funs_GEQ[Comparator::Pair(t_float,t_int)]=greatEqual<float,int>;
	funs_GEQ[Comparator::Pair(t_float,t_float)]=greatEqual<float,float>;
	funs_GEQ[Comparator::Pair(t_float,t_string)]=greatEqual<float,char*>;

	funs_GEQ[Comparator::Pair(t_string,t_int)]=greatEqual<char*,int>;
	funs_GEQ[Comparator::Pair(t_string,t_float)]=greatEqual<char*,float>;
	funs_GEQ[Comparator::Pair(t_string,t_string)]=greatEqual<char*,char*>;

	funs_GEQ[Comparator::Pair(t_date,t_date)]=greatEqual<date,date>;
	funs_GEQ[Comparator::Pair(t_time,t_time)]=greatEqual<time_duration,time_duration>;
	funs_GEQ[Comparator::Pair(t_datetime,t_datetime)]=greatEqual<ptime,ptime>;
	funs_GEQ[Comparator::Pair(t_decimal,t_decimal)]=greatEqual<NValue*,NValue*>;

	funs_L[Comparator::Pair(t_u_long,t_u_long)]=LESS<unsigned long,unsigned long>;
}
void Comparator::initialize_EQ()
{
	funs_EQ[Comparator::Pair(t_int,t_int)]=equal<int,int>;
	funs_EQ[Comparator::Pair(t_int,t_float)]=equal<int,float>;
//	funs_EQ[Comparator::Pair(t_int,t_string)]=equal<int,char*>;

	funs_EQ[Comparator::Pair(t_float,t_int)]=equal<float,int>;
	funs_EQ[Comparator::Pair(t_float,t_float)]=equal<float,float>;
//	funs_EQ[Comparator::Pair(t_float,t_string)]=equal<float,char*>;

//	funs_EQ[Comparator::Pair(t_string,t_int)]=equal<char*,int>;
//	funs_EQ[Comparator::Pair(t_string,t_float)]=equal<char*,float>;
	funs_EQ[Comparator::Pair(t_string,t_string)]=equal<char*,char*>;
	funs_EQ[Comparator::Pair(t_u_long,t_u_long)]=equal<unsigned long,unsigned long>;

	funs_EQ[Comparator::Pair(t_date,t_date)]=equal<date,date>;
	funs_EQ[Comparator::Pair(t_time,t_time)]=equal<time_duration,time_duration>;
	funs_EQ[Comparator::Pair(t_datetime,t_datetime)]=equal<ptime,ptime>;
	funs_EQ[Comparator::Pair(t_decimal,t_decimal)]=equal<NValue*,NValue*>;
}
Comparator::Comparator(column_type x, column_type y, Comparator::comparison c):pair(x,y),compareType(c) {
	// TODO Auto-generated constructor stub
	iniatilize();
	assert(compare!=0);
}
Comparator::Comparator(const Comparator & r){
	this->pair=r.pair;
	this->compareType=r.compareType;
	iniatilize();
}

Comparator::~Comparator() {
	// TODO Auto-generated destructor stub
}

void Comparator::iniatilize()
{
	if(funs_L.empty())
	{
		Comparator::initialize_L();
	}
	if(funs_GEQ.empty())
	{
		Comparator::initialize_GEQ();
	}
	if(funs_EQ.empty())
	{
		Comparator::initialize_EQ();
	}
	switch(compareType)
	{
		case Comparator::L:
		{
			if(funs_L.find(Comparator::Pair(pair.first.type,pair.second.type))!=funs_L.end())
			{
//			compare=funs_L[Comparator::Pair(pair.first.type,pair.second.type)];break;
				compare=funs_L.at(Comparator::Pair(pair.first.type,pair.second.type));break;
			}
			else
			{
				printf("Error!\n");
			}

		}
		case Comparator::GEQ:
		{
			if(funs_GEQ.find(Comparator::Pair(pair.first.type,pair.second.type))!=funs_GEQ.end())
			{
			compare=funs_GEQ[Comparator::Pair(pair.first.type,pair.second.type)];break;
			}

		}
		case Comparator::EQ:
		{
			if(funs_EQ.find(Comparator::Pair(pair.first.type,pair.second.type))!=funs_EQ.end())
			{
//			compare=funs_EQ[Comparator::Pair(pair.first.type,pair.second.type)];break;
				compare=funs_EQ.at(Comparator::Pair(pair.first.type,pair.second.type));
				if(compare==0){
					printf("value:::---->>> %x\n",funs_EQ.at(Comparator::Pair(pair.first.type,pair.second.type)));
					assert(false);
				}
				break;
			}

		}
		default:
		{
			//TODO: Handle the exception.
			printf("Undefined Comparator type!\n");
			assert(false);
		}
	}
	assert(compare!=0);
}
