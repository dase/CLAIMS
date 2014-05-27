/*
    Copyright 2011, Spyros Blanas.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
using std::string;

#include "hash.h"

#ifdef DEBUG
#include <cassert>
#endif 

/**
 * Returns the base 2 logarithm of the next higher power of two.
 */
unsigned int getlogarithm(unsigned int k) {
	--k;
	int m = 1 << (sizeof(int)*8-1);
	for (unsigned int i=0; i<sizeof(int)*8; ++i) {
		if ((m >> i) & k)
			return sizeof(int)*8-i;
	}
	return 0;
}

PartitionFunction::PartitionFunction(int min, int max, unsigned int k) : min_(min), max_(max) {
	if (k==0 || k==1)
		k_=1;
	else
		k_ = getlogarithm(k);
}
bool PartitionFunction::equal(PartitionFunction* function_)const{
	return (getPartitionFashion()==function_->getPartitionFashion())
			&&
			(getNumberOfPartitions()==function_->getNumberOfPartitions());
}
//unsigned int PartitionFunction::getNumberOfPartitions()const {
//	return 1 << k_;
//}

PartitionFunction::partition_fashion ModuloHashFunction::getPartitionFashion()const{
	return hash_f;
}
PartitionFunction::partition_fashion RoundRobinPartitionFunction::getPartitionFashion()const{
	return round_robin_f;
}
PartitionFunction::partition_fashion UniformRangePartitionFunction::getPartitionFashion()const{
	return range_f;
}

PartitionFunction* PartitionFunctionFactory::createModuloFunction(const int &range, const int &min, const int &skipbits){
	return new ModuloHashFunction(min, range, skipbits);
}

PartitionFunction* PartitionFunctionFactory::createUniformRangeHashFunction(const int &range, const int &min, const int &max){
	return new UniformRangePartitionFunction(min, max, range);
}
PartitionFunction* PartitionFunctionFactory::createRoundRobinPartitionHashFunction(int range){
	return new RoundRobinPartitionFunction(range);
}
PartitionFunction* PartitionFunctionFactory::createGeneralModuloFunction(const int & range, const int &min,const int &skipbits){
	return new GeneralModuloFunction(range,min,skipbits);
}


PartitionFunction::partition_fashion GeneralModuloFunction::getPartitionFashion()const{
	return hash_f;
}
