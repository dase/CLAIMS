/*
 * lock.h
 *
 *  Created on: Feb 20, 2014
 *      Author: wangli
 */

#ifndef LOCK_H_
#define LOCK_H_



#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>

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

    bool try_acquire(){
    	return pthread_mutex_trylock(&m)==0;
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

    void post(int times=1){
    	for(int i=0;i<times;i++){
    		sem_post(&sem);
    	}
    }

    void wait()
    {
        sem_wait(&sem);
    }

    /*
     * The timed_wait() shall return true if the calling process/thread successfully
     * performed the semaphore lock operation on the semaphore. If the calling is
     * unsuccessful, the state of the lock shall be unchanged and the function shall
     * return false.
     */
    bool timed_wait(int millisecond){
    	timespec time;
    	time.tv_sec=millisecond/1000;
    	millisecond=millisecond%(1000);
    	time.tv_nsec=millisecond*(long)1000*1000;
    	return sem_timedwait(&sem,&time)==0;
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
	void UnregisterOneThread(){
		pthread_mutex_lock(&m_l_SyncLock);
		m_nThreads--;
		assert(m_nThreads>=0);
		if(m_nThreads<=m_nSyncCount){
			pthread_cond_broadcast(&m_cv_SyncCV);
		}
		pthread_mutex_unlock(&m_l_SyncLock);
	}

	virtual ~Barrier() {
		pthread_mutex_destroy(&m_l_SyncLock);
		pthread_cond_destroy(&m_cv_SyncCV);
	}
	void setEmpty(){
		m_nThreads=0;
		m_nSyncCount=0;
	}

	void reset(){
		m_nSyncCount=0;
	}
	/*
	 * One must call setEmpty() before the second's calling of Arrive()
	 */
	void Arrive(){
		pthread_mutex_lock(&m_l_SyncLock);
		m_nSyncCount++;
//		printf("cpu processor test: %d\n",m_nSyncCount);
		if(m_nSyncCount >= m_nThreads) {
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

/**
 * Non-recursive spinlock. Using `xchg` and `ldstub` as in PostgresSQL.
 */
class SpineLock {
	public:
	SpineLock() : _l(0) { }

		/** Call blocks and retunrs only when it has the lock. */
		inline void acquire()
		{
			while(tas(&_l)) {
#if defined(__i386__) || defined(__x86_64__)
				__asm__ __volatile__ ("pause\n");
#endif
			}
		}
		inline bool try_lock()
		{
			return !tas(&_l);
		}
		inline int fakelock()
		{
			return _l==0?0:1;
		}
		inline int getvalue()
		{
			//return _l==0?0:1;
			return 0;
		}

		/** Unlocks the lock object. */
		inline void release()
		{
			_l = 0;
		}

	private:
		inline int tas(volatile char* lock)
		{
			register char res = 1;
#if defined(__i386__) || defined(__x86_64__)
			__asm__ __volatile__ (
					"lock xchgb %0, %1\n"
					: "+q"(res), "+m"(*lock)
					:
					: "memory", "cc");
#elif defined(__sparc__)
			__asm__ __volatile__ (
					"ldstub [%2], %0"
					: "=r"(res), "+m"(*acquire)
					: "r"(acquire)
					: "memory");
#else
#error TAS not defined for this architecture.
#endif
			return res;
		}

		volatile char _l ;//__attribute__((aligned(64)))

};
#endif /* LOCK_H_ */
