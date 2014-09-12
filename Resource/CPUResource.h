/*
 * CPUResource.h
 *
 *  Created on: Sep 11, 2014
 *      Author: wangli
 */

#ifndef CPURESOURCE_H_
#define CPURESOURCE_H_
#include <vector>

class CPUResource {
public:
	struct socket{
		int cores;
		int busy_cores;
		int getNumberOfFreeCores();
		bool applyCore();
	};
	CPUResource();
	virtual ~CPUResource();
	/** apply for a core available on any socket **/
	bool applyCore();

	/** return the core to the Cpu resource
	 *  socket_index should be given if the caller is
	 *  not running of the thread bound to the core.
	 */
	void freeCore(int socket_index=-1);
private:
	int number_of_sockets_;
	std::vector<socket> sockets_;
};

#endif /* CPURESOURCE_H_ */
