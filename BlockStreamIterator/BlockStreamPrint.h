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
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "BlockStreamIteratorBase.h"
#include "../common/Schema/Schema.h"
class BlockStreamPrint:public BlockStreamIteratorBase{
public:
	struct State{
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
		std::string spliter_;
		std::vector<std::string> attribute_name_list_;
		State(){};
		~State();
		State( Schema* const & schema,BlockStreamIteratorBase*const& child,const unsigned& block_size,std::vector<std::string> attribute_name,std::string spliter="\t")
		:schema_(schema),child_(child),block_size_(block_size),spliter_(spliter),attribute_name_list_(attribute_name){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ &block_size_& spliter_ & attribute_name_list_;
		}
	};
	BlockStreamPrint();
	BlockStreamPrint(State state);
	virtual ~BlockStreamPrint();
	bool open(const PartitionOffset& offset);
	bool next(BlockStreamBase* block);
	bool close();
	void print();
private:
	State state_;
	BlockStreamBase* block_buffer_;
	std::vector<int> column_wides;

	unsigned long tuple_count_;
	/* for boost::serializtion*/
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar &  boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* BLOCKSTREAMPRINT_H_ */
