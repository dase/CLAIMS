/*
 * ExpandableBlockStreamBuffer.h
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMBUFFER_H_
#define EXPANDABLEBLOCKSTREAMBUFFER_H_

#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../common/Schema/Schema.h"
#include "../common/Block/DynamicBlockBuffer.h"
#include "../Environment.h"

class ExpandableBlockStreamBuffer : public BlockStreamIteratorBase {
 public:
  struct State {
    friend class ExpandableBlockStreamBuffer;

   public:
    State(Schema* input, BlockStreamIteratorBase* child,
          const unsigned block_size,
          const PartitionOffset partitoin_offset = 0);
    State();

   private:
    Schema* input_;
    BlockStreamIteratorBase* child_;
    unsigned block_size_;
    PartitionOffset partition_offset_;

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& input_& child_& block_size_& partition_offset_;
    }
  };
  ExpandableBlockStreamBuffer();
  ExpandableBlockStreamBuffer(State);
  virtual ~ExpandableBlockStreamBuffer();
  bool Open(const PartitionOffset& part_off = 0);
  bool Next(BlockStreamBase* block);
  bool Close();

 private:
  bool createBlockStream(BlockStreamBase*&) const;
  void deallocateBlockStream(BlockStreamBase*&) const;
  bool ChildExhausted();
  static void* worker(void* arg);

 private:
  State state_;
  DynamicBlockBuffer block_buffer_;
  DynamicBlockBuffer::Iterator block_buffer_iterator_;
  unsigned finished_thread_count_;
  unsigned registered_thread_count_;
  semaphore sema_open_;

  volatile bool open_finished_;
  semaphore sema_open_finished_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<BlockStreamIteratorBase>(*this) &
        state_;
  }
};

#endif /* EXPANDABLEBLOCKSTREAMBUFFER_H_ */
