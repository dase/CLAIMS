/*
 * ExpandableBlockStreamFilter.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: wangli
 */
#include <limits>
#include "ExpandableBlockStreamFilter.h"
#include "../../utility/warmup.h"
#include "../../utility/rdtsc.h"

ExpandableBlockStreamFilter::ExpandableBlockStreamFilter(State state)
:state_(state){
	initialize_expanded_status();
}

ExpandableBlockStreamFilter::ExpandableBlockStreamFilter()
{
	initialize_expanded_status();
}

ExpandableBlockStreamFilter::~ExpandableBlockStreamFilter() {
	// TODO Auto-generated destructor stub
}

ExpandableBlockStreamFilter::State::State(Schema* schema, BlockStreamIteratorBase* child,
		std::vector<AttributeComparator> comparator_list
		,unsigned block_size)
:schema_(schema),child_(child),comparator_list_(comparator_list),block_size_(block_size){

}


bool ExpandableBlockStreamFilter::open(const PartitionOffset& part_off){

	AtomicPushFreeBlockStream(BlockStreamBase::createBlock(state_.schema_,state_.block_size_));


	filter_thread_context* ftc=new filter_thread_context();
	ftc->block_for_asking_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	ftc->temp_block_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	ftc->block_stream_iterator_=ftc->block_for_asking_->createIterator();

	initContext(ftc);

	if(tryEntryIntoSerializedSection()){
		tuple_after_filter_=0;
		const bool child_open_return=state_.child_->open(part_off);
		setOpenReturnValue(child_open_return);
		broadcaseOpenFinishedSignal();
	}
	else
	{
		waitForOpenFinished();
		return state_.child_->open(part_off);
	}
}



bool ExpandableBlockStreamFilter::next(BlockStreamBase* block){


	void* tuple_from_child;
	void* tuple_in_block;
	bool pass_filter;
	filter_thread_context* tc=(filter_thread_context*)getContext();

	while((tuple_from_child=tc->block_stream_iterator_->currentTuple())>0){
		pass_filter=true;
		for(unsigned i=0;i<state_.comparator_list_.size();i++){

			if(!state_.comparator_list_[i].filter(state_.schema_->getColumnAddess(state_.comparator_list_[i].get_index(),tuple_from_child))){
				pass_filter=false;
				break;
			}
		}
		if(pass_filter){

			const unsigned bytes=state_.schema_->getTupleActualSize(tuple_from_child);
			if((tuple_in_block=block->allocateTuple(bytes))>0){
				/* the block has space to hold this tuple*/
//					state_.schema_->copyTuple(tuple_from_child,tuple_in_block);
				/* the block has space to hold this tuple,
				 * copyTuple can be used in the hashtable,
				 * but here we must use the block insert
				 * modified by zhanglei for the variable supported!*/
				block->insert(tuple_in_block,tuple_from_child,bytes);
				tuple_after_filter_++;
				tc->block_stream_iterator_->increase_cur_();
			}
			else{
				/* the block is full, before we return, we pop the remaining block.*/
				return true;
			}
		}
		else{
			tc->block_stream_iterator_->increase_cur_();
		}
	}

	/* When the program arrivals here, it means that there is no remaining block or the remaining block
	 * is exhausted, so we read a new block from the child.
	 */


	tc->block_for_asking_->setEmpty();
	tc->block_stream_iterator_->~BlockStreamTraverseIterator();



	while(state_.child_->next(tc->block_for_asking_)){
//		continue;
//		tc->temp_block_->deepCopy(tc->block_for_asking_);
//		memcpy(tc->temp_block_->getBlock(),tc->block_for_asking_->getBlock(),tc->block_for_asking_->getSerializedBlockSize());
//		((BlockStreamFix*)tc->temp_block_)->free_=(char*)((BlockStreamFix*)tc->temp_block_)->getBlock()+state_.schema_->getTupleMaxSize()*((BlockStreamFix*)tc->block_for_asking_)->getTuplesInBlock();
//		continue;
//
//		unsigned long long int warmup_tick=curtick();
//		Unit temp=warmup(tc->block_for_asking_,tc->block_for_asking_->getSerializedBlockSize());
//		printf("%ld cycles for warmup\n",curtick()-warmup_tick,temp);

//		unsigned long long int process_block=curtick();

		tc->block_stream_iterator_=tc->block_for_asking_->createIterator();
//		tc->block_stream_iterator_=tc->temp_block_->createIterator();

		/*
		 * TODO: The following lines are the same as the some lines above,
		 * so consider put them into a method.
		 */
		while((tuple_from_child=tc->block_stream_iterator_->currentTuple())>0){
			pass_filter=true;
			for(unsigned i=0;i<state_.comparator_list_.size();i++){
				if(!state_.comparator_list_[i].filter(state_.schema_->getColumnAddess(state_.comparator_list_[i].get_index(),tuple_from_child))){
					pass_filter=false;
					break;
				}
			}
//			if(*(unsigned long*)tuple_from_child!=0){
//				pass_filter=false;
//			}
			if(pass_filter){

				const unsigned bytes=state_.schema_->getTupleActualSize(tuple_from_child);
				if((tuple_in_block=block->allocateTuple(bytes))>0){
					/* the block has space to hold this tuple*/
//					state_.schema_->copyTuple(tuple_from_child,tuple_in_block);
					/* the block has space to hold this tuple,
					 * copyTuple can be used in the hashtable,
					 * but here we must use the block insert
					 * modified by zhanglei for the variable supported!*/
					block->insert(tuple_in_block,tuple_from_child,bytes);
					tuple_after_filter_++;
					tc->block_stream_iterator_->increase_cur_();
				}
				else{
					/* the block is full, before we return, we pop the remaining block.*/
					return true;
				}
			}
			else{
				tc->block_stream_iterator_->increase_cur_();
			}

		}
//		printf("block processing cycles: %ld\n",curtick()-process_block);
		/* the block_for_asking is exhausted, but the block is not full*/
		tc->block_stream_iterator_->~BlockStreamTraverseIterator();
		tc->block_for_asking_->setEmpty();
	}
	/* the child iterator is exhausted, but the block is not full.*/

	if(!block->Empty()){
		return true;
	}
	else{
//		delete tc;
		return false;
	}
}

