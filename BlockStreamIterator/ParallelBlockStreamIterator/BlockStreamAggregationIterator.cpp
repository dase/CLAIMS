/*
* BlockStreamAggregationIterator.cpp
*
* Created on: 2013-9-9
* Author: casa
*/

#include "BlockStreamAggregationIterator.h"
#include "../../Debug.h"
#include "../../rdtsc.h"

//#define Expand_count 5

BlockStreamAggregationIterator::BlockStreamAggregationIterator(State state)
:state_(state),open_finished_(false), open_finished_end_(false),hashtable_(0),hash_(0),bucket_cur_(0){
        sema_open_.set_value(1);
        sema_open_end_.set_value(1);
        initialize_expanded_status();
}

BlockStreamAggregationIterator::BlockStreamAggregationIterator()
:open_finished_(false), open_finished_end_(false),hashtable_(0),hash_(0),bucket_cur_(0){
        sema_open_.set_value(1);
        sema_open_end_.set_value(1);
        initialize_expanded_status();
}

BlockStreamAggregationIterator::~BlockStreamAggregationIterator() {

}

BlockStreamAggregationIterator::State::State(
                 Schema *input,
                 Schema *output,
                 BlockStreamIteratorBase *child,
                 std::vector<unsigned> groupByIndex,
                 std::vector<unsigned> aggregationIndex,
                 std::vector<State::aggregation> aggregations,
                 unsigned nbuckets,
                 unsigned bucketsize,
                 unsigned block_size)
                :input(input),
                 output(output),
                 child(child),
                 groupByIndex(groupByIndex),
         aggregationIndex(aggregationIndex),
         aggregations(aggregations),
         nbuckets(nbuckets),
         bucketsize(bucketsize),
         block_size(block_size){

        }

