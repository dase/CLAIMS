/*
 * Executor.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: wangli
 */


#include "../IteratorExecutorMaster.h"
#include "../IteratorExecutorSlave.h"
#include "../../Environment.h"
static int testIteratorExecutor(){
	Environment::getInstance(true);
	sleep(1);
	IteratorExecutorMaster::getInstance()->Propogation(int(0),"127.0.0.1");

	while(true){
		sleep(10);
	}
}

