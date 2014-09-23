/*
 * CPUResource.h
 *
 *  Created on: Sep 11, 2014
 *      Author: wangli
 */

#ifndef CPURESOURCE_H_
#define CPURESOURCE_H_
#include <vector>
#include "../utility/lock.h"

class CPUResourceManager {
public:
	struct core{
		/* the offset in its socket*/
		int offset;

		/* false if is allocated */
		bool available;

		/* the id of this Core,
		 * this value is specified at the system startup */
		int id;
	};
	struct socket{
		int busy_cores;
		semaphore sem_in_socket_available_cores;
		int number_of_cores;
		std::vector<core> cores;
		socket(int number_of_cores);
		socket();

		void addCore(int core_id);
		int getNumberOfFreeCores();

		/* apply for a core, return the id of the allocated core or return -1 if no cores are currently available.*/
		int tryApplyCore();

		/* apply for a core. This function will be blocked until a core is
		 * successfully obtained.
		 * return the id of allocated core.
		 */
		int applyCore();

		/* return a core to the CPU resource manager*/
		bool freeCore(int core_id);

		/* print the availability of the cores on this socket*/
		void print();
	};
	static CPUResourceManager* getInstance();
	CPUResourceManager();
	virtual ~CPUResourceManager();

	/** apply for a core available on any socket,
	 * this function returns the id of the core, or return -1 when there
	 *  is not any core available. **/
	int tryApplyCore();

	/** apply for a core, return id of allocated core, blocked if no cores are available.*/
	int applyCore();

	/** return the core to the Cpu resource
	 *  core_id should be given if the caller is
	 *  not running of the thread bound to the core.
	 */
	void freeCore(int core_id=-1);

	/**
	 * print the status of the cores in an array form.
	 */
	void print();
private:
	semaphore sem_available_cores_;
	int number_of_sockets_;
	std::vector<socket> sockets_;
	static CPUResourceManager* instance_;
};

#endif /* CPURESOURCE_H_ */
