/*
 * Estimation.h
 *
 *  Created on: 2014年1月22日
 *      Author: volt
 */

#ifndef ESTIMATION_H_
#define ESTIMATION_H_
#include "../../ids.h"
class Estimation {
public:
	Estimation();
	virtual ~Estimation();

	static unsigned long estEqualOper(AttributeID attrID, void *para);
	static unsigned long estRangeOper(AttributeID attrID, void *lowPara, void *upperPara);

};

#endif /* ESTIMATION_H_ */
