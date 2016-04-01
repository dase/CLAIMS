/*
 * IteratorExecutorSlave.h
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#ifndef ITERATOREXECUTORSLAVE_H_
#define ITERATOREXECUTORSLAVE_H_
#include <string>
#include <libconfig.h++>
#include <iostream>
#include <set>
#include "../common/Message.h"
#include "../common/Logging.h"
#include "../utility/lock.h"
#include "../utility/Timer.h"

using namespace std;
class PhysicalQueryPlan;
class IteratorExecutorSlave {
 public:
  IteratorExecutorSlave();

  virtual ~IteratorExecutorSlave();
  static void executePhysicalQueryPlan(PhysicalQueryPlan plan);

 public:
  void createNewThreadAndRun(PhysicalQueryPlan*);

 private:
  static void* run_iterator(void*);

 protected:
  Logging* logging_;

 private:
  std::string slave_id;
  SpineLock lock_;
  std::set<pthread_t> busy_thread_list_;
};

#endif /* ITERATOREXECUTORSLAVE_H_ */
