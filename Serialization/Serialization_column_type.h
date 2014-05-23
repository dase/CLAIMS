/*
 * Serialization_column_type.h
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#ifndef SERIALIZATION_COLUMN_TYPE_H_
#define SERIALIZATION_COLUMN_TYPE_H_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "../common/data_type.h"

class Serialization_column_type:public column_type
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & type;
	}
public:
	Serialization_column_type(data_type type,int size=0)
	:column_type(type,size)
	{

	}
//	Serialization_column_type(column_type c) const
//	{
//		this->operate=c.operate;
//		this->size=c.size;
//		this->type=c.type;
//	}
};


#endif /* SERIALIZATION_COLUMN_TYPE_H_ */
