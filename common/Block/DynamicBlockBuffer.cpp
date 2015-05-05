/*
 * DynamicBlockBuffer.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */

#include "DynamicBlockBuffer.h"

//#include <assert.h>

#include "BlockStream.h"

DynamicBlockBuffer::DynamicBlockBuffer() {
	// TODO Auto-generated constructor stub

}
DynamicBlockBuffer::DynamicBlockBuffer(const DynamicBlockBuffer& r){
	this->block_list_=r.block_list_;
	this->lock_=r.lock_;
}

DynamicBlockBuffer::~DynamicBlockBuffer() {
	for(unsigned i=0;i<block_list_.size();i++){
		delete block_list_[i];
	}
}

bool DynamicBlockBuffer::appendNewBlock(BlockStreamBase* new_block){
	block_list_.push_back(new_block);
	return true;
}
bool DynamicBlockBuffer::atomicAppendNewBlock(BlockStreamBase* new_block){
	lock_.acquire();
	const bool ret=appendNewBlock(new_block);
	lock_.release();
	return ret;
}

BlockStreamBase* DynamicBlockBuffer::getBlock(unsigned index)const{
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
BlockStreamBase* DynamicBlockBuffer::Iterator::nextBlock(){
	BlockStreamBase* ret;
	ret=dbb_->getBlock(cur_);
	if(ret>0){
		cur_++;
	}
	return ret;
}
BlockStreamBase* DynamicBlockBuffer::Iterator::atomicNextBlock(){

	lock_.acquire();
	BlockStreamBase* ret=nextBlock();
	lock_.release();
	return ret;
}
DynamicBlockBuffer::Iterator DynamicBlockBuffer::createIterator()const{
	return Iterator(this);
}
void DynamicBlockBuffer::destory(){
	DynamicBlockBuffer::Iterator it=this->createIterator();
	BlockStreamBase* block_to_deallocate;
	while(block_to_deallocate=it.nextBlock()){
		delete block_to_deallocate;
	}
	this->block_list_.clear();
}
unsigned DynamicBlockBuffer::getNumberOfBlocks(){
	lock_.acquire();
	unsigned ret;
	ret= block_list_.size();
	lock_.release();
	return ret;
}
unsigned long DynamicBlockBuffer::getNumberOftuples()const{
	unsigned long ret=0;
	DynamicBlockBuffer::Iterator it=this->createIterator();
	BlockStreamBase* block;
	while(block=(BlockStreamBase*)it.nextBlock()){
		ret+=block->getTuplesInBlock();
	}
	return ret;
}
