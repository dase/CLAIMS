/*
 * Mapping.cpp
 *
 *  Created on: 2014-2-28
 *      Author: casa
 */

#include "Mapping.h"

Mapping::Mapping() {
	// TODO 自动生成的构造函数存根

}

Mapping::~Mapping() {
	// TODO 自动生成的析构函数存根
}

bool Mapping::atomicPushExpressionMapping(ExpressionMapping &expressiong_mapping){
	ExpressionMappingArray_.push_back(expressiong_mapping);
	return true;
}

ExpressionMapping Mapping::atomicPopExpressionMapping(int i){
	return ExpressionMappingArray_.at(i);
}
