/*
 * BlockStreamNestLoopJoinIterator.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: imdb
 */

#include "../physical_query_plan/BlockStreamNestLoopJoinIterator.h"

#include "../common/Block/BlockStream.h"
#include "../Executor/expander_tracker.h"
#include "../physical_query_plan/BlockStreamJoinIterator.h"
BlockStreamNestLoopJoinIterator::BlockStreamNestLoopJoinIterator()
    : PhysicalOperator(2, 1) {
  InitExpandedStatus();
}

BlockStreamNestLoopJoinIterator::~BlockStreamNestLoopJoinIterator() {
  // TODO Auto-generated destructor stub
}
BlockStreamNestLoopJoinIterator::BlockStreamNestLoopJoinIterator(State state)
    : state_(state), PhysicalOperator(2, 1) {
  InitExpandedStatus();
}
BlockStreamNestLoopJoinIterator::State::State(
    BlockStreamIteratorBase *child_left, BlockStreamIteratorBase *child_right,
    Schema *input_schema_left, Schema *input_schema_right,
    Schema *output_schema, unsigned block_size)
    : child_left(child_left),
      child_right(child_right),
      input_schema_left(input_schema_left),
      input_schema_right(input_schema_right),
      output_schema(output_schema),
      block_size_(block_size) {}
bool BlockStreamNestLoopJoinIterator::Open(
    const PartitionOffset &partition_offset) {
  RegisterExpandedThreadToAllBarriers();
  //	AtomicPushFreeHtBlockStream(BlockStreamBase::createBlock(state_.input_schema_left,state_.block_size_));
  //	AtomicPushFreeBlockStream(BlockStreamBase::createBlock(state_.input_schema_right,state_.block_size_));
  unsigned long long int timer;
  bool winning_thread = false;
  if (TryEntryIntoSerializedSection(0))  // the first thread of all need to do
  {
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_desc, "nest loop build", 0));
    winning_thread = true;
    timer = curtick();
    //		unsigned output_index=0;
    //		for(unsigned i=0;i<state_.joinIndex_left.size();i++){
    //			joinIndex_left_to_output[i]=output_index;
    //			output_index++;
    //		}
    //		for(unsigned i=0;i<state_.payload_left.size();i++){
    //			payload_left_to_output[i]=output_index;
    //			output_index++;
    //		}
    //		for(unsigned i=0;i<state_.payload_right.size();i++){
    //			payload_right_to_output[i]=output_index;
    //			output_index++;
    //		}
    blockbuffer = new DynamicBlockBuffer();
  }
  state_.child_left->Open(partition_offset);
  BarrierArrive(0);
  join_thread_context *jtc = new join_thread_context();
  createBlockStream(jtc->block_for_asking_);
  while (state_.child_left->Next(jtc->block_for_asking_)) {
    blockbuffer->atomicAppendNewBlock(jtc->block_for_asking_);
    createBlockStream(jtc->block_for_asking_);
  }

  delete jtc->block_for_asking_;
  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    UnregisterExpandedThreadToAllBarriers(1);
    return true;
  }
  BarrierArrive(1);  //??ERROR
                     //	join_thread_context* jtc=new join_thread_context();
  jtc->block_for_asking_ = BlockStreamBase::createBlock(
      state_.input_schema_right, state_.block_size_);
  jtc->block_stream_iterator_ = jtc->block_for_asking_->createIterator();
  InitContext(jtc);
  state_.child_right->Open(partition_offset);
  return true;
}
bool BlockStreamNestLoopJoinIterator::Next(BlockStreamBase *block) {
  void *tuple_from_buffer_child;
  void *tuple_from_right_child;
  void *result_tuple;
  BlockStreamBase *buffer_block = NULL;
  join_thread_context *jtc = (join_thread_context *)GetContext();
  while (1) {
    while ((tuple_from_right_child =
                jtc->block_stream_iterator_->currentTuple()) > 0) {
      //			for(int
      //i=0;i<state_.input_schema_right->columns.size();i++)
      //			{
      //				cout<<state_.input_schema_right->getcolumn(i).operate->toString(state_.input_schema_right->getColumnAddess(i,tuple_from_right_child))<<endl;
      //			}
      //			cout<<endl;
      jtc->buffer_iterator_ = blockbuffer->createIterator();
      while ((buffer_block = jtc->buffer_iterator_.nextBlock()) > 0) {
        jtc->buffer_stream_iterator_->~BlockStreamTraverseIterator();
        jtc->buffer_stream_iterator_ = buffer_block->createIterator();
        while ((tuple_from_buffer_child =
                    jtc->buffer_stream_iterator_->currentTuple()) > 0) {
          //					for(int
          //i=0;i<state_.input_schema_left->columns.size();i++)
          //					{
          //						cout<<state_.input_schema_left->getcolumn(i).operate->toString(state_.input_schema_left->getColumnAddess(i,tuple_from_buffer_child))<<endl;
          //					}
          //					cout<<endl;

          if ((result_tuple = block->allocateTuple(
                   state_.output_schema->getTupleMaxSize())) > 0) {
            const unsigned copyed_bytes = state_.input_schema_left->copyTuple(
                tuple_from_buffer_child, result_tuple);
            state_.input_schema_right->copyTuple(
                tuple_from_right_child, (char *)result_tuple + copyed_bytes);
          } else {
            return true;
          }
          jtc->buffer_stream_iterator_->increase_cur_();
        }
      }

      jtc->block_stream_iterator_->increase_cur_();
    }
    jtc->block_for_asking_->setEmpty();

    if (state_.child_right->Next(jtc->block_for_asking_) == false) {
      if (block->Empty() == true) {
        return false;
      } else {
        return true;
      }
    }
    jtc->block_stream_iterator_->~BlockStreamTraverseIterator();
    jtc->block_stream_iterator_ = jtc->block_for_asking_->createIterator();
  }
  return Next(block);
}
bool BlockStreamNestLoopJoinIterator::Close() {
  InitExpandedStatus();
  DestoryAllContext();
  delete blockbuffer;
  state_.child_left->Close();
  state_.child_right->Close();
  return true;
}
bool BlockStreamNestLoopJoinIterator::createBlockStream(
    BlockStreamBase *&target) const {
  target = BlockStreamBase::createBlock(state_.input_schema_left,
                                        state_.block_size_);
  if (target == 0) {
    assert(false);
    return 0;
  } else {
    return 1;
  }
}

void BlockStreamNestLoopJoinIterator::Print() {
  printf("NestLoopJoin\n");
  printf("------Join Left-------\n");
  state_.child_left->Print();
  printf("------Join Right-------\n");
  state_.child_right->Print();
}
