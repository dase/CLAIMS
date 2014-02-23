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

BlockStreamProjectIterator::State::State(Schema * input, Schema* output, std::vector<BlockStreamIteratorBase *> children, unsigned blocksize, Expression *expr)
:input_(input),output_(output),children_(children),block_size_(blocksize),expr_(expr){

}

bool BlockStreamProjectIterator::open(const PartitionOffset& partition_offset){
	//first
	BlockStreamBase *bsb=new BlockStreamFix(BLOCK_SIZE_CAO,4);
	free_block_stream_list_.push_back(bsb);

	for(unsigned i=0;i<state_.children_.size();i++){
			if(!state_.children_[i]->open(partition_offset)){
					//TODO: handle the failure
					return false;
			}
	}
	return true;
}

bool BlockStreamProjectIterator::next(BlockStreamBase *block){
	unsigned total_length_=0;
	for(unsigned i=0;i<state_.inputs_.size();i++){
			total_length_+=state_.inputs_[i]->getTupleMaxSize();
	}
	void *tuple=0;
	void *column_in_combinedTuple=0;
	void *combinedTuple_=memalign(cacheline_size,state_.output_->getTupleMaxSize());;
	void *cur=0;

	remaining_block rb;
	if(atomicPopRemainingBlock(rb)){
		while(1){
			for(unsigned j=0;j<state_.children_.size();j++){
				if((cur=rb.bsti_list_[j]->currentTuple())==0){
					rb.buffer_[j]->setEmpty();
					if(state_.children_[j]->next(rb.buffer_[j])==false){
						if(!block->Empty()){
							atomicPushRemainingBlock(rb);
							return true;
						}
						return false;
					}
					rb.bsti_list_[j]->reset();
					cur=rb.bsti_list_[j]->currentTuple();
				}
				column_in_combinedTuple=state_.output_->getColumnAddess(j,combinedTuple_);
				state_.output_->columns[j].operate->assign(cur,column_in_combinedTuple);
			}
			if((tuple=block->allocateTuple(total_length_))>0){
				memcpy(tuple,combinedTuple_,total_length_);
				for(unsigned k=0;k<state_.children_.size();k++){
					rb.bsti_list_[k]->increase_cur_();
				}
			}
			else{
				atomicPushRemainingBlock(rb);
				return true;
			}
		}
	}


	lock_.acquire();
	std::vector<BlockStreamBase *> v_bsb;
	if(!free_block_stream_list_.empty()){
		v_bsb=free_block_stream_list_.front();
		free_block_stream_list_.pop_front();
	}
	else{
		return false;
	}
	lock_.release();

	for(unsigned i=0;i<v_bsb.size();i++){
		v_bsb[i]->setEmpty();
		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator=v_bsb[i]->createIterator();
		rb.bsti_list_.push_back(traverse_iterator);
	}

	atomicPushRemainingBlock(remaining_block(v_bsb,rb.bsti_list_));

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
