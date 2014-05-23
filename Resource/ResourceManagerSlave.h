/*
 * ResourceManagerSlave.h
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#ifndef RESOURCEMANAGERSLAVE_H_
#define RESOURCEMANAGERSLAVE_H_
#include <Theron/Theron.h>
#include "../common/Message.h"
#include "../common/Logging.h"
typedef int NodeID;
class ResourceManagerSlave {
public:
	ResourceManagerSlave();
	virtual ~ResourceManagerSlave();
	NodeID Register();
	void RegisterStorageSlave();
	void ReportStorageBudget(StorageBudgetMessage&);

private:
	Theron::Framework *framework_;
	Logging* logging_;
};

#endif /* RESOURCEMANAGERSLAVE_H_ */
