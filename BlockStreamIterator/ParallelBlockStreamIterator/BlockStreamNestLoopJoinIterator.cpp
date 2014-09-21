/*
 * BlockStreamNestLoopJoinIterator.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: imdb
 */

#include "BlockStreamNestLoopJoinIterator.h"
#include "BlockStreamJoinIterator.h"
#include "../../Executor/ExpanderTracker.h"
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
						std::vector<unsigned> joinIndex_left,
						std::vector<unsigned> joinIndex_right,
						std::vector<unsigned> payload_left,
						std::vector<unsigned> payload_right,
				       unsigned block_size)
		:child_left(child_left),
		 child_right(child_right),
		 input_schema_left(input_schema_left),
		 input_schema_right(input_schema_right),
		 output_schema(output_schema),
		 joinIndex_left(joinIndex_left),
		 joinIndex_right(joinIndex_right),
		 payload_left(payload_left),
		 payload_right(payload_right),

		 block_size_(block_size){

		}
bool BlockStreamNestLoopJoinIterator::open(const PartitionOffset& partition_offset)
{
//	RegisterNewThreadToAllBarriers();
//	AtomicPushFreeHtBlockStream(BlockStreamBase::createBlock(state_.input_schema_left,state_.block_size_));
//	AtomicPushFreeBlockStream(BlockStreamBase::createBlock(state_.input_schema_right,state_.block_size_));
	unsigned long long int timer;
	bool winning_thread=false;
	if(tryEntryIntoSerializedSection(0))//what's for?
	{
		ExpanderTracker::getInstance()->addNewStageEndpoint(pthread_self(),LocalStageEndPoint(stage_desc,"nest loop build",0));
		winning_thread=true;
		timer=curtick();
		unsigned output_index=0;
		for(unsigned i=0;i<state_.joinIndex_left.size();i++){
			joinIndex_left_to_output[i]=output_index;
			output_index++;
		}
		for(unsigned i=0;i<state_.payload_left.size();i++){
			payload_left_to_output[i]=output_index;
			output_index++;
		}
		for(unsigned i=0;i<state_.payload_right.size();i++){
			payload_right_to_output[i]=output_index;
			output_index++;
		}

	}
	blockbuffer=new DynamicBlockBuffer();
	state_.child_left->open(partition_offset);
	barrierArrive(0);

	BlockStreamBase* block_for_asking;
	createBlockStream(block_for_asking);

	while(state_.child_left->next(block_for_asking))
	{
		blockbuffer->atomicAppendNewBlock(block_for_asking);
		createBlockStream(block_for_asking);
	}
	block_for_asking->~Block();
	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
//		unregisterNewThreadToAllBarriers(1);
		return true;
	}
	barrierArrive(1);

	state_.child_right->open(partition_offset);
	return true;
}
bool BlockStreamNestLoopJoinIterator::next(BlockStreamBase *block)
{
	void *tuple_from_buffer_child;
	void *tuple_from_right_child;
	void *result_tuple;
	join_thread_context* jtc_buffer=new join_thread_context();

	join_thread_context* jtc_right=new join_thread_context();
	jtc_right->block_for_asking_=BlockStreamBase::createBlock(state_.input_schema_right,state_.block_size_);
	jtc_right->block_stream_iterator_=jtc_right->block_for_asking_->createIterator();

	while(state_.child_right->next(jtc_right->block_for_asking_))
	{
		while((tuple_from_right_child=jtc_right->block_stream_iterator_->currentTuple())>0)
		{
			while(jtc_buffer->block_for_asking_=blockbuffer->createIterator().nextBlock())
			{
				jtc_buffer->block_stream_iterator_=jtc_buffer->block_for_asking_->createIterator();
				while(tuple_from_buffer_child=jtc_buffer->block_stream_iterator_->currentTuple())
				{
					if((result_tuple=block->allocateTuple(state_.output_schema->getTupleMaxSize()))>0)
					{
						const unsigned copyed_bytes=state_.input_schema_left->copyTuple(tuple_from_buffer_child,result_tuple);
						state_.input_schema_right->copyTuple(tuple_from_right_child,result_tuple+copyed_bytes);
					}
					else
					{
						return true;
					}
				}
			}
			jtc_right->block_stream_iterator_->increase_cur_();
		}
		jtc_right->block_for_asking_->setEmpty();
	}
//	jtc_buffer->block_for_asking_->~Block();
//	jtc_buffer->block_stream_iterator_->~BlockStreamTraverseIterator();
//	jtc_right->block_for_asking_->~Block();
//	jtc_right->block_stream_iterator_->~BlockStreamTraverseIterator();

	if(state_.child_right->next(jtc_right->block_for_asking_)==false)
	{
		if(block->Empty()==true)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return next(block);
}
bool BlockStreamNestLoopJoinIterator::close()
{
//	initialize_expanded_status();
//	destoryAllContext();
	state_.child_left->close();
	state_.child_right->close();
	return true;
}
bool BlockStreamNestLoopJoinIterator::createBlockStream(BlockStreamBase*& target)const
{
	target=BlockStreamBase::createBlock(state_.input_schema_left,state_.block_size_);
	if(target==0)
	{
		assert(false);
		return 0;
	}
	else
	{
		return 1;
	}
}

void BlockStreamNestLoopJoinIterator::print()
{
	printf("NestLoopJoin\n");
	printf("------Join Left-------\n");
	state_.child_left->print();
	printf("------Join Right-------\n");
	state_.child_right->print();
}



