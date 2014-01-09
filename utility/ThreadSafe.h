/*
 * ThreadSafe.h
 *
 *  Created on: Dec 26, 2013
 *      Author: wangli
 */

#ifndef THREADSAFE_H_
#define THREADSAFE_H_
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <malloc.h>
#include "../lock.h"

class ThreadSafe {
public:
	ThreadSafe();
	virtual ~ThreadSafe();


static hostent* gethostbyname_ts(hostent& v,const char* host){
		struct hostent* shared;
		gethostbyname_lock_.lock();
		shared=gethostbyname(host);
		v=*shared;
		gethostbyname_lock_.unlock();
		return shared;
	}
private:

	static SpineLock gethostbyname_lock_;
};

#endif /* THREADSAFE_H_ */
