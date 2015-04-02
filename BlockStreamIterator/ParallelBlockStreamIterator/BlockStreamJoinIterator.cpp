/*
 * BlockStreamJoinIterator.cpp
 *
 *  Created on: 2013-8-27
 *      Author: casa
 */

#include "BlockStreamJoinIterator.h"
#include "../../Executor/ExpanderTracker.h"
#include "../../codegen/ExpressionGenerator.h"
#include "../../Config.h"
#include "../../utility/rdtsc.h"

//#define _DEBUG_

BlockStreamJoinIterator::BlockStreamJoinIterator(State state)
:state_(state),hash(0),hashtable(0),reached_end(0),ExpandableBlockStreamIteratorBase(barrier_number(2),serialized_section_number(1)),eftt_(0),memcpy_(0),memcat_(0){
//	sema_open_.set_value(1);
	initialize_expanded_status();
}

BlockStreamJoinIterator::BlockStreamJoinIterator()
:hash(0),hashtable(0),reached_end(0),ExpandableBlockStreamIteratorBase(barrier_number(2),serialized_section_number(1)),eftt_(0),memcpy_(0),memcat_(0){
//	sema_open_.set_value(1);
	initialize_expanded_status();
}

BlockStreamJoinIterator::~BlockStreamJoinIterator() {
	
}

BlockStreamJoinIterator::State::State(BlockStreamIteratorBase *child_left,
				       BlockStreamIteratorBase *child_right,
				       Schema *input_schema_left,
				       Schema *input_schema_right,
				       Schema *output_schema,
				       Schema *ht_schema,
				       std::vector<unsigned> joinIndex_left,
				       std::vector<unsigned> joinIndex_right,
				       std::vector<unsigned> payload_left,
				       std::vector<unsigned> payload_right,
				       unsigned ht_nbuckets,
				       unsigned ht_bucketsize,
				       unsigned block_size)
		:child_left(child_left),
		 child_right(child_right),
		 input_schema_left(input_schema_left),
		 input_schema_right(input_schema_right),
		 output_schema(output_schema),
		 ht_schema(ht_schema),
		 joinIndex_left(joinIndex_left),
		 joinIndex_right(joinIndex_right),
		 payload_left(payload_left),
		 payload_right(payload_right),
		 ht_nbuckets(ht_nbuckets),
		 ht_bucketsize(ht_bucketsize),
		 block_size_(block_size){

		}

