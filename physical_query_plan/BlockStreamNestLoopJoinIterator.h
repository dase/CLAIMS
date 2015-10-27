/*
 * BlockStreamNestLoopJoinIterator.h
 *
 *  Created on: Jul 14, 2014
 *      Author: imdb
 */

#ifndef BLOCKSTREAMNESTLOOPJOINITERATOR_H_
#define BLOCKSTREAMNESTLOOPJOINITERATOR_H_
#include <boost/serialization/base_object.hpp>
#include "../Debug.h"
#include "../Executor/expander_tracker.h"
#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../physical_query_plan/BlockStreamJoinIterator.h"
#include "../physical_query_plan/physical_operator.h"
class BlockStreamNestLoopJoinIterator : public PhysicalOperator {
 public:
  class join_thread_context : public ThreadContext {
   public:
    BlockStreamBase *block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator *block_stream_iterator_;
    DynamicBlockBuffer::Iterator buffer_iterator_;
    BlockStreamBase::BlockStreamTraverseIterator *buffer_stream_iterator_;
  };
  struct remaining_block {
    remaining_block(BlockStreamBase *bsb_right,
                    BlockStreamBase::BlockStreamTraverseIterator *bsti)
        : bsb_right_(bsb_right), blockstream_iterator(bsti){};
    remaining_block() : bsb_right_(0), blockstream_iterator(0){};
    remaining_block(const remaining_block &r) {
      bsb_right_ = r.bsb_right_;
      blockstream_iterator = r.blockstream_iterator;
      buffer_iterator_ = r.buffer_iterator_;
      buffer_stream_iterator_ = r.buffer_stream_iterator_;
    }
    BlockStreamBase *bsb_right_;
    BlockStreamBase::BlockStreamTraverseIterator *blockstream_iterator;
    DynamicBlockBuffer::Iterator *buffer_iterator_;
    BlockStreamBase::BlockStreamTraverseIterator *buffer_stream_iterator_;
  };

  class State {
    friend class BlockStreamNestLoopJoinIterator;

   public:
    State(BlockStreamIteratorBase *child_left,
          BlockStreamIteratorBase *child_right, Schema *input_schema_left,
          Schema *input_schema_right, Schema *output_schema,
          unsigned block_size);
    State(){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &child_left &child_right &input_schema_left &input_schema_right &
          output_schema &block_size_;
    }

   public:
    BlockStreamIteratorBase *child_left, *child_right;
    Schema *input_schema_left, *input_schema_right;
    Schema *output_schema;
    unsigned block_size_;
  };
  BlockStreamNestLoopJoinIterator();
  virtual ~BlockStreamNestLoopJoinIterator();
  BlockStreamNestLoopJoinIterator(State state);
  bool Open(const PartitionOffset &partition_offset = 0);
  bool Next(BlockStreamBase *block);
  bool Close();
  void Print();

 private:
  bool createBlockStream(BlockStreamBase *&) const;
  bool atomicPopRemainingBlock(remaining_block &rb);
  void atomicPushRemainingBlock(remaining_block rb);
  BlockStreamBase *AtomicPopFreeBlockStream();
  void AtomicPushFreeBlockStream(BlockStreamBase *block);
  BlockStreamBase *AtomicPopFreeHtBlockStream();
  void AtomicPushFreeHtBlockStream(BlockStreamBase *block);

  DynamicBlockBuffer *blockbuffer;
  std::map<unsigned, unsigned> joinIndex_left_to_output;
  /* payload_left map to the output*/
  std::map<unsigned, unsigned> payload_left_to_output;
  /* payload_right map to the output*/
  std::map<unsigned, unsigned> payload_right_to_output;

  State state_;
  Lock lock_;
  unsigned produced_tuples;
  unsigned consumed_tuples_from_right;
  unsigned consumed_tuples_from_left;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

#endif /* BLOCKSTREAMNESTLOOPJOINITERATOR_H_ */
