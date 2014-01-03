/*
* BlockStreamAggregationIterator.cpp
*
* Created on: 2013-9-9
* Author: casa
*/

#include "BlockStreamAggregationIterator.h"
#include "../../Debug.h"
#include "../../rdtsc.h"

BlockStreamAggregationIterator::BlockStreamAggregationIterator(State state)
:state_(state),open_finished_(false), open_finished_end_(false),hashtable_(0),hash_(0),bucket_cur_(0){
        sema_open_.set_value(1);
        sema_open_end_.set_value(1);
        barrier_=new Barrier(1);
}

BlockStreamAggregationIterator::BlockStreamAggregationIterator()
:open_finished_(false), open_finished_end_(false),hashtable_(0),hash_(0),bucket_cur_(0){
        sema_open_.set_value(1);
        sema_open_end_.set_value(1);
        barrier_=new Barrier(1);
}

BlockStreamAggregationIterator::~BlockStreamAggregationIterator() {
        // TODO 自动生成的析构函数存根
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
	state_.child->open(partition_offset);
	cout<<"in the open of aggregation"<<endl;
#ifdef TIME
		startTimer(&timer);
#endif
	AtomicPushFreeHtBlockStream(BlockStreamBase::createBlock(state_.input,state_.block_size));
	if(sema_open_.try_wait()){
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
	}
		else{
				while (!open_finished_) {
						usleep(1);
				}
		}
		cout<<"............................................"<<endl;

		void *cur=0;
		unsigned bn;
		bool key_exist;
		void* it_cur;
		void *key_in_input_tuple;
		void *key_in_hash_table;
		void *value_in_input_tuple;
		void *value_in_hash_table;
		void* new_tuple_in_hash_table;
		unsigned allocated_tuples_in_hashtable=0;
//        void *tuple=memalign(cacheline_size,state_.output->getTupleMaxSize());
		BasicHashTable::Iterator tmp_it=hashtable_->CreateIterator();

		unsigned long long one=1;
		BlockStreamBase *bsb=AtomicPopFreeHtBlockStream();
		bsb->setEmpty();
//        cout<<"KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK"<<endl;
		unsigned consumed_tuples=0;
		unsigned matched_tuples=0;
//		allocated_tuples_in_hashtable=0;
		while(state_.child->next(bsb)){
			printf("Aggregation open consumes one block from child!\n");
//			printf("Aggregation open consumed tuples=%d\n",consumed_tuples);
//			bsb->setEmpty();
//			continue;
				BlockStreamBase::BlockStreamTraverseIterator *bsti=bsb->createIterator();
				bsti->reset();

				while(cur=bsti->currentTuple()){
					consumed_tuples++;

					bn=state_.input->getcolumn(state_.groupByIndex[0]).operate->getPartitionValue(state_.input->getColumnAddess(state_.groupByIndex[0],cur),hash_);
//					assert(bn==0);
//					state_.input->displayTuple(cur,"|*|");

//					printf("Aggregation hash key:%d\n",bn);
//						bn=hash_->get_partition_value(*(double *)(state_.input->getColumnAddess(state_.groupByIndex[0],cur)));
//                        bn=hash_->get_partition_value(*(int *)(state_.input->getColumnAddess(state_.groupByIndex[0],cur)));
						hashtable_->placeIterator(tmp_it,bn);
						key_exist=false;
						while((it_cur=tmp_it.readCurrent())!=0){

//							key_exist=false;
							for(unsigned i=0;i<state_.groupByIndex.size();i++){
									key_in_input_tuple=state_.input->getColumnAddess(state_.groupByIndex[i],cur);
									key_in_hash_table=state_.output->getColumnAddess(inputGroupByToOutput_[i],it_cur);
									if(state_.input->getcolumn(state_.groupByIndex[i]).operate->equal(key_in_input_tuple,key_in_hash_table)){
											key_exist=true;
									}
									else{
											key_exist=false;
											break;
									}
							}

								if(key_exist){
//                                        getchar();
									matched_tuples++;
//									break;
										for(unsigned i=0;i<state_.aggregationIndex.size();i++){
												value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
												value_in_hash_table=state_.output->getColumnAddess(inputAggregationToOutput_[i],it_cur);

//												lock_.acquire();
												hashtable_->atomicUpdateTuple(bn,value_in_hash_table,value_in_input_tuple,aggregationFunctions_[i]);
//												lock_.release();
//                                                cout<<"in the key_exist func value in the hash table is: "<<*reinterpret_cast<int *>(value_in_hash_table)<<endl;
//                                                getchar();
										}
										break;
								}
								else{
										tmp_it.increase_cur_();
								}
						}

						if(key_exist){
								bsti->increase_cur_();
								continue;
						}
//						bsti->increase_cur_();
//						continue;
//                        lock_.acquire();
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
//						printf("output:  ");
//						state_.output->displayTuple(new_tuple_in_hash_table);

//                        lock_.release();
						bsti->increase_cur_();
				}
//				printf("Aggregation open consumed tuples=%d\n",consumed_tuples);
				bsb->setEmpty();
		}
		printf("Aggregation consumed %d tuples , %d allocation, %d matched!\n",consumed_tuples,allocated_tuples_in_hashtable,matched_tuples);





		barrier_->Arrive();

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
		cout<<"reach the end of the open"<<endl;

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
//        cout<<"cao!!!!!!!!!!!!!!!!!!!!!"<<endl;
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
        printf("Aggregation->next() returns %d tuples!\n",block->getTuplesInBlock());
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
        cout<<"aggregation finished!"<<endl;
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
