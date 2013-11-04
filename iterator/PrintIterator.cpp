/*
 * PrintIterator.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: wangli
 */

#include <malloc.h>

#include "PrintIterator.h"
#include "../configure.h"
PrintIterator::PrintIterator(State state):state(state) {
	// TODO Auto-generated constructor stub

}

PrintIterator::~PrintIterator() {
	// TODO Auto-generated destructor stub

}
bool PrintIterator::open()
{
	tuple=memalign(cacheline_size,state.input->getTupleMaxSize());
	return state.child->open();
}
bool PrintIterator::close()
{
	free(tuple);
	return state.child->close();
}
