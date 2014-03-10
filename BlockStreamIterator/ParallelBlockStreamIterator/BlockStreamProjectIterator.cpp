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

BlockStreamProjectIterator::State::State(Schema * input, Schema* output, BlockStreamIteratorBase * children, unsigned blocksize, Mapping map, vector<ExpressItem_List> v_ei)
:input_(input),output_(output),children_(children),block_size_(blocksize),map_(map),v_ei_(v_ei){

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
	unsigned total_length_=state_.output_->getTupleMaxSize();
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
				for(unsigned i=0;i<state_.v_ei_.size();i++){
					ExpressionItem result;
					ExpressItem_List toCalc;
					int variable_=0;
					for(unsigned j=0;j<state_.v_ei_[i].size();j++){
						ExpressionItem ei;
						if(state_.v_ei_[i][j].type==ExpressionItem::variable_type){
							int nth=state_.map_.atomicPopExpressionMapping(i).at(variable_); //n-th column in tuple
//							int m=*(int*)state_.input_->getColumnAddess(nth,cur);
							//put the nth column of the tuple into the Expression and turn it to a const
							ei.setValue(state_.input_->getColumnAddess(nth,cur),state_.input_->getcolumn(nth).type);
							variable_++;
						}
						else if(state_.v_ei_[i][j].type==ExpressionItem::const_type){
							ei.setData(state_.v_ei_[i][j].content.data);
						}
						else{
							ei.setOperator(state_.v_ei_[i][j].getOperatorName().c_str());
						}
						ei.print_value();
						toCalc.push_back(ei);
					}
					ExpressionCalculator::calcuate(toCalc,result);
//					result.print_value();
					copyColumn(tuple,result,state_.output_->getcolumn(i).get_length());
					tuple=(char *)tuple+state_.output_->getcolumn(i).get_length();
				}
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

bool BlockStreamProjectIterator::copyColumn(void *&tuple,ExpressionItem &result,int length){
	switch(result.return_type){
		case t_int:{
			memcpy(tuple,&result.content.data.value._int,length);
			break;
		}
		case t_float:{
			memcpy(tuple,&result.content.data.value._float,length);
			break;
		}
		default:{
			cout<<"missing the operator!!!"<<endl;
			break;
		}
	}
}
