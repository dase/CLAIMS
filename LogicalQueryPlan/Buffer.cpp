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
	// TODO Auto-generated destructor stub
}
Dataflow Buffer::getDataflow(){
	dataflow_=child_->getDataflow();
	return dataflow_;
}
BlockStreamIteratorBase* Buffer::getIteratorTree(const unsigned & blocksize){
	BlockStreamIteratorBase* child=child_->getIteratorTree(blocksize);
	Schema* schema=this->getSchema(dataflow_.attribute_list_);
	ExpandableBlockStreamBuffer::State expander_state(schema,child,blocksize);
	BlockStreamIteratorBase* ret=new ExpandableBlockStreamBuffer(expander_state);
	return ret;
}
