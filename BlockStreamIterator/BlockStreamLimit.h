/*
 * BlockStreamTopN.h
 *
 *  Created on: Dec 31, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMTOPN_H_
#define BLOCKSTREAMTOPN_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "BlockStreamIteratorBase.h"

class BlockStreamLimit: public BlockStreamIteratorBase {
public:
	struct State{
		friend class BlockStreamLimit;
	public:
		State(Schema* schema,BlockStreamIteratorBase* child,unsigned long limits,unsigned block_size,unsigned long start_position=0);
		State();
	private:
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned long limits_;
		unsigned block_size_;
		unsigned long start_position_;
	private:
	    friend class boost::serialization::access;
	    template<class Archive>
	    void serialize(Archive & ar, const unsigned int version){
	            ar & schema_ & child_ & limits_ & block_size_ & start_position_;
	    }
	};
	BlockStreamLimit();
	BlockStreamLimit(State state);
	virtual ~BlockStreamLimit();
	bool open(const PartitionOffset&);
	bool next(BlockStreamBase*);
	bool close();
private:
	inline bool limitExhausted()const{
		return received_tuples_>=state_.limits_;
	}
	inline bool shouldSkip()const{
		return tuple_cur_<state_.start_position_;
	}
private:
	State state_;
	unsigned received_tuples_;
	BlockStreamBase* block_for_asking_;

	/* the index of current tuple*/
	unsigned long tuple_cur_;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
            ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
    }
};

#endif /* BLOCKSTREAMTOPN_H_ */
