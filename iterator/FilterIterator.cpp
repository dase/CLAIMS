/*
 * FilterIterator.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: wangli
 */
#include <malloc.h>
#include "FilterIterator.h"
#include "../configure.h"
#include "../Serialization/RegisterDerivedClass.h"


FilterIterator::FilterIterator(State state)
:state(state)
{
	// TODO Auto-generated constructor stub

}

FilterIterator::~FilterIterator() {
	// TODO Auto-generated destructor stub

}

bool FilterIterator::open()
{


	value=new int;
	*(int*)value=100;
	compare=state.ComparatorList[0].compare;

	state.child->open();
	return true;
}

bool FilterIterator::close()
{
	return true;
}

FilterIterator::AttributeComparator::AttributeComparator(column_type x,column_type y,Comparator::comparison c,unsigned index, void* _value)
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
FilterIterator::AttributeComparator::AttributeComparator(column_type x,Comparator::comparison c,unsigned index, void* _value)
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
FilterIterator::AttributeComparator::AttributeComparator(const  FilterIterator::AttributeComparator & c_a)
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

FilterIterator::AttributeComparator::~AttributeComparator()
{
	free(value_);
}
void FilterIterator::try_it()
{
	std::ostringstream ostr;
	boost::archive::text_oarchive oa(ostr);
	//	oa.register_type(static_cast<SingleColumnScanIterator *>(NULL));
	Register_Schemas<boost::archive::text_oarchive>(oa);
//	Register_Iterators(oa);
}
