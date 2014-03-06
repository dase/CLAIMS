/*
 * Mapping.h
 *
 *  Created on: 2014-2-28
 *      Author: casa
 */

#ifndef MAPPING_H_
#define MAPPING_H_

#include <vector>
#include <pair>

//typedef std::pair<int,int> MapPair;
typedef std::vector<int> ExpressionMapping;

class Mapping {
public:
	Mapping();
	virtual ~Mapping();

	bool atomicPushExpressionMapping(ExpressionMapping &expression_mapping);
	ExpressionMapping atomicPopExpressionMapping(int i);

//private:
	std::vector<ExpressionMapping> ExpressionMappingArray_;
};

#endif /* MAPPING_H_ */
