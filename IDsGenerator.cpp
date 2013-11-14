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

}

IDsGenerator::~IDsGenerator() {
	// TODO Auto-generated destructor stub
}

unsigned long long int IDsGenerator::generateUniqueExchangeID(){
	return exchange_id_cursor_++;
}

IDsGenerator* IDsGenerator::getInstance(){
	if(instance_==0){
		return new IDsGenerator();
	}
	else{
		return instance_;
	}
}
