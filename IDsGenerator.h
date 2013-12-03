/*
 * IDsGenerator.h
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#ifndef IDSGENERATOR_H_
#define IDSGENERATOR_H_

class IDsGenerator {
public:
	static IDsGenerator* getInstance();
	virtual ~IDsGenerator();
	unsigned long long int generateUniqueExchangeID();
private:
	IDsGenerator();
	unsigned long long int exchange_id_cursor_;
	static IDsGenerator* instance_;
};

#endif /* IDSGENERATOR_H_ */