bool BlockStreamJoinIterator::open(const PartitionOffset& partition_offset){
#ifdef TIME
	startTimer(&timer);
#endif

	RegisterExpandedThreadToAllBarriers();

	AtomicPushFreeHtBlockStream(BlockStreamBase::createBlock(state_.input_schema_left,state_.block_size_));
	AtomicPushFreeBlockStream(BlockStreamBase::createBlock(state_.input_schema_right,state_.block_size_));

	unsigned long long int timer;
	bool winning_thread=false;
	if(tryEntryIntoSerializedSection(0)){
		ExpanderTracker::getInstance()->addNewStageEndpoint(pthread_self(),LocalStageEndPoint(stage_desc,"Hash join build",0));
		winning_thread=true;
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

		hash=PartitionFunctionFactory::createBoostHashFunction(state_.ht_nbuckets);
//		PartitionFunction* hash_test=PartitionFunctionFactory::createBoostHashFunction(4);
		unsigned long long hash_table_build=curtick();
		hashtable=new BasicHashTable(state_.ht_nbuckets,state_.ht_bucketsize,state_.input_schema_left->getTupleMaxSize());
//		printf("Hash table construction time:%4.4f\n",getSecond(hash_table_build));
#ifdef	_DEBUG_
		consumed_tuples_from_left=0;
#endif
		QNode* expr = createEqualJoinExpression(state_.ht_schema,state_.input_schema_right,state_.joinIndex_left,state_.joinIndex_right);
		ticks start=curtick();
		if(Config::enable_codegen){
			eftt_=getExprFuncTwoTuples(expr, state_.ht_schema,state_.input_schema_right);
			memcpy_=getMemcpy(state_.ht_schema->getTupleMaxSize());
			memcat_=getMemcat(state_.ht_schema->getTupleMaxSize(),state_.input_schema_right->getTupleMaxSize());
		}
		if(eftt_){
			cff_=BlockStreamJoinIterator::isMatchCodegen;
			printf("Codegen(Join) succeed(%4.3fms)!\n",getMilliSecond(start));
		}
		else{
			cff_=BlockStreamJoinIterator::isMatch;
			printf("Codegen(Join) failed!\n");
		}
		delete expr;
		timer=curtick();
	}


	/**
	 * For performance concern, the following line should place just after "RegisterNewThreadToAllBarriers();"
	 * in order to accelerate the open response time.
	 */
	state_.child_left->open(partition_offset);
	barrierArrive(0);
	BasicHashTable::Iterator tmp_it=hashtable->CreateIterator();

	void *cur;
	void *tuple_in_hashtable;
	unsigned bn;

	void *key_in_input;
	void *key_in_hashtable;
	void *value_in_input;
	void *value_in_hashtable;

//	initContext(state_.input_schema_left,state_.block_size_);

	join_thread_context* jtc=new join_thread_context();
	jtc->block_for_asking_=BlockStreamBase::createBlock(state_.input_schema_left,state_.block_size_);
	jtc->block_stream_iterator_=jtc->block_for_asking_->createIterator();
//	thread_context& ct=getContext();
//	BlockStreamBase *bsb=AtomicPopFreeHtBlockStream();

	const Schema* input_schema=state_.input_schema_left->duplicateSchema();
	const Operate* op=input_schema->getcolumn(state_.joinIndex_left[0]).operate->duplicateOperator();
	const unsigned buckets=state_.ht_nbuckets;
//	consumed_tuples_from_left=0;
//	Operate* op=state_.input_schema_left->getcolumn(state_.joinIndex_left[0]).operate->duplicateOperator();

	unsigned long long int start=curtick();
	unsigned long long int processed_tuple_count=0;

	unsigned long wunai=1501764;
//	while(wunai--)
//		hashtable->atomicAllocate(0);




	while(state_.child_left->next(jtc->block_for_asking_)){
//		continue;
//		BlockStreamBase::BlockStreamTraverseIterator *bsti=bsb->createIterator();
		jtc->block_stream_iterator_->~BlockStreamTraverseIterator();
		jtc->block_stream_iterator_=jtc->block_for_asking_->createIterator();

//		bsti->reset();
		while(cur=jtc->block_stream_iterator_->nextTuple()){
//			hashtable->atomicAllocate(0);
//			continue;
#ifdef	_DEBUG_
			processed_tuple_count++;
			lock_.acquire();
			consumed_tuples_from_left++;
			lock_.release();
#endif
//			continue;
			const void* key_addr=input_schema->getColumnAddess(state_.joinIndex_left[0],cur);
			bn=op->getPartitionValue(key_addr,buckets);
//			printf("%d\t",bn);
//			if(bn!=0){
//				continue;
//			}
//			continue;
			tuple_in_hashtable=hashtable->atomicAllocate(bn);
//			tuple_in_hashtable=hashtable->allocate(bn);
			/* copy join index columns*/
//			for(unsigned i=0;i<state_.joinIndex_left.size();i++){
//				key_in_input=state_.input_schema_left->getColumnAddess(state_.joinIndex_left[i],cur);
//				key_in_hashtable=state_.ht_schema->getColumnAddess(joinIndex_left_to_output[i],tuple_in_hashtable);
//				state_.input_schema_left->getcolumn(state_.joinIndex_left[i]).operate->assignment(key_in_input,key_in_hashtable);
//
//			}
//			/* copy left payload columns*/
//			for(unsigned i=0;i<state_.payload_left.size();i++){
//				value_in_input=state_.input_schema_left->getColumnAddess(state_.payload_left[i],cur);
//				value_in_hashtable=state_.ht_schema->getColumnAddess(payload_left_to_output[i],tuple_in_hashtable);
//				state_.input_schema_left->getcolumn(state_.payload_left[i]).operate->assignment(value_in_input,value_in_hashtable);
//			}
			if(memcpy_)
				memcpy_(tuple_in_hashtable,cur);
			else
				input_schema->copyTuple(cur,tuple_in_hashtable);

//			lock_.release();
		}
//		bsb->setEmpty();
		jtc->block_for_asking_->setEmpty();
//		bsti->~BlockStreamTraverseIterator();
	}

//	BasicHashTable::Iterator it=hashtable->CreateIterator();//disable

//	printf("%d cycles per tuple!\n",(curtick()-start)/processed_tuple_count);


	unsigned tmp=0;
#ifdef	_DEBUG_
	tuples_in_hashtable=0;

	produced_tuples=0;
	consumed_tuples_from_right=0;
#endif
	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
		unregisterExpandedThreadToAllBarriers(1);
//		printf("<<<<<<<<<<<<<<<<<Join open detected call back signal!>>>>>>>>>>>>>>>>>\n");
		return true;
	}

	barrierArrive(1);
	if(winning_thread){
//		hashtable->report_status();
//		hashtable->~BasicHashTable();
		printf("Hash Table Build time: %4.4f\n",getMilliSecond(timer));
	}
	/* destory the context for the left child*/
	jtc->block_for_asking_->~Block();
	jtc->block_stream_iterator_->~BlockStreamTraverseIterator();


