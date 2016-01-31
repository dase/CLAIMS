/*
 * Comparator.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wangli
 *  Modifed on: Mar 26, 2014
 *  	  Author: yukai
 */
#ifndef __COMPARATOR_CPP_
#define __COMPARATOR_CPP_
#include "Comparator.h"
#include <stdio.h>
#include <stdlib.h>
#include "types/NValue.hpp"
#include "types/decimal.h"
using claims::common::Decimal;
// =
template<typename X,typename Y>
bool equal(const void *x,const void *y){
	return (*(X*)x)==(*(Y*)y);
}
template<>
bool equal<char*, char*>(const void *x, const void *y){
	return (strcmp((char*)x, (char*)y)) == 0;
}
template<>
bool equal<Decimal*, Decimal*>(const void *x,const void *y){
	return ((Decimal*)x)->op_equals(*(Decimal*)y);
}

// != operator
template<typename X, typename Y>
inline bool notEqual(const void *x, const void *y){
	return !equal<X,Y>(x,y);
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
	return (strcmp((char*)x,(char*)y) == -1);
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
bool LESS<Decimal*, Decimal*>(const void *x,const void *y){
//	if (((NValue*)x)->op_equals(*(NValue*)y))
//		return false;
//	NValue tmp = ((NValue*)x)->op_min(*(NValue*)y);
//	if (tmp.op_equals(*(NValue*)x))
//		return true;
//	return false;
	return ((Decimal*)x)->compare(*(Decimal*)y)==VALUE_COMPARE_LESSTHAN;
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

// > operator
template<typename X, typename Y>
bool great(const void *x, const void *y){
	return *(X*)x > *(Y*)y;
}

template<>
bool great<int,char*>(const void* x,const  void* y)
{
	return *(int*)x>atoi((char*)y);
}
template<>
bool great<float,char*>(const void* x,const  void* y)
{
	return *(float*)x>atof((char*)y);
}
template<>
bool great<char*,char*>(const void* x,const  void* y)
{
	return (strcmp((char*)x,(char*)y) == 1);
}

template<>
bool great<char*,int>(const void* x,const  void* y)
{
	return atoi((char*)y)>*(int*)x;
}
template<>
bool great<char*,float>(const void* x,const  void* y)
{
	return atof((char*)y)>*(float*)x;
}

template<>
bool great<Decimal*, Decimal*>(const void *x,const void *y){
	if (((Decimal*)x)->op_equals(*(Decimal*)y))
		return false;
	Decimal tmp = ((Decimal*)x)->op_max(*(Decimal*)y);
	if (tmp.op_equals(*(Decimal*)x))
		return true;
	return false;
}

// <= operator
template<typename X,typename Y>
inline bool lessEqual(const void* x,const void* y)
{
	return !great<X,Y>(x,y);
}

////////////////////
std::map<Comparator::Pair,comFun> Comparator::funs_L;
std::map<Comparator::Pair,comFun> Comparator::funs_GEQ;
std::map<Comparator::Pair,comFun> Comparator::funs_EQ;
std::map<Comparator::Pair,comFun> Comparator::funs_NEQ;
std::map<Comparator::Pair,comFun> Comparator::funs_G;
std::map<Comparator::Pair,comFun> Comparator::funs_LEQ;
Lock Comparator::lock_;
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
	funs_L[Comparator::Pair(t_decimal,t_decimal)]=LESS<Decimal*,Decimal*>;

	funs_L[Comparator::Pair(t_u_long,t_u_long)]=LESS<unsigned long,unsigned long>;
}
void Comparator::initialize_GEQ()
{
//	lock_.acquire();
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
	funs_GEQ[Comparator::Pair(t_decimal,t_decimal)]=greatEqual<Decimal*,Decimal*>;

	funs_GEQ[Comparator::Pair(t_u_long,t_u_long)]=greatEqual<unsigned long,unsigned long>;
//	lock_.release();
}
void Comparator::initialize_EQ()
{
//	lock_.acquire();
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
	funs_EQ[Comparator::Pair(t_decimal,t_decimal)]=equal<Decimal*,Decimal*>;
//	lock_.release();
}

void Comparator::initialize_NEQ()
{
	funs_NEQ[Comparator::Pair(t_int,t_int)]=notEqual<int,int>;
	funs_NEQ[Comparator::Pair(t_int,t_float)]=notEqual<int,float>;
//	funs_NEQ[Comparator::Pair(t_int,t_string)]=notEqual<int,char*>;

	funs_NEQ[Comparator::Pair(t_float,t_int)]=notEqual<float,int>;
	funs_NEQ[Comparator::Pair(t_float,t_float)]=notEqual<float,float>;
//	funs_NEQ[Comparator::Pair(t_float,t_string)]=notEqual<float,char*>;

//	funs_NEQ[Comparator::Pair(t_string,t_int)]=notEqual<char*,int>;
//	funs_NEQ[Comparator::Pair(t_string,t_float)]=notEqual<char*,float>;
	funs_NEQ[Comparator::Pair(t_string,t_string)]=notEqual<char*,char*>;
	funs_NEQ[Comparator::Pair(t_u_long,t_u_long)]=notEqual<unsigned long,unsigned long>;

	funs_NEQ[Comparator::Pair(t_date,t_date)]=notEqual<date,date>;
	funs_NEQ[Comparator::Pair(t_time,t_time)]=notEqual<time_duration,time_duration>;
	funs_NEQ[Comparator::Pair(t_datetime,t_datetime)]=notEqual<ptime,ptime>;
	funs_NEQ[Comparator::Pair(t_decimal,t_decimal)]=notEqual<Decimal*,Decimal*>;
}


void Comparator::initialize_G()
{
	funs_G[Comparator::Pair(t_int,t_int)]=great<int,int>;
	funs_G[Comparator::Pair(t_int,t_float)]=great<int,float>;
//	funs_G[Comparator::Pair(t_int,t_string)]=great<int,char*>;

	funs_G[Comparator::Pair(t_float,t_int)]=great<float,int>;
	funs_G[Comparator::Pair(t_float,t_float)]=great<float,float>;
//	funs_G[Comparator::Pair(t_float,t_string)]=great<float,char*>;

//	funs_G[Comparator::Pair(t_string,t_int)]=great<char*,int>;
//	funs_G[Comparator::Pair(t_string,t_float)]=great<char*,float>;
	funs_G[Comparator::Pair(t_string,t_string)]=great<char*,char*>;
	funs_G[Comparator::Pair(t_u_long,t_u_long)]=great<unsigned long,unsigned long>;

	funs_G[Comparator::Pair(t_date,t_date)]=great<date,date>;
	funs_G[Comparator::Pair(t_time,t_time)]=great<time_duration,time_duration>;
	funs_G[Comparator::Pair(t_datetime,t_datetime)]=great<ptime,ptime>;
	funs_G[Comparator::Pair(t_decimal,t_decimal)]=great<Decimal*,Decimal*>;
}

void Comparator::initialize_LEQ()
{
	funs_LEQ[Comparator::Pair(t_int,t_int)]=lessEqual<int,int>;
	funs_LEQ[Comparator::Pair(t_int,t_float)]=lessEqual<int,float>;
//	funs_LEQ[Comparator::Pair(t_int,t_string)]=lessEqual<int,char*>;

	funs_LEQ[Comparator::Pair(t_float,t_int)]=lessEqual<float,int>;
	funs_LEQ[Comparator::Pair(t_float,t_float)]=lessEqual<float,float>;
//	funs_LEQ[Comparator::Pair(t_float,t_string)]=lessEqual<float,char*>;

//	funs_LEQ[Comparator::Pair(t_string,t_int)]=lessEqual<char*,int>;
//	funs_LEQ[Comparator::Pair(t_string,t_float)]=lessEqual<char*,float>;
	funs_LEQ[Comparator::Pair(t_string,t_string)]=lessEqual<char*,char*>;
	funs_LEQ[Comparator::Pair(t_u_long,t_u_long)]=lessEqual<unsigned long,unsigned long>;

	funs_LEQ[Comparator::Pair(t_date,t_date)]=lessEqual<date,date>;
	funs_LEQ[Comparator::Pair(t_time,t_time)]=lessEqual<time_duration,time_duration>;
	funs_LEQ[Comparator::Pair(t_datetime,t_datetime)]=lessEqual<ptime,ptime>;
	funs_LEQ[Comparator::Pair(t_decimal,t_decimal)]=lessEqual<Decimal*,Decimal*>;
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
	lock_.acquire();
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
	if(funs_NEQ.empty())
	{
		Comparator::initialize_NEQ();
	}
	if(funs_G.empty())
	{
		Comparator::initialize_G();
	}
	if(funs_LEQ.empty())
	{
		Comparator::initialize_LEQ();
	}
	lock_.release();
	switch(compareType)
	{
		case Comparator::L:
		{

			if(funs_L.find(Comparator::Pair(pair.first.type,pair.second.type))!=funs_L.end())
			{
				compare=funs_L.at(Comparator::Pair(pair.first.type,pair.second.type));
			}
			else
			{
				assert(false);
				printf("Error!\n");
			}
			break;
		}
		case Comparator::GEQ:
		{
			if(funs_GEQ.find(Comparator::Pair(pair.first.type,pair.second.type))!=funs_GEQ.end())
			{
				compare=funs_GEQ[Comparator::Pair(pair.first.type,pair.second.type)];
			}
			else{
				assert(false);
			}
			break;
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
			}
			break;
		}
		case Comparator::NEQ:
		{
			if(funs_NEQ.find(Comparator::Pair(pair.first.type,pair.second.type))!=funs_NEQ.end())
			{
				compare=funs_NEQ.at(Comparator::Pair(pair.first.type,pair.second.type));
			}
			break;
		}
		case Comparator::G:
		{
			if(funs_G.find(Comparator::Pair(pair.first.type,pair.second.type))!=funs_G.end())
			{
				compare=funs_G.at(Comparator::Pair(pair.first.type,pair.second.type));
			}
			break;
		}
		case Comparator::LEQ:
		{
			if(funs_LEQ.find(Comparator::Pair(pair.first.type,pair.second.type))!=funs_LEQ.end())
			{
				compare=funs_LEQ[Comparator::Pair(pair.first.type,pair.second.type)];
			}
			break;
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
#endif