bool BlockStreamAggregationIterator::open(const PartitionOffset& partition_offset){
	barrier_.RegisterOneThread();
	state_.child->open(partition_offset);
	cout<<"in the open of aggregation"<<endl;
#ifdef TIME
		startTimer(&timer);
#endif
	RegisterNewThreadToBarrier();
	AtomicPushFreeHtBlockStream(BlockStreamBase::createBlock(state_.input,state_.block_size));
	if(completeForInitializationJob()){
		unsigned outputindex=0;
		for(unsigned i=0;i<state_.groupByIndex.size();i++)
		{
				inputGroupByToOutput_[i]=outputindex++;
		}
		for(unsigned i=0;i<state_.aggregationIndex.size();i++)
		{
				inputAggregationToOutput_[i]=outputindex++;
		}

		for(unsigned i=0;i<state_.aggregations.size();i++)
		{
			switch(state_.aggregations[i])
			{
				case BlockStreamAggregationIterator::State::count:
				{
					aggregationFunctions_.push_back(state_.output->getcolumn(inputAggregationToOutput_[i]).operate->GetIncreateByOneFunction());
					break;
				}
				case BlockStreamAggregationIterator::State::min:
				{
					aggregationFunctions_.push_back(state_.output->getcolumn(inputAggregationToOutput_[i]).operate->GetMINFunction());
					break;
				}
				case BlockStreamAggregationIterator::State::max:
				{
					aggregationFunctions_.push_back(state_.output->getcolumn(inputAggregationToOutput_[i]).operate->GetMAXFunction());
					break;
				}
				case BlockStreamAggregationIterator::State::sum:
				{
					aggregationFunctions_.push_back(state_.output->getcolumn(inputAggregationToOutput_[i]).operate->GetADDFunction());
					break;
				}
				default:
				{
					printf("invalid aggregation function!\n");
				}
			}
		}
		hash_=PartitionFunctionFactory::createGeneralModuloFunction(state_.nbuckets);
		hashtable_=new BasicHashTable(state_.nbuckets,state_.bucketsize,state_.output->getTupleMaxSize());
		open_finished_=true;
		broadcaseOpenFinishedSignal();
	}
	else{
//		while (!open_finished_) {
//			usleep(1);
//		}
		waitForOpenFinished();
	}
//		cout<<"............................................"<<endl;

	void *cur=0;
	unsigned bn;
	bool key_exist;
	void* tuple_in_hashtable;
	void *key_in_input_tuple;
	void *key_in_hash_table;
	void *value_in_input_tuple;
	void *value_in_hash_table;
	void* new_tuple_in_hash_table;
	unsigned allocated_tuples_in_hashtable=0;
//        void *tuple=memalign(cacheline_size,state_.output->getTupleMaxSize());
	BasicHashTable::Iterator ht_it=hashtable_->CreateIterator();

	unsigned long long one=1;
	BlockStreamBase *bsb=AtomicPopFreeHtBlockStream();
	bsb->setEmpty();

	unsigned consumed_tuples=0;
	unsigned matched_tuples=0;
//		allocated_tuples_in_hashtable=0;

		/*
		 * group-by aggregation
		 */
	if(!state_.groupByIndex.empty())
	while(state_.child->next(bsb)){
	//	printf("Aggregation open consumes one block from child!\n");
	//	printf("Aggregation open consumed tuples=%d\n",consumed_tuples);
	//	bsb->setEmpty();
	//	continue;
		BlockStreamBase::BlockStreamTraverseIterator *bsti=bsb->createIterator();
		bsti->reset();

		while(cur=bsti->currentTuple()){
			consumed_tuples++;

			bn=state_.input->getcolumn(state_.groupByIndex[0]).operate->getPartitionValue(state_.input->getColumnAddess(state_.groupByIndex[0],cur),hash_);
			hashtable_->placeIterator(ht_it,bn);
			key_exist=false;
			while((tuple_in_hashtable=ht_it.readCurrent())!=0){
	//			key_exist=false;
				for(unsigned i=0;i<state_.groupByIndex.size();i++){
					key_in_input_tuple=state_.input->getColumnAddess(state_.groupByIndex[i],cur);
					key_in_hash_table=state_.output->getColumnAddess(inputGroupByToOutput_[i],tuple_in_hashtable);
//					for(unsigned i=0;i<state_.aggregationIndex.size();i++){
//						/**
//						 * use if-else here is a kind of ugly.
//						 * TODO: use a function which is initialized according to the aggregation function.
//						 */
//						if(state_.aggregations[i]==State::count){
//								value_in_input_tuple=&one;
//						}
//						else{
//								value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
//						}
//						value_in_hash_table=state_.output->getColumnAddess(inputAggregationToOutput_[i],new_tuple_in_hash_table);
//						state_.input->getcolumn(state_.aggregationIndex[i]).operate->assignment(value_in_input_tuple,value_in_hash_table);
//					}
					if(state_.input->getcolumn(state_.groupByIndex[i]).operate->equal(key_in_input_tuple,key_in_hash_table)){
						key_exist=true;
					}
					else{
						key_exist=false;
						break;
					}
				}
				if(key_exist){
	//              getchar();
					matched_tuples++;
	//				break;
					for(unsigned i=0;i<state_.aggregationIndex.size();i++){
						value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
						value_in_hash_table=state_.output->getColumnAddess(inputAggregationToOutput_[i],tuple_in_hashtable);

						hashtable_->atomicUpdateTuple(bn,value_in_hash_table,value_in_input_tuple,aggregationFunctions_[i]);

					}
					break;
				}
				else{
					ht_it.increase_cur_();
				}
			}

			if(key_exist){
				bsti->increase_cur_();
				continue;
			}
//			bsti->increase_cur_();
//			continue;
//          lock_.acquire();
			//if the key doesn't exist, so we can allocate a space for it, and init the func of the hashtable
			new_tuple_in_hash_table=hashtable_->atomicAllocate(bn);
			allocated_tuples_in_hashtable++;


			for(unsigned i=0;i<state_.groupByIndex.size();i++){
				key_in_input_tuple=state_.input->getColumnAddess(state_.groupByIndex[i],cur);
				key_in_hash_table=state_.output->getColumnAddess(inputGroupByToOutput_[i],new_tuple_in_hash_table);
				state_.input->getcolumn(state_.groupByIndex[i]).operate->assignment(key_in_input_tuple,key_in_hash_table);
			}

			for(unsigned i=0;i<state_.aggregationIndex.size();i++){
				/**
				 * use if-else here is a kind of ugly.
				 * TODO: use a function which is initialized according to the aggregation function.
				 */
				if(state_.aggregations[i]==State::count){
						value_in_input_tuple=&one;
				}
				else{
						value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
				}
				value_in_hash_table=state_.output->getColumnAddess(inputAggregationToOutput_[i],new_tuple_in_hash_table);
				state_.input->getcolumn(state_.aggregationIndex[i]).operate->assignment(value_in_input_tuple,value_in_hash_table);
			}
//			printf("output:  ");
//			state_.output->displayTuple(new_tuple_in_hash_table);

//          lock_.release();
			bsti->increase_cur_();
		}
//		printf("Aggregation open consumed tuples=%d\n",consumed_tuples);
		bsb->setEmpty();
	}
	else{
		/**
		 * scalar aggregation, e.i., all tuples are in the same group.
		 */
		while(state_.child->next(bsb)){
//			printf("One block is used!!!!!!\n");
			BlockStreamBase::BlockStreamTraverseIterator *bsti=bsb->createIterator();
			bsti->reset();
			while(cur=bsti->currentTuple()){
//				if(state_.aggregations[0]==BlockStreamAggregationIterator::State::sum){
//				state_.output->displayTuple(cur);
//				}
				consumed_tuples++;
				bn=0;
				hashtable_->placeIterator(ht_it,bn);
				key_exist=false;
				if((tuple_in_hashtable=ht_it.readCurrent())!=0){
					key_exist=true;
					matched_tuples++;
					for(unsigned i=0;i<state_.aggregationIndex.size();i++){
						value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
						value_in_hash_table=state_.output->getColumnAddess(inputAggregationToOutput_[i],tuple_in_hashtable);
						hashtable_->atomicUpdateTuple(bn,value_in_hash_table,value_in_input_tuple,aggregationFunctions_[i]);
					}
					bsti->increase_cur_();
				}
				else{
					new_tuple_in_hash_table=hashtable_->atomicAllocate(bn);
					allocated_tuples_in_hashtable++;
					for(unsigned i=0;i<state_.aggregationIndex.size();i++){
						/**
						 * use if-else here is a kind of ugly.
						 * TODO: use a function which is initialized according to the aggregation function.
						 */
						if(state_.aggregations[i]==State::count){
							value_in_input_tuple=&one;
						}
						else{
							value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
						}
						value_in_hash_table=state_.output->getColumnAddess(inputAggregationToOutput_[i],new_tuple_in_hash_table);
						state_.input->getcolumn(state_.aggregationIndex[i]).operate->assignment(value_in_input_tuple,value_in_hash_table);
					}
					bsti->increase_cur_();
				}
			}
			bsb->setEmpty();
		}
	}
//				if(key_exist){
//						bsti->increase_cur_();
//						continue;
//				}
//				new_tuple_in_hash_table=hashtable_->atomicAllocate(bn);
//				allocated_tuples_in_hashtable++;
////				for(unsigned i=0;i<state_.groupByIndex.size();i++){
////						key_in_input_tuple=state_.input->getColumnAddess(state_.groupByIndex[i],cur);
////						key_in_hash_table=state_.output->getColumnAddess(inputGroupByToOutput_[i],new_tuple_in_hash_table);
////						state_.input->getcolumn(state_.groupByIndex[i]).operate->assignment(key_in_input_tuple,key_in_hash_table);
////				}
//				for(unsigned i=0;i<state_.aggregationIndex.size();i++){
//						/**
//						 * use if-else here is a kind of ugly.
//						 * TODO: use a function which is initialized according to the aggregation function.
//						 */
//						if(state_.aggregations[i]==State::count){
//								value_in_input_tuple=&one;
//						}
//						else{
//								value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
//						}
//						value_in_hash_table=state_.output->getColumnAddess(inputAggregationToOutput_[i],new_tuple_in_hash_table);
//						state_.input->getcolumn(state_.aggregationIndex[i]).operate->assignment(value_in_input_tuple,value_in_hash_table);
//				}
////						printf("output:  ");
////						state_.output->displayTuple(new_tuple_in_hash_table);
//
////                        lock_.release();
//				bsti->increase_cur_();
//		}
			//				printf("Aggregation open consumed tuples=%d\n",consumed_tuples);
//							bsb->setEmpty();
//		}
//		}
//		printf("Aggregation consumed %d tuples , %d allocation, %d matched!\n",consumed_tuples,allocated_tuples_in_hashtable,matched_tuples);




		barrierArrive();
//		barrier_.Arrive();
		cout<<"Aggregation hash table is built!\n"<<endl;
		if(sema_open_end_.try_wait()){
//                cout<<"================================================"<<endl;
				it_=hashtable_->CreateIterator();
				bucket_cur_=0;
				hashtable_->placeIterator(it_,bucket_cur_);
				open_finished_end_=true;
		}
		else{
				while (!open_finished_end_) {
								usleep(1);
						}
		}
	#ifdef TIME
			stopTimer(&timer);
			printf("<+++++++>: time consuming: %lld, %f\n",timer,timer/(double)CPU_FRE);
	#endif
//		cout<<"reach the end of the open"<<endl;

//		BasicHashTable::Iterator it=hashtable_->CreateIterator();
//		unsigned tmp=0;
//		while(hashtable_->placeIterator(it,bucket_cur_)){
//			void* tuple;
//			while(tuple=it.readCurrent()){
//				state_.output->displayTuple(tuple);
////				printf("%d\n",tmp++);
//				it.increase_cur_();
//			}
//			bucket_cur_++;
//		}
//		bucket_cur_=0;
//		return true;
}

