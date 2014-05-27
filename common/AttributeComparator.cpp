/*
 * AttributeComparator.cpp
 *
 *  Created on: May 24, 2014
 *      Author: wangli
 */

#include "AttributeComparator.h"


AttributeComparator::AttributeComparator(column_type x,column_type y,Comparator::comparison c,unsigned index, void* _value)
:Comparator(x,y,c),index(index)
{
	value_=memalign(cacheline_size,y.get_length());
	y.operate->assignment(_value,value_);
	assert(compare!=0);
//	for(unsigned i=0;i<pair.second.get_length();i++)
//	{
//		valuebytes.push_back(*(char*)(value+i));
//	}
}
AttributeComparator::AttributeComparator(column_type x,Comparator::comparison c,unsigned index, void* _value)
:Comparator(x,x,c),index(index)
{
	value_=memalign(cacheline_size,x.get_length());
	x.operate->assignment(_value,value_);
	assert(compare!=0);
//	for(unsigned i=0;i<pair.second.get_length();i++)
//	{
//		valuebytes.push_back(*(char*)(value+i));
//	}
}
AttributeComparator::AttributeComparator(const  AttributeComparator & c_a)
:Comparator(c_a.pair.first,c_a.pair.second,c_a.compareType)
{

	value_=memalign(cacheline_size,c_a.pair.second.get_length());
	c_a.pair.second.operate->assignment(c_a.value_,value_);
	index=c_a.index;
	assert(compare!=0);
//	for(unsigned i=0;i<c_a.pair.second.get_length();i++)
//	{
//		valuebytes.push_back(*(char*)(value+i));
//	}

}
AttributeComparator::~AttributeComparator(){
	free(value_);
}
