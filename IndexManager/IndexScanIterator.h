/*
 * IndexScanIterator.h
 *
 *  Created on: Mar 25, 2014
 *      Author: scdong
 */

#ifndef INDEXSCANITERATOR_H_
#define INDEXSCANITERATOR_H_
#include "../common/Schema/Schema.h"
#include "../common/ids.h"
#include "../common/Block/BlockStream.h"
#include "../physical_operator/physical_operator.h"
#include "CSBPlusTree.h"
#include "../storage/PartitionStorage.h"

using claims::physical_operator::PhysicalOperator;

class IndexScanIterator : public PhysicalOperator {
 public:
  struct remaining_block {
    remaining_block()
        : iter_result_vector(0), block_off(0), block(0), iterator(0) {
      result_set = new map<index_offset, vector<index_offset>*>;
      iter_result_map = result_set->end();
    }
    remaining_block(
        map<index_offset, vector<index_offset>*>* result_set,
        map<index_offset, vector<index_offset>*>::iterator iter_result_map,
        vector<index_offset>::iterator iter_result_vector,
        unsigned short block_off, BlockStreamBase* block,
        BlockStreamBase::BlockStreamTraverseIterator* iterator)
        : result_set(result_set),
          iter_result_map(iter_result_map),
          iter_result_vector(iter_result_vector),
          block_off(block_off),
          block(block),
          iterator(iterator) {}

    map<index_offset, vector<index_offset>*>* result_set;
    map<index_offset, vector<index_offset>*>::iterator iter_result_map;
    vector<index_offset>::iterator iter_result_vector;
    unsigned short block_off;

    BlockStreamBase* block;
    BlockStreamBase::BlockStreamTraverseIterator* iterator;
  };

  struct query_range {
    query_range() : value_low(0), value_high(0) {
      valuebytes_low.clear();
      valuebytes_high.clear();
    }
    query_range(void* value_low_, comparison comp_low_, void* value_high_,
                comparison comp_high_, column_type c_type_)
        : value_low(value_low_),
          comp_low(comp_low_),
          value_high(value_high_),
          comp_high(comp_high_),
          c_type(c_type_) {
      valuebytes_low.clear();
      valuebytes_high.clear();
    }

    void* value_low;
    comparison comp_low;
    void* value_high;
    comparison comp_high;
    column_type c_type;
    std::vector<char> valuebytes_low, valuebytes_high;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      if (valuebytes_low.empty() && valuebytes_high.empty() &&
          c_type.operate != 0) {
        for (unsigned i = 0; i < c_type.get_length(); i++) {
          valuebytes_low.push_back(*((char*)value_low + i));
          valuebytes_high.push_back(*((char*)value_high + i));
        }
      }

      ar& valuebytes_low& comp_low& valuebytes_high& comp_high& c_type;

      if (value_low == 0 || value_high == 0) {
        value_low = malloc(c_type.get_length());  // newmalloc
        value_high = malloc(c_type.get_length());  // newmalloc
        for (unsigned i = 0; i < c_type.get_length(); i++) {
          *((char*)value_low + i) = valuebytes_low[i];
          *((char*)value_high + i) = valuebytes_high[i];
        }
      }
    }
  };

  class State {
    friend class IndexScanIterator;

   public:
    State() {}
    //		State(ProjectionID projection_id, Schema* schema, unsigned long
    //index_id, void* value_low, void* value_high, unsigned block_size);
    State(ProjectionID projection_id, Schema* schema, unsigned long index_id,
          vector<query_range> query_range__, unsigned block_size);

   public:
    ProjectionID projection_id_;
    Schema* schema_;

    unsigned long index_id_;
    //		void* value_low_;
    //		void* value_high_;
    vector<query_range> query_range_;
    unsigned block_size_;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      //			ar & projection_id_ & schema_ & index_id_ &
      //value_low_ & value_high_ & block_size_;
      ar& projection_id_& schema_& index_id_& query_range_& block_size_;
    }
  };

 public:
  IndexScanIterator();
  IndexScanIterator(State state);
  virtual ~IndexScanIterator();
  bool Open(const PartitionOffset& partition_off = 0);
  bool Next(BlockStreamBase* block);
  bool Close();
  void Print() { printf("IndexScanIterator\n"); }

 private:
  State state_;
  PartitionStorage::PartitionReaderItetaor* partition_reader_iterator_;
  ChunkReaderIterator* chunk_reader_iterator_;
  std::map<ChunkID, void*> csb_index_list_;

  std::list<remaining_block> remaining_block_list_;
  std::list<BlockStreamBase*> block_stream_list_;

  Lock lock_;

  void atomicPushRemainingBlock(remaining_block rb);
  bool atomicPopRemainingBlock(remaining_block& rb);
  void AtomicPushBlockStream(BlockStreamBase* block);
  BlockStreamBase* AtomicPopBlockStream();

  bool askForNextBlock(remaining_block& rb);

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

#endif /* INDEXSCANITERATOR_H_ */
