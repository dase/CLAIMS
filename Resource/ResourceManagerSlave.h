/*
 * ResourceManagerSlave.h
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#ifndef RESOURCEMANAGERSLAVE_H_
#define RESOURCEMANAGERSLAVE_H_
#include <Theron/Theron.h>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/Message.h"
#include "../common/Logging.h"
#include "CPUResource.h"
typedef int NodeID;
class InstanceResourceManager {
public:
	InstanceResourceManager();
	virtual ~InstanceResourceManager();
	NodeID Register();
	void ReportStorageBudget(StorageBudgetMessage&);
	void setStorageBudget(unsigned long memory, unsigned long disk);
private:
	Theron::Framework *framework_;
	Logging* logging_;
	CPUResource cpu_resource_;
};

#endif /* RESOURCEMANAGERSLAVE_H_ */
