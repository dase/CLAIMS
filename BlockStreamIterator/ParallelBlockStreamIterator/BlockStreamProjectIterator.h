/*
 * BlockStreamProjectIterator.h
 *
 *  Created on: 2014-2-17
 *      Author: casa
 */

#ifndef BLOCKSTREAMPROJECTITERATOR_H_
#define BLOCKSTREAMPROJECTITERATOR_H_

#include "../BlockStreamIteratorBase.h"
#include "../../utility/Expression.h"

#include <iostream>
#include <vector>
#include <map>
#include <list>
using namespace std;

class BlockStreamProjectIterator:public BlockStreamIteratorBase {
public:
	struct remaining_block{
		remaining_block(BlockStreamBase * bsb,BlockStreamBase::BlockStreamTraverseIterator * bsti_)
		:bsb_(bsb),bsti_(bsti){};
		remaining_block():bsb_(0),bsti_(0){};
//        void * combinedTuple_;
        BlockStreamBase * bsb_;
        BlockStreamBase::BlockStreamTraverseIterator * bsti_;
	};

	class State{
		friend class BlockStreamProjectIterator;
	public:
		State(Schema *input, Schema* output, BlockStreamIteratorBase * children, unsigned blocksize, Expression *expr);
		State(){};
	public:
		Schema *input_;
		Schema *output_;
		/* Recently, the expression is supporting the reduce the number of the input table!!!
		 * TODO: support the multi-to-multi between the input table and the select list, this expr
		 * is the result of the getIteratorTree to construct a schema. getDataflow() can generate a
		 * schema by using the SQLExpression and Expression can be computed by SQLExpression
		 * */
		Expression *expr_;
		unsigned block_size_;
		BlockStreamIteratorBase *children_;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & input_ & output_ & children_ & expr_ & block_size_;
		}
	};
	BlockStreamProjectIterator(State state);
	BlockStreamProjectIterator();
	virtual ~BlockStreamProjectIterator();

	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase *block);
	bool close();

private:
	bool atomicPopRemainingBlock(remaining_block & rb);
	void atomicPushRemainingBlock(remaining_block rb);
private:
	State state_;

	std::list<remaining_block> remaining_block_list_;
	std::list<BlockStreamBase *> free_block_stream_list_;

	Lock lock_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* BLOCKSTREAMPROJECTITERATOR_H_ */