bool BlockStreamAggregationIterator::next(BlockStreamBase *block){
        //内存有可能不连续，所以，不能复制整个块
        void *cur_in_ht;
        void *tuple;
        ht_cur_lock_.acquire();
        while(it_.readCurrent()!=0||(hashtable_->placeIterator(it_,bucket_cur_))!=false){
                while((cur_in_ht=it_.readCurrent())!=0){
//                	printf("Address:%x\n",cur_in_ht);
//                	state_.output->displayTuple(cur_in_ht,"\t");
                        if((tuple=block->allocateTuple(hashtable_->getHashTableTupleSize()))!=0){
                                memcpy(tuple,cur_in_ht,hashtable_->getHashTableTupleSize());
//                                cout<<"tuple in the hashtable: "<<*reinterpret_cast<int *>(cur_in_ht)<<"--"<<*(reinterpret_cast<int *>(cur_in_ht)+1)<<endl;
//                        getchar();
                                it_.increase_cur_();

                        }
                        else{
                                ht_cur_lock_.release();
//                                printf("Aggregation->next() returns %d tuples!\n",block->getTuplesInBlock());
                                return true;
                        }
                }
                bucket_cur_++;

        }
        ht_cur_lock_.release();
//        printf("Aggregation->next() returns %d tuples!\n",block->getTuplesInBlock());
//        usleep(1000);
        if(block->Empty()){
//        	   printf("<<<<<<<<<<<<<<<<<<Aggregation next bucket_cur=%d\n",bucket_cur_);
        	   return false;
        }
        else{
//        	printf("<<<<<<<<<<<<<<<<<<Aggregation next bucket_cur=%d\n",bucket_cur_);
			return true;
        }
}

