/*
 * BlockStreamAggregationIterator.cpp
 *
 * Created on: 2013-9-9
 * Author: casa
 */

#include "BlockStreamAggregationIterator.h"
#include "../../Debug.h"
#include "../../utility/rdtsc.h"
#include "../../Executor/ExpanderTracker.h"

BlockStreamAggregationIterator::BlockStreamAggregationIterator(State state)
:state_(state),open_finished_(false), open_finished_end_(false),hashtable_(0),hash_(0),bucket_cur_(0),ExpandableBlockStreamIteratorBase(4,3){
	sema_open_.set_value(1);
	sema_open_end_.set_value(1);
	initialize_expanded_status();
	assert(state_.hashSchema);
}

BlockStreamAggregationIterator::BlockStreamAggregationIterator()
:open_finished_(false), open_finished_end_(false),hashtable_(0),hash_(0),bucket_cur_(0),ExpandableBlockStreamIteratorBase(4,3){
	sema_open_.set_value(1);
	sema_open_end_.set_value(1);
	initialize_expanded_status();
}

BlockStreamAggregationIterator::~BlockStreamAggregationIterator() {
	delete state_.input;
	delete state_.hashSchema;
	delete state_.output;
	delete state_.child;
}

BlockStreamAggregationIterator::State::State(
		Schema *input,
		Schema *output,
		Schema *hashSchema,
		BlockStreamIteratorBase *child,
		std::vector<unsigned> groupByIndex,
		std::vector<unsigned> aggregationIndex,
		std::vector<State::aggregation> aggregations,
		unsigned nbuckets,
		unsigned bucketsize,
		unsigned block_size,
		std::vector<unsigned>avgIndex,
		bool isPartitionNode
):input(input),
output(output),
hashSchema(hashSchema),
child(child),
groupByIndex(groupByIndex),
aggregationIndex(aggregationIndex),
aggregations(aggregations),
nbuckets(nbuckets),
bucketsize(bucketsize),
block_size(block_size),
avgIndex(avgIndex),
isPartitionNode(isPartitionNode){

}

