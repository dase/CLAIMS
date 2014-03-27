/*
 * MonitorableBuffer.h
 *
 *  Created on: Mar 27, 2014
 *      Author: wangli
 */

#ifndef MONITORABLEBUFFER_H_
#define MONITORABLEBUFFER_H_

class MonitorableBuffer {
public:
	MonitorableBuffer();
	virtual ~MonitorableBuffer();
	virtual double getBufferUsage()=0;
	virtual long getReceivedDataSizeInKbytes() =0;
};

#endif /* MONITORABLEBUFFER_H_ */
