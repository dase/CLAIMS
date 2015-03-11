/*
 * ThreadPool.h
 *
 *  Created on: 2014-8-17
 *      Author: yukai
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <iostream>
#include <pthread.h>
#include <queue>
#include <semaphore.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "./common/Logging.h"

using namespace std;

static bool g_thread_pool_used = true;
//static bool g_thread_pool_used = false;

class ThreadPool
{
private:
	struct Task{
		Task();
		Task(void (*f)(void *), void *a, bool e):func(f),arg(a),end(e){}

		void (*func)(void *arg);	// pointer to function
		void *arg;						// the parameter of function
		bool end;						// whether exit thread
		static void destroy_task(Task *task){
			// todo:the arg has no specified type
//			delete task->arg;

			delete task;
		};
	};
public:
	ThreadPool();
	virtual ~ThreadPool();

	bool Thread_Pool_init(int thread_count_in_pool_);
	static void *thread_exec(void *arg);
	void *thread_exec_with_cond(void *arg);
	void bind_cpu();

	//arg can't be a class, because delete void * won't execute destructor function
	void add_task(void (*)(void *), void *arg, bool e = false);
	static void destroy_pool(ThreadPool * tp);

private:
//	pthread_cond_t free_thread_cond;
//	pthread_mutex_t cond_lock;

//	attfinity_start_cpu;

	sem_t undo_task_sem;	// undo task count

	pthread_t *thread_list_;

	queue<Task*> task_queue_;
	pthread_mutex_t task_queue_lock;

	int thread_count;
	int free_thread_count;
	pthread_mutex_t free_thread_count_lock;

	int task_count;
	int undo_task_count;
	pthread_mutex_t undo_task_count_lock;

};

#endif /* THREADPOOL_H_ */
