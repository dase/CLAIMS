/*
 * MonitorableBuffer.h
 *
 *  Created on: Mar 27, 2014
 *      Author: wangli
 */

#ifndef MONITORABLEBUFFER_H_
#define MONITORABLEBUFFER_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
class MonitorableBuffer {
public:
	MonitorableBuffer();
	virtual ~MonitorableBuffer();
	virtual double getBufferUsage()=0;
	virtual long getReceivedDataSizeInKbytes() =0;
	void setInputComplete();
	bool inputComplete()const;
private:
	bool input_complete_;
};

#endif /* MONITORABLEBUFFER_H_ */
