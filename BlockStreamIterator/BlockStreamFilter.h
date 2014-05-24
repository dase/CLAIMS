/*
 * BlockStreamFilter.h
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMFILTER_H_
#define BLOCKSTREAMFILTER_H_
#include "BlockStreamIteratorBase.h"
#include "../common/Schema/Schema.h"
#include "../common/Comparator.h"
#include "../common/Block/BlockStream.h"
#include "../common/AttributeComparator.h"
class BlockStreamFilter:public BlockStreamIteratorBase {
public:
	class State{
	public:
		State(Schema* s, BlockStreamIteratorBase* child,std::vector<AttributeComparator> comparator_list,unsigned block_size );
		Schema* schema_;
		std::vector<AttributeComparator> comparator_list_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
	};
	BlockStreamFilter(State state);
	virtual ~BlockStreamFilter();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase* block);
	bool close();
private:
	State state_;

	/* The block used to asking the child for data.*/
	BlockStreamBase* block_for_asking_;

	BlockStreamBase::BlockStreamTraverseIterator* block_for_asking_iterator_;
};

#endif /* BLOCKSTREAMFILTER_H_ */
