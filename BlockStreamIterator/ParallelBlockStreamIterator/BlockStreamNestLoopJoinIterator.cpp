/*
 * BlockStreamNestLoopJoinIterator.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: imdb
 */

#include "BlockStreamNestLoopJoinIterator.h"

BlockStreamNestLoopJoinIterator::BlockStreamNestLoopJoinIterator() {
	initialize_expanded_status();
}

BlockStreamNestLoopJoinIterator::~BlockStreamNestLoopJoinIterator() {
	// TODO Auto-generated destructor stub
}
BlockStreamNestLoopJoinIterator::BlockStreamNestLoopJoinIterator(State state)
:state_(state){
	initialize_expanded_status();
}
BlockStreamNestLoopJoinIterator::State::State(BlockStreamIteratorBase *child_left,
				       BlockStreamIteratorBase *child_right,
				       Schema *input_schema_left,
				       Schema *input_schema_right,
				       Schema *output_schema,
				       unsigned block_size)
		:child_left(child_left),
		 child_right(child_right),
		 input_schema_left(input_schema_left),
		 input_schema_right(input_schema_right),
		 output_schema(output_schema),
		 block_size_(block_size){

		}
bool BlockStreamNestLoopJoinIterator::open(const PartitionOffset& partition_offset)
{
	cout<<"-------------OPEN------------"<<endl;
	state_.child_left->open(partition_offset);
	state_.child_right->open(partition_offset);
	return true;
}
bool BlockStreamNestLoopJoinIterator::next(BlockStreamBase *block)
{
	cout<<"-------------------------"<<endl;
	void *tuple_from_left_child;
	void *tuple_from_right_child;
	void *result_tuple;
	join_thread_context* jtc_left=new join_thread_context();
	jtc_left->block_for_asking_=BlockStreamBase::createBlock(state_.input_schema_left,state_.block_size_);
	jtc_left->block_stream_iterator_=jtc_left->block_for_asking_->createIterator();
	join_thread_context* jtc_right=new join_thread_context();
	jtc_right->block_for_asking_=BlockStreamBase::createBlock(state_.input_schema_right,state_.block_size_);
	jtc_right->block_stream_iterator_=jtc_right->block_for_asking_->createIterator();
	while(state_.child_left->next(jtc_left->block_for_asking_))
	{
		while((tuple_from_left_child=jtc_left->block_stream_iterator_->currentTuple())>0)
		{
			while(state_.child_right->next(jtc_right->block_for_asking_))
			{
				while((tuple_from_right_child=jtc_right->block_stream_iterator_->currentTuple())>0)
				{
					if((result_tuple=block->allocateTuple(state_.output_schema->getTupleMaxSize()))>0)
					{
						const unsigned copyed_bytes=state_.input_schema_left->copyTuple(tuple_from_left_child,result_tuple);
						state_.input_schema_right->copyTuple(tuple_from_right_child,result_tuple+copyed_bytes);
						cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;
					}
					else
					{
						return true;//?????????
					}
					jtc_right->block_stream_iterator_->increase_cur_();
				}
				jtc_right->block_for_asking_->setEmpty();
			}
			jtc_left->block_stream_iterator_->increase_cur_();
		}
		jtc_left->block_for_asking_->setEmpty();
	}
}
bool BlockStreamNestLoopJoinIterator::close()
{
//	initialize_expanded_status();
//	destoryAllContext();
	state_.child_left->close();
	state_.child_right->close();
	return true;
}
void BlockStreamNestLoopJoinIterator::print()
{
	printf("NestLoopJoin\n");
	printf("------Join Left-------\n");
	state_.child_left->print();
	printf("------Join Right-------\n");
	state_.child_right->print();
}
