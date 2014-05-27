/*
 * TimeOutReceiver.cpp
 *
 *  Created on: Oct 25, 2013
 *      Author: wangli
 */

#include "TimeOutReceiver.h"
#include "../utility/rdtsc.h"
//
TimeOutReceiver::TimeOutReceiver(Theron::EndPoint *endpoint)
:Theron::Receiver(*endpoint){

}
TimeOutReceiver::TimeOutReceiver(Theron::EndPoint *endpoint,const char* name)
:Theron::Receiver(*endpoint,name){

}

TimeOutReceiver::~TimeOutReceiver() {
}
unsigned TimeOutReceiver::TimeOutWait(unsigned expected_message_count,unsigned time_out_in_ms){
	unsigned long long int start=curtick();
	unsigned count(0);
	while(count<expected_message_count&&getMilliSecond(start)<time_out_in_ms){
		count+=Consume(expected_message_count-count);
	}
	return count;
}

