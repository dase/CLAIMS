/*
 * ExpandableBlockStreamIteratorBase.h
 *
 *  Created on: Mar 8, 2014
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMITERATORBASE_H_
#define EXPANDABLEBLOCKSTREAMITERATORBASE_H_


#include <boost/unordered/unordered_map.hpp>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "BlockStreamIteratorBase.h"
#include "../utility/lock.h"
#include "../common/hashtable.h"

using boost::unordered::unordered_map;
class thread_context{
public:
	virtual ~thread_context(){

	}
};
typedef int barrier_number;
typedef int serialized_section_number;
class ExpandableBlockStreamIteratorBase: public BlockStreamIteratorBase {
public:

	ExpandableBlockStreamIteratorBase(unsigned number_of_barrier=1,unsigned number_of_seriliazed_section=1);
	virtual ~ExpandableBlockStreamIteratorBase();
	/**
	 * Because that boost::serialization will allocate an instance of this class
	 * when deserializing, and hence the following three virtual method cannot be
	 * pure.
	 */
	virtual bool open(const PartitionOffset& part_off=0){assert(false);};
	virtual bool next(BlockStreamBase*){assert(false);};
	virtual bool close(){assert(false);};

protected:


	/* this function initialize the state of expanded iterator.
	 * Should be called in the constructor and close() of the expanded iterator implementation.
	 */
	void initialize_expanded_status();

	/* Return true, if not any thread has obtain the entry into the serialized section
	 * with id=tserialized_section_id; otherwise, return false*/
	bool tryEntryIntoSerializedSection(unsigned serialized_section_id=0);

	/*
	 * Register to all the barriers that a new thread has been registered. Accordingly, barriers
	 * could increase the expected number of threads by 1.
	 */
	void RegisterExpandedThreadToAllBarriers();

	/*
	 * When an expanded thread is about to exit, call this method before exit to remove the count
	 * in barriers.
	 * Please note that
	 */
	void unregisterExpandedThreadToAllBarriers(unsigned barrier_index=0);

	void barrierArrive(unsigned barrier_index=0);

	void initContext(thread_context* tc);
	thread_context* getContext();

	/*
	 * This method is called when a thread wants its and all the others' context
	 */
	void destoryAllContext( );

	/*
	 * This method is call when a thread wants to destroy its own context
	 */
	void destorySelfContext();

	bool checkTerminateRequest();

	/*
	 * This function may be called by multiple expanded threads to update the return value
	 * for the open function.
	 * If any error happens at any error, the open_ret_ should be false.
	 */
	void setReturnStatus(bool ret);

	bool getReturnStatus()const;
protected:
	unsigned number_of_registered_expanded_threads_;

private:
	/* the return value of open() */
	volatile bool open_ret_;


	pthread_mutex_t sync_lock_;
	pthread_cond_t  sync_cv_;

	/* the semaphore used to guarantee that only the first thread does the real initialization work.*/
	semaphore* seriliazed_section_entry_key_;

	Barrier* barrier_;

	unsigned number_of_barrier_;
	unsigned number_of_seriliazed_section_;

	Lock lock_number_of_registered_expanded_threads_;


	boost::unordered_map<pthread_t,thread_context*> context_list_;
	Lock context_lock_;


private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this);
	}
};

#endif /* EXPANDABLEBLOCKSTREAMITERATORBASE_H_ */
