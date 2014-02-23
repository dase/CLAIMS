/*
 * Expression.h
 *
 *  Created on: 2014-2-23
 *      Author: casa
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class Expression {
public:
	Expression(std::vector<int> map);
	virtual ~Expression();

private:
	std::vector<int> map_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<Expression>(*this) & map_;
	}
};

#endif /* EXPRESSION_H_ */
