/*
 * CombinedIterator.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: wangli
 */
#include <malloc.h>

#include "CombinedIterator.h"
#include "../configure.h"

CombinedIterator::CombinedIterator(CombinedIterator::State state)
:state(state)
{}

CombinedIterator::~CombinedIterator() {
	// TODO Auto-generated destructor stub
	free(tuple);
}

bool CombinedIterator::open()
{
	unsigned total_length=0;
	for(unsigned i=0;i<state.inputs.size();i++)
	{
		total_length+=state.inputs[i]->getTupleMaxSize();
	}
	tuple=memalign(cacheline_size,total_length);
	for(unsigned i=0;i<state.children.size();i++)
	{
		if(!state.children[i]->open())
		{
			//TODO: handle the failure
			return false;
		}
	}
	return true;
}

bool CombinedIterator::next(void* desc)
{
	unsigned accum_len=0;
	for(unsigned i=0;i<state.children.size();i++)
	{
		if(state.children[i]->next((char*)desc+accum_len)==false)
			return false;
		accum_len+=state.inputs[i]->getTupleActualSize((char*)desc+accum_len);
	}
	return true;
}
bool CombinedIterator::close()
{
	for(unsigned i=0;i<state.children.size();i++)
	{
		state.children[i]->close();
	}
	return true;
}



