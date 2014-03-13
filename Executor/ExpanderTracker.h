/*
 * ExpanderTracker.h
 *
 *  Created on: Mar 12, 2014
 *      Author: wangli
 */

#ifndef EXPANDERTRACKER_H_
#define EXPANDERTRACKER_H_

#include <pthread.h>
#include <boost/unordered_map.hpp>
#include <map>
#include "../utility/synch.h"
typedef pthread_t expanded_thread_id;

struct ExpandedThreadStatus{
	bool call_back_;
};

class ExpanderTracker {
public:
	static ExpanderTracker* getInstance();
	virtual ~ExpanderTracker();
	/*
	 * Call this method when a new expanded thread is created, and the
	 * expander tracker will maintain a status of this thread.
	 */
	bool registerNewExpandedThreadStatus(expanded_thread_id id);

	/*
	 * Call this method just before a expanded thread finishes its work
	 * such that the expander tracker could delete its status.
	 */
	bool deleteExpandedThreadStatus(expanded_thread_id id);

	/*
	 * Check whether an expanded thread specified by expanded_thread_id
	 * has callback request. Return false when the given expanded thread
	 * id is not in the list.
	 */
	bool isExpandedThreadCallBack(expanded_thread_id id);

	/*
	 * Call this method if you want to callback the expanded thread specified
	 * by the expanded thread id. Return false if the thread id does not exists
	 * in expander tracker or the thread has be called back.
	 */
	bool callbackExpandedThread(expanded_thread_id id);
private:
	ExpanderTracker();
private:
	static ExpanderTracker* instance_;

	Lock lock_;
	/*
	 * A unordered map from expanded thread id to expanded thread status
	 */
public://for debug, this should be private!
	std::map<expanded_thread_id,ExpandedThreadStatus> id_to_status_;
};

#endif /* EXPANDERTRACKER_H_ */
