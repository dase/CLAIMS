/*
 * BlockStreamCombinedIterator.cpp
 *
 *  Created on: 2013-8-27
 *      Author: casa
 */

#include "../physical_query_plan/BlockStreamCombinedIterator.h"


BlockStreamCombinedIterator::BlockStreamCombinedIterator(State state)
:state_(state){

}

BlockStreamCombinedIterator::State::State(std::vector<Schema *> inputs, Schema* output, std::vector<BlockStreamIteratorBase *> children)
:inputs_(inputs),output_(output),children_(children){

}

BlockStreamCombinedIterator::~BlockStreamCombinedIterator() {

}

bool BlockStreamCombinedIterator::Open(const PartitionOffset& partition_offset){
	//first
	std::vector<BlockStreamBase *> v_b;
	for(unsigned i=0;i<state_.children_.size();i++){
		BlockStreamBase *bsb=new BlockStreamFix(BLOCK_SIZE_CAO,4);
		v_b.push_back(bsb);
	}

	free_block_stream_list_.push_back(v_b);

	for(unsigned i=0;i<state_.children_.size();i++){
			if(!state_.children_[i]->Open(partition_offset)){
					//TODO: handle the failure
					return false;
			}
	}
	return true;
}

bool BlockStreamCombinedIterator::Next(BlockStreamBase *block){
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
					if(state_.children_[j]->Next(rb.buffer_[j])==false){
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

	return Next(block);
}

bool BlockStreamCombinedIterator::Close(){
        for(unsigned i=0;i<state_.children_.size();i++){
                state_.children_[i]->Close();
        }
        return true;
}


bool BlockStreamCombinedIterator::atomicPopRemainingBlock(remaining_block & rb){
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

void BlockStreamCombinedIterator::atomicPushRemainingBlock(remaining_block rb){
	lock_.acquire();
	remaining_block_list_.push_back(rb);
	lock_.release();
}
