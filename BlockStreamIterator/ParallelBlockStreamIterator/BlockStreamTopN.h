/*
 * BlockStreamTopN.h
 *
 *  Created on: Dec 31, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMTOPN_H_
#define BLOCKSTREAMTOPN_H_

#include "../BlockStreamIteratorBase.h"

class BlockStreamTopN: public BlockStreamIteratorBase {
public:
	struct State{
		friend class BlockStreamTopN;
	public:
		State(Schema* schema,BlockStreamIteratorBase* child,unsigned limits,unsigned block_size);
		State();
	private:
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned limits_;
		unsigned block_size_;
	private:
	    friend class boost::serialization::access;
	    template<class Archive>
	    void serialize(Archive & ar, const unsigned int version){
	            ar & schema_ & child_ & limits_ & block_size_;
	    }
	};
	BlockStreamTopN();
	BlockStreamTopN(State state);
	virtual ~BlockStreamTopN();
	bool open(const PartitionOffset&);
	bool next(BlockStreamBase*);
	bool close();
private:
	State state_;
	unsigned received_tuples_;
	BlockStreamBase* block_for_asking_;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
            ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
    }
};

#endif /* BLOCKSTREAMTOPN_H_ */
