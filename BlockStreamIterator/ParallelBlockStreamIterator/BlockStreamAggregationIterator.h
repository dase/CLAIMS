/*
 * BlockStreamAggregationIterator.h
 *
 *  Created on: 2013-9-9
 *      Author: casa
 */

#ifndef BLOCKSTREAMAGGREGATIONITERATOR_H_
#define BLOCKSTREAMAGGREGATIONITERATOR_H_

#include <vector>
#include <map>
using namespace std;

#include "../BlockStreamIteratorBase.h"
#include "../../Schema/Schema.h"
#include "../../Block/synch.h"
#include "../../hashtable.h"
#include "../../hash.h"

class BlockStreamAggregationIterator:public BlockStreamIteratorBase{
public:
	struct State{
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

	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase *block);
	bool close();

	BlockStreamBase* AtomicPopFreeHtBlockStream();
	void AtomicPushFreeHtBlockStream(BlockStreamBase* block);

public:
	BlockStreamAggregationIterator::State state_;

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
	volatile bool open_finished_;
	volatile bool open_finished_end_;
	Lock lock_;
	Barrier *barrier_;

	//hashtable traverse and in the next func
	Lock ht_cur_lock_;
	unsigned bucket_cur_;
	BasicHashTable::Iterator it_;
};

#endif /* BLOCKSTREAMAGGREGATIONITERATOR_H_ */
