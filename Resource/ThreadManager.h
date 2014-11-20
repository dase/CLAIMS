/*
 * ThreadManager.h
 *
 *  Created on: Sep 25, 2014
 *      Author: wangli
 */

#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_
#include <pthread.h>
#include <set>

class ThreadManager {
public:
	static ThreadManager* getInstance();
	virtual ~ThreadManager();
	void add(pthread_t t);
	void remove(pthread_t t);
	int getNumOfThreads()const;
private:
	ThreadManager();
	std::set<pthread_t> threads_;
	static ThreadManager* instance_;
};

#endif /* THREADMANAGER_H_ */
