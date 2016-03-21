/*
 * PortManager.h
 *
 *  Created on: Aug 11, 2013
 *      Author: wangli
 */

#ifndef PORTMANAGER_H_
#define PORTMANAGER_H_
#include <vector>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../utility/lock.h"
class PortManager {
public:
	PortManager();
	virtual ~PortManager();
	static PortManager* getInstance();
	int applyPort();
	void returnPort(unsigned port);
private:
	void getConfigure();
	void generateFreePorts();
private:
	static PortManager* _instance;
	unsigned start;//the beginning number of the available ports
	unsigned end;//the finishing number of the available ports
	std::vector<unsigned> free_ports;
	std::vector<unsigned> used_ports;
	Lock l;//handle multiple thread
};

#endif /* PORTMANAGER_H_ */
