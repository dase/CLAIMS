/*
 * BlockStreamFilter.h
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMFILTER_H_
#define BLOCKSTREAMFILTER_H_
#include "BlockStreamIteratorBase.h"
#include "../Schema/Schema.h"
#include "../Comparator.h"
#include "../iterator/FilterIterator.h"
#include "../Block/BlockStream.h"
class BlockStreamFilter:public BlockStreamIteratorBase {
public:
	class State{
	public:
		State(Schema* s, BlockStreamIteratorBase* child,std::vector<FilterIterator::AttributeComparator> comparator_list,unsigned block_size );
		Schema* schema_;
		std::vector<FilterIterator::AttributeComparator> comparator_list_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
	};
	BlockStreamFilter(State state);
	virtual ~BlockStreamFilter();
	bool open();
	bool next(BlockStreamBase* block);
	bool close();
private:
	State state_;

	/* The block used to asking the child for data.*/
	BlockStreamBase* block_for_asking_;

	BlockStreamBase::BlockStreamTraverseIterator* block_for_asking_iterator_;
};

#endif /* BLOCKSTREAMFILTER_H_ */
