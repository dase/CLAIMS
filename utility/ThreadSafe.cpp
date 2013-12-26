/*
 * ThreadSafe.cpp
 *
 *  Created on: Dec 26, 2013
 *      Author: wangli
 */

#include "ThreadSafe.h"
SpineLock ThreadSafe::gethostbyname_lock_;
ThreadSafe::ThreadSafe() {
	// TODO Auto-generated constructor stub

}

ThreadSafe::~ThreadSafe() {
	// TODO Auto-generated destructor stub
}

