/*
 * data_distribution_generator.cpp
 *
 *  Created on: Aug 17, 2014
 *      Author: wangli
 */


#include "data_distribution_generator.h"

ColumnDistribution::~ColumnDistribution(){

}

UniformInt::UniformInt(int min, int max, int seed):gen_(rd_()),dis_(min,max) {
	gen_.seed(seed);
}


UniformInt::~UniformInt() {
}

void UniformInt::giveRandomValue(void* column_addr) {
	*(int*)column_addr=dis_(gen_);
}

int test_data_distribution_generator() {
	ColumnDistribution* rci=new UniformInt(0,10);
    int value=0;
	for(int i=0;i<16;i++){
		rci->giveRandomValue(&value);
		printf("%d\n",value);
		}
}

ColumnDistribution::ColumnDistribution() {
}

void ColumnDistribution::giveRandomValue(void* column_addr) {
	printf("RandomColumn::giveRandomValue cannot be called, because RandomColumn is a base class.\n");
}
