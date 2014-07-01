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
#include <set>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../BlockStreamIteratorBase.h"
#include "../../common/Schema/Schema.h"
#include "../../common/Block/BlockStreamBuffer.h"
#include "../../utility/ExpandabilityShrinkability.h"
#include "../../common/Logging.h"
#include "../../utility/lock.h"

#define EXPANDER_BUFFER_SIZE 100

class BlockStreamExpander:public BlockStreamIteratorBase,public ExpandabilityShrinkability {
public:
	class State{
	public:
		friend class BlockStreamExpander;
		State(Schema* schema,BlockStreamIteratorBase* child,unsigned thread_count,unsigned block_size, unsigned block_count_in_buffer=10);
		State(){};
	public:
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned init_thread_count_;
		unsigned block_size_;
		unsigned block_count_in_buffer_;
		PartitionOffset partition_offset;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ & init_thread_count_ & block_size_ & block_count_in_buffer_;
		}
	};
	BlockStreamExpander(State state);
	BlockStreamExpander();
	virtual ~BlockStreamExpander();
	bool open(const PartitionOffset& partitoin_offset=0);
	bool next(BlockStreamBase* block);
	bool close();
	void print();
private:
	static void* expanded_work(void* arg);
	static void* coordinate_work(void* arg);
	bool ChildExhausted();
	bool createNewExpandedThread();
	void terminateExpandedThread(pthread_t pid);

	bool Expand();

	bool Shrink();

	void addIntoInWorkingExpandedThreadList(pthread_t pid);
	bool removeFromInWorkingExpandedThreadList(pthread_t pid);

	void addIntoBeingCalledBackExpandedThreadList(pthread_t pid);
	void removeFromBeingCalledBackExpandedThreadList(pthread_t pid);

	unsigned getDegreeOfParallelism();
private:
	State state_;

	/*
	 * The set of threads that are working normally.
	 */
	std::set<pthread_t> in_work_expanded_thread_list_;

	pthread_t coordinate_pid_;

	ExpanderID expander_id_;

	Lock exclusive_expanding_;
	/*
	 * The set of threads that have been called back but have not
	 * finished the remaining work yet.
	 */
	std::set<pthread_t> being_called_bacl_expanded_thread_list_;

	BlockStreamBuffer* block_stream_buffer_;
	volatile unsigned finished_thread_count_;
	volatile unsigned thread_count_;

	/*
	 * whether at least one work thread has successfully finished!
	 */
	volatile bool input_data_complete_;

	volatile bool one_thread_finished_;

	/*
	 * this is a map storing the semaphore pointer for each thread to shrink.
	 * the shrink() is blocked by sema until the thread is successful shrunk.
	 */
	std::map<pthread_t,semaphore*> tid_to_shrink_semaphore;

	Lock lock_;
	/*
	 * The following code is for boost serialization.
	 */

	unsigned long int received_tuples_;

	Logging* logging_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) &state_;
	}
};

#endif /* BLOCKSTREAMEXPENDER_H_ */
