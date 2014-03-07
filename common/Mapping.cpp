/*
 * Mapping.cpp
 *
 *  Created on: 2014-2-28
 *      Author: casa
 */

#include "Mapping.h"

Mapping::Mapping() {

}

Mapping::~Mapping() {

}

bool Mapping::atomicPushExpressionMapping(ExpressionMapping &expressiong_mapping){
	ExpressionMappingArray_.push_back(expressiong_mapping);
	return true;
}

ExpressionMapping Mapping::atomicPopExpressionMapping(int i){
	return ExpressionMappingArray_.at(i);
}

std::vector<ExpressionMapping> Mapping::getMapping(){
	return ExpressionMappingArray_;
}
