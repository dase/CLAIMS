/*
 * BlockStreamNestLoopJoinIterator.h
 *
 *  Created on: Jul 14, 2014
 *      Author: imdb
 */

#ifndef BLOCKSTREAMNESTLOOPJOINITERATOR_H_
#define BLOCKSTREAMNESTLOOPJOINITERATOR_H_
#include "../ExpandableBlockStreamIteratorBase.h"
#include <boost/serialization/base_object.hpp>


class BlockStreamNestLoopJoinIterator: public ExpandableBlockStreamIteratorBase {
public:
	class join_thread_context:public thread_context{
	public:
		BlockStreamBase* block_for_asking_;
		BlockStreamBase::BlockStreamTraverseIterator* block_stream_iterator_;
	};
	class State
	{
		friend class BlockStreamNestLoopJoinIterator;
	public:
		State(BlockStreamIteratorBase *child_left,
				BlockStreamIteratorBase *child_right,
				Schema *input_schema_left,
				Schema *input_schema_right,
				Schema *output_schema,
				unsigned block_size
				);
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & child_left & child_right & input_schema_left & input_schema_right & output_schema & block_size_;
	}
	public:
		BlockStreamIteratorBase *child_left,*child_right;
		Schema *input_schema_left,*input_schema_right;
		Schema *output_schema;
		unsigned block_size_;
	};
	BlockStreamNestLoopJoinIterator();
	virtual ~BlockStreamNestLoopJoinIterator();
	BlockStreamNestLoopJoinIterator(State state);
	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase *block);
	bool close();
	void print();
private:
	State state_;
	Lock lock_;
	unsigned produced_tuples;
	unsigned consumed_tuples_from_right;
	unsigned consumed_tuples_from_left;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
            ar & boost::serialization::base_object<ExpandableBlockStreamIteratorBase>(*this) & state_;
    }

};

#endif /* BLOCKSTREAMNESTLOOPJOINITERATOR_H_ */
