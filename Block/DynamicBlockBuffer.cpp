/*
 * DynamicBlockBuffer.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */
#include <assert.h>
#include "DynamicBlockBuffer.h"

DynamicBlockBuffer::DynamicBlockBuffer() {
	// TODO Auto-generated constructor stub

}

DynamicBlockBuffer::~DynamicBlockBuffer() {
	// TODO Auto-generated destructor stub
}

bool DynamicBlockBuffer::appendNewBlock(Block* new_block){
	block_list_.push_back(new_block);
	return true;
}
bool DynamicBlockBuffer::atomicAppendNewBlock(Block* new_block){
	lock_.acquire();
	const bool ret=appendNewBlock(new_block);
	lock_.release();
	return ret;
}

Block* DynamicBlockBuffer::getBlock(unsigned index)const{
	if(index<block_list_.size()){
		return block_list_.at(index);
	}
	return 0;
}
DynamicBlockBuffer::Iterator::Iterator()
:cur_(0),dbb_(0){
}
DynamicBlockBuffer::Iterator::Iterator(const DynamicBlockBuffer* dbb)
:cur_(0),dbb_(dbb){
}
DynamicBlockBuffer::Iterator::Iterator(const Iterator & it){
	this->cur_=it.cur_;
	this->dbb_=it.dbb_;
	this->lock_=it.lock_;
}
Block* DynamicBlockBuffer::Iterator::nextBlock(){
	Block* ret;
	ret=dbb_->getBlock(cur_);
	if(ret>0){
		cur_++;
	}
	return ret;
}
Block* DynamicBlockBuffer::Iterator::atomicNextBlock(){

	lock_.acquire();
	Block* ret=nextBlock();
	lock_.release();
	return ret;
}
DynamicBlockBuffer::Iterator DynamicBlockBuffer::createIterator()const{
	return Iterator(this);
}
void DynamicBlockBuffer::destory(){
	DynamicBlockBuffer::Iterator it=this->createIterator();
	Block* block_to_deallocate;
	while(block_to_deallocate=it.nextBlock()){
		block_to_deallocate->~Block();
	}
}
unsigned DynamicBlockBuffer::getNumberOfBlocks(){
	lock_.acquire();
	unsigned ret;
	ret= block_list_.size();
	lock_.release();
	return ret;
}
