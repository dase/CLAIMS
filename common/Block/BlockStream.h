/*
 * BlockStream.h
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAM_H_
#define BLOCKSTREAM_H_
#include <string>
#include <iostream>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../../common/rename.h"
#include "Block.h"

#include "../../common/Schema/Schema.h"
using namespace std;

class BlockStreamBase : public Block {
 public:
  class BlockStreamTraverseIterator {
   public:
    explicit BlockStreamTraverseIterator(BlockStreamBase* block_stream_base)
        : block_stream_base_(block_stream_base), cur(0){};
    ~BlockStreamTraverseIterator(){};
    inline void* nextTuple() { return block_stream_base_->getTuple(cur++); }

    /* get the current tuple of the iterator without increasing cur_
     * Usually, increase_cur_() is called after.
     */
    inline void* currentTuple() const {
      return block_stream_base_->getTuple(cur);
    }

    /*
     * get certain tuple, for random access
     */
    inline void* getTuple(unsigned tuple_off) const {
      return block_stream_base_->getTuple(tuple_off);
    }

    /* This function is usually called after currentTuple()  */
    inline void increase_cur_() { cur++; }

    inline void reset() { cur = 0; }

    inline unsigned get_cur() const { return cur; }
    inline void set_cur(unsigned c) { cur = c; }

   private:
    BlockStreamBase* block_stream_base_;
    unsigned cur;
  };
  friend class BlockStreamTraverseIterator;
  BlockStreamBase(unsigned block_size) : Block(block_size){};
  BlockStreamBase(unsigned block_size, void* start_addr)
      : Block(block_size, start_addr){};

  virtual BlockStreamBase* createBlockAndDeepCopy() = 0;

  virtual ~BlockStreamBase(){};

  virtual void* allocateTuple(unsigned bytes) = 0;
  virtual void setEmpty() = 0;
  virtual bool Empty() const = 0;
  virtual bool Full() const = 0;
  virtual void* getBlockDataAddress() = 0;
  //	virtual void setBlockDataAddress(void* addr)=0;
  virtual unsigned getTuplesInBlock() const = 0;

  virtual unsigned getBlockCapacityInTuples() const = 0;

  /* copy a block in the storage layer into the BlockStream, the member
   * variables (e.g., _free) are
   * also updated according the new data.
   */
  virtual void copyBlock(void* addr, unsigned length) = 0;
  virtual bool insert(void* dest, void* src, unsigned bytes) = 0;
  /**
   * deep copy from block, including block content and member variables that
   * maintain the block status.
   * the user should guarantee that rest block and desc block are of equal
   * derived class type(e.g., both
   * are BlockStreamFix), otherwise the copy may be incomplete.
   */
  virtual void deepCopy(const Block* block) = 0;

  virtual void constructFromBlock(const Block& block) = 0;

  virtual bool switchBlock(BlockStreamBase& block) = 0;

  /* serialize the Block Stream into the Block which can be sent through the
   * network.*/
  virtual bool serialize(Block& block) const = 0;

  /* convert the Block from the network into the content of current instance*/
  virtual bool deserialize(Block* block) = 0;

  /* return the actual size to hold this stream block, including the data and
   * some auxiliary structure.*/
  virtual unsigned getSerializedBlockSize() const = 0;

  BlockStreamTraverseIterator* createIterator() {
    return new BlockStreamTraverseIterator(this);
  };
  static BlockStreamBase* createBlock(const Schema* const& schema,
                                      unsigned block_size);
  static BlockStreamBase* createBlock2(const Schema* const& schema,
                                       unsigned block_size);
  /**
   * @li:I add this function in order to end the chaos of setting the block size
   *  when initializing the BlockStream. -_-
   */
  static BlockStreamBase* createBlockWithDesirableSerilaizedSize(
      Schema* schema, unsigned block_size);

 protected:
  virtual void* getTuple(unsigned offset) const = 0;
};

class BlockStreamFix : public BlockStreamBase {
  friend class BlockStreamBase;
  struct tail_info {
    unsigned tuple_count;
  };

 public:
  BlockStreamFix(unsigned block_size, unsigned tuple_size);
  BlockStreamFix(unsigned block_size, unsigned tuple_size, void* start_addr,
                 unsigned ntuples);
  virtual ~BlockStreamFix();

