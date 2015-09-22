/*
 * Buffer.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */

#include "Buffer.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamBuffer.h"
Buffer::Buffer(LogicalOperator* child)
:child_(child){
	// TODO Auto-generated constructor stub

}

Buffer::~Buffer() {
	if(child_>0){
		delete child_;
	}
}
Dataflow Buffer::GetDataflow(){
	dataflow_=child_->GetDataflow();
	return dataflow_;
}
BlockStreamIteratorBase* Buffer::GetIteratorTree(const unsigned & blocksize){
	BlockStreamIteratorBase* child=child_->GetIteratorTree(blocksize);
	Schema* schema=this->getSchema(dataflow_.attribute_list_);
	ExpandableBlockStreamBuffer::State expander_state(schema,child,blocksize);
	BlockStreamIteratorBase* ret=new ExpandableBlockStreamBuffer(expander_state);
	return ret;
}
