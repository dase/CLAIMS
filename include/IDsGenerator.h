/*
 * IDsGenerator.h
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#ifndef IDSGENERATOR_H_
#define IDSGENERATOR_H_

#include "common/ids.h"
#include "utility/lock.h"

class IDsGenerator {
public:
	static IDsGenerator* getInstance();
	virtual ~IDsGenerator();
	unsigned long long int generateUniqueExchangeID();
	ExpanderID getUniqueExpanderID();
private:
	IDsGenerator();
	unsigned long long int exchange_id_cursor_;
	ExpanderID expander_id_cursor_;
	Lock lock_;
	static IDsGenerator* instance_;
};

#endif /* IDSGENERATOR_H_ */
