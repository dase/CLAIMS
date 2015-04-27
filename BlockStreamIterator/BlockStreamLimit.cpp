/*
 * BlockStreamTopN.cpp
 *
 *  Created on: Dec 31, 2013
 *      Author: wangli
 */

#include "BlockStreamLimit.h"
BlockStreamLimit::BlockStreamLimit()
:received_tuples_(0),block_for_asking_(0){

}
BlockStreamLimit::BlockStreamLimit(State state)
:state_(state),received_tuples_(0),block_for_asking_(0){

}

BlockStreamLimit::State::State(Schema* schema,BlockStreamIteratorBase* child,unsigned long limits,unsigned block_size,unsigned long start_position)
:schema_(schema),child_(child),limits_(limits),block_size_(block_size),start_position_(start_position){
}
BlockStreamLimit::State::State()
:schema_(0),child_(0),limits_(0),block_size_(0){}

BlockStreamLimit::~BlockStreamLimit() {
	// TODO Auto-generated destructor stub
}

bool BlockStreamLimit::open(const PartitionOffset& par){
	tuple_cur_=0;
	block_for_asking_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	received_tuples_=0;
	return state_.child_->open(par);
}
bool BlockStreamLimit::next(BlockStreamBase* block){

	while(state_.child_->next(block_for_asking_)){
		void* tuple_from_child;
		BlockStreamBase::BlockStreamTraverseIterator* it=block_for_asking_->createIterator();
		while((tuple_from_child=it->currentTuple())){
			if(!limitExhausted()){
				if(!shouldSkip()){
					const unsigned tuple_size=state_.schema_->getTupleActualSize(tuple_from_child);
					void* target_tuple;
					if((target_tuple=block->allocateTuple(tuple_size))>0){
						state_.schema_->copyTuple(tuple_from_child,target_tuple);
						received_tuples_++;
						tuple_cur_++;
						it->increase_cur_();
					}
					else{
						it->~BlockStreamTraverseIterator();
						return true;
					}
				}
				else{
					tuple_cur_++;
					it->increase_cur_();
					continue;
				}
//
//				const unsigned tuple_size=state_.schema_->getTupleActualSize(tuple_from_child);
//				void* target_tuple;
//				if(received_tuples_<state_.limits_&&(target_tuple=block->allocateTuple(tuple_size))){
//					state_.schema_->copyTuple(tuple_from_child,target_tuple);
//					received_tuples_++;
//					it->increase_cur_();
//				}
//				// either the limits are exhausted or the block is full
//				else if (received_tuples_>=state_.limits_){// if the limits are exhausted
//					it->~BlockStreamTraverseIterator();
//					return !block->Empty();
//				}
//				else{// the block is full
//					it->~BlockStreamTraverseIterator();
//					return true;
//				}
			}
			else{
				/**
				 * if the limit has already been exhausted, the current loop breaks
				 * to fetch the next block from child iterator.
				 * TODO: ideally, fetching blocks from child iterator in cases that
				 * the limit is exhausted is not necessary. However, in the current
				 * implementation, the child iterator sub-tree leaded by exchange
				 * lower iterator cannot be closed if not all the blocks are called.
				 */
				break; //to consume the next block;
			}
		}

	}
	return !block->Empty();

}
bool BlockStreamLimit::close(){
	state_.child_->close();
	block_for_asking_->~BlockStreamBase();
	return true;
}

void BlockStreamLimit::print() {
	printf("Limit %ld\n",state_.limits_);
	state_.child_->print();
}
