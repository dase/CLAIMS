/*
 * TimeOutReceiver.h
 * The class extends Theron::Receiver by supporting timeout Wait.
 *  Created on: Oct 25, 2013
 *      Author: wangli
 */
#include <Theron/Theron.h>
#ifndef TIMEOUTRECEIVER_H_
#define TIMEOUTRECEIVER_H_

class TimeOutReceiver:public Theron::Receiver {
public:
	TimeOutReceiver(Theron::EndPoint *endpoint);
	TimeOutReceiver(Theron::EndPoint *endpoint,const char* name);
	virtual ~TimeOutReceiver();
	/**
	 * This function supports timeout Wait.
	 * The calling of this method will be blocked until one of the following conditions satisfied.
	 * (1) expected number of messages are received before the timeout. Then the return value is the number
	 * 		of expected messages.
	 * (2) The time is out before expected number of messages are received. Then the return value is the number
	 * 		of received messages.
	 * E.g., TimeOutWait(10,1000)==10 will return true if 10 messages are received within 1 second, and will false
	 * if less than 10 messages are received within 1 second.
	 *
	 */
	unsigned TimeOutWait(unsigned expected_message_count,unsigned time_out_in_ms);
};

#endif /* TIMEOUTRECEIVER_H_ */
