/*
 * Mapping.h
 *
 *  Created on: 2014-2-28
 *      Author: casa
 */

#ifndef MAPPING_H_
#define MAPPING_H_

#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
//typedef std::pair<int,int> MapPair;
typedef std::vector<int> ExpressionMapping;

class Mapping {
public:
	Mapping();
	virtual ~Mapping();

	bool atomicPushExpressionMapping(ExpressionMapping &expression_mapping);
	ExpressionMapping atomicPopExpressionMapping(int i);

	std::vector<ExpressionMapping> getMapping();

private:
	std::vector<ExpressionMapping> ExpressionMappingArray_;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ExpressionMappingArray_;
	}
};

#endif /* MAPPING_H_ */
