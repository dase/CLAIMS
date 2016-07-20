/*
 * BlockStream.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */
#include <malloc.h>
#include <assert.h>
#include <glog/logging.h>
#include <algorithm>
#include "../../configure.h"
#include "./BlockStream.h"

BlockStreamFix::BlockStreamFix(unsigned block_size, unsigned tuple_size)
    : BlockStreamBase(block_size), tuple_size_(tuple_size) {
  free_ = start;
}
BlockStreamFix::BlockStreamFix(unsigned block_size, unsigned tuple_size,
                               void* start_addr, unsigned ntuples)
    : BlockStreamBase(block_size, start_addr),
      tuple_size_(tuple_size),
      free_((char*)start_addr + tuple_size * ntuples) {
  assert(free_ - start <= BlockSize);
}

BlockStreamFix::~BlockStreamFix() {}

void BlockStreamFix::setEmpty() { free_ = start; }

BlockStreamBase* BlockStreamBase::createBlock(const Schema* const& schema,
                                              unsigned block_size) {
  if (schema->getSchemaType() == Schema::fixed) {
    return new BlockStreamFix(block_size - sizeof(BlockStreamFix::tail_info),
                              schema->getTupleMaxSize());
  } else {
    return new BlockStreamVar(block_size, schema);
  }
}

BlockStreamBase* BlockStreamBase::createBlock2(const Schema* const& schema,
                                               unsigned block_size) {
  if (schema->getSchemaType() == Schema::fixed) {
    return new BlockStreamFix(block_size - sizeof(BlockStreamFix::tail_info),
                              schema->getTupleMaxSize(), 0, 0);
  } else {
    return new BlockStreamVar(block_size, schema);
  }
}

BlockStreamBase* BlockStreamBase::createBlockWithDesirableSerilaizedSize(
    Schema* schema, unsigned block_size) {
  if (schema->getSchemaType() == Schema::fixed) {
    return new BlockStreamFix(block_size - sizeof(BlockStreamFix::tail_info),
                              schema->getTupleMaxSize());
  } else {
    /*
     * TODO: support variable-length BlockStream
     */
    return 0;
  }
}

void* BlockStreamFix::getBlockDataAddress() { return start; }

// void BlockStreamFix::setBlockDataAddress(void* addr){
//	data_=(char*)addr;
//}

bool BlockStreamFix::switchBlock(BlockStreamBase& block) {
  BlockStreamFix* blockfix = (BlockStreamFix*)&block;
  assert(blockfix->BlockSize == BlockSize);
  std::swap(blockfix->start, start);
  std::swap(blockfix->free_, free_);
  std::swap(blockfix->tuple_size_, tuple_size_);
  return true;
}

void BlockStreamFix::copyBlock(void* addr, unsigned length) {
  assert(length <= BlockSize);
  memcpy(start, addr, length);
  free_ = start + length;
}

bool BlockStreamFix::insert(void* dest, void* src, unsigned bytes) {
  memcpy(dest, src, bytes);
  return true;
}

void BlockStreamFix::deepCopy(const Block* block) {
  assert(this->BlockSize == block->getsize());
  memcpy(start, block->getBlock(), block->getsize());
  const BlockStreamFix* temp = dynamic_cast<const BlockStreamFix*>(block);
  this->tuple_size_ = temp->tuple_size_;
  this->free_ = start + tuple_size_ * temp->getTuplesInBlock();
  // [(free_ - start) / tuple_size_];
  assert(free_ - start <= BlockSize);
}

bool BlockStreamFix::Empty() const { return start == free_; }

bool BlockStreamFix::serialize(Block& block) const {
  assert(block.getsize() >= BlockSize + sizeof(tail_info));

  /*copy the content*/
  memcpy(block.getBlock(), start, BlockSize);

  /* copy the needed data for deserialization*/

  /*get tail_info*/
  tail_info* tail = (tail_info*)((char*)block.getBlock() + block.getsize() -
                                 sizeof(tail_info));
  tail->tuple_count = (free_ - start) / tuple_size_;

  if (tail->tuple_count * tuple_size_ > BlockSize) {
    printf("tuple count=%d in serialize()\n", tail->tuple_count);
    assert(false);
  }
  //  LOG(ERROR) << "||||" << tail->tuple_count;

  //	/* the number of tuples*/
  //	int*
  // tuple_count=(int*)((char*)block.getBlock()+block.getsize()-sizeof(tail_info));
  //	*tuple_count=(free_-start)/tuple_size_;
  //
  //	if(*tuple_count*tuple_size_>BlockSize){
  //		printf("tuple count=%d in serialize()\n",*tuple_count);
  //		assert(false);
  //	}

  //
  //	/* set last block bytes*/
  //	int*
  // last_block_bytes=(int*)((char*)block.getBlock()+block.getsize()-sizeof(int));
  //	if(Empty()){
  //		*last_block_bytes=0;
  //	}
  //	else{
  //		*last_block_bytes=1;
  //	}

  return true;
}

unsigned BlockStreamFix::getBlockCapacityInTuples() const {
  return (BlockSize - sizeof(tail_info)) / tuple_size_;
}

