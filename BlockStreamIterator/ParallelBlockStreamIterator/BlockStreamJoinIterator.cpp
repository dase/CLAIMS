/*
 * BlockStreamJoinIterator.cpp
 *
 *  Created on: 2013-8-27
 *      Author: casa
 */

#include "BlockStreamJoinIterator.h"

BlockStreamJoinIterator::BlockStreamJoinIterator(State state)
:state_(state),hash(0),hashtable(0),open_finished_(false),reached_end(0){
	sema_open_.set_value(1);
	barrier_=new Barrier(3);
}

BlockStreamJoinIterator::BlockStreamJoinIterator()
:hash(0),hashtable(0),open_finished_(false),reached_end(0){
	sema_open_.set_value(1);
	barrier_=new Barrier(3);
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
	state_.child_left->open(partition_offset);
	AtomicPushFreeHtBlockStream(BlockStreamBase::createBlock(state_.input_schema_left,state_.block_size_));
	AtomicPushFreeBlockStream(BlockStreamBase::createBlock(state_.input_schema_right,state_.block_size_));
	cout<<"AtomicPushFreeBlockStream\n\n"<<endl;
	cout<<"join open begin"<<endl;
	if(sema_open_.try_wait()){
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
		/* Currently, the block is 4096, and the table in build phase is left one*/
		hash=PartitionFunctionFactory::createBoostHashFunction(state_.ht_nbuckets);
		hashtable=new BasicHashTable(state_.ht_nbuckets,state_.ht_bucketsize,state_.input_schema_left->getTupleMaxSize());
		cout<<"in the open master "<<endl;
		open_finished_=true;
	}else{
		while (!open_finished_) {
			usleep(1);
		}
	}

	//hashtable createIterator的好处就是创建的都是可读的对象，不需要加锁
//	lock_.acquire();
	BasicHashTable::Iterator tmp_it=hashtable->CreateIterator();
//	lock_.release();
	void *cur;
	void *tuple_in_hashtable;
	unsigned bn;

	void *key_in_input;
	void *key_in_hashtable;
	void *value_in_input;
	void *value_in_hashtable;
	BlockStreamBase *bsb=AtomicPopFreeHtBlockStream();
	PartitionFunction* hash_test=PartitionFunctionFactory::createBoostHashFunction(4);
	cout<<"in the hashtable build stage!"<<endl;
//	consumed_tuples_from_left=0;
	while(state_.child_left->next(bsb)){
		BlockStreamBase::BlockStreamTraverseIterator *bsti=bsb->createIterator();

		bsti->reset();
		while(cur=bsti->nextTuple()){
			consumed_tuples_from_left++;
//
//			if(state_.ht_schema->getncolumns()>20)
//			state_.ht_schema->displayTuple(cur,"|B|"); ///for debug
			/* Currently, the join index is [0]-th column, so the hash table is based on the hash value of [0]-th column*/
//			bn=hash->get_partition_value(*(unsigned long*)(state_.input_schema_left->getColumnAddess(state_.joinIndex_left[0],cur)));
//			bn=state_.input_schema_left->getcolumn(0).operate->getPartitionValue(state_.input_schema_left->getColumnAddess(state_.joinIndex_left[0],cur),hash);

			bn=state_.input_schema_left->getcolumn(state_.joinIndex_left[0]).operate->getPartitionValue(state_.input_schema_left->getColumnAddess(state_.joinIndex_left[0],cur),hash);
//			const unsigned test_bn=state_.input_schema_left->getcolumn(state_.joinIndex_left[0]).operate->getPartitionValue(state_.input_schema_left->getColumnAddess(state_.joinIndex_left[0],cur),hash_test);
//			if(rand()%10000<3){
//				printf("key:%d\n",test_bn);
//			}
//			hashtable->placeIterator(tmp_it,bn);

//			lock_.acquire();
			tuple_in_hashtable=hashtable->atomicAllocate(bn);
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
			state_.input_schema_left->copyTuple(cur,tuple_in_hashtable);

//			lock_.release();
		}
		bsb->setEmpty();
	}
//	printf("<<<<<<<<<<<<<<<<Join Open consumes %d tuples\n",consumed_tuples_from_left);
	BasicHashTable::Iterator it=hashtable->CreateIterator();
	unsigned tmp=0;
	tuples_in_hashtable=0;
//	PartitionFunction* hash_tmp=PartitionFunctionFactory::createGeneralModuloFunction(4);
//	while(hashtable->placeIterator(it,tmp++)){
//		void* tuple;
//		while(tuple=it.readCurrent()){
////			printf("join key:%s\n",(state_.input_schema_left->getcolumn(state_.joinIndex_left[0]).operate->toString(state_.input_schema_left->getColumnAddess(state_.joinIndex_left[0],tuple)).c_str()));
//			tuples_in_hashtable++;
//			unsigned bn=state_.input_schema_left->getcolumn(state_.joinIndex_left[0]).operate->getPartitionValue(state_.input_schema_left->getColumnAddess(state_.joinIndex_left[0],tuple),hash_tmp);
//			if(rand()%1000<3)
//			printf("partition key of left tuple:%d\n",bn);
//			it.increase_cur_();
//		}
//	}
//	cout<<"join open end"<<endl;
	produced_tuples=0;
	consumed_tuples_from_right=0;
//	water_mark=0;
	barrier_->Arrive();
	cout<<"pass the arrive of barrier!!!"<<endl;
	state_.child_right->open(partition_offset);
//	cout<<"PartitionOffset:"<<partition_offset<<endl;
//	sleep(1);
	return true;
}

