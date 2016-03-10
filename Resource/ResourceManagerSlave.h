/*
 * ResourceManagerSlave.h
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#ifndef RESOURCEMANAGERSLAVE_H_
#define RESOURCEMANAGERSLAVE_H_
#include "../common/Message.h"
#include "CPUResource.h"
typedef int NodeID;
/*
 * just for reporting storage budget
 */
class InstanceResourceManager {
 public:
  InstanceResourceManager();
  virtual ~InstanceResourceManager();
  void ReportStorageBudget(StorageBudgetMessage&);
  void setStorageBudget(unsigned long memory, unsigned long disk);

 private:
  CPUResourceManager cpu_resource_;
};

#endif /* RESOURCEMANAGERSLAVE_H_ */
