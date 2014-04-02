/*
 * BlockStreamInIterator.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: scdong
 */

#include "BlockStreamInIterator.h"

BlockStreamInIterator::BlockStreamInIterator(State state)
:state_(state), hash(0), hashtable(0), open_finished_(false), reached_end(0){
	// TODO Auto-generated constructor stub
	sema_open_.set_value(1);
	barrier_ = new Barrier(1);
}

BlockStreamInIterator::BlockStreamInIterator()
:hash(0), hashtable(0), open_finished_(false), reached_end(0){
	// TODO Auto-generated constructor stub
	sema_open_.set_value(1);
	barrier_ = new Barrier(1);
}

BlockStreamInIterator::~BlockStreamInIterator() {
	// TODO Auto-generated destructor stub
}

BlockStreamInIterator::State::State(BlockStreamIteratorBase* child_set,
		BlockStreamIteratorBase* child_in, Schema* schema_child_set, Schema* schema_child_in,
		unsigned index_child_set, unsigned index_child_in, unsigned block_size, unsigned ht_nbuckets,
		unsigned ht_bucketsize)
:child_set(child_set), child_in(child_in), schema_child_set(schema_child_set),
 schema_child_in(schema_child_in), index_child_set(index_child_set), index_child_in(index_child_in),
 block_size_(block_size), ht_nbuckets(ht_nbuckets), ht_bucketsize(ht_bucketsize){

}

BlockStreamInIterator::State::State(BlockStreamIteratorBase* child_set,
		BlockStreamIteratorBase* child_in, Schema* schema_child_set, Schema* schema_child_in,
		unsigned index_child_set, unsigned index_child_in, unsigned block_size)
:child_set(child_set), child_in(child_in), schema_child_set(schema_child_set),
 schema_child_in(schema_child_in), index_child_set(index_child_set), index_child_in(index_child_in),
 block_size_(block_size), ht_nbuckets(1024), ht_bucketsize(64){

}

bool BlockStreamInIterator::open(const PartitionOffset& partition_offset)
{
	state_.child_set->open(partition_offset);
	state_.child_in->open(partition_offset);
	AtomicPushFreeHtBlockStream(BlockStreamBase::createBlock(state_.schema_child_set, state_.block_size_));
	AtomicPushFreeBlockStream(BlockStreamBase::createBlock(state_.schema_child_in, state_.block_size_));

	if (sema_open_.try_wait())
	{
		//initialize hash table, use the child_set to build hash table
		hash = PartitionFunctionFactory::createBoostHashFunction(state_.ht_nbuckets);
		vector<unsigned> ht_index;
		ht_index.push_back(state_.index_child_set);
		hashtable = new BasicHashTable(state_.ht_nbuckets, state_.ht_bucketsize, (state_.schema_child_set->getSubSchema(ht_index))->getTupleMaxSize());
		ht_index.clear();
		open_finished_ = true;
	}
	else
	{
		while (!open_finished_)
			usleep(1);
	}

	void* cur_tuple = NULL;
	void* tuple_in_hashtable = NULL;
	unsigned bn = 0;

	BlockStreamBase* bsb = AtomicPopFreeHtBlockStream();
	while (state_.child_set->next(bsb))
	{
		BlockStreamBase::BlockStreamTraverseIterator* bsti = bsb->createIterator();
		bsti->reset();
		while (cur_tuple = bsti->nextTuple())
		{
			bn = state_.schema_child_set->getcolumn(state_.index_child_set).operate->getPartitionValue(state_.schema_child_set->getColumnAddess(state_.index_child_set, cur_tuple), state_.ht_nbuckets);
			tuple_in_hashtable = hashtable->atomicAllocate(bn);
			state_.schema_child_set->getcolumn(state_.index_child_set).operate->assignment(state_.schema_child_set->getColumnAddess(state_.index_child_set, cur_tuple), tuple_in_hashtable);
		}
		bsb->setEmpty();
	}
	barrier_->Arrive();
	printf("-----------In Iterator Open Successful!-----------\n");
	return true;
}