bool BlockStreamJoinIterator::next(BlockStreamBase *block){
	unsigned bn;
	void *result_tuple;
	void *tuple_from_right_child;
	void *tuple_in_hashtable;
	void *key_in_input;
	void *key_in_hashtable;
	void *column_in_joinedTuple;
	void *joinedTuple=memalign(cacheline_size,state_.output_schema->getTupleMaxSize());
	bool key_exit;

	remaining_block rb;

	PartitionFunction* hash_tmp=PartitionFunctionFactory::createGeneralModuloFunction(4);
	while(true){
		if(atomicPopRemainingBlock(rb)){
			while((tuple_from_right_child=rb.blockstream_iterator->currentTuple())>0){
				unsigned bn=state_.input_schema_right->getcolumn(state_.joinIndex_right[0]).operate->getPartitionValue(state_.input_schema_right->getColumnAddess(state_.joinIndex_right[0],tuple_from_right_child),hash_tmp);
				while((tuple_in_hashtable=rb.hashtable_iterator_.readCurrent())>0){
					key_exit=true;
					for(unsigned i=0;i<state_.joinIndex_right.size();i++){
						key_in_input=state_.input_schema_right->getColumnAddess(state_.joinIndex_right[i],tuple_from_right_child);
						key_in_hashtable=state_.ht_schema->getColumnAddess(state_.joinIndex_left[i],tuple_in_hashtable);
						if(!state_.input_schema_right->getcolumn(state_.joinIndex_right[i]).operate->equal(key_in_input,key_in_hashtable)){
							key_exit=false;
							break;
						}
					}
					if(key_exit){
						if((result_tuple=block->allocateTuple(state_.output_schema->getTupleMaxSize()))>0){
							produced_tuples++;
							const unsigned copyed_bytes=state_.input_schema_left->copyTuple(tuple_in_hashtable,result_tuple);
							state_.input_schema_right->copyTuple(tuple_from_right_child,result_tuple+copyed_bytes);
						}
						else{
							atomicPushRemainingBlock(rb);
							free(joinedTuple);
							return true;
						}
					}
					BasicHashTable::Iterator tmp=rb.hashtable_iterator_;
					rb.hashtable_iterator_.increase_cur_();
				}
				rb.blockstream_iterator->increase_cur_();
				consumed_tuples_from_right++;

				if((tuple_from_right_child=rb.blockstream_iterator->currentTuple())){
					bn=state_.input_schema_right->getcolumn(state_.joinIndex_right[0]).operate->getPartitionValue(state_.input_schema_right->getColumnAddess(state_.joinIndex_right[0],tuple_from_right_child),hash);
					hashtable->placeIterator(rb.hashtable_iterator_,bn);
				}
			}
			AtomicPushFreeBlockStream(rb.bsb_right_);
		}
		rb.bsb_right_=AtomicPopFreeBlockStream();//1 1 1
		rb.bsb_right_->setEmpty();
		rb.hashtable_iterator_=hashtable->CreateIterator();
		if(state_.child_right->next(rb.bsb_right_)==false){
			if(block->Empty()==true){
				AtomicPushFreeBlockStream(rb.bsb_right_);
				free(joinedTuple);
				printf("****join next produces %d tuples while consumed %d tuples from right child and %d tuples from left, hash table has %d tuples\n",produced_tuples,consumed_tuples_from_right,consumed_tuples_from_left,tuples_in_hashtable);
				return false;
			}
			else{
				AtomicPushFreeBlockStream(rb.bsb_right_);
				free(joinedTuple);
				return true;
			}
		}
		rb.blockstream_iterator=rb.bsb_right_->createIterator();
		if((tuple_from_right_child=rb.blockstream_iterator->currentTuple())){
			bn=state_.input_schema_right->getcolumn(state_.joinIndex_right[0]).operate->getPartitionValue(state_.input_schema_right->getColumnAddess(state_.joinIndex_right[0],tuple_from_right_child),hash);
			hashtable->placeIterator(rb.hashtable_iterator_,bn);
		}
		atomicPushRemainingBlock(rb);
	}
	return next(block);
}

bool BlockStreamJoinIterator::close(){
	cout<<"join finished!"<<endl;

#ifdef TIME
	stopTimer(&timer);
	printf("time consuming: %lld, %f\n",timer,timer/(double)CPU_FRE);
#endif
	sema_open_.post();
	open_finished_=false;
//	barrier_->~Barrier();
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







