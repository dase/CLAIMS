/*
 * ResourceManagerSlave.h
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#ifndef RESOURCEMANAGERSLAVE_H_
#define RESOURCEMANAGERSLAVE_H_
#include <Theron/Theron.h>
#include "../Message.h"
class ResourceManagerSlave {
public:
	ResourceManagerSlave();
	virtual ~ResourceManagerSlave();
	NodeID Register(std::string ip,unsigned port);
	void RegisterStorageSlave();
	void ReportStorageBudget(RegisterStorageMessage&);

private:
	Theron::Framework *framework_;
};

#endif /* RESOURCEMANAGERSLAVE_H_ */
