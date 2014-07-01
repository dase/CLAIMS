/*
 * ExpandableBlockStreamRandomMemAccess.h
 *
 *  Created on: 2013年9月16日
 *      Author: SCDONG
 */

#ifndef EXPANDABLEBLOCKSTREAMRANDOMMEMACCESS_H_
#define EXPANDABLEBLOCKSTREAMRANDOMMEMACCESS_H_

#include <string>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "../BlockStreamIteratorBase.h"
#include "../../common/Schema/Schema.h"
#include "../../utility/lock.h"
class ExpandableBlockStreamRandomMemAccess:public BlockStreamIteratorBase {
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
	friend class ExpandableBlockStreamRandomMemAccess;
	public:
		State(std::string file_name,BlockStreamIteratorBase* child, Schema* f_schema, Schema* c_schema, unsigned block_size);
		State(){};
	private:
		std::string filename_;
		BlockStreamIteratorBase* child_;
		Schema* f_schema_;
		Schema* c_schema_;
		unsigned block_size_;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & f_schema_ & c_schema_ & filename_ & child_ & block_size_;
		}
	};
	ExpandableBlockStreamRandomMemAccess(State state);
	ExpandableBlockStreamRandomMemAccess();
	virtual ~ExpandableBlockStreamRandomMemAccess();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase* block);
	bool close();
private:
	/* allocate a block from the underling storage, return false if the underly
	 * data is exhausted.
	 */
	bool atomicPopRemainingBlock(remaining_block & rb);
	void atomicPushRemainingBlock(remaining_block rb);
	BlockStreamBase* AtomicPopFreeBlockStream();
	void AtomicPushFreeBlockStream(BlockStreamBase* block);
private:

	State state_;

	/* the semaphore used to guarantee that only one thread does the real work
	 * in the open function the open function
	 */
	semaphore sema_open_;

	volatile bool open_finished_;

	std::list<remaining_block> remaining_block_list;
	std::list<BlockStreamBase*> free_block_stream_list_;

	int fd_;
	unsigned long file_length_;
	char* base_;
	char* data_;
	Lock lock_;

	/*
	 * The following code is for boost serialization.
	 */
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* EXPANDABLEBLOCKSTREAMRANDOMMEMACCESS_H_ */
