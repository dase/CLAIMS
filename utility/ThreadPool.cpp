/*
 * ThreadPool.cpp
 *
 *  Created on: 2014-8-17
 *      Author: yukai
 *
 *      TODO: if task is more than thread, create some thread that execute
 *function once
 *		可以试着添加负载均衡
 *		如果每个线程需要分配内存，为了节省内存，可以设置线程第一次运行才分配内存，优先让已经分配到内存的线程运行
 *
 */
#include "ThreadPool.h"
#include <unistd.h>
#include <sys/syscall.h>
#define __USE_GNU  // 启用CPU_ZERO等相关的宏

ThreadPool::ThreadPool() {}

ThreadPool::~ThreadPool() {}

bool ThreadPool::Thread_Pool_init(int thread_count_in_pool_) {
  bool success = true;
  thread_count = thread_count_in_pool_;
  free_thread_count = 0;
  undo_task_count = 0;

  pthread_mutex_init(&free_thread_count_lock, NULL);
  pthread_mutex_init(&undo_task_count_lock, NULL);
  pthread_mutex_init(&task_queue_lock, NULL);

  sem_init(&undo_task_sem, 0, 0);  // init semaphore

  thread_list_ = (pthread_t *)malloc(thread_count_in_pool_ * sizeof(pthread_t));
  while (!task_queue_.empty()) {
    task_queue_.pop();
  }

  for (int i = 0; i < thread_count; ++i) {
    if (pthread_create(&thread_list_[i], NULL, thread_exec, this) !=
        0) {  // if any failed, return false
      cout << "ERROR: create pthread failed!" << endl;
      success = false;
      break;
    }
    ++free_thread_count;
  }
  assert(free_thread_count == thread_count);
  // ThreadPoolLogging::log("thread pool init %d free thread\n",
  // free_thread_count);
  return success;
}

// TODO：
// 可以把f与a封装为一个类对象，比如Task，不同的任务可以继承Task，Task中有run函数，Task由智能指针管理销毁
void ThreadPool::add_task(void *(*f)(void *), void *a, bool e) {
  Task *t = new Task(f, a, e);
  pthread_mutex_lock(&task_queue_lock);
  task_queue_.push(t);
  pthread_mutex_unlock(&task_queue_lock);

  sem_post(&undo_task_sem);
}

void *ThreadPool::thread_exec(void *arg) {
  ThreadPool *thread_pool = (ThreadPool *)arg;
  Task *task = NULL;

  thread_pool->bind_cpu();

  // every thread execute a endless loop, waiting for task, and exit when
  // receive a task with end member of 'true'
  while (1) {
    sem_wait(&(thread_pool->undo_task_sem));

    pthread_mutex_lock(&(thread_pool->task_queue_lock));
    if (!thread_pool->task_queue_.empty()) {
      task = thread_pool->task_queue_.front();
      thread_pool->task_queue_.pop();
    }
    pthread_mutex_unlock(&(thread_pool->task_queue_lock));

    if (task != NULL) {
      if (task->end)  // it means destory this thread
        break;
      ThreadPoolLogging::log(
          "thread (id=%ld,offset=%lx) in thread pool is executing..\n",
          syscall(__NR_gettid), pthread_self());
      (*(task->func))(task->arg);
      ThreadPoolLogging::log(
          "thread (id=%ld,offset=%lx) in thread pool finished executing..\n",
          syscall(__NR_gettid), pthread_self());

      Task::destroy_task(task);  //  TODO: consider whether destroy task
      task = NULL;
    }

    //  sem_post(&task_sem);
  }
  pthread_exit(NULL);
  return NULL;
}
/*
void *ThreadPool::thread_exec_with_cond(void *arg){
        ThreadPool *thread_pool = (ThreadPool*)arg;
        Task *task = new Task();

        while (1){
                pthread_mutex_lock(&cond_lock);
                while (free_thread_count == 0){
                        pthread_cond_wait(&free_thread_cond, &cond_lock);
                }

                pthread_mutex_lock(&free_thread_count_lock);
                --free_thread_count;
                pthread_mutex_unlock(&free_thread_count_lock);

                pthread_mutex_unlock(&cond_lock);

                pthread_mutex_lock(&task_queue_lock);
                if (!thread_pool->task_queue_.empty()){
                        task = thread_pool->task_queue_.pop();
                }
                pthread_mutex_unlock(&task_queue_lock);

                (*(task->func))(task->args);

                pthread_mutex_lock(&free_thread_count_lock);
                ++free_thread_count;
                pthread_mutex_unlock(&free_thread_count_lock);
        }

}
*/

void ThreadPool::bind_cpu() {
  //将该子线程的状态设置为detached,则该线程运行结束后会自动释放所有资源,不要使父线程因为调用pthread_join而阻塞
  pthread_detach(pthread_self());

  static volatile int current_cpu = 0;
  int cpu_count = sysconf(_SC_NPROCESSORS_CONF);
  int insert_cpu = __sync_fetch_and_add(&current_cpu, 1) % cpu_count;

  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(insert_cpu, &mask);
  int ret = pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
  if (ret == -1) {
    ThreadPoolLogging::elog("thread %ld bind cpu failed,ret = %d. %s",
                            syscall(__NR_gettid), ret, strerror(errno));
  } else {
    ThreadPoolLogging::log(
        "thread (tid=%ld offset=%lx) stiffened cpu=%ld (start=%ld count=%ld)",
        syscall(__NR_gettid), pthread_self(), insert_cpu, 0, cpu_count);
  }
}

void ThreadPool::destroy_pool(ThreadPool *tp) {
  // destory every thread
  for (int i = 0; i < tp->thread_count;
       ++i) {  // send destory task to every thread
    tp->add_task(NULL, NULL, true);
  }
  for (int i = 0; i < tp->thread_count; ++i) {
    pthread_join(tp->thread_list_[i], NULL);
  }
  while (!tp->task_queue_.empty()) {
    Task *temp = tp->task_queue_.front();
    tp->task_queue_.pop();
    Task::destroy_task(temp);  // TODO: consider whether destroy task
  }

  sem_destroy(&tp->undo_task_sem);
  pthread_mutex_destroy(&tp->free_thread_count_lock);
  pthread_mutex_destroy(&tp->undo_task_count_lock);
  pthread_mutex_destroy(&tp->task_queue_lock);

  delete tp->thread_list_;
}
