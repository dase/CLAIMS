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

IteratorExecutorSlave::IteratorExecutorSlave() {}
IteratorExecutorSlave::~IteratorExecutorSlave() {}
void IteratorExecutorSlave::createNewThreadAndRun(PhysicalQueryPlan* it) {
  pthread_t thread;
  void** arg = new void* [2];
  arg[0] = it;
  arg[1] = this;
  pthread_create(&thread, NULL, run_iterator, arg);
  lock_.acquire();
  busy_thread_list_.insert(thread);
  lock_.release();

  LOG(INFO) << "A new Running thread is created!";
}
void* IteratorExecutorSlave::run_iterator(void* arg) {
  pthread_detach(pthread_self());
  PhysicalQueryPlan* it = (PhysicalQueryPlan*)(*(void**)arg);
  IteratorExecutorSlave* Pthis = (IteratorExecutorSlave*)(*((void**)arg + 1));
  executePhysicalQueryPlan(*it);
  it->destory();
  delete it;
  LOG(INFO) << "A iterator tree is successfully executed!\n";
  Pthis->lock_.acquire();
  assert(Pthis->busy_thread_list_.find(pthread_self()) !=
         Pthis->busy_thread_list_.end());
  Pthis->busy_thread_list_.erase(pthread_self());
  Pthis->lock_.release();
  delete[]((void**)arg);
}

void IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan plan) {
  //	int core=CPUResourceManager::getInstance()->applyCore();
  //	printf("--------\n After apply:");
  //	CPUResourceManager::getInstance()->print();
  plan.run();
  //	CPUResourceManager::getInstance()->freeCore(core);
}