//	hashtable->report_status();

//	destorySelfContext();
	/* create and initialized the context for the right child*/

	jtc->block_for_asking_=BlockStreamBase::createBlock(state_.input_schema_right,state_.block_size_);
	jtc->block_stream_iterator_=jtc->block_for_asking_->createIterator();
	initContext(jtc);

//	printf("join open consume %d tuples\n",consumed_tuples_from_left);

	state_.child_right->open(partition_offset);
	return true;
}

bool BlockStreamJoinIterator::next(BlockStreamBase *block){
//	return false;
	unsigned bn;
	void *result_tuple;
	void *tuple_from_right_child;
	void *tuple_in_hashtable;
	void *key_in_input;
	void *key_in_hashtable;
	void *column_in_joinedTuple;
	void *joinedTuple=memalign(cacheline_size,state_.output_schema->getTupleMaxSize());
	bool key_exit;

//	remaining_block rb;
	join_thread_context* jtc=(join_thread_context*)getContext();

	while(true){
			while((tuple_from_right_child=jtc->block_stream_iterator_->currentTuple())>0)
			{
				unsigned bn=state_.input_schema_right->getcolumn(state_.joinIndex_right[0]).operate->getPartitionValue(state_.input_schema_right->getColumnAddess(state_.joinIndex_right[0],tuple_from_right_child),state_.ht_nbuckets);

//				jtc->block_stream_iterator_->increase_cur_();
//				continue;

				while((tuple_in_hashtable=jtc->hashtable_iterator_.readCurrent())>0)
				{
					cff_(tuple_in_hashtable,tuple_from_right_child,&key_exit,state_.joinIndex_left,state_.joinIndex_right,state_.ht_schema,state_.input_schema_right,eftt_);
					if(key_exit){
						if((result_tuple=block->allocateTuple(state_.output_schema->getTupleMaxSize()))>0){
							produced_tuples++;
							if(memcat_)
								memcat_(result_tuple,tuple_in_hashtable,tuple_from_right_child);
							else{
								const unsigned copyed_bytes=state_.input_schema_left->copyTuple(tuple_in_hashtable,result_tuple);
								state_.input_schema_right->copyTuple(tuple_from_right_child,result_tuple+copyed_bytes);
							}
						}
						else{
							free(joinedTuple);
							return true;
						}
					}
					jtc->hashtable_iterator_.increase_cur_();
				}
				jtc->block_stream_iterator_->increase_cur_();
#ifdef	_DEBUG_
				consumed_tuples_from_right++;
#endif
				if((tuple_from_right_child=jtc->block_stream_iterator_->currentTuple())){
					bn=state_.input_schema_right->getcolumn(state_.joinIndex_right[0]).operate->getPartitionValue(state_.input_schema_right->getColumnAddess(state_.joinIndex_right[0],tuple_from_right_child),state_.ht_nbuckets);
					hashtable->placeIterator(jtc->hashtable_iterator_,bn);
				}
			}
		jtc->block_for_asking_->setEmpty();

		jtc->hashtable_iterator_=hashtable->CreateIterator();
		if(state_.child_right->next(jtc->block_for_asking_)==false){
			if(block->Empty()==true){
				free(joinedTuple);
				return false;
			}
			else{
				free(joinedTuple);
				return true;
			}
		}
		delete jtc->block_stream_iterator_;
		jtc->block_stream_iterator_=jtc->block_for_asking_->createIterator();
		if((tuple_from_right_child=jtc->block_stream_iterator_->currentTuple())){
			bn=state_.input_schema_right->getcolumn(state_.joinIndex_right[0]).operate->getPartitionValue(state_.input_schema_right->getColumnAddess(state_.joinIndex_right[0],tuple_from_right_child),state_.ht_nbuckets);
			hashtable->placeIterator(jtc->hashtable_iterator_,bn);
		}
	}
	return next(block);
}

