/*
 * BlockContainer.cpp
 *
 *  Created on: Sep 7, 2013
 *      Author: wangli
 */
#include <assert.h>
#include "BlockContainer.h"
#include <string.h>

#include "BlockStream.h"
BlockContainer::BlockContainer(unsigned block_size)
    : Block(block_size), actual_size_(0) {}

BlockContainer::~BlockContainer() {
  // TODO Auto-generated destructor stub
}

unsigned BlockContainer::GetMaxSize() const { return this->getsize(); }
unsigned BlockContainer::GetCurSize() const { return actual_size_; }
unsigned BlockContainer::GetRestSizeToHandle() const {
  return getsize() - actual_size_;
}
void BlockContainer::reset() { actual_size_ = 0; }
void BlockContainer::IncreaseActualSize(unsigned size) {
  assert(size + actual_size_ <= this->getsize());
  actual_size_ += size;
}
bool BlockContainer::IsEmpty() const {
  const BlockStreamFix::tail_info tail =
      *(BlockStreamFix::tail_info*)((char*)start + BlockSize -
                                    sizeof(BlockStreamFix::tail_info));
  return tail.tuple_count == 0;
}
void BlockContainer::copy(BlockContainer& block) {
  assert(this->getsize() == block.getsize());
  memcpy(this->getBlock(), block.getBlock(), block.getsize());
  actual_size_ = block.GetCurSize();
}
