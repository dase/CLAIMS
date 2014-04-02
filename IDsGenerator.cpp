/*
 * IDsGenerator.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#include "IDsGenerator.h"
IDsGenerator* IDsGenerator::instance_=0;
IDsGenerator::IDsGenerator() {
	exchange_id_cursor_=0;
	expander_id_cursor_=0;

}

IDsGenerator::~IDsGenerator() {
	// TODO Auto-generated destructor stub
}

unsigned long long int IDsGenerator::generateUniqueExchangeID(){
	unsigned long long int ret;
	lock_.acquire();
	ret=exchange_id_cursor_++;
	lock_.release();
	return ret;
}
ExpanderID IDsGenerator::getUniqueExpanderID(){
	ExpanderID ret;
	lock_.acquire();
	ret=expander_id_cursor_++;
	lock_.release();
	return ret;
}

IDsGenerator* IDsGenerator::getInstance(){
	if(instance_==0){
		instance_= new IDsGenerator();
	}
		return instance_;

}
