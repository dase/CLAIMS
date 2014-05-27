/*
 * PortManager.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: wangli
 */

#include "PortManager.h"
#include <libconfig.h++>
#include <iostream>
#include "../common/Logging.h"
#include "../Config.h"
//#include "Debug.h"
PortManager* PortManager::_instance=0;
PortManager::PortManager() {
	getConfigure();
	generateFreePorts();
	Logging_PortManager("PortManager Created, the Ports range is [%u,%u]",start,end);
}

PortManager::~PortManager() {
	// TODO Auto-generated destructor stub
}

PortManager* PortManager::getInstance(){
	if(_instance==0){
		_instance=new PortManager();
	}
	return _instance;
}
void PortManager::getConfigure(){
	libconfig::Config cfg;
	cfg.readFile(Config::config_file.c_str());
	start=(unsigned)cfg.lookup("PortManager.start");
	end=(unsigned)cfg.lookup("PortManager.end");
}
void PortManager::generateFreePorts(){
	for(int i=end;i>=start;i--){
		free_ports.push_back(i);
	}
}
int PortManager::applyPort(){
	l.acquire();
	if(free_ports.size()==0)
		return -1;
	int ret=free_ports.back();
	free_ports.pop_back();
	l.release();
	return ret;
}
void PortManager::returnPort(unsigned port){
	l.acquire();
	free_ports.push_back(port);
	l.release();
}
