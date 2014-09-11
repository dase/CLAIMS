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
#include "ResourceInfo.h"
typedef int NodeID;
class ResourceManagerSlave {
public:
	ResourceManagerSlave();
	virtual ~ResourceManagerSlave();
	NodeID Register();
	void ReportStorageBudget(StorageBudgetMessage&);
	void setStorageBudget(unsigned long memory, unsigned long disk);
private:
	Theron::Framework *framework_;
	Logging* logging_;
	ResourceInfo resource_info_;
};

#endif /* RESOURCEMANAGERSLAVE_H_ */
