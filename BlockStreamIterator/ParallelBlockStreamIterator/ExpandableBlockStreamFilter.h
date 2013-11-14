/*
 * ExpandableBlockStreamFilter.h
 *
 *  Created on: Aug 28, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMFILTER_H_
#define EXPANDABLEBLOCKSTREAMFILTER_H_
#include <list>
#include "../BlockStreamIteratorBase.h"
#include "../../Schema/Schema.h"
#include "../../Comparator.h"
#include "../../iterator/FilterIterator.h"
#include "../../Block/BlockStream.h"
#include "../../Block/synch.h"
class ExpandableBlockStreamFilter:public BlockStreamIteratorBase {
public:

	/* struct to hold the remaining data when the next is returned but the block from the child
	 *  iterator is not exhausted.*/
	struct remaining_block{
		remaining_block(BlockStreamBase* block,BlockStreamBase::BlockStreamTraverseIterator* iterator):block(block),iterator(iterator){};
		remaining_block():block(0),iterator(0){};
		BlockStreamBase* block;
		BlockStreamBase::BlockStreamTraverseIterator* iterator;
	};


	class State{
	public:
		friend class ExpandableBlockStreamFilter;
		State(Schema* s, BlockStreamIteratorBase* child,std::vector<FilterIterator::AttributeComparator> comparator_list,unsigned block_size );
		State(){};
	public:
		Schema* schema_;
		std::vector<FilterIterator::AttributeComparator> comparator_list_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & comparator_list_ & child_ & block_size_;
		}
	};


	ExpandableBlockStreamFilter(State state);
	ExpandableBlockStreamFilter();
	virtual ~ExpandableBlockStreamFilter();
	bool open();
	bool next(BlockStreamBase* block);
	bool close();

private:
	bool atomicPopRemainingBlock(remaining_block & rb);
	void atomicPushRemainingBlock(remaining_block rb);
	BlockStreamBase* AtomicPopFreeBlockStream();
	void AtomicPushFreeBlockStream(BlockStreamBase* block);
private:
	State state_;
	std::list<remaining_block> remaining_block_list_;
	std::list<BlockStreamBase*> free_block_stream_list_;
	semaphore sem_open_;
	volatile bool open_finished_;
	Lock lock_;
	/* the following code is for boost serialization*/
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) &state_;
	}
};

#endif /* EXPANDABLEBLOCKSTREAMFILTER_H_ */
