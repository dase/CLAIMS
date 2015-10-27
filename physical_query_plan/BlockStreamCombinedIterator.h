/*
 * BlockStreamCombinedIterator.h
 *
 *  Created on: 2013-8-27
 *      Author: casa
 */

#ifndef BLOCKSTREAMCOMBINEDITERATOR_H_
#define BLOCKSTREAMCOMBINEDITERATOR_H_

#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../common/Schema/Schema.h"
#include "../configure.h"

#include <list>
#include <vector>
#include <utility>
#include <malloc.h>
#include "../../utility/lock.h"
using namespace std;

#define BLOCK_SIZE_CAO 4096

class BlockStreamCombinedIterator : public BlockStreamIteratorBase {
 public:
  struct remaining_block {
    remaining_block(
        std::vector<BlockStreamBase *> buffer,
        std::vector<BlockStreamBase::BlockStreamTraverseIterator *> bsti_list)
        : buffer_(buffer), bsti_list_(bsti_list){};
    remaining_block() : buffer_(0), bsti_list_(0){};
    //        void * combinedTuple_;
    std::vector<BlockStreamBase *> buffer_;
    std::vector<BlockStreamBase::BlockStreamTraverseIterator *> bsti_list_;
  };

  class State {
    friend class BlockStreamCombinedIterator;

   public:
    State(std::vector<Schema *> inputs, Schema *output,
          std::vector<BlockStreamIteratorBase *> children);
    State(){};

   private:
    std::vector<Schema *> inputs_;
    Schema *output_;
    std::vector<BlockStreamIteratorBase *> children_;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &inputs_ &output_ &children_;
    }
  };

  BlockStreamCombinedIterator(State state);
  BlockStreamCombinedIterator(){};
  virtual ~BlockStreamCombinedIterator();

  bool Open(const PartitionOffset &partition_offset = 0);
  bool Next(BlockStreamBase *block);
  bool Close();

 private:
  bool atomicPopRemainingBlock(remaining_block &rb);
  void atomicPushRemainingBlock(remaining_block rb);

 private:
  State state_;

  std::list<remaining_block> remaining_block_list_;
  std::list<std::vector<BlockStreamBase *> > free_block_stream_list_;

  Lock lock_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<BlockStreamIteratorBase>(*this) &
        state_;
  }
};

#endif /* BLOCKSTREAMCOMBINEDITERATOR_H_ */
