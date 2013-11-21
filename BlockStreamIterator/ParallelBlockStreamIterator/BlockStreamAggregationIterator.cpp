/*
 * BlockStreamAggregationIterator.cpp
 *
 *  Created on: 2013-9-9
 *      Author: casa
 */

#include "BlockStreamAggregationIterator.h"

BlockStreamAggregationIterator::BlockStreamAggregationIterator(State state)
:state_(state),open_finished_(false), open_finished_end_(false){
	sema_open_.set_value(1);
	sema_open_end_.set_value(1);
	barrier_=new Barrier(1);
}

BlockStreamAggregationIterator::BlockStreamAggregationIterator()
:open_finished_(false), open_finished_end_(false){
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
	cout<<"in the open of aggregation"<<endl;
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

		hash_=PartitionFunctionFactory::createModuloFunction(state_.nbuckets);
		hashtable_=new BasicHashTable(state_.nbuckets,state_.bucketsize,state_.output->getTupleMaxSize());
		open_finished_=true;
//		cout<<"in the ================================"<<endl;
	}
	else{
//		cout<<"in ==========================="<<endl;
		while (!open_finished_) {
			usleep(1);
		}
	}

	cout<<"............................................"<<endl;
//	getchar();
	state_.child->open(partition_offset);
//	cout<<"aggr open hashtable finished!"<<endl;

	void *cur=0;
	unsigned bn;
	bool key_exist;
	void* it_cur;
	void *key_in_input_tuple;
	void *key_in_hash_table;
	void *value_in_input_tuple;
	void *value_in_hash_table;
	void* new_tuple_in_hash_table;
//	void *tuple=memalign(cacheline_size,state_.output->getTupleMaxSize());
	BasicHashTable::Iterator tmp_it=hashtable_->CreateIterator();

	unsigned long long one=1;
	BlockStreamBase *bsb=AtomicPopFreeHtBlockStream();
	bsb->setEmpty();
//	cout<<"KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK"<<endl;
	while(state_.child->next(bsb)){
		BlockStreamBase::BlockStreamTraverseIterator *bsti=bsb->createIterator();
		bsti->reset();
//		while(cur=bsti->currentTuple()){
//			cout<<"tuple "<<*reinterpret_cast<int *>(cur)<<endl;
//			bsti->increase_cur_();
//		}
//		getchar();
		while(cur=bsti->currentTuple()){
			bn=hash_->get_partition_value(*(int *)(state_.input->getColumnAddess(state_.groupByIndex[0],cur)));
			hashtable_->placeIterator(tmp_it,bn);
			key_exist=false;
			while((it_cur=tmp_it.readCurrent())!=0){
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
//					getchar();
					for(unsigned i=0;i<state_.aggregationIndex.size();i++){
						value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
						value_in_hash_table=state_.output->getColumnAddess(inputAggregationToOutput_[i],it_cur);

						lock_.acquire();
						hashtable_->UpdateTuple(bn,value_in_hash_table,value_in_input_tuple,aggregationFunctions_[i]);
						lock_.release();
//						cout<<"in the key_exist func value in the hash table is: "<<*reinterpret_cast<int *>(value_in_hash_table)<<endl;
//						getchar();
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
			lock_.acquire();
			//if the key doesn't exist, so we can allocate a space for it, and init the func of the hashtable
			new_tuple_in_hash_table=hashtable_->allocate(bn);

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
			lock_.release();
			bsti->increase_cur_();
		}
		bsb->setEmpty();
	}
	cout<<"{{{{{{{{{{{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}"<<endl;
	barrier_->Arrive();
	cout<<"{{{{{{{{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}}}}}}}}}}}"<<endl;
	if(sema_open_end_.try_wait()){
//		cout<<"================================================"<<endl;
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
	cout<<"reach the end of the open"<<endl;
	return true;
}

bool BlockStreamAggregationIterator::next(BlockStreamBase *block){
	//内存有可能不连续，所以，不能复制整个块
	void *cur_in_ht;
	void *tuple;
//	cout<<"cao!!!!!!!!!!!!!!!!!!!!!"<<endl;
	ht_cur_lock_.acquire();
	while((hashtable_->placeIterator(it_,bucket_cur_))!=false){
		while((cur_in_ht=it_.readCurrent())!=0){
			if((tuple=block->allocateTuple(hashtable_->getHashTableTupleSize()))!=0){
				memcpy(tuple,cur_in_ht,hashtable_->getHashTableTupleSize());
				cout<<"tuple in the hashtable: "<<*reinterpret_cast<int *>(cur_in_ht)<<"--"<<*(reinterpret_cast<int *>(cur_in_ht)+1)<<endl;
//			getchar();
				it_.increase_cur_();
			}
			else{
				ht_cur_lock_.release();
				return true;
			}
		}
		bucket_cur_++;
	}
	ht_cur_lock_.release();
	if(block->Empty())
		return false;
	else
		return true;
}

bool BlockStreamAggregationIterator::close(){
	cout<<"++++++++++++++++++++++++++++++++++++++"<<endl;
	state_.child->close();
	hash_->~PartitionFunction();
	hashtable_->~BasicHashTable();
	for(unsigned i=0;i<ht_free_block_stream_list_.size();i++){
		free(ht_free_block_stream_list_.front());
		ht_free_block_stream_list_.pop_front();
	}
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
