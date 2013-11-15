/*
 * BlockStreamPrint.h
 *
 *  Created on: Nov 19, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMPRINT_H_
#define BLOCKSTREAMPRINT_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "BlockStreamIteratorBase.h"
#include "../Schema/Schema.h"
class BlockStreamPrint:public BlockStreamIteratorBase{
public:
	struct State{
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
		std::string spliter_;
		State(){};
		State( Schema* const & schema,BlockStreamIteratorBase*const& child,const unsigned& block_size,std::string spliter="|")
		:schema_(schema),child_(child),block_size_(block_size),spliter_(spliter){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ &block_size_& spliter_;
		}
	};
	BlockStreamPrint();
	BlockStreamPrint(State state);
	virtual ~BlockStreamPrint();
	bool open(const ProjectionOffset& offset);
	bool next(BlockStreamBase* block);
	bool close();
private:
	State state_;
	BlockStreamBase* block_buffer_;
	/* for boost::serializtion*/
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar &  boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* BLOCKSTREAMPRINT_H_ */