bool BlockStreamJoinIterator::close(){
//	cout<<"join finished!"<<endl;

#ifdef TIME
	stopTimer(&timer);
	printf("time consuming: %lld, %f\n",timer,timer/(double)CPU_FRE);
#endif
//	sema_open_.post();
	BlockStreamJoinLogging::log("Consumes %ld tuples from left child!");
	initialize_expanded_status();
	destoryAllContext();
	open_finished_=false;
//	barrier_.setEmpty();
	free_block_stream_list_.clear();
	ht_free_block_stream_list_.clear();
	remaining_block_list_.clear();
//	hash->~PartitionFunction();
	hashtable->~BasicHashTable();
//	ht_schema->~Schema();
	state_.child_left->close();
	state_.child_right->close();
	return true;
}
void BlockStreamJoinIterator::print(){
	printf("Join: buckets:%d\n",state_.ht_nbuckets);
	printf("------Join Left-------\n");
	state_.child_left->print();
	printf("------Join Right-------\n");

	state_.child_right->print();

}
bool BlockStreamJoinIterator::atomicPopRemainingBlock(remaining_block & rb){
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

void BlockStreamJoinIterator::atomicPushRemainingBlock(remaining_block rb){
	lock_.acquire();
	remaining_block_list_.push_back(rb);
	lock_.release();
}

BlockStreamBase* BlockStreamJoinIterator::AtomicPopFreeBlockStream(){
	assert(!free_block_stream_list_.empty());
	lock_.acquire();
	BlockStreamBase *block=free_block_stream_list_.front();
	free_block_stream_list_.pop_front();
	lock_.release();
	return block;
}

void BlockStreamJoinIterator::AtomicPushFreeBlockStream(BlockStreamBase* block){
	lock_.acquire();
	free_block_stream_list_.push_back(block);
	lock_.release();
}

BlockStreamBase* BlockStreamJoinIterator::AtomicPopFreeHtBlockStream(){
	assert(!ht_free_block_stream_list_.empty());
	lock_.acquire();
	BlockStreamBase *block=ht_free_block_stream_list_.front();
	ht_free_block_stream_list_.pop_front();
	lock_.release();
	return block;
}

void BlockStreamJoinIterator::AtomicPushFreeHtBlockStream(BlockStreamBase* block){
	lock_.acquire();
	ht_free_block_stream_list_.push_back(block);
	lock_.release();
}

inline void BlockStreamJoinIterator::isMatch(void* l_tuple_addr,
		void* r_tuple_addr, void* return_addr,vector<int>& l_join_index, vector<int>& r_join_index, Schema* l_schema, Schema* r_schema,expr_func_two_tuples func) {
	bool key_exit=true;
	for(unsigned i=0;i<r_join_index.size();i++){
		void* key_in_input=r_schema->getColumnAddess(r_join_index[i],r_tuple_addr);
		void* key_in_hashtable=l_schema->getColumnAddess(l_join_index[i],l_tuple_addr);
		if(!r_schema->getcolumn(r_join_index[i]).operate->equal(key_in_input,key_in_hashtable)){
			key_exit=false;
			break;
		}
	}
	*(bool*)return_addr=key_exit;
}

inline void BlockStreamJoinIterator::isMatchCodegen(void* l_tuple_addr,
		void* r_tuple_addr, void* return_addr, vector<int>& l_join_index,
		vector<int>& r_join_index, Schema* l_schema, Schema* r_schema, expr_func_two_tuples func) {
	func(l_tuple_addr,r_tuple_addr,return_addr);
}
