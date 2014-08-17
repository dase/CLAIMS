/*
 * data_distribution_generator.h
 *
 *  Created on: Aug 17, 2014
 *      Author: wangli
 */

#ifndef DATA_DISTRIBUTION_GENERATOR_H_
#define DATA_DISTRIBUTION_GENERATOR_H_
#include <stdio.h>
//#define __GXX_EXPERIMENTAL_CXX0X__
#include <random>


class ColumnDistribution{
public:
	ColumnDistribution();
	virtual ~ColumnDistribution();
	virtual void giveRandomValue(void * column_addr);
};
class UniformInt: public ColumnDistribution{
public:
	UniformInt(int min=0,int max=1024*1024,int seed =0);
	~UniformInt();
	virtual void giveRandomValue(void* column_addr);
private:
	std::random_device rd_;
	std::mt19937 gen_;
	std::uniform_int_distribution<> dis_;

};


//
//
//std::uniform_int_distribution<> get_int_uniform_dis_engin(int min=0, int max=1024*1024, int seed=777){
//    std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_int_distribution<> dis(min, max);
//	return dis(min,max);
////	gen.seed(seed);
////	return gen;
//}


int test_data_distribution_generator();


#endif /* DATA_DISTRIBUTION_GENERATOR_H_ */
