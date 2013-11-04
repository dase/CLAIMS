/*
 * ColumnAppenderBase.h
 *
 *  Created on: May 8, 2013
 *      Author: wangli
 */

#ifndef COLUMNAPPENDERBASE_H_
#define COLUMNAPPENDERBASE_H_

#define oid_t unsigned

class ColumnAppenderBase {
public:
	ColumnAppenderBase(){};

	virtual ~ColumnAppenderBase(){};
	virtual bool open()=0;
	virtual bool append(void* src)=0;
	virtual bool append(void* src, oid_t oid)=0;
	virtual void close()=0;
};


#endif /* COLUMNAPPENDERBASE_H_ */
