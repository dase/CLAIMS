/*
 * BlockStream.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */
#include <malloc.h>
#include "../configure.h"
#include "BlockStream.h"
#include "BlockWritable.h"
#include <assert.h>
BlockStreamFix::BlockStreamFix(unsigned block_size,unsigned tuple_size)
:block_size_(block_size),tuple_size_(tuple_size){
	data_=(char*)memalign(PAGE_SIZE,block_size_);
	free_=data_;
}

BlockStreamFix::~BlockStreamFix() {
	free(data_);
	data_=0;
}

void BlockStreamFix::setEmpty(){
	free_=data_;
}


BlockStreamBase* BlockStreamBase::createBlock(Schema* schema,unsigned block_size){
	if(schema->getSchemaType()==Schema::fixed){
		return new BlockStreamFix(block_size,schema->getTupleMaxSize());
	}
	else{
		/*
		 * TODO: support variable-length BlockStream
		 */
		return 0;
	}
}

void* BlockStreamFix::getBlockDataAddress(){
	return data_;
}

void BlockStreamFix::setBlockDataAddress(void* addr){
	data_=(char*)addr;
}

bool BlockStreamFix::switchBlock(BlockStreamBase& block){
	BlockStreamFix* blockfix=(BlockStreamFix*)&block;
	assert(blockfix->block_size_==block_size_);

	/* swap the data pointer */
	char* data_temp;
	data_temp=blockfix->data_;
	blockfix->data_=(this->data_);
	this->data_=data_temp;

	/* swap the free pointer */
	char* free_temp;
	free_temp=blockfix->free_;
	blockfix->free_=this->free_;
	this->free_=free_temp;

	return true;


}

void BlockStreamFix::copyBlock(void* addr, unsigned length){
	assert(length<=block_size_);
	memcpy(data_,addr,length);
	free_=data_+length;
}

bool BlockStreamFix::Empty() const{
	return data_==free_;
}

bool BlockStreamFix::serialize(Block & block) const{
	assert(block.getsize()>=block_size_+2*sizeof(int));

	/*copy the content*/
	memcpy(block.getBlock(),data_,block_size_);

	/* copy the needed data for deserialization*/

	/* the number of tuples*/
	int* tuple_count=(int*)((char*)block.getBlock()+block.getsize()-sizeof(int)*2);
	*tuple_count=(free_-data_)/tuple_size_;

	/* set last block bytes*/
	int* last_block_bytes=(int*)((char*)block.getBlock()+block.getsize()-sizeof(int));
	if(Empty()){
		*last_block_bytes=0;
	}
	else{
		*last_block_bytes=1;
	}

	return true;


}

bool BlockStreamFix::deserialize(Block * block){
	assert(block->getsize()>=block_size_+2*sizeof(int));

	/* copy the content*/
	memcpy(data_,block->getBlock(),block_size_);

	/* copy the member varaible*/

	/* the number of tuples*/
	int* tuple_count=(int*)((char*)block->getBlock()+block->getsize()-sizeof(int)*2);
	free_=data_+(*tuple_count)*tuple_size_;

	return true;



}
unsigned BlockStreamFix::getSerializedBlockSize()const {
	return block_size_+2*sizeof(int);
}
unsigned BlockStreamFix::getTuplesInBlock()const{
	return (free_-data_)/tuple_size_;
}

