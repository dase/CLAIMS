/*
 * BlockStreamAggregationIterator.h
 *
 * Created on: 2013-9-9
 * Author: casa
 */

#ifndef BLOCKSTREAMAGGREGATIONITERATOR_H_
#define BLOCKSTREAMAGGREGATIONITERATOR_H_

#include <vector>
#include <map>
#include "../physical_query_plan/physical_operator.h"
#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../common/hashtable.h"
#include "../common/hash.h"
#include "../common/ExpandedThreadTracker.h"
#include "../Debug.h"
#include "../utility/lock.h"
#include "../common/Schema/Schema.h"
#include "../common/Expression/queryfunc.h"
using namespace std;
class BlockStreamAggregationIterator : public PhysicalOperator {
 public:
  class State {
    friend class BlockStreamAggregationIterator;

   public:
    enum aggregation { sum, min, max, count, avg };
    enum AggNodeType { Hybrid_Agg_Global, Hybrid_Agg_Private, Not_Hybrid_Agg };
    State(Schema *input, Schema *output, Schema *hashSchema,
          BlockStreamIteratorBase *child, std::vector<unsigned> groupByIndex,
          std::vector<unsigned> aggregationIndex,
          std::vector<aggregation> aggregations, unsigned nbuckets,
          unsigned bucketsize, unsigned block_size,
          std::vector<unsigned> avgIndex, AggNodeType agg_node_type);
    State() : hashSchema(0), input(0), output(0), child(0){};
    ~State(){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &input &output &hashSchema &child &groupByIndex &aggregationIndex &
          aggregations &nbuckets &bucketsize &block_size &avgIndex &
              agg_node_type;
    }

   public:
    Schema *input;
    Schema *output;
    Schema *hashSchema;
    BlockStreamIteratorBase *child;
    std::vector<unsigned> groupByIndex;
    std::vector<unsigned> aggregationIndex;
    std::vector<aggregation> aggregations;
    unsigned nbuckets;
    unsigned bucketsize;
    unsigned block_size;
    std::vector<unsigned> avgIndex;
    AggNodeType agg_node_type;
  };
  BlockStreamAggregationIterator(State state);
  BlockStreamAggregationIterator();
  virtual ~BlockStreamAggregationIterator();

  bool Open(const PartitionOffset &partition_offset);
  bool Next(BlockStreamBase *block);
  bool Close();
  void Print();

 private:
  /* prepare all sorts of indices to facilitate aggregate*/
  void prepareIndex();

  /* prepare the aggregation functions */
  void prepareAggregateFunctions();

 public:
  State state_;

 private:
  BasicHashTable *hashtable_;
  PartitionFunction *hash_;
  std::map<unsigned, unsigned> inputGroupByToOutput_;
  std::map<unsigned, unsigned> inputAggregationToOutput_;
  std::vector<fun> globalAggregationFunctions_;
  std::vector<fun> privateAggregationFunctions_;

  // hashtable traverse and in the next func
  Lock ht_cur_lock_;
  unsigned bucket_cur_;
  BasicHashTable::Iterator it_;

  PerformanceInfo *perf_info_;

//        unsigned allocated_tuples_in_hashtable;
#ifdef TIME
  unsigned long long timer;
#endif

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &boost::serialization::base_object<PhysicalOperator>(*this) & state_;
  }
};

#endif /* BLOCKSTREAMAGGREGATIONITERATOR_H_ */
