/*
 * ExpandableBlockStreamBuffer.h
 *
 *  Created on: Dec 20, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMBUFFER_H_
#define EXPANDABLEBLOCKSTREAMBUFFER_H_

#include "BlockStreamIteratorBase.h"
#include "../Schema/Schema.h"
#include "../Block/DynamicBlockBuffer.h"
class BlockStreamResultCollector:public BlockStreamIteratorBase {
public:

	struct State{
		friend class BlockStreamResultCollector;
	public:
		State(Schema* input,BlockStreamIteratorBase* child,const unsigned block_size,std::vector<std::string> column_header=std::vector<std::string>(),const PartitionOffset partitoin_offset=0);
		State();
	private:
		Schema* input_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
		PartitionOffset partition_offset_;
		std::vector<std::string> column_header_;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & input_ & child_ & block_size_ & partition_offset_ & column_header_;
		}
	};
	BlockStreamResultCollector();
	BlockStreamResultCollector(State);
	virtual ~BlockStreamResultCollector();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase* block);
	bool close();
	void print();
	ResultSet* getResultSet();
	unsigned long getNumberOftuples()const;
private:
	bool createBlockStream(BlockStreamBase*&)const;
	void deallocateBlockStream(BlockStreamBase*&)const;
	bool ChildExhausted();
	static void* worker(void* arg);
private:
	State state_;
//	DynamicBlockBuffer block_buffer_;
//	DynamicBlockBuffer::Iterator block_buffer_iterator_;
	ResultSet block_buffer_;
	ResultSet::Iterator block_buffer_iterator_;
	unsigned finished_thread_count_;
	unsigned registered_thread_count_;
	semaphore sema_open_;

	volatile bool open_finished_;
	semaphore sema_open_finished_;
	semaphore sema_input_complete_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar &  boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_ ;
	}
};

#endif /* EXPANDABLEBLOCKSTREAMBUFFER_H_ */