 public:
  inline void* allocateTuple(unsigned bytes) {
    if (free_ + bytes <= start + BlockSize - sizeof(tail_info)) {
      void* ret = free_;
      free_ += bytes;
      return ret;
    }
    return 0;
  }
  inline void setTuplesInBlock(unsigned n) {
    free_ = start + n * tuple_size_;
    assert(free_ <= start + BlockSize - sizeof(tail_info));
  }
  void setEmpty();

  /* get [offset]-th tuple of the block */
  inline void* getTuple(unsigned offset) const {
    void* ret = start + offset * tuple_size_;
    if (ret >= free_) {
      return 0;
    }
    return ret;
  }
  bool Empty() const;
  bool Full() const {
    return free_ + tuple_size_ > start + BlockSize - sizeof(tail_info);
  }
  void* getBlockDataAddress();
  //	void setBlockDataAddress(void* addr);
  bool switchBlock(BlockStreamBase& block);
  void copyBlock(void* addr, unsigned length);
  bool insert(void* dest, void* src, unsigned bytes);
  void deepCopy(const Block* block);
  bool serialize(Block& block) const;
  bool deserialize(Block* block);
  unsigned getSerializedBlockSize() const;
  unsigned getBlockCapacityInTuples() const;
  unsigned getTuplesInBlock() const;
  /* construct the BlockStream from a storage level block,
   * which last four bytes indicate the number of tuples in the block.*/
  void constructFromBlock(const Block& block);
  BlockStreamBase* createBlockAndDeepCopy();

 protected:
  // public:
  unsigned tuple_size_;
  char* free_;  // should be protected.
};

/* only on the situation of tuple size is little than a block size*/
class BlockStreamVar : public BlockStreamBase {
  struct tail_info {
    unsigned tuple_count;
  };

 public:
  // BlockSize is 64k-4 because of the tuple_count is the member of the class
  BlockStreamVar(unsigned block_size, const Schema* const& schema);
  virtual ~BlockStreamVar(){};
  /* get the [offset]-th tuple of the block*/
  inline void* getTuple(unsigned offset) const;
  /* bytes is the length of the constructed tuple*/
  inline void* allocateTuple(unsigned bytes);
  /* for change the free_front_ and the free_end_*/
  bool insert(void* dest, void* src, unsigned bytes);
  /* turn the BlockStreamBase full of data structure to block*/
  bool serialize(Block& block) const;
  /* build the BlockStreamBase from the blcok when extract the data into
   * in-memory data structure, and this is the same as the deserialize
   * */
  void constructFromBlock(const Block& block);
  bool insert(void* dest, void* src) {
    int bytes = schema_->getTupleActualSize(src);
    memcpy(dest, src, bytes);
    int* free_end = (int*)free_end_;
    *free_end = free_front_ - start;
    free_end_ = free_end_ - sizeof(int);
    return true;
  }

  /*set the block empty*/
  void setEmpty() {
    free_front_ = start;
    free_end_ = start + BlockSize - (attributes_ + 1) * 4;
  }

  bool deserialize(Block* block){};

  /* for debug*/
  void toDisk(BlockStreamBase* bsb, string path) {
    int filed;
    filed = FileOpen(path.c_str(), O_WRONLY | O_CREAT);
    write(filed, bsb->getBlock(), 1024 * 64 - 4);
  };

  void printSchema() {
    int columns = schema_->getncolumns();
    int* schema_info =
        (int*)((char*)start + BlockSize - sizeof(int) * (columns + 1));
    for (unsigned i = 0; i < schema_->getncolumns(); i++) {
      cout << "the schema is:" << *(schema_info + i) << endl;
    }
    cout << "the tuple count is:" << *(schema_info + columns) << endl;
  }

  /* whether is empty, if empty, return true, not false */
  bool Empty() const { return free_front_ == start; };
  bool Full() const { assert(false); }
  void* getBlockDataAddress(){};
  bool switchBlock(BlockStreamBase& block){};
  void copyBlock(void* addr, unsigned length){};
  void deepCopy(const Block* block){};
  unsigned getSerializedBlockSize() const {};
  unsigned getBlockCapacityInTuples() const { assert(false); };
  unsigned getTuplesInBlock() const {};
  BlockStreamBase* createBlockAndDeepCopy();

 private:
  const Schema* schema_;
  unsigned attributes_;
  unsigned var_attributes_;
  /* free_front_ can be added for less computing*/
  char* free_front_;
  /* can be in end directly*/
  char* free_end_;
  /* how many attributes in the tuple*/
  unsigned cur_tuple_size_;
};

#endif /* BLOCKSTREAM_H_ */
