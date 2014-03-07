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
:BlockStreamBase(block_size),tuple_size_(tuple_size){
	free_=start;
}

BlockStreamFix::~BlockStreamFix() {
}

void BlockStreamFix::setEmpty(){
	free_=start;
}


BlockStreamBase* BlockStreamBase::createBlock(Schema* schema,unsigned block_size) {
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
static BlockStreamBase* BlockStreamBase::createBlockWithDesirableSerilaizedSize(Schema* schema,unsigned block_size){
	if(schema->getSchemaType()==Schema::fixed){
		return new BlockStreamFix(block_size-sizeof(BlockStreamFix::tail_info),schema->getTupleMaxSize());
	}
	else{
		/*
		 * TODO: support variable-length BlockStream
		 */
		return 0;
	}
}

void* BlockStreamFix::getBlockDataAddress(){
	return start;
}

//void BlockStreamFix::setBlockDataAddress(void* addr){
//	data_=(char*)addr;
//}

bool BlockStreamFix::switchBlock(BlockStreamBase& block){
	BlockStreamFix* blockfix=(BlockStreamFix*)&block;
	assert(blockfix->BlockSize==BlockSize);

	/* swap the data pointer */
	char* data_temp;
	data_temp=blockfix->start;
	blockfix->start=(this->start);
	this->start=data_temp;

	/* swap the free pointer */
	char* free_temp;
	free_temp=blockfix->free_;
	blockfix->free_=this->free_;
	this->free_=free_temp;

	return true;


}

void BlockStreamFix::copyBlock(void* addr, unsigned length){
	assert(length<=BlockSize);
	memcpy(start,addr,length);
	free_=start+length;
}
void BlockStreamFix::deepCopy(const Block* block){
	assert(this->BlockSize>=block->getsize());
	memcpy(start,block->getBlock(),block->getsize());
	this->tuple_size_=((BlockStreamFix*)block)->tuple_size_;
	this->free_=start+tuple_size_*((BlockStreamFix*)block)->getTuplesInBlock();
}

bool BlockStreamFix::Empty() const{
	return start==free_;
}

bool BlockStreamFix::serialize(Block & block) const{
	assert(block.getsize()>=BlockSize+sizeof(tail_info));

	/*copy the content*/
	memcpy(block.getBlock(),start,BlockSize);

	/* copy the needed data for deserialization*/

	/*get tail_info*/
	tail_info* tail=(tail_info*)((char*)block.getBlock()+block.getsize()-sizeof(tail_info));
	tail->tuple_count=(free_-start)/tuple_size_;

	if(tail->tuple_count*tuple_size_>BlockSize){
		printf("tuple count=%d in serialize()\n",tail->tuple_count);
		assert(false);
	}


//	/* the number of tuples*/
//	int* tuple_count=(int*)((char*)block.getBlock()+block.getsize()-sizeof(tail_info));
//	*tuple_count=(free_-start)/tuple_size_;
//
//	if(*tuple_count*tuple_size_>BlockSize){
//		printf("tuple count=%d in serialize()\n",*tuple_count);
//		assert(false);
//	}

//
//	/* set last block bytes*/
//	int* last_block_bytes=(int*)((char*)block.getBlock()+block.getsize()-sizeof(int));
//	if(Empty()){
//		*last_block_bytes=0;
//	}
//	else{
//		*last_block_bytes=1;
//	}

	return true;


}

bool BlockStreamFix::deserialize(Block * block){
	assert(block->getsize()>=BlockSize+sizeof(tail_info));

	/* copy the content*/
	memcpy(start,block->getBlock(),BlockSize);

	/* copy the member varaible*/

	/*get tail_info*/
	const tail_info tail=*(tail_info*)((char*)block->getBlock()+block->getsize()-sizeof(tail_info));

	if(tail.tuple_count*tuple_size_>BlockSize){
		printf("tuple count:%d in deserialize()\n",tail.tuple_count);
		assert(false);
	}
//	/* the number of tuples*/
//	int* tuple_count=(int*)((char*)block->getBlock()+block->getsize()-sizeof(tail_info));
//	if(*tuple_count*tuple_size_>BlockSize){
//		printf("tuple count:%d in deserialize()\n",*tuple_count);
//		assert(false);
//	}


	free_=(char*)start+(tail.tuple_count)*tuple_size_;

	return true;



}
unsigned BlockStreamFix::getSerializedBlockSize()const {
	return BlockSize+sizeof(tail_info);
}
unsigned BlockStreamFix::getTuplesInBlock()const{
	return (free_-start)/tuple_size_;
}
void BlockStreamFix::constructFromBlock(const Block& block){
	/*set block size*/
	assert(BlockSize==block.getsize()-sizeof(tail_info));

	/* copy the content*/
	memcpy(start,block.getBlock(),BlockSize);

	/*get tail info*/
	tail_info* tail=(tail_info*)((char*)block.getBlock()+block.getsize()-sizeof(tail_info));
	if(tail->tuple_count*tuple_size_>BlockSize){
		printf("tuple count=%d,tuple size=%d, BlockSize=%d in constructFromBlock()\n",tail->tuple_count,tuple_size_,BlockSize);
		assert(false);
	}


	/* the number of tuples*/
//	int* tuple_count=(int*)((char*)block.getBlock()+block.getsize()-sizeof(int));
//
//	/** a remedy for the data loading bug...
//	 *
//	 */
////	*tuple_count=BlockSize/tuple_size_;
//
//	if(*tuple_count*tuple_size_>BlockSize){
//		printf("tuple count=%d,tuple size=%d, BlockSize=%d in constructFromBlock()\n",*tuple_count,tuple_size_,BlockSize);
//		assert(false);
//	}


	free_=(char*)start+(tail->tuple_count)*tuple_size_;
}

