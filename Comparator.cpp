/*
 * Comparator.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wangli
 */

#include "Comparator.h"
#include <stdio.h>
#include <stdlib.h>
///////////////////////less
template<typename X, typename Y>
bool less(const void* x,const  void* y)
{
	return *(X*)x<*(Y*)y;
}

// =
template<typename X,typename Y>
bool equal(const void *x,const void *y){
	return (*(X*)x)==(*(Y*)y);
}

template<>
bool less<int,char*>(const void* x,const  void* y)
{
	return *(int*)x<atoi((char*)y);
}
template<>
bool less<float,char*>(const void* x,const  void* y)
{
	return *(float*)x<atof((char*)y);
}
template<>
bool less<char*,char*>(const void* x,const  void* y)
{
	return strcmp((char*)x,(char*)y);
}

template<>
bool less<char*,int>(const void* x,const  void* y)
{
	return atoi((char*)y)<*(int*)x;
}
template<>
bool less<char*,float>(const void* x,const  void* y)
{
	return atof((char*)y)<*(float*)x;
}
////////////////////////////////////////////

////////////////////////////////////////////
/*
 * greatEqual
 */
template<typename X,typename Y>
inline bool greatEqual(const void* x,const void* y)
{
	return !less<X,Y>(x,y);
}

////////////////////
std::map<Comparator::Pair,comFun> Comparator::funs_L;
std::map<Comparator::Pair,comFun> Comparator::funs_GEQ;
std::map<Comparator::Pair,comFun> Comparator::funs_EQ;
void Comparator::initialize_L()
{
	funs_L[Comparator::Pair(t_int,t_int)]=less<int,int>;
	funs_L[Comparator::Pair(t_int,t_float)]=less<int,float>;
	funs_L[Comparator::Pair(t_int,t_string)]=less<int,char*>;

	funs_L[Comparator::Pair(t_float,t_int)]=less<float,int>;
	funs_L[Comparator::Pair(t_float,t_float)]=less<float,float>;
	funs_L[Comparator::Pair(t_float,t_string)]=less<float,char*>;

	funs_L[Comparator::Pair(t_string,t_int)]=less<char*,int>;
	funs_L[Comparator::Pair(t_string,t_float)]=less<char*,float>;
	funs_L[Comparator::Pair(t_string,t_string)]=less<char*,char*>;
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
}
Comparator::Comparator(column_type x, column_type y, Comparator::comparison c):pair(x,y),compareType(c) {
	// TODO Auto-generated constructor stub
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
			compare=funs_L[Comparator::Pair(pair.first.type,pair.second.type)];break;
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
			compare=funs_EQ[Comparator::Pair(pair.first.type,pair.second.type)];break;
			}
		}
		default:
		{
			//TODO: Handle the exception.
			printf("Undefined Comparator type!\n");
		}
	}
}