bool BlockStreamAggregationIterator::open(const PartitionOffset& partition_offset){
	barrier_.RegisterOneThread();
	RegisterExpandedThreadToAllBarriers();

	if(tryEntryIntoSerializedSection(0)){
		ExpanderTracker::getInstance()->addNewStageEndpoint(pthread_self(),LocalStageEndPoint(stage_desc,"Aggregation",0));
	}
	barrierArrive(0);

	state_.child->open(partition_offset);
	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
		unregisterExpandedThreadToAllBarriers();
		return true;
	}

	if(tryEntryIntoSerializedSection(1)){
		unsigned outputindex=0;
		for(unsigned i=0;i<state_.groupByIndex.size();i++)
		{
			inputGroupByToOutput_[i]=outputindex++;	// index of group by attributes from input To output index
			//				cout<<"i= "<<i<<" gbi= "<<state_.groupByIndex[i]<<"  igtoi= "<<inputGroupByToOutput_[i]<<endl;
		}
		for(unsigned i=0;i<state_.aggregationIndex.size();i++)
		{
			inputAggregationToOutput_[i]=outputindex++;	// index of aggregation attributes from input To output index
			//				cout<<"i= "<<i<<" ai= "<<state_.aggregationIndex[i]<<"  iatoi= "<<inputAggregationToOutput_[i]<<endl;
		}
		for(unsigned i=0;i<state_.aggregations.size();i++)
		{
			switch(state_.aggregations[i])
			{
			case BlockStreamAggregationIterator::State::count:
			{
				aggregationFunctions_.push_back(state_.hashSchema->getcolumn(inputAggregationToOutput_[i]).operate->GetIncreateByOneFunction());
				break;
			}
			case BlockStreamAggregationIterator::State::min:
			{
				aggregationFunctions_.push_back(state_.hashSchema->getcolumn(inputAggregationToOutput_[i]).operate->GetMINFunction());
				break;
			}
			case BlockStreamAggregationIterator::State::max:
			{
				aggregationFunctions_.push_back(state_.hashSchema->getcolumn(inputAggregationToOutput_[i]).operate->GetMAXFunction());
				break;
			}
			case BlockStreamAggregationIterator::State::sum:
			{
				aggregationFunctions_.push_back(state_.hashSchema->getcolumn(inputAggregationToOutput_[i]).operate->GetADDFunction());
				break;
			}
			default://for avg has changed to sum and count
			{
				printf("invalid aggregation function!\n");
			}
			}
		}
		hash_=PartitionFunctionFactory::createGeneralModuloFunction(state_.nbuckets);
		hashtable_=new BasicHashTable(state_.nbuckets,state_.bucketsize,state_.hashSchema->getTupleMaxSize());//
		open_finished_=true;

	}


	barrierArrive(1);

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
	BasicHashTable::Iterator ht_it=hashtable_->CreateIterator();

	unsigned long long one=1;
	//	BlockStreamBase *bsb=AtomicPopFreeHtBlockStream();
	BlockStreamBase *bsb=BlockStreamBase::createBlock(state_.input,state_.block_size);
	bsb->setEmpty();

	unsigned consumed_tuples=0;
	unsigned matched_tuples=0;

	/*
	 * group-by aggregation
	 */
	if(!state_.groupByIndex.empty()||true)
	{
		while(state_.child->next(bsb))	// get every block from child
		{
			BlockStreamBase::BlockStreamTraverseIterator *bsti=bsb->createIterator();
			bsti->reset();
			while((cur=bsti->currentTuple())!=0)	// get every tuple from block
			{
				consumed_tuples++;

				/* get the corresponding bucket index according to the first column in
				 * group-by attributes.
				 * Note that bn is always 0 for scalar aggregation.
				 */
				bn=0;
				if(state_.groupByIndex.size()>0)
					bn=state_.input->getcolumn(state_.groupByIndex[0]).operate->getPartitionValue(state_.input->getColumnAddess(state_.groupByIndex[0],cur),state_.nbuckets);

				hashtable_->lockBlock(bn);
				hashtable_->placeIterator(ht_it,bn);
				key_exist=false;
				while((tuple_in_hashtable=ht_it.readCurrent())!=0)
				{
					/* set key_exist flag to true such that the the case for scalar
					 * aggregation (i.e., aggregation with no group-by attributes)
					 * could be considered as passed the group by value verification.
					 */
					key_exist=true;
					for(unsigned i=0;i<state_.groupByIndex.size();i++)
					{
						key_in_input_tuple=state_.input->getColumnAddess(state_.groupByIndex[i],cur);
						key_in_hash_table=state_.hashSchema->getColumnAddess(inputGroupByToOutput_[i],tuple_in_hashtable);
						if(!state_.input->getcolumn(state_.groupByIndex[i]).operate->equal(key_in_input_tuple,key_in_hash_table))
						{
							key_exist=false;
							break;
						}
					}
					if(key_exist)	// hash table have the key (the value in group-by attribute)
					{
						matched_tuples++;
						for(unsigned i=0;i<state_.aggregationIndex.size();i++)
						{
							value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
							value_in_hash_table=state_.hashSchema->getColumnAddess(inputAggregationToOutput_[i],tuple_in_hashtable);
							hashtable_->UpdateTuple(bn,value_in_hash_table,value_in_input_tuple,aggregationFunctions_[i]);
						}
						break;
					}
					else
					{
						ht_it.increase_cur_();
					}
				}
				if(key_exist)
				{
					bsti->increase_cur_();
					hashtable_->unlockBlock(bn);
					continue;
				}
				new_tuple_in_hash_table=hashtable_->allocate(bn);
				hashtable_->unlockBlock(bn);
				allocated_tuples_in_hashtable++;
				for(unsigned i=0;i<state_.groupByIndex.size();i++)
				{
					key_in_input_tuple=state_.input->getColumnAddess(state_.groupByIndex[i],cur);
					key_in_hash_table=state_.hashSchema->getColumnAddess(inputGroupByToOutput_[i],new_tuple_in_hash_table);
					state_.input->getcolumn(state_.groupByIndex[i]).operate->assignment(key_in_input_tuple,key_in_hash_table);
				}

				for(unsigned i=0;i<state_.aggregationIndex.size();i++)
				{
					/**
					 * use if-else here is a kind of ugly.
					 * TODO: use a function which is initialized according to the aggregation function.
					 */
					if(state_.aggregations[i]==State::count)
					{
						value_in_input_tuple=&one;
					}
					else
					{
						value_in_input_tuple=state_.input->getColumnAddess(state_.aggregationIndex[i],cur);
					}
					value_in_hash_table=state_.hashSchema->getColumnAddess(inputAggregationToOutput_[i],new_tuple_in_hash_table);
					state_.hashSchema->getcolumn(inputAggregationToOutput_[i]).operate->assignment(value_in_input_tuple,value_in_hash_table);
				}
				bsti->increase_cur_();
			}
			bsb->setEmpty();
		}
	}




		if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
			unregisterExpandedThreadToAllBarriers(1);
			return true;
		}
		barrierArrive(2);

		if(tryEntryIntoSerializedSection(2)){
//			hashtable_->report_status();
				it_=hashtable_->CreateIterator();
				bucket_cur_=0;
				hashtable_->placeIterator(it_,bucket_cur_);
				open_finished_end_=true;
				ExpanderTracker::getInstance()->addNewStageEndpoint(pthread_self(),LocalStageEndPoint(stage_src,"Aggregation  ",0));
				perf_info_=ExpanderTracker::getInstance()->getPerformanceInfo(pthread_self());
				perf_info_->initialize();
		}
		barrierArrive(3);

	delete bsb;