BlockStreamBase* BlockStreamFix::createBlockAndDeepCopy() {
  BlockStreamBase* ret;
  if (this->isIsReference()) {
    ret = new BlockStreamFix(this->BlockSize, this->tuple_size_,
                             this->getBlock(), this->getTuplesInBlock());
  } else {
    ret = new BlockStreamFix(this->BlockSize, this->tuple_size_);
    ret->deepCopy(this);
  }
  return ret;
}
bool BlockStreamFix::deserialize(Block* block) {
  assert(block->getsize() >= BlockSize + sizeof(tail_info));

  /* copy the content*/
  memcpy(start, block->getBlock(), BlockSize);

  /* copy the member varaible*/

  /*get tail_info*/
  const tail_info tail = *(tail_info*)((char*)block->getBlock() +
                                       block->getsize() - sizeof(tail_info));

  if (tail.tuple_count * tuple_size_ > BlockSize) {
    printf("tuple count:%d in deserialize()\n", tail.tuple_count);
    assert(false);
  }
  //	/* the number of tuples*/
  //	int*
  // tuple_count=(int*)((char*)block->getBlock()+block->getsize()-sizeof(tail_info));
  //	if(*tuple_count*tuple_size_>BlockSize){
  //		printf("tuple count:%d in deserialize()\n",*tuple_count);
  //		assert(false);
  //	}

  free_ = (char*)start + (tail.tuple_count) * tuple_size_;

  return true;
}
unsigned BlockStreamFix::getSerializedBlockSize() const {
  return BlockSize + sizeof(tail_info);
}
unsigned BlockStreamFix::getTuplesInBlock() const {
  return (free_ - start) / tuple_size_;
}
void BlockStreamFix::constructFromBlock(const Block& block) {
  /*set block size*/
  assert(BlockSize == block.getsize() - sizeof(tail_info));

  /* copy the content*/
  memcpy(start, block.getBlock(), BlockSize);

  /*get tail info*/
  tail_info* tail = (tail_info*)((char*)block.getBlock() + block.getsize() -
                                 sizeof(tail_info));
  if (tail->tuple_count * tuple_size_ > BlockSize) {
    printf(
        "tuple count=%d,tuple size=%d, BlockSize=%d in constructFromBlock()\n",
        tail->tuple_count, tuple_size_, BlockSize);
    assert(false);
  }

  /* the number of tuples*/
  //	int*
  // tuple_count=(int*)((char*)block.getBlock()+block.getsize()-sizeof(int));
  //
  //	/** a remedy for the data loading bug...
  //	 *
  //	 */
  ////	*tuple_count=BlockSize/tuple_size_;
  //
  //	if(*tuple_count*tuple_size_>BlockSize){
  //		printf("tuple count=%d,tuple size=%d, BlockSize=%d in
  // constructFromBlock()\n",*tuple_count,tuple_size_,BlockSize);
  //		assert(false);
  //	}

  free_ = (char*)start + (tail->tuple_count) * tuple_size_;
}

BlockStreamVar::BlockStreamVar(unsigned block_size, const Schema* const& schema)
    : BlockStreamBase(block_size),
      schema_(schema),
      cur_tuple_size_(0),
      var_attributes_(0) {
  attributes_ = schema->getncolumns();
  int* schema_info =
      (int*)((char*)start + block_size - sizeof(int) * attributes_);
  for (unsigned i = 0; i < attributes_; i++) {
    switch (schema->columns[i].type) {
      case t_int:
        *(schema_info + i) = 1;
        break;
      case t_float:
        *(schema_info + i) = 2;
        break;
      case t_double:
        *(schema_info + i) = 3;
        break;
      case t_u_long:
        *(schema_info + i) = 4;
        break;
      case t_string:
        *(schema_info + i) = 5;
        var_attributes_++;
        break;
      default:
        cout << "no type!" << endl;
        break;
    }
  }
  free_front_ = start;
  free_end_ = (char*)start + block_size - sizeof(unsigned) * (attributes_ + 1);
}

void* BlockStreamVar::getTuple(unsigned offset) const {
  /*compute the address of the offset[-th] tuple*/
  int start_of_tuple =
      *(int*)(start + BlockSize - (attributes_ + 1 + offset) * 4);
  // void *ret=start+start_of_tuple+var_attributes_*4;
  void* ret = start + start_of_tuple;
  cout << "free_end_-free_front_= " << free_end_ - free_front_ << endl;
  if (ret > free_front_) {
    return 0;
  } else {
    return ret;
  }
}

void* BlockStreamVar::allocateTuple(unsigned bytes) {
  if (free_front_ + bytes <= free_end_) {
    void* ret = free_front_;
    return ret;
  } else {
    cout << "the allocateTuple is not success!!!" << endl;
    return 0;
  }
}

void BlockStreamVar::constructFromBlock(const Block& block) {
  /*set block size*/
  assert(BlockSize == block.getsize() - sizeof(tail_info));

  /* copy the content*/
  memcpy(start, block.getBlock(), BlockSize);

  /*get tail info*/
  tail_info* tail = (tail_info*)((char*)block.getBlock() + block.getsize() -
                                 sizeof(tail_info));

  free_end_ = (char*)start + BlockSize - (attributes_ + tail->tuple_count) * 4;
  free_front_ = (char*)start + *(int*)free_end_;
}

bool BlockStreamVar::insert(void* dest, void* src, unsigned bytes) {
  memcpy(dest, src, bytes);
  int* free_end = (int*)free_end_;
  *free_end = free_front_ - start;
  free_front_ += bytes;
  free_end_ = free_end_ - sizeof(int);
  cur_tuple_size_++;
  return true;
}

bool BlockStreamVar::serialize(Block& block) const {
  assert(block.getsize() >= BlockSize + sizeof(tail_info));

  /*copy the content*/
  memcpy(block.getBlock(), start, BlockSize);

  /* copy the needed data for deserialization*/

  /*get tail_info*/
  tail_info* tail = (tail_info*)((char*)block.getBlock() + block.getsize() -
                                 sizeof(tail_info));
  tail->tuple_count = cur_tuple_size_;
  return true;
}

BlockStreamBase* BlockStreamVar::createBlockAndDeepCopy() { return 0; }
