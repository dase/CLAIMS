/*
 * ExchangeBase.h
 *
 *  Created on: Jun 20, 2013
 *      Author: wangli
 */

#ifndef EXCHANGEBASE_H_
#define EXCHANGEBASE_H_
#include "../iterator.hpp"
class ExchangeBase: public Iterator {
public:
	ExchangeBase();
	virtual ~ExchangeBase();
	virtual bool open()=0;
	virtual bool next(void*)=0;
	virtual bool close()=0;
};

#endif /* EXCHANGEBASE_H_ */