bool BlockStreamInIterator::next (BlockStreamBase* block)
{
	unsigned bn;
	remaining_block rb;
	void* tuple_from_child_in = NULL;
	void* tuple_in_output_block = NULL;
	void* tuple_in_hashtable;
	void* key_in_input;
	bool passIn = false;
	BasicHashTable::Iterator hashtable_iterator = hashtable->CreateIterator();

	if (atomicPopRemainingBlock(rb))
	{
		while ((tuple_from_child_in = rb.blockstream_iterator->currentTuple()) > 0)
		{
			passIn = false;
			bn = state_.schema_child_in->getcolumn(state_.index_child_in).operate->getPartitionValue(state_.schema_child_in->getColumnAddess(state_.index_child_in, tuple_from_child_in), state_.ht_nbuckets);
			hashtable->placeIterator(hashtable_iterator, bn);
			while ((tuple_in_hashtable = hashtable_iterator.readnext()) > 0)
			{
				key_in_input = state_.schema_child_in->getColumnAddess(state_.index_child_in, tuple_from_child_in);
				if (state_.schema_child_in->getcolumn(state_.index_child_in).operate->equal(tuple_in_hashtable, key_in_input))
				{
					passIn = true;
					break;
				}
			}
			if (passIn)
			{
				const unsigned bytes = state_.schema_child_in->getTupleMaxSize();
				if ((tuple_in_output_block = block->allocateTuple(bytes)) > 0)
				{
					state_.schema_child_in->copyTuple(tuple_from_child_in, tuple_in_output_block);
					rb.blockstream_iterator->increase_cur_();
				}
				else {
					atomicPushRemainingBlock(rb);
					return true;
				}
			}
			else
				rb.blockstream_iterator->increase_cur_();
		}
		AtomicPushFreeBlockStream(rb.bsb_in_);
	}

	BlockStreamBase* block_for_asking = AtomicPopFreeBlockStream();
	block_for_asking->setEmpty();
	while (state_.child_in->next(block_for_asking))
	{
		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator = block_for_asking->createIterator();
		while ((tuple_from_child_in = traverse_iterator->currentTuple()) > 0)
		{
			passIn = false;
			bn = state_.schema_child_in->getcolumn(state_.index_child_in).operate->getPartitionValue(state_.schema_child_in->getColumnAddess(state_.index_child_in, tuple_from_child_in), state_.ht_nbuckets);
			hashtable->placeIterator(hashtable_iterator, bn);
			while ((tuple_in_hashtable = hashtable_iterator.readCurrent()) != 0)
			{
				key_in_input = state_.schema_child_in->getColumnAddess(state_.index_child_in, tuple_from_child_in);
				if (state_.schema_child_in->getcolumn(state_.index_child_in).operate->equal(tuple_in_hashtable, key_in_input))
				{
					passIn = true;
					break;
				}
				hashtable_iterator.increase_cur_();
			}
			if (passIn)
			{
				const unsigned bytes = state_.schema_child_in->getTupleMaxSize();
				if ((tuple_in_output_block = block->allocateTuple(bytes)) > 0)
				{
					state_.schema_child_in->copyTuple(tuple_from_child_in, tuple_in_output_block);
					traverse_iterator->increase_cur_();
				}
				else {
					atomicPushRemainingBlock(remaining_block(block_for_asking, traverse_iterator));
					return true;
				}
			}
			else
				traverse_iterator->increase_cur_();
		}
		traverse_iterator->~BlockStreamTraverseIterator();
		block_for_asking->setEmpty();
	}
	AtomicPushFreeBlockStream(block_for_asking);
	if(!block->Empty())
		return true;
	return false;
}

bool BlockStreamInIterator::close()
{
	sema_open_.post();
	open_finished_ = false;
//	barrier_->~Barrier();
	free_block_stream_list_.clear();
	ht_free_block_stream_list_.clear();
	remaining_block_list_.clear();
//	hash->~PartitionFunction();
	hashtable->~BasicHashTable();
	state_.child_set->close();
	state_.child_in->close();
	return true;
}




bool BlockStreamInIterator::atomicPopRemainingBlock(remaining_block & rb){
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

void BlockStreamInIterator::atomicPushRemainingBlock(remaining_block rb){
	lock_.acquire();
	remaining_block_list_.push_back(rb);
	lock_.release();
}

BlockStreamBase* BlockStreamInIterator::AtomicPopFreeBlockStream(){
	assert(!free_block_stream_list_.empty());
	lock_.acquire();
	BlockStreamBase *block=free_block_stream_list_.front();
	free_block_stream_list_.pop_front();
	lock_.release();
	return block;
}

void BlockStreamInIterator::AtomicPushFreeBlockStream(BlockStreamBase* block){
	lock_.acquire();
	free_block_stream_list_.push_back(block);
	lock_.release();
}

BlockStreamBase* BlockStreamInIterator::AtomicPopFreeHtBlockStream(){
	assert(!ht_free_block_stream_list_.empty());
	lock_.acquire();
	BlockStreamBase *block=ht_free_block_stream_list_.front();
	ht_free_block_stream_list_.pop_front();
	lock_.release();
	return block;
}

void BlockStreamInIterator::AtomicPushFreeHtBlockStream(BlockStreamBase* block){
	lock_.acquire();
	ht_free_block_stream_list_.push_back(block);
	lock_.release();
}
