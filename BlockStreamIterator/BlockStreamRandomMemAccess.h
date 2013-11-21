/*
 * BlockStreamRandomMemAccess.h
 *
 *  Created on: 2013年9月9日
 *      Author: SCDONG
 */

#ifndef BLOCKSTREAMRANDOMMEMACCESS_H_
#define BLOCKSTREAMRANDOMMEMACCESS_H_

#include <vector>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include "BlockStreamIteratorBase.h"
#include "../Schema/Schema.h"
#include "../Block/BlockStream.h"

class BlockStreamRandomMemAccess :public BlockStreamIteratorBase {
public:
	class State
	{
		friend class BlockStreamRandomMemAccess;
	public:
		State(std::string file_name, BlockStreamIteratorBase* child, Schema* schema_in_, Schema* schema_out_, unsigned block_size);
	private:
		std::string filename_;
		BlockStreamIteratorBase* child_;
		Schema* schema_in_;
		Schema* schema_out_;
		unsigned block_size_;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & filename_ & child_ & schema_in_ & schema_out_ & block_size_;
		}
	};
public:
	BlockStreamRandomMemAccess(State state);
	virtual ~BlockStreamRandomMemAccess();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase *block);
	bool close();
private:
	State state_;
	BlockStreamBase* block_for_asking_;
	BlockStreamBase::BlockStreamTraverseIterator* block_for_asking_iterator_;

	int fd_;
	void* base_;
	char* data_;
	long file_length_;
	long oid_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
};


#endif /* BLOCKSTREAMRANDOMMEMACCESS_H_ */
