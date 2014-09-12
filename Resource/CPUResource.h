/*
 * CPUResource.h
 *
 *  Created on: Sep 11, 2014
 *      Author: wangli
 */

#ifndef CPURESOURCE_H_
#define CPURESOURCE_H_
#include <vector>

class CPUResourceManager {
public:
	struct socket{
		int cores;
		int busy_cores;
		semaphore sem_cores;
		socket(int number_of_cores);
		int getNumberOfFreeCores();
		/* apply for a core, return false if no cores is currently available.*/
		bool tryApplyCore();
		/* apply for a core. This function will be blocked until a core is
		 * successfully obtained.
		 */
		void applyCore();
		/* return a core to the CPU resource manager*/
		void returnCore();
	};
	CPUResourceManager();
	virtual ~CPUResourceManager();
	/** apply for a core available on any socket,
	 * this function returns false when there is no cores available. **/
	bool tryApplyCore();

	/** apply for a core, blocked if no cores are available.*/
	void applyCore();

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