bool BlockStreamAggregationIterator::close(){
//        cout<<"aggregation finished!"<<endl;
    initialize_expanded_status();
        sema_open_.post();
        sema_open_end_.post();
//        lock_.~Lock();
//        ht_cur_lock_.~Lock();
        open_finished_=false;
        open_finished_end_=false;
//        barrier_->~Barrier();
//        hash_->~PartitionFunction();
        hashtable_->~BasicHashTable();
        ht_free_block_stream_list_.clear();
        aggregationFunctions_.clear();
        inputAggregationToOutput_.clear();
        inputGroupByToOutput_.clear();
//        bucket_cur_=0;
//        state_.~State();
        state_.child->close();
        return true;
}
void BlockStreamAggregationIterator::print(){
	printf("Aggregation:\n");
	printf("---------------\n");
	state_.child->print();
}
BlockStreamBase* BlockStreamAggregationIterator::AtomicPopFreeHtBlockStream(){
        assert(!ht_free_block_stream_list_.empty());
        lock_.acquire();
        BlockStreamBase *block=ht_free_block_stream_list_.front();
        ht_free_block_stream_list_.pop_front();
        lock_.release();
        return block;
}

void BlockStreamAggregationIterator::AtomicPushFreeHtBlockStream(BlockStreamBase* block){
        lock_.acquire();
        ht_free_block_stream_list_.push_back(block);
        lock_.release();
}
