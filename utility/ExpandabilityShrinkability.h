/*
 * ExpandabilityShrinkability.h
 *
 *  Created on: Mar 27, 2014
 *      Author: wangli
 */

#ifndef EXPANDABILITYSHRINKABILITY_H_
#define EXPANDABILITYSHRINKABILITY_H_

class ExpandabilityShrinkability {
public:
	ExpandabilityShrinkability();
	virtual ~ExpandabilityShrinkability();
	virtual bool Expand()=0;
	virtual bool Shrink()=0;
	virtual unsigned GetDegreeOfParallelism()=0;
};

#endif /* EXPANDABILITYSHRINKABILITY_H_ */
