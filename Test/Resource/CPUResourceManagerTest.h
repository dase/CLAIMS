/*
 * CPUResourceManagerTest.h
 *
 *  Created on: Sep 13, 2014
 *      Author: wangli
 */

#ifndef CPURESOURCEMANAGERTEST_H_
#define CPURESOURCEMANAGERTEST_H_
#include "../../Resource/CPUResource.h"


static int cpu_resource_manager_test(){
	CPUResourceManager  crm;
	crm.print();

	int core=crm.applyCore();
	crm.print();

	crm.freeCore(core);
	crm.print();

	crm.applyCore();
	crm.print();

	crm.applyCore();
	crm.print();

	crm.applyCore();
	crm.print();

	crm.applyCore();
	crm.print();

	if(crm.tryApplyCore()>=0)
		printf("try ok!\n");
	else
		printf("try fails!\n");
	crm.print();

	crm.applyCore();
	crm.print();
}


#endif /* CPURESOURCEMANAGERTEST_H_ */
