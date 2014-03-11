/*
 * synch.h
 *
 *  Created on: Mar 9, 2014
 *      Author: wangli
 */

#ifndef SYNCH_H_
#define SYNCH_H_


#include "../Block/synch.h"
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

class Lock
{
    pthread_mutex_t m;
public:
    Lock() {
        pthread_mutex_init(&m, NULL);
    }

    ~Lock() {
        pthread_mutex_destroy(&m);
    }

    void acquire() {
        pthread_mutex_lock(&m);
    }

    void release() {
        pthread_mutex_unlock(&m);
    }

    void destroy() {
    	pthread_mutex_destroy(&m);
    }
};

class semaphore
{
private:
    sem_t sem;
public:
    semaphore(int initialValue=0)
    {
        sem_init(&sem, 0, initialValue);
    }

    void set_value(int value){
    	sem_init(&sem, 0, value);
    }

    void post()
    {
        sem_post(&sem);
    }

    void wait()
    {
        sem_wait(&sem);
    }

    bool try_wait(){
    	return sem_trywait(&sem)==0;//if trywait() is successful, return 0, otherwise return -1.
    }

    void destroy(){
    	sem_destroy(&sem);
    }
    int get_value() {
    	int ret;
    	sem_getvalue(&sem,&ret);
    	return ret;
    }
};

class Barrier {
public:
	Barrier(int nThreads=0){
		m_nThreads = nThreads;
		int ret;
		ret = pthread_mutex_init(&m_l_SyncLock, NULL);
		if(ret!=0)
			printf("pthread_mutex_init failed at barrier creation.\n");
//		printf("the init of barrier\n");
		ret = pthread_cond_init(&m_cv_SyncCV, NULL);
		if(ret!=0)
			printf("pthread_cond_init failed at barrier creation.\n");

		m_nSyncCount = 0;
	}
	void RegisterOneThread(){
		pthread_mutex_lock(&m_l_SyncLock);
		m_nThreads++;
		pthread_mutex_unlock(&m_l_SyncLock);
//		printf("Barrier:: new thread registered!\n\n\n");
	}

	virtual ~Barrier() {
		pthread_mutex_destroy(&m_l_SyncLock);
		pthread_cond_destroy(&m_cv_SyncCV);
	}
	void setEmpty(){
		m_nThreads=0;
		m_nSyncCount=0;
	}

	/*
	 * One must call setEmpty() before the second's calling of Arrive()
	 */
	void Arrive(){
		pthread_mutex_lock(&m_l_SyncLock);
		m_nSyncCount++;
//		printf("cpu processor test: %d\n",m_nSyncCount);
		if(m_nSyncCount == m_nThreads) {
//			printf("arrive the nthreads\n");
//			printf("arrive the broadcast\n");
			pthread_cond_broadcast(&m_cv_SyncCV);
		}
		else {
//			printf("arrive the wait\n");
			pthread_cond_wait(&m_cv_SyncCV, &m_l_SyncLock);

		}

		pthread_mutex_unlock(&m_l_SyncLock);
	}

private:
	int             m_nThreads;
	pthread_mutex_t m_l_SyncLock;
	pthread_cond_t  m_cv_SyncCV;
	volatile int    m_nSyncCount;
};
#endif /* SYNCH_H_ */
