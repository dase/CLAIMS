/*
 * Sort.cpp
 *
 *  Created on: 2014-3-1
 *      Author: casa
 */

#include "Sort.h"

Sort::Sort(LogicalOperator *child, int sortByKey)
:child_(child),sortByKey_(sortByKey){

}

Sort::~Sort() {

}

Dataflow Sort::getDataflow(){
	return child_->getDataflow();
}

BlockStreamIteratorBase *Sort::getIteratorTree(const unsigned& blocksize){
	BlockStreamIteratorBase *child=child_->getIteratorTree(blocksize);
	BlockStreamSortIterator::State state;
	state.block_size_=blocksize;
	state.child_=child;
	state.orderbyKey_=sortByKey_;
	Dataflow dataflow=child_->getDataflow();
	state.input_=getSchema(dataflow.attribute_list_);
	return new BlockStreamSortIterator(state);
}
