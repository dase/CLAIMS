/*
 * BlockStreamFilter.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#include "BlockStreamFilter.h"

BlockStreamFilter::BlockStreamFilter(State state)
:state_(state),block_for_asking_iterator_(0),block_for_asking_(0){
	// TODO Auto-generated constructor stub

}

BlockStreamFilter::~BlockStreamFilter() {
	// TODO Auto-generated destructor stub
}

BlockStreamFilter::State::State(Schema* schema, BlockStreamIteratorBase* child,
		std::vector<FilterIterator::AttributeComparator> comparator_list
		,unsigned block_size)
:schema_(schema),child_(child),comparator_list_(comparator_list),block_size_(block_size){

}

bool BlockStreamFilter::open(const PartitionOffset& part_off){

	block_for_asking_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	block_for_asking_iterator_=block_for_asking_->createIterator();

	return state_.child_->open(part_off);
}

bool BlockStreamFilter::next(BlockStreamBase* block){
	void* tuple_in_child;
	void* tuple_in_block;
	while((tuple_in_child=block_for_asking_iterator_->nextTuple())){
		if((tuple_in_block=block->allocateTuple(state_.schema_->getTupleActualSize(tuple_in_child)))>0){
			state_.schema_->copyTuple(tuple_in_child,tuple_in_block);
			block_for_asking_iterator_->increase_cur_();
		}
		else{
			return true;
		}
	}
	/* when the program arrivals here, it means that the tuple in block_for_asking is exhausted, so we call the child
	 * iterator for the new data.
	 */
	block_for_asking_->setEmpty();
	if(state_.child_->next(block_for_asking_)){
		block_for_asking_iterator_->reset();
		return next(block);
	}
	else{
		/*
		 * the child iterator is also exhausted, so we return false.
		 */
		return false;
	}

}

bool BlockStreamFilter::close(){
	block_for_asking_->~BlockStreamBase();
	return true;
}
