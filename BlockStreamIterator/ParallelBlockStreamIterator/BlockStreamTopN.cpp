/*
 * BlockStreamTopN.cpp
 *
 *  Created on: Dec 31, 2013
 *      Author: wangli
 */

#include "BlockStreamTopN.h"
BlockStreamTopN::BlockStreamTopN()
:received_tuples_(0),block_for_asking_(0){

}
BlockStreamTopN::BlockStreamTopN(State state)
:state_(state),received_tuples_(0),block_for_asking_(0){

}

BlockStreamTopN::State::State(Schema* schema,BlockStreamIteratorBase* child,unsigned limits,unsigned block_size)
:schema_(schema),child_(child),limits_(limits),block_size_(block_size){
}
BlockStreamTopN::State::State()
:schema_(0),child_(0),limits_(0),block_size_(0){}

BlockStreamTopN::~BlockStreamTopN() {
	// TODO Auto-generated destructor stub
}

bool BlockStreamTopN::open(const PartitionOffset& par){
	block_for_asking_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	received_tuples_=0;
	return state_.child_->open(par);
}
bool BlockStreamTopN::next(BlockStreamBase* block){

	while(state_.child_->next(block_for_asking_)){
		void* tuple_from_child;
		BlockStreamBase::BlockStreamTraverseIterator* it=block_for_asking_->createIterator();
		while((tuple_from_child=it->currentTuple())){
			const unsigned tuple_size=state_.schema_->getTupleActualSize(tuple_from_child);
			void* target_tuple;
			if(received_tuples_<state_.limits_&&(target_tuple=block->allocateTuple(tuple_size))){
				state_.schema_->copyTuple(tuple_from_child,target_tuple);
				received_tuples_++;
				it->increase_cur_();
			}
			// either the limits are exhausted or the block is full
			else if (received_tuples_>=state_.limits_){// if the limits are exhausted
				it->~BlockStreamTraverseIterator();
				return !block->Empty();
			}
			else{// the block is full
				it->~BlockStreamTraverseIterator();
				return true;
			}
		}

	}
	return false;

}
bool BlockStreamTopN::close(){
	state_.child_->close();
	block_for_asking_->~BlockStreamBase();
	return true;
}
