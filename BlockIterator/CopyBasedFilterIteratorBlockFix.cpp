/*
 * FilterIteratorBlockFix.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: wangli
 */

#include "CopyBasedFilterIteratorBlockFix.h"

CopyBasedFilterIteratorBlockFix::CopyBasedFilterIteratorBlockFix(State state)
:state(state){
	// TODO Auto-generated constructor stub

}

CopyBasedFilterIteratorBlockFix::~CopyBasedFilterIteratorBlockFix() {
	// TODO Auto-generated destructor stub
}

bool CopyBasedFilterIteratorBlockFix::open(){
	block_for_asking=new BlockFix(4096,state.schema);
	block_for_asking->setEmpty();
	state.child->open();
	received_last_block=false;
	return true;
}

bool CopyBasedFilterIteratorBlockFix::close(){
	return true;
}
