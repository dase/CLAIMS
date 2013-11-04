/*
 * BlockContainer.cpp
 *
 *  Created on: Sep 7, 2013
 *      Author: wangli
 */
#include <assert.h>
#include "BlockContainer.h"

BlockContainer::BlockContainer(unsigned block_size)
:Block(block_size),actual_size_(0){

}

BlockContainer::~BlockContainer() {
	// TODO Auto-generated destructor stub
}

unsigned BlockContainer::GetMaxSize()const{
	return this->getsize();
}
unsigned BlockContainer::GetCurSize()const{
	return actual_size_;
}
unsigned BlockContainer::GetRestSize()const{
	return getsize()-actual_size_;
}
void BlockContainer::reset(){
	actual_size_=0;
}
void BlockContainer::IncreaseActualSize(unsigned size){
	assert(size+actual_size_<=this->getsize());
	actual_size_+=size;
}