bool ExpandableBlockStreamFilter::close(){
	initialize_expanded_status();
	open_finished_=false;

	for(unsigned i=0;i<free_block_stream_list_.size();i++){
		free_block_stream_list_.front()->~BlockStreamBase();
		free_block_stream_list_.pop_front();
	}

	destoryAllContext();

	free_block_stream_list_.clear();
	state_.child_->close();
	return true;
}
void ExpandableBlockStreamFilter::print(){
	printf("Filter size=%d\n",state_.comparator_list_.size());

	printf("---------------\n");
	state_.child_->print();
}
bool ExpandableBlockStreamFilter::atomicPopRemainingBlock(remaining_block & rb){
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

void ExpandableBlockStreamFilter::atomicPushRemainingBlock(remaining_block rb){
	lock_.acquire();
	remaining_block_list_.push_back(rb);
	lock_.release();
}

BlockStreamBase* ExpandableBlockStreamFilter::AtomicPopFreeBlockStream(){
	assert(!free_block_stream_list_.empty());
	lock_.acquire();
	BlockStreamBase *block=free_block_stream_list_.front();
	free_block_stream_list_.pop_front();
	lock_.release();
	return block;
}
void ExpandableBlockStreamFilter::AtomicPushFreeBlockStream(BlockStreamBase* block){
	lock_.acquire();
	free_block_stream_list_.push_back(block);
	lock_.release();
}
thread_context ExpandableBlockStreamFilter::popContext(){
	lock_.acquire();
	assert(context_list_.find(pthread_self())!=context_list_.cend());
	thread_context ret= context_list_[pthread_self()];
	context_list_.erase(pthread_self());
//	printf("Thread %lx is poped!\n",pthread_self());
	lock_.release();
	return ret;
}

void ExpandableBlockStreamFilter::pushContext(const thread_context& tc){
	lock_.acquire();
	assert(context_list_.find(pthread_self())==context_list_.cend());
	context_list_[pthread_self()]=tc;
//	printf("Thread %lx is pushed!\n",pthread_self());
	lock_.release();
}
//void ExpandableBlockStreamFilter::destoryContext(thread_context& tc){
//	lock_.acquire();
//	assert(context_list_.find(pthread_self())!=context_list_.cend());
//	context_list_[pthread_self()].block_for_asking_->~BlockStreamBase();
//	context_list_[pthread_self()].iterator_->~BlockStreamTraverseIterator();
//	context_list_.erase(pthread_self());
//	lock_.release();
////	tc->block_for_asking_->~BlockStreamBase();
////	tc->iterator_->~BlockStreamTraverseIterator();
//}

