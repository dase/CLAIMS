/*
 * ExpandableBlockStreamIteratorBase.h
 *
 *  Created on: Mar 8, 2014
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMITERATORBASE_H_
#define EXPANDABLEBLOCKSTREAMITERATORBASE_H_


#include "BlockStreamIteratorBase.h"
#include "../utility/synch.h"
class ExpandableBlockStreamIteratorBase: public BlockStreamIteratorBase {
public:
	ExpandableBlockStreamIteratorBase();
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
	/* this function is called by the threads which wait for the finish of the open call
	 *  by other threads*/
	void waitForOpenFinished();

	/**
	 * Thin method is called when the thread which takes the responsibility of initializing
	 * finished the open to wake up all the pending threads waiting for the open finishing.
	 */
	void broadcaseOpenFinishedSignal();

	/* this function initialize the state of expanded iterator.
	 * Should be called in the constructor and close() of the expanded iterator implementation.
	 */
	void initialize_expanded_status();

	/* Return true, if the current thread call this method first; otherwise, return false*/
	bool completeForInitializationJob();

	void setOpenReturnValue(bool value);
	bool getOpenReturnValue()const;
protected:
	/* the return value of open() */
	volatile bool open_ret_;

	/* whether open is finished or not*/
	volatile bool open_finished_;

	pthread_mutex_t sync_lock_;
	pthread_cond_t  sync_cv_;

	/* the semaphore used to guarantee that only the first thread does the real initialization work.*/
	semaphore sema_compete_open_;


private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) ;
	}
};

#endif /* EXPANDABLEBLOCKSTREAMITERATORBASE_H_ */
