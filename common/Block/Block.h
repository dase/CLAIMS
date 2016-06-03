/*
 * Block.h
 *
 *  Created on: Jun 22, 2013
 *      Author: wangli
 */

#ifndef BLOCK_H_
#define BLOCK_H_
#include <assert.h>
#include <algorithm>

class Block {
 public:
  Block(unsigned BlockSize);
  Block(const Block& block);
  Block(const unsigned& size, const void* start);
  virtual ~Block();
  void* getBlock() const;
  void setBlock(void*);
  unsigned getsize() const;
  unsigned setsize(const unsigned& size);
  inline bool SwitchBlock(Block* block) {
    assert(block->BlockSize == BlockSize &&
           block->isReference_ == isReference_ && isReference_ == false);
    std::swap(block->start, start);
    return true;
  }
  bool isIsReference() const { return isReference_; }

  void setIsReference(bool isReference);

 protected:
  unsigned BlockSize;
  char* start;

 private:
  /* true, if it references to others, which means that the block does not need
   * to free the memory reference in destructor.
   */
  bool isReference_;
};

#endif /* BLOCK_H_ */
