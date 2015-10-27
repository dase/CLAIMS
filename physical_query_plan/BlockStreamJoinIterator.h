/*
 * BlockStreamJoinIterator.h
 *
 *  Created on: 2013-8-27
 *      Author: casa
 */

#ifndef BLOCKSTREAMJOINITERATOR_H_
#define BLOCKSTREAMJOINITERATOR_H_

#include "../Debug.h"
#include "../utility/rdtsc.h"
#include "../common/hash.h"
#include "../common/hashtable.h"
#include "../codegen/ExpressionGenerator.h"

#include <iostream>
#include <vector>
#include <map>
#include <list>

#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../physical_query_plan/physical_operator.h"
#ifdef DMALLOC
#include "dmalloc.h"
#endif
using namespace std;

class BlockStreamJoinIterator : public PhysicalOperator {
 public:
  class join_thread_context : public ThreadContext {
   public:
    ~join_thread_context();
    BlockStreamBase* l_block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator* l_block_stream_iterator_;
    BlockStreamBase* r_block_for_asking_;
    BlockStreamBase::BlockStreamTraverseIterator* r_block_stream_iterator_;
    BasicHashTable::Iterator hashtable_iterator_;
  };

  class State {
    friend class BlockStreamJoinIterator;

   public:
    State(BlockStreamIteratorBase* child_left,
          BlockStreamIteratorBase* child_right, Schema* input_schema_left,
          Schema* input_schema_right, Schema* output_schema, Schema* ht_schema,
          std::vector<unsigned> joinIndex_left,
          std::vector<unsigned> joinIndex_right,
          std::vector<unsigned> payload_left,
          std::vector<unsigned> payload_right, unsigned ht_nbuckets,
          unsigned ht_bucketsize, unsigned block_size);

    State(){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& child_left& child_right& input_schema_left& input_schema_right&
          output_schema& ht_schema& joinIndex_left& joinIndex_right&
              payload_left& payload_right& ht_nbuckets& ht_bucketsize&
                  block_size_;
    }

   public:
    // input and output
    BlockStreamIteratorBase* child_left, *child_right;
    Schema* input_schema_left, *input_schema_right;
    Schema* output_schema, *ht_schema;

    // how to join
    std::vector<unsigned> joinIndex_left;
    std::vector<unsigned> joinIndex_right;
    std::vector<unsigned> payload_left;
    std::vector<unsigned> payload_right;

    // hashtable
    unsigned ht_nbuckets;
    unsigned ht_bucketsize;

    unsigned block_size_;
  };
  BlockStreamJoinIterator(State state);
  BlockStreamJoinIterator();
  virtual ~BlockStreamJoinIterator();

  bool Open(const PartitionOffset& partition_offset = 0);
  bool Next(BlockStreamBase* block);
  bool Close();
  void Print();

 private:
  ThreadContext* CreateContext();
  static void isMatch(void* l_tuple_addr, void* r_tuple_addr, void* return_addr,
                      vector<unsigned>& l_join_index,
                      vector<unsigned>& r_join_index, Schema* l_schema,
                      Schema* r_schema, expr_func_two_tuples func);
  static void isMatchCodegen(void* l_tuple_addr, void* r_tuple_addr,
                             void* return_addr, vector<unsigned>& l_join_index,
                             vector<unsigned>& r_join_index, Schema* l_schema,
                             Schema* r_schema, expr_func_two_tuples func);
  //	static void copy_to_hashtable(void* desc, void* src, Schema* );
 private:
  State state_;
  /* joinIndex map to the output*/
  std::map<unsigned, unsigned> joinIndex_left_to_output;
  /* payload_left map to the output*/
  std::map<unsigned, unsigned> payload_left_to_output;
  /* payload_right map to the output*/
  std::map<unsigned, unsigned> payload_right_to_output;

  PartitionFunction* hash;
  BasicHashTable* hashtable;
  Schema* ht_schema;

  typedef void (*condition_filter_func)(void*, void*, void*, vector<unsigned>&,
                                        vector<unsigned>&, Schema*, Schema*,
                                        expr_func_two_tuples);
  condition_filter_func cff_;
  expr_func_two_tuples eftt_;
  llvm_memcpy memcpy_;
  llvm_memcat memcat_;

  typedef void (*void_voids_voids)(void*, void*, Schema*);
  typedef void (*void_voids_voids_voids)(void*, void*, void*, Schema*, Schema*);

  // debug
  unsigned produced_tuples;
  unsigned consumed_tuples_from_right;
  unsigned consumed_tuples_from_left;
  unsigned tuples_in_hashtable;
  unsigned water_mark;

#ifdef TIME
  unsigned long long timer;
#endif

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

#endif /* BLOCKSTREAMJOINITERATOR_H_ */
