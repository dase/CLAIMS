/*
 * BlockStreamExpender.h
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMEXPANDER_H_
#define BLOCKSTREAMEXPANDER_H_
#include <pthread.h>
#include <vector>
#include "../BlockStreamIteratorBase.h"
#include "../../Schema/Schema.h"
#include "../../Block/BlockStreamBuffer.h"
class BlockStreamExpander:public BlockStreamIteratorBase {
public:
	class State{
	public:
		friend class BlockStreamExpander;
		State(Schema* schema,BlockStreamIteratorBase* child,unsigned thread_count,unsigned block_size, unsigned block_count_in_buffer);
		State(){};
	private:
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned thread_count_;
		unsigned block_size_;
		unsigned block_count_in_buffer_;
		PartitionOffset partition_offset;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ & thread_count_ & block_size_ & block_count_in_buffer_;
		}
	};
	BlockStreamExpander(State state);
	BlockStreamExpander();
	virtual ~BlockStreamExpander();
	bool open(const PartitionOffset& partitoin_offset=0);
	bool next(BlockStreamBase* block);
	bool close();
private:
	static void* expanded_work(void* arg);
	bool ChildExhausted();

private:
	State state_;
	std::vector<pthread_t> expanded_thread_list_;
	BlockStreamBuffer* block_stream_buffer_;
	volatile unsigned finished_thread_count_;
	/*
	 * The following code is for boost serialization.
	 */
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) &state_;
	}
};

#endif /* BLOCKSTREAMEXPENDER_H_ */
