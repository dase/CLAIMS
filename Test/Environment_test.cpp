/*
 * Environment_test.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */
#include <iostream>
#include "../Environment.h"

int main234234(){
	std::cout<<"Master(0) or Slave(1)?"<<std::endl;
	int ismaster;
	std::cin>>ismaster;
	if(ismaster==0){
		std::cout<<"It's master."<<std::endl;
		Environment::getInstance(true);
	}
	else{
		std::cout<<"It's slave."<<std::endl;
		Environment::getInstance();
	}
	std::cout<<"Sleep~"<<std::endl;
	while(true){
		sleep(1);
	}
}