//	hashtable_->report_status();
}

/*
 * In the current implementation, the lock is used based on the entire
 * hash table, which will definitely reduce the degree of parallelism.
 * But it is for now, assuming that the aggregated results are small.
 *
 */
bool BlockStreamAggregationIterator::next(BlockStreamBase *block){
	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
		unregisterExpandedThreadToAllBarriers(3);
		printf("<<<<<<<<<<<<<<<<<Aggregation next detected call back signal!>>>>>>>>>>>>>>>>>\n");
		return false;
	}
	void *cur_in_ht;
	void *tuple;
	void *key_in_hash_tuple;
	void *key_in_output_tuple;
	ht_cur_lock_.acquire();
	while(it_.readCurrent()!=0||(hashtable_->placeIterator(it_,bucket_cur_))!=false){
		while((cur_in_ht=it_.readCurrent())!=0){
			if((tuple=block->allocateTuple(state_.output->getTupleMaxSize()))!=0){//??
				if(state_.avgIndex.size()>0&&state_.isPartitionNode==false)
				{
					for(unsigned i=0;i<state_.groupByIndex.size();i++){
						key_in_hash_tuple=state_.hashSchema->getColumnAddess(inputGroupByToOutput_[i],cur_in_ht);
						key_in_output_tuple=state_.output->getColumnAddess(inputGroupByToOutput_[i],tuple);
						state_.output->getcolumn(inputGroupByToOutput_[i]).operate->assignment(key_in_hash_tuple,key_in_output_tuple);
					}
					state_.avgIndex.push_back(-1);//边界点
					int aggsize=state_.aggregationIndex.size()-1;
					unsigned i=0,j=0;
					unsigned long  fm=(*(unsigned long *)state_.hashSchema->getColumnAddess(inputAggregationToOutput_[aggsize],cur_in_ht));
					for(;i<aggsize;i++)
					{
						if(state_.avgIndex[j]==i)	//avgIndex save the index of avg in aggregations,see Aggregation.cpp:116
						{
							assert(state_.aggregations[i]==State::sum);
							j++;
							void *unknowntype=state_.hashSchema->getColumnAddess(inputAggregationToOutput_[i],cur_in_ht); //get the value in hash table
							if(fm==0){
								key_in_hash_tuple=unknowntype;
							}else{
								// TODO: precision of avg result is not enough
								switch(state_.hashSchema->columns[inputAggregationToOutput_[i]].type)
								{
									case t_int:
									{
										int  tmp=*(int *)unknowntype;
										if(fm!=0)
										tmp=(tmp/fm);
										key_in_hash_tuple=&tmp;
									}break;
									case t_float:
									{
										float  tmp=*(float *)unknowntype;
										if(fm!=0)
										tmp=(tmp/fm);
										key_in_hash_tuple=&tmp;
									}break;
									case t_double:
									{
										double  tmp=*(double *)unknowntype;
										if(fm!=0)
										tmp=(tmp/fm);
										key_in_hash_tuple=&tmp;
									}break;
									case t_u_long:
									{
										unsigned long  tmp=*(unsigned long *)unknowntype;
										if(fm!=0)
										tmp=(tmp/fm);
										key_in_hash_tuple=&tmp;
									}break;
									case t_string:
									{
										key_in_hash_tuple=unknowntype;
									}break;
									case t_date:
									{
										key_in_hash_tuple=unknowntype;
									}break;
									case t_time:
									{
										key_in_hash_tuple=unknowntype;
									}break;
									case t_datetime:
									{
										key_in_hash_tuple=unknowntype;
									}break;
									case t_decimal:
									{
										NValue  tmp=*(NValue *)unknowntype;
										if(fm!=0)
										{
											stringstream ss;
											ss<<fm;
											tmp=tmp.op_divide(tmp.getDecimalValueFromString(ss.str()));
										}
									//	cout<<"agg---iterator---next tmp=  "<<tmp<<"  z= "<<tmp<<"  m=  "<<fm<<endl;
										key_in_hash_tuple=&tmp;
									}break;
									case t_smallInt:
									{
										short  tmp=*(short *)unknowntype;
										if(fm!=0)
										tmp=(tmp/fm);
	//										cout<<"agg---iterator---next tmp=  "<<tmp<<"  z= "<<tmp<<"  m=  "<<fm<<endl;
										key_in_hash_tuple=&tmp;
									}break;
									case t_u_smallInt:
									{
										unsigned short  tmp=*(unsigned short *)unknowntype;
										if(fm!=0)
										tmp=(tmp/fm);
	//										cout<<"agg---iterator---next tmp=  "<<tmp<<"  z= "<<tmp<<"  m=  "<<fm<<endl;
										key_in_hash_tuple=&tmp;
									}break;
									default:
									{
										printf("BlockStreamAggregation.cpp unknown type\n");
									}
								}
							}
						}
						else{
							key_in_hash_tuple=state_.hashSchema->getColumnAddess(inputAggregationToOutput_[i],cur_in_ht);
						}
						key_in_output_tuple=state_.output->getColumnAddess(inputAggregationToOutput_[i],tuple);
						//	cout<<"assign  i="<<i<<"   hash_value=   "<<*(unsigned long *)key_in_hash_tuple<<endl;
						state_.output->getcolumn(inputAggregationToOutput_[i]).operate->assignment(key_in_hash_tuple,key_in_output_tuple);
					}
					//	cout<<"agg val partition node i="<<i<<" mpvalue= "<<state_.aggregationIndex[i]<<" value=  "<<(*(unsigned long*)state_.hashSchema->getColumnAddess(inputAggregationToOutput_[i],cur_in_ht))<<endl;
				}
				else{
					memcpy(tuple,cur_in_ht,state_.output->getTupleMaxSize());
				}
				it_.increase_cur_();
			}
			else{
				ht_cur_lock_.release();
				perf_info_->processed_one_block();
				return true;
			}
		}
		bucket_cur_++;
	}
	ht_cur_lock_.release();
	if(block->Empty()){
		return false;
	}
	else{
		perf_info_->processed_one_block();
		return true;
	}
}

bool BlockStreamAggregationIterator::close(){

	initialize_expanded_status();
	sema_open_.post();
	sema_open_end_.post();

	open_finished_=false;
	open_finished_end_=false;

	delete hashtable_;
	ht_free_block_stream_list_.clear();
	aggregationFunctions_.clear();
	inputAggregationToOutput_.clear();
	inputGroupByToOutput_.clear();

	state_.child->close();
	return true;
}
void BlockStreamAggregationIterator::print(){
	printf("Aggregation:  %d buckets in hash table\n",state_.nbuckets);
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





