/*
 * IteratorExecutorSlave.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#include "IteratorExecutorSlave.h"
#include <glog/logging.h>
#include "../Environment.h"
#include "../Resource/CPUResource.h"
#include "../utility/print_tool.h"
#include "../utility/thread_pool.h"

#define USE_THREAD_POOL

IteratorExecutorSlave::IteratorExecutorSlave() {}
IteratorExecutorSlave::~IteratorExecutorSlave() {}
void IteratorExecutorSlave::createNewThreadAndRun(PhysicalQueryPlan* it) {
  void** arg = new void* [2];
  arg[0] = it;
  arg[1] = this;

#ifndef USE_THREAD_POOL
  pthread_t thread;
  int error = pthread_create(&thread, NULL, run_iterator, arg);
  if (error != 0) {
    LOG(ERROR) << it->get_query_id() << " , " << it->get_segment_id_()
               << " IteratorExecutorSlave Failed to create thread";
    return;
  }
#else
  Environment::getInstance()->getThreadPool()->AddTask(run_iterator, arg);
#endif
  //  lock_.acquire();
  //  busy_thread_list_.insert(thread);
  //  lock_.release();

  LOG(INFO) << it->get_query_id() << " , " << it->get_segment_id_()
            << "A new Running thread is created!";
}
void* IteratorExecutorSlave::run_iterator(void* arg) {
#ifndef USE_THREAD_POOL
  pthread_detach(pthread_self());
#endif
  PhysicalQueryPlan* it = (PhysicalQueryPlan*)(*(void**)arg);
  IteratorExecutorSlave* Pthis = (IteratorExecutorSlave*)(*((void**)arg + 1));
  executePhysicalQueryPlan(*it);
  it->destory();
  delete it;
  LOG(INFO) << "A iterator tree is successfully executed!\n";

  //  Pthis->lock_.acquire();
  //  assert(Pthis->busy_thread_list_.find(pthread_self()) !=
  //         Pthis->busy_thread_list_.end());
  //  Pthis->busy_thread_list_.erase(pthread_self());
  //  Pthis->lock_.release();
  delete[]((void**)arg);
}

void IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan plan) {
  //	int core=CPUResourceManager::getInstance()->applyCore();
  //	printf("--------\n After apply:");
  //	CPUResourceManager::getInstance()->print();
  plan.run();
  //	CPUResourceManager::getInstance()->freeCore(core);
}
