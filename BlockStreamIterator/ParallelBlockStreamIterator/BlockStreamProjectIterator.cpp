/*
 * BlockStreamProjectIterator.cpp
 *
 *  Created on: 2014-2-17
 *      Author: casa
 */

#include "BlockStreamProjectIterator.h"

BlockStreamProjectIterator::BlockStreamProjectIterator() {
	// TODO 自动生成的构造函数存根

}

BlockStreamProjectIterator::~BlockStreamProjectIterator() {
	// TODO 自动生成的析构函数存根
}

BlockStreamProjectIterator::BlockStreamProjectIterator(State state)
:state_(state){

}

BlockStreamProjectIterator::State::State(Schema * input, Schema* output, BlockStreamIteratorBase * children, unsigned blocksize, Expression *expr)
:input_(input),output_(output),children_(children),block_size_(blocksize),expr_(expr){

}

bool BlockStreamProjectIterator::open(const PartitionOffset& partition_offset){
	//first
	state_.children_->open(partition_offset);
	BlockStreamBase *bsb=new BlockStreamFix(64*1024-sizeof(unsigned),state_.input_->getTupleMaxSize());
	free_block_stream_list_.push_back(bsb);
//	for(unsigned i=0;i<state_.children_.size();i++){
//			if(!state_.children_[i]->open(partition_offset)){
//					//TODO: handle the failure
//					return false;
//			}
//	}
	return true;
}

bool BlockStreamProjectIterator::next(BlockStreamBase *block){
	unsigned total_length_=0;
	total_length_+=state_.output_->getTupleMaxSize();
	void *tuple=0;
	void *column_in_combinedTuple=0;
	void *combinedTuple_=memalign(cacheline_size,state_.output_->getTupleMaxSize());;
	void *cur=0;

	remaining_block rb;
	if(atomicPopRemainingBlock(rb)){
		while(1){
			if((cur=rb.bsti_->currentTuple())==0){
				rb.bsb_->setEmpty();
				if(state_.children_->next(rb.bsb_)==false){
					if(!block->Empty()){
						atomicPushRemainingBlock(rb);
						return true;
					}
					return false;
				}
				rb.bsti_->reset();
				cur=rb.bsti_->currentTuple();
			}
			if((tuple=block->allocateTuple(total_length_))>0){
				int cur_now=state_.expr_->func()(*(int *)cur,state_.expr_->val_);
				memcpy(tuple,&cur_now,total_length_);
				/* Recently, we can use choosing the first column
				 * TODO:here we can do some mapping by using the func pointer in Expression*/
				rb.bsti_->increase_cur_();
			}
			else{
				atomicPushRemainingBlock(rb);
				return true;
			}
		}
	}


	lock_.acquire();
	BlockStreamBase * v_bsb;
	if(!free_block_stream_list_.empty()){
		v_bsb=free_block_stream_list_.front();
		free_block_stream_list_.pop_front();
	}
	else{
		return false;
	}
	lock_.release();

	v_bsb->setEmpty();
	BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator=v_bsb->createIterator();

	atomicPushRemainingBlock(remaining_block(v_bsb,traverse_iterator));

	return next(block);
}

bool BlockStreamProjectIterator::close(){
		state_.children_->close();
        return true;
}


bool BlockStreamProjectIterator::atomicPopRemainingBlock(remaining_block & rb){
	lock_.acquire();
	if(remaining_block_list_.size()>0){
		rb=remaining_block_list_.front();
		remaining_block_list_.pop_front();
		lock_.release();
		return true;
	}
	else{
		lock_.release();
		return false;
	}
}

void BlockStreamProjectIterator::atomicPushRemainingBlock(remaining_block rb){
	lock_.acquire();
	remaining_block_list_.push_back(rb);
	lock_.release();
}
