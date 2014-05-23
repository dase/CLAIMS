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
using namespace std;

#include "../BlockStreamIteratorBase.h"
#include "../ExpandableBlockStreamIteratorBase.h"
#include "../../common/hashtable.h"
#include "../../common/hash.h"
#include "../../Debug.h"
#include "../../utility/lock.h"
#include "../../common/Schema/Schema.h"

class BlockStreamAggregationIterator:public ExpandableBlockStreamIteratorBase{
public:
        class State{
                friend class BlockStreamAggregationIterator;
        public:
                enum aggregation{sum,min,max,count};
                State(Schema *input,
                         Schema *output,
                         BlockStreamIteratorBase *child,
                         std::vector<unsigned> groupByIndex,
                         std::vector<unsigned> aggregationIndex,
                         std::vector<aggregation> aggregations,
                         unsigned nbuckets,
                         unsigned bucketsize,
                         unsigned block_size
                         );
                State(){};
                ~State(){};
                friend class boost::serialization::access;
                template<class Archive>
                void serialize(Archive & ar, const unsigned int version){
                        ar & input & output & child & groupByIndex & aggregationIndex & aggregations & nbuckets & bucketsize & block_size;
                }
        public:
		Schema *input;
		Schema *output;
         BlockStreamIteratorBase *child;
         std::vector<unsigned> groupByIndex;
         std::vector<unsigned> aggregationIndex;
         std::vector<aggregation> aggregations;
         unsigned nbuckets;
         unsigned bucketsize;
         unsigned block_size;

        };
        BlockStreamAggregationIterator(State state);
        BlockStreamAggregationIterator();
        virtual ~BlockStreamAggregationIterator();

        bool open(const PartitionOffset& partition_offset);
        bool next(BlockStreamBase *block);
        bool close();
        void print();
        BlockStreamBase* AtomicPopFreeHtBlockStream();
        void AtomicPushFreeHtBlockStream(BlockStreamBase* block);

public:
        State state_;

private:
        BasicHashTable *hashtable_;
        PartitionFunction *hash_;
        std::map<unsigned,unsigned> inputGroupByToOutput_;
        std::map<unsigned,unsigned> inputAggregationToOutput_;
        std::vector<fun> aggregationFunctions_;

        //in the open func and build the hashtable
        std::list<BlockStreamBase *> ht_free_block_stream_list_;
        semaphore sema_open_;
        semaphore sema_open_end_;
        bool open_finished_;
        bool open_finished_end_;
        Lock lock_;
        Barrier barrier_;

        //hashtable traverse and in the next func
        Lock ht_cur_lock_;
        unsigned bucket_cur_;
        BasicHashTable::Iterator it_;
//        unsigned allocated_tuples_in_hashtable;
#ifdef TIME
        unsigned long long timer;
#endif

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
            ar & boost::serialization::base_object<ExpandableBlockStreamIteratorBase>(*this) & state_;
    }
};

#endif /* BLOCKSTREAMAGGREGATIONITERATOR_H_ */
